#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#define BUFSIZE 1024
#define MAXCMDC 4

int systhr_create(void * (*start_func)(void *), void *arg){
  int status = 0;
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  status = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  if(status == 0) status = pthread_create(&tid, &attr, start_func, arg);
  if(status != 0) status = pthread_create(&tid, 0,     start_func, arg);
  return status;
}

void mem_error(const char *str){
  fputs(str, stderr); fputc('\n', stderr);
  exit(1);
}

struct cmd {
  int  c;
  char *v[MAXCMDC];
};

char buf[BUFSIZE];

/* input -> struc cmd */
struct cmd recv_command(){
  struct cmd r;
  char p, c, *b = buf;
  fgets(b, BUFSIZE, stdin);
  r.c = 0;
  for(p = 0, c = *b ; c ; p = c, c= *++b)
    if(c == ' ' || c == '\n')
      *b = c = 0;
    else if (p == 0 && r.c < MAXCMDC)
      r.v[r.c++] = b;
  return r;
}

pthread_mutex_t send_mut = PTHREAD_MUTEX_INITIALIZER;

/* struc cmd -> output */
void send_command(struct cmd cmd){
  int i;
  for(i=0; i<cmd.c-1; i++){
    fputs(cmd.v[i], stdout); fputc(' ', stdout);
  }
  if(cmd.c > 0){
    fputs(cmd.v[cmd.c-1], stdout); fputc('\n', stdout);
  }
}

void proto_error(const char *str, struct cmd cmd){
  int i;
  fputs(str, stderr); fputc('\n', stderr);
  for(i=0; i<cmd.c-1; i++){
    fputs(cmd.v[i], stderr); fputc(' ', stderr);
  }
  if(cmd.c > 0){
    fputs(cmd.v[cmd.c-1], stderr); fputc('\n', stderr);
  }
  exit(1);
}

void read_to_eol(){
  int c;
  while((c = getc(stdin)) != EOF) if (c == '\n') break;
}
void write_eol(){ putc('\n', stdout); }
void flush_send(){ fflush(stdout); }

struct task;
struct thread_data;

void do_task_body(struct thread_data *, void *);
void send_task_body(struct thread_data *, void *);
void *recv_task_body(struct thread_data *);
void send_rslt_body(struct thread_data *, void *);
void recv_rslt_body(struct thread_data *, void *);

enum task_stat {
  TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE,
  TASK_NONE, TASK_SUSPENDED
};

enum task_home_stat {
  TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED,
  /* 結果待ちは? => task のほうでわかる? */
  TASK_HOME_DONE
};

struct task {
  enum task_stat stat;
  struct task* next;
  struct task* prev;
  void *body;
  int ndiv;
  char rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int id;
  struct task_home* next;
  void *body;
  char task_head[256];
};

struct thread_data {
  struct task_home * volatile req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  struct task *task_free;
  struct task *task_top;
  struct task_home* treq_free;
  struct task_home* treq_top;
  struct task_home* sub;
  pthread_mutex_t mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char id_str[32];
  char buf[BUFSIZE];
};

struct thread_data thread_data[64];

/*
  リモートに treq してもらった task (copy)
    - treq まえに allocate
    x ときどき，リストから DONE となったものを除く
      (それなら，rack を得るまで DONE にしないようにする
       active なスレッド数は別管理すればよい?)
    x  ときどきでなくて，rack を得たときでよい?
    - rslt を送ったら自分で消える．

  分割して作った task (home/orig) => task_home
    - thread_data の sub からのリストとなり，id は重複しないように付ける．
    x 最初から，STARTED，すぐに treq 元に送る．
    o treq の時点で ALLOCATED にしては?
    - rslt がきたら，DONE にして，rack を返す
    - DONE になっていたら，分割元task がマージして消去

  ?? task_home をそのまま同じノードで処理するケースもあるのか??
 */
/*
    treq <task_head> <treq_head>
      <task_head>  タスク返答先
      <treq_head>  要求送信先

    task <ndiv> <rslt_head> <task_head>
      <ndiv>       分割回数 負荷のサイズの目安 (sp2が子の判断に使う)
      <rslt_head>  結果返答先
      <task_head>  タスク送信先

    rslt <rslt_head>
      <rslt_head>  結果送信先

    rack <task_head>
      <task_head>  rack送信先
      (w_rack カウンタを使うべき)

    none <task_head>
      <task_head>  (no)タスク送信先
 */
/*
   [ prev  ] -> [ prev  ] -> [ prev  ]  -> 
<- [ next  ] <- [ next  ] <- [ next  ] <- 
 */

void flush_treq_with_none_1(struct thread_data *thr){
  /* thr->mut と send_mut は lock 済 */
  struct cmd rcmd;
  struct task_home *hx;
  while(hx = thr->treq_top){
    rcmd.c = 2;
    rcmd.v[0] = "none";
    rcmd.v[1] = hx->task_head;
    send_command(rcmd);
    thr->treq_top = hx->next;
    hx->next = thr->treq_free;
    thr->treq_free = hx;
  }
}

void flush_treq_with_none(struct thread_data *thr){
  /* treq がたまっていたら，none を送る．
     thr->mut は lock 済 */
  if(thr->treq_top){
    pthread_mutex_lock(&send_mut);
    flush_treq_with_none_1(thr);
    flush_send();
    pthread_mutex_unlock(&send_mut);
  }
}

void recv_exec_send(struct thread_data *thr, char *treq_head){
  /* thr->mut は lock 済 */
  struct cmd rcmd;
  struct task *tx;
  long delay;
  int old_ndiv;
  /* 最初にxをとる

     タスクをとろうとしている間に，rslt が届いたら工夫するのか?
     単に，rslt 待ちについては，wait しないで，定期的にみる．
     みたときに，なければ，task 待ちにしてしまうというのが簡単?
     だって task 待ちにしたら treq をだすのだし．
     ここでは，task 待ちのする場合

     task については，treq 時点で場所を作っておく => ノード内並列にも

     同時スレッド数は超えないようには，別途セマフォで
  */

  /* あとでとどく none が届くまで待つ */
  while(thr->w_none > 0) {
    pthread_cond_wait(&thr->cond, &thr->mut);
    /* rslt が到着していたら */
    if(thr->sub && thr->sub->stat == TASK_HOME_DONE)
      return;
  }

  /* allocate */
  tx = thr->task_free;
  if(!tx)
    mem_error("not enough task memory");
  thr->task_top = tx;
  thr->task_free = tx->prev;

  delay = 2 * 1000 * 1000;
  do{

    /* 最初に treq がたまっていたら，none を送る */
    flush_treq_with_none(thr);

    tx->stat = TASK_ALLOCATED;
    rcmd.c = 3;
    rcmd.v[0] = "treq";
    rcmd.v[1] = thr->id_str;
    rcmd.v[2] = treq_head;
    pthread_mutex_lock(&send_mut);
    send_command(rcmd);
    flush_send();
    pthread_mutex_unlock(&send_mut);
    /* recv_task で初期化されるのを待つ */
    do{
      pthread_cond_wait(&thr->cond, &thr->mut);

      /* rslt が到着していたら 特別に先にさせる */
      if(tx->stat != TASK_INITIALIZED
	 && thr->sub && thr->sub->stat == TASK_HOME_DONE){
	if(tx->stat == TASK_NONE)
	  goto Lnone;
	thr->w_none++;
	goto Lnone;
      }
    } while(tx->stat == TASK_ALLOCATED);

    if(tx->stat == TASK_NONE){
      struct timespec t1;
      struct timeval now;
      long nsec;
      gettimeofday(&now, 0);
      nsec = now.tv_usec * 1000;
      /* nsec += 10 * 1000 * 1000; */
      nsec += delay;
      delay += delay;
      if(delay > 40 * 1000 * 1000)
	delay = 40 * 1000 * 1000;
      t1.tv_nsec = (nsec > 999999999) ? nsec - 999999999 : nsec;
      t1.tv_sec = now.tv_sec + ((nsec > 999999999) ? 1 : 0);
      pthread_cond_timedwait(&thr->cond_r, &thr->mut, &t1);

      if(thr->sub && thr->sub->stat == TASK_HOME_DONE)
	goto Lnone;
    }
  }while(tx->stat != TASK_INITIALIZED);

  /*
    none, task, rslt を同時に待てないか?
    rack を使う限りでは，treq 中に rslt が届いた場合はその後
    かならず none が戻ってくる．
    none を待たずに，w_none を inc して，none が届いたときには
    "struct task の書き込み/signal"せずに dec だけすればよさそう．

    rack を使わないなら，rslt の後，task が届くかも．
    現在処理中の task が task_top でなくてもよいなら問題ない．
    そのためには，task_top でないときに分かるようにするとともに
    treq 時に thread番号に，task番号を加える必要があると思う．
   */

  /* TASK_INITIALIZED */
  tx->stat = TASK_STARTED;
  old_ndiv = thr->ndiv;
  thr->ndiv = tx->ndiv;

  /* ここに セマフォ? */

  pthread_mutex_unlock(&thr->mut);
  do_task_body(thr, tx->body);
  pthread_mutex_lock(&thr->mut);

  /* task の処理完了後は，その task_home に send_rslt する */
  rcmd.c = 2;
  rcmd.v[0] = "rslt";
  rcmd.v[1] = tx->rslt_head;
  pthread_mutex_lock(&send_mut);
  send_command(rcmd);
  /* body ではなく，do_task_body の return value にするのはどうか? */
  send_rslt_body(thr, tx->body);
  /* または，x->body.h.class->send_rslt_body(x->body); */
  write_eol();
  /* 最後にも treq がたまっていたら，none を送る */
  flush_treq_with_none_1(thr);
  flush_send();
  pthread_mutex_unlock(&send_mut);
  thr->w_rack++;
  thr->ndiv = old_ndiv;
 Lnone:
  thr->task_free = tx;
  thr->task_top = tx->next;
}

void *worker(void *arg){
  struct thread_data *thr = arg;
  pthread_mutex_lock(&thr->mut);
  for(;;)
    recv_exec_send(thr, "any");
  pthread_mutex_unlock(&thr->mut);
}

unsigned int num_thrs = 1;

void recv_task(struct cmd cmd){
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  size_t len;
  if(cmd.c < 4)
    proto_error("wrong task", cmd);
  /* id を <task_head> に含める */
  id = atoi(cmd.v[3]);
  if(! (id<num_thrs))
    proto_error("wrong task_head", cmd);
  thr = thread_data+id;
  pthread_mutex_lock(&thr->mut);
  tx = thr->task_top;
  len = strlen(cmd.v[2]);
  if(len > 254)
    proto_error("too long rslt_head for task", cmd);
  strncpy(tx->rslt_head, cmd.v[2], len+1);
  tx->ndiv = atoi(cmd.v[1]);
  tx->body = recv_task_body(thr);
  read_to_eol();
  /* task を受け取った後，ノード内に待っているスレッド(ワーカ)を起こす */
  tx->stat = TASK_INITIALIZED;
  /* スレッド数の上限を超えるなら，後で(どれかのスレッドが終わるとき)
     signal するべきか?
     それよりは，別にセマフォで上限を管理するほうが簡単  */
  pthread_cond_broadcast(&thr->cond); /* pthread_cond_signal? */
  pthread_mutex_unlock(&thr->mut);
}

void recv_none(struct cmd cmd){
  struct thread_data *thr;
  unsigned int id;
  size_t len;
  if(cmd.c < 2)
    proto_error("wrong none", cmd);
  /* id を <task_head> に含める */
  id = atoi(cmd.v[1]);
  if(! (id<num_thrs))
    proto_error("wrong task_head", cmd);
  thr = thread_data+id;
  pthread_mutex_lock(&thr->mut);
  if(thr->w_none > 0)
    thr->w_none--;
  else
    thr->task_top->stat = TASK_NONE;
  pthread_cond_broadcast(&thr->cond); /* pthread_cond_signal? */
  pthread_mutex_unlock(&thr->mut);
}

/*
  rack について
    rslt には <rack_head> はない => <task_head> を使う
    treq 中なら，rack を返さないとしているが? (sp00b)
      rack を返したときには，ワーカに結果を取り込ませようと試みる

    treq 中に rslt をもらったら，すぐに rack を返さず，
    その treq は必ず none されるはずだから，none に対応してrack を返す．
    なぜ?  sp.memo に書いた SP: 以降 のように，FIFO性がないと，treq を
           rack が追い越してしまうことがあるから．
    => とりあえずは，FIFO性を仮定してそのまま
 */
void recv_rslt(struct cmd cmd){
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int tid, sid;
  char *b;
  if(cmd.c < 2)
    proto_error("wrong rslt", cmd);
  b = cmd.v[1];
  tid = atoi(b);
  b = strchr(b, ':');
  if(!b)
    proto_error("wrong rslt_head", cmd);
  sid = atoi(b+1);
  thr = thread_data+tid;
  pthread_mutex_lock(&thr->mut);
  /*
  hx = thr->sub;
  while(hx){
    fprintf(stderr,
	    "hx %d %d %s (%d,%d)\n", hx, hx->id, hx->task_head, tid, sid);
    hx = hx->next;
  }
  */
  hx = thr->sub;
  while(hx && hx->id != sid)
    hx = hx->next;
  if(!hx)
    proto_error("wrong rslt_head", cmd);
  recv_rslt_body(thr, hx->body);
  read_to_eol();
  /* rack を返す もっと後のほうがよい? */
  rcmd.c = 2;
  rcmd.v[0] = "rack"; rcmd.v[1] = hx->task_head;
  pthread_mutex_lock(&send_mut);
  send_command(rcmd);
  flush_send();
  pthread_mutex_unlock(&send_mut);
  /* hx 中に記録された task_head に rack を後で送るなら，
     ここではないが，まだ free されたくないので，つなぎなおすかも  */
  hx->stat = TASK_HOME_DONE;
#if 1
  if(hx == thr->sub){
    pthread_cond_broadcast(&thr->cond_r);
    pthread_cond_broadcast(&thr->cond);
  }
#endif
  pthread_mutex_unlock(&thr->mut);
}

/*
  worker がしばらく受け取ってくれなくても，
  treq は受け取っておくほうがよさそう．
  複数の treq をリストにして受け取らせてもよいのでは?
  treq は task_home の形にしておいてもよいかも．
  task_home は スタックアロケートのつもりだったが...
 */

int try_treq(struct cmd cmd, unsigned int id){
  struct task_home *hx;
  struct thread_data *thr;
  size_t len;
  int avail = 0;


  thr = thread_data+id;
  pthread_mutex_lock(&thr->mut);
  if(thr->task_top && thr->task_top->stat == TASK_STARTED && thr->w_rack == 0)
    avail = 1;

  /*
  fprintf(stderr, "try_treq id:%d, task_top:%ld stat=%d(%d) wrack:%d \n", 
	  id, thr->task_top, thr->task_top->stat, TASK_STARTED, thr->w_rack);
  */

  if(avail){
    hx = thr->treq_free;
    if(!hx)
      mem_error("not enough task_home memory");
    thr->treq_free = hx->next;
    hx->next = thr->treq_top;
    hx->stat = TASK_HOME_ALLOCATED;
    len = strlen(cmd.v[1]);
    if(len > 254)
      proto_error("too long task_head for treq", cmd);
    strncpy(hx->task_head, cmd.v[1], len+1);
    thr->treq_top = hx;
    thr->req = hx;
  }
  pthread_mutex_unlock(&thr->mut);
  return avail;
}

/*
  treq をためておくか問題
    自分も treq 中のときは，
      none を返すさないとデッドロックの危険．
      順序がつけられてループしなければ，またせてもよい．
    自分は動いているが渡すものがないとき
      none を返すと何度もいってくるかも．
      none を返さないと，他にいけばよいのに待つことになるかも．
      一度 none で拒否されたら覚えておき，treq 受け付けますと
      いう宣伝がくるまではそのノードには treq しないとかは...
 */

void recv_treq(struct cmd cmd){
  /*
    task id が指定されている場合と，any の場合がある．
    any の場合は，any でとってきたもののほうが大きいのでは?
    => そもそも，すべての task が動いているわけではない．
       すくなくとも，rslt まちで，別 task を立ち上げた task に
       req してもだめ．
    * thread 単位で req 先をもつならそれはそれで OK
    * task 単位で req 先をもつなら，
      + task の req 先を見つけられるかという問題
      + 今動いているものに req すべき
        regulation (セマフォ)で，停止中の task に req してももらえないかも
         -> any だったら対象からはずす．
            または， 特定のtask なら，一時的に regu をはずす?
            regu しないならしないで，それは，進まないものが出る．
            一度，reguをはずしても，task_send後に regu を確認
         -> ちょっと余分に thread を使っても，タスク分割がその分
            進みが悪くなるなら，結局，遅延隠蔽にならない可能性が高い．
            遅延隠蔽のためには，thread が眠っていても task が分割可能
            だとよい．寝る前に分割しておくとか...
            -> スレッド数1で先に分割するのはあまり意味はないかも．
                 => もぐらない効果はある?
               それに先に分割するぐらいなら，スレッド数を倍でよい?
    */
  struct cmd rcmd;
  unsigned int id;
  if(cmd.c < 3)
    proto_error("wrong treq", cmd);
  /* id を <task_head> に含める */
  if(strcmp(cmd.v[2], "any") == 0){
    for(id=0;id<num_thrs;id++)
      if(try_treq(cmd, id)) break;
    if(id != num_thrs)
      return;
  }else{
    id = atoi(cmd.v[2]);
    if(! (id<num_thrs))
      proto_error("wrong task_head", cmd);
    if(try_treq(cmd, id))
      return;
  }
  /* treq できなかった場合 */

  /* none を返す */
  rcmd.c = 2;
  rcmd.v[0] = "none"; rcmd.v[1] = cmd.v[1];
  pthread_mutex_lock(&send_mut);
  send_command(rcmd);
  flush_send();
  pthread_mutex_unlock(&send_mut);
}

void recv_rack(struct cmd cmd){
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  size_t len;
  if(cmd.c < 2)
    proto_error("wrong rack", cmd);
  /* id を <task_head> に含める */
  id = atoi(cmd.v[1]);
  if(! (id<num_thrs))
    proto_error("wrong task_head", cmd);
  thr = thread_data+id;
  pthread_mutex_lock(&thr->mut);
  thr->w_rack--;
  pthread_mutex_unlock(&thr->mut);
}

main(int argc, char *argv[]){
  int i, j;
  struct cmd cmd;

  /* ここで thread_data の初期化, task の 双方向list も */
  num_thrs = 1;
  for(i=0; i<num_thrs; i++){
    struct thread_data *thr = thread_data+i;
    struct task *tx;
    struct task_home *hx;
    thr->id = i;
    thr->w_rack = 0;
    thr->w_none = 0;
    thr->ndiv = 0;
    pthread_mutex_init(&thr->mut, 0);
    pthread_cond_init(&thr->cond, 0);
    pthread_cond_init(&thr->cond_r, 0);
    sprintf(thr->id_str, "%d", i);

    tx = (struct task *)malloc(sizeof(struct task) * 512);
    thr->task_top = 0;
    thr->task_free = tx;
    for(j=0; j<511; j++){
      tx[j].prev = &tx[j+1];
      tx[j+1].next = &tx[j];
    }
    tx[0].next = 0;
    tx[511].prev = 0;
    
    hx = (struct task_home *)malloc(sizeof(struct task_home) * 512);
    thr->treq_free = hx;
    thr->treq_top = 0;
    thr->sub = 0;
    for(j=0; j<511; j++)
      hx[j].next = &hx[j+1];
    hx[511].next = 0;

    /* ここでワーカをfork 
     同じノード内で(共有メモリで)でもやりくりできるか?  */
    systhr_create(worker, thr);
  }

  while(cmd = recv_command(), (cmd.c > 0 && strcmp(cmd.v[0],"exit") != 0))
    if(strcmp(cmd.v[0],"task") == 0)
      recv_task(cmd);
    else if(strcmp(cmd.v[0],"rslt") == 0)
      recv_rslt(cmd);
    else if(strcmp(cmd.v[0],"treq") == 0)
      recv_treq(cmd);
    else if(strcmp(cmd.v[0],"none") == 0)
      recv_none(cmd);
    else if(strcmp(cmd.v[0],"rack") == 0)
      recv_rack(cmd);
    else {
      proto_error("wrong cmd", cmd);
    }
  exit(0);
}

#define FF int (*_bk)(), struct thread_data *_thr
#define FA _bk, _thr

#define DO_TWO(type1, struct1, work1, work2, put_task1, get_rslt1)	\
do{									\
  type1 struct1[1];							\
  int spawned = 0;							\
  {									\
    int (*_bk2)() = _bk;						\
    int _bk(){								\
      if(spawned)							\
	return 0;							\
      _bk2();								\
      if(_thr->treq_top){						\
	(put_task1);							\
	spawned = 1;							\
	make_and_send_task(_thr, struct1);				\
	return 1;							\
      }									\
      return 0;								\
    }									\
    if(_thr->req)							\
      handle_req(FA);							\
    (work1);								\
  }									\
  if(spawned){								\
    wait_rslt(_thr);							\
    (get_rslt1);							\
  } else {								\
    (work2);								\
  }									\
}while(0)

void handle_req(FF){
  pthread_mutex_lock(&_thr->mut);
  if(_thr->req){
    _bk();
    /* ここで 残っていたら，send none する代わりに，
       STARTED でなくなったら none する手も．
       _thr->req  != 0 のままにする */
    _thr->req = _thr->treq_top;
  }
  pthread_mutex_unlock(&_thr->mut);
}

void make_and_send_task(struct thread_data *thr, void *body){
  struct cmd tcmd;
  struct task_home *hx=thr->treq_top;
  thr->treq_top = hx->next;
  hx->next = thr->sub;
  thr->sub = hx;
  hx->body = body;
  hx->id = hx->next ? hx->next->id + 1 : 0;
  hx->stat = TASK_HOME_INITIALIZED;
  tcmd.c = 3;
  tcmd.v[0] = "task";
  /* ズルなので注意 */
  sprintf(thr->buf, "%d %s:%d", ++thr->ndiv, thr->id_str, hx->id);
  tcmd.v[1] = thr->buf;
  tcmd.v[2] = hx->task_head;
  pthread_mutex_lock(&send_mut);
  send_command(tcmd);
  send_task_body(thr, body);
  write_eol();
  flush_send();
  pthread_mutex_unlock(&send_mut);
}

void wait_rslt(struct thread_data *thr){
  struct timespec t1;
  struct timeval now;
  long nsec;
  struct task_home *sub;
  pthread_mutex_lock(&thr->mut);
  sub = thr->sub;
  while(sub->stat != TASK_HOME_DONE){
#if 1
#if 0
    flush_treq_with_none(thr);
    thr->task_top->stat = TASK_SUSPENDED;
#endif
    gettimeofday(&now, 0);
    nsec = now.tv_usec * 1000;
    nsec += 5 * 000 * 000;
    /* nsec += 10 * 1000 * 1000; */
    t1.tv_nsec = (nsec > 999999999) ? nsec - 999999999 : nsec;
    t1.tv_sec = now.tv_sec + ((nsec > 999999999) ? 1 : 0);
    pthread_cond_timedwait(&thr->cond_r, &thr->mut, &t1);
#if 0
    thr->task_top->stat = TASK_STARTED;
#endif
    if(sub->stat == TASK_HOME_DONE)
      break;
#endif
    /* fprintf(stderr, "sub %d\n", sub); */

    recv_exec_send(thr, sub->task_head);
  }
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  /* fprintf(stderr, "nsub %d\n", thr->sub); */
  pthread_mutex_unlock(&thr->mut);
}


/*    ****************************************    */


void outp_int(int n){ printf(" %d", n); }
int  inp_int(){ int t; scanf(" %ld", &t); return t; }


/* app */

double
elapsed_time(struct timeval tp[2])
{
  return tp[1].tv_sec-tp[0].tv_sec+1e-6*(tp[1].tv_usec-tp[0].tv_usec);
}


/*
  (0,0)     ...   (0, nx-1)
  (1,0)     ...   (1, nx-1)
   ...             ...
  (ny-1,0)  ...   (ny-1, nx-1)
*/

/* eventually, may use different nx(s) */

void decomp_lu(FF, int n, double *a, int nx);
void decomp_r(FF, int n1, int n2, double *b, double *a, int nx);
void decomp_d(FF, int n1, int n2, double *b, double *a, int nx);
void decomp_rd(FF, int n1, int n2, int n3, double *b1, double *b2, 
	       double *a, int nx);


typedef enum { TASK_CMD, TASK_LU, TASK_R, TASK_D, TASK_RD } task_tp;

struct task_lu {
  task_tp tt;
  int n1, n2, n3;
  double *b1, *b2, *a;
  int nx;
};

void output_task(task_tp tt, int n1, int n2, int n3,
		 double *b1, double *b2, double *a, int nx);
void input_rslt(task_tp tt, int n1, int n2, int n3,
		double *b1, double *b2, double *a, int nx);

void put_task(struct task_lu *st, task_tp tt, int n1, int n2, int n3,
	      double *b1, double *b2, double *a, int nx){
  st->tt = tt; st->n1 = n1; st->n2 = n2;  st->n3 = n3; 
  st->b1 = b1; st->b2 = b2; st->a = a; st->nx = nx;
}

void merge_task(struct task_lu *x, double *a){
  int n2=x->n2, n3=x->n3, nx=x->nx;
  double *tmp=x->a;
  int i, j;
  for(i=0;i<n2;i++)
    for(j=0;j<n3;j++)
      a[i*nx+j] += tmp[i*n3+j];
  free(tmp);
}

void
decomp_lu_0(int n, double *a, int nx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"lu_0: %d %d\n", n, a);
#endif
  {
    double w = 1.0/a[0];
    for(j=1;j<n;j++) a[j] *= w;
  }
  for(i=1;i<n;i++)
    {
      for(k=0;k<i-1;k++)
	{
	  double aik = a[i*nx+k];
	  for(j=k+1;j<n;j++) a[i*nx+j] -= aik * a[k*nx+j];
	}
      {
	double aik = a[i*nx+i-1];
	double w;
	a[i*nx+i] -= aik * a[(i-1)*nx+i];
	w = 1.0/a[i*nx+i];
	for(j=i+1;j<n;j++) a[i*nx+j] = w * (a[i*nx+j] - aik * a[(i-1)*nx+j]);
      }
    }
}

void
decomp_lu(FF, int n, double *a, int nx)
{
  struct timeval tp[2];
  
  if(n <= 4)
    {
      decomp_lu_0(n, a, nx);
      return;
    }
  {
    /* int n1 = (n/4), n2 = n-n1; */
    /* int n1 = (n>8)?(n/4):(n/2), n2 = n-n1; */
    int n1 = (n>16)?(n/4):(n/2), n2 = n-n1;
    decomp_lu(FA, n1, a, nx);
    // seq
    DO_TWO(struct task_lu, st, 
	   decomp_r(FA, n1, n2, a, a+n1, nx),
	   decomp_d(FA, n1, n2, a, a+(n1*nx), nx),
	   put_task(st, TASK_D, n1, n2, 0, a, 0, a+(n1*nx), nx),
	   0);
    // output_task(TASK_D, n1, n2, 0, a, 0, a+(n1*nx), nx)
    // input_rslt(TASK_D, n1, n2, 0, a, 0, a+(n1*nx), nx)

    // seq
    decomp_rd(FA, n1, n2, n2, a+(n1*nx), a+n1, a+((n1*nx)+n1), nx);
    // seq
    decomp_lu(FA, n2, a+((n1*nx)+n1), nx);
  }
}


void
decomp_r_0(int n1, int n2, double *b, double *a, int nx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"r_0: %d %d %d %d\n", n1, n2, b, a);
#endif
  {
    double w = 1.0/b[0];
    for(j=0;j<n2;j++) a[j] *= w;
  }
  for(i=1;i<n1;i++)
    {
      for(k=0;k<i-1;k++)
	{
	  double aik = b[i*nx+k];
	  for(j=0;j<n2;j++) a[i*nx+j] -= aik * a[k*nx+j];
	}
      {
	double aik = b[i*nx+i-1];
	double w = 1.0/b[i*nx+i];
	for(j=0;j<n2;j++) a[i*nx+j] = w * (a[i*nx+j] - aik * a[(i-1)*nx+j]);
      }
    }


}

void
decomp_r(FF, int n1, int n2, double *b, double *a, int nx)
{
  if(n1 <= 4)
    {
      decomp_r_0(n1, n2, b, a, nx);
      return;
    }
  {
    int n1_1 = n1/2, n1_2 = n1-n1_1;
    decomp_r(FA, n1_1, n2, b, a, nx);
    // seq
    decomp_rd(FA, n1_1, n1_2, n2, b+(n1_1*nx), a, a+(n1_1*nx), nx);
    // seq
    decomp_r(FA, n1_2, n2, b+(n1_1*nx+n1_1), a+(n1_1*nx), nx);
  }
}

void
decomp_d_0(int n1, int n2, double *b, double *a, int nx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"d_0: %d %d %d %d\n", n1, n2, b, a);
#endif
  for(i=0;i<n2;i++)
    {
      for(k=0;k<n1-1;k++)
	{
	  double aik = a[i*nx+k];
	  for(j=k+1;j<n1;j++) a[i*nx+j] -= aik * b[k*nx+j];
	}

    }
}

void
decomp_d(FF, int n1, int n2, double *b, double *a, int nx)
{
  if(n2 <= 4)
    {
      decomp_d_0(n1, n2, b, a, nx);
      return;
    }
  {
    int n2_1 = n2/2, n2_2 = n2-n2_1;
    DO_TWO(struct task_lu, st, decomp_d(FA, n1, n2_1, b, a, nx),
	   decomp_d(FA, n1, n2_2, b, a+(n2_1*nx), nx),
	   put_task(st, TASK_D, n1, n2_2, 0, b, 0, a+(n2_1*nx), nx),
	   0);
    // output_task(TASK_D, n1, n2_2, 0, b, 0, a+(n2_1*nx), nx),
    // input_rslt(TASK_D, n1, n2_2, 0, b, 0, a+(n2_1*nx), nx)
  }
}

void
decomp_rd_0(int n1, int n2, int n3, 
	    double *b1, double *b2, double *a, int nx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"rd_0: %d %d %d %d %d %d\n", n1, n2, n3, b1, b2, a);
#endif
  for(i=0;i<n2;i++)
    for(k=0;k<n1;k++)
      {
	double aik = b1[i*nx+k];
	for(j=0;j<n3;j++)
	  a[i*nx+j] -= aik * b2[k*nx+j];
      }
}

void
decomp_rd(FF, int n1, int n2, int n3, 
	  double *b1, double *b2, double *a, int nx)
{
  if(n1 <= 4 && n2 <= 4)
    {
      decomp_rd_0(n1, n2, n3, b1, b2, a, nx);
      return;
    }
  if(n1 > n2)
    {
      int n1_1 = n1/2, n1_2 = n1 - n1_1;
      DO_TWO(struct task_lu, st,
	     decomp_rd(FA, n1_1, n2, n3, b1, b2, a, nx),
	     decomp_rd(FA, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx),
	     put_task(st, TASK_RD, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), 0, nx),
	     merge_task(st, a));
      // output_task(TASK_RD, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx),
      // input_rslt(TASK_RD, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx)
    }
  else
    {
      int n2_1 = n2/2, n2_2 = n2 - n2_1;
      DO_TWO(struct task_lu, st,
	     decomp_rd(FA, n1, n2_1, n3, b1, b2, a, nx),
	     decomp_rd(FA, n1, n2_2, n3, b1+(n2_1*nx), b2, a+(n2_1*nx), nx),
	     put_task(st, TASK_RD, n1, n2_2, n3, 
		      b1+(n2_1*nx), b2, 0, nx),
	     merge_task(st, a+(n2_1*nx)));
      // output_task(TASK_RD, n1, n2_2, n3, 
      //    b1+(n2_1*nx), b2, a+(n2_1*nx), nx),
      // input_rslt(TASK_RD, n1, n2_2, n3, 
      //    b1+(n2_1*nx), b2, a+(n2_1*nx), nx)
    }
}


void
decomp_lu_1(FF, int n, double *a, int nx)
{
  int i;
  for(i=0;i<n;i+=16){
    int n1 = 16;
    if (n1 > n-i) n1 = n-i;
    decomp_d_0(i, n1, a, a+(i*nx), nx);
    decomp_rd(FA, i, n1, n-i, a+(i*nx), a+i, a+(i*nx+i), nx);
    decomp_lu_0(n1, a+(i*nx+i), nx);
    decomp_r_0(n1, n-i-n1, a+(i*nx+i), a+(i*nx+i+n1), nx);
  }
}

void
genmat(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      a[i*nx+j] = n - abs(i-j);
}

void
printmat(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      fprintf(stderr,"%6.3lf%c", a[i*nx+j], (j==n-1) ? '\n' : ' ');
  putc('\n', stderr);
}

void
copy_l(int n, double *a, double *l, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      l[i*nx+j] = (i<j) ? 0.0 : a[i*nx+j];
}

void
copy_u(int n, double *a, double *u, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      u[i*nx+j] = (i==j) ? 1.0 : (i>j) ? 0.0 : a[i*nx+j];
}

void
transpose(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++) 
    for(j=0;j<i;j++)
      {
	double t1 = a[i*nx+j], t2 = a[j*nx+i];
	a[i*nx+j] = t2, a[j*nx+i] = t1;
      }
}

void
matmul(int n, double *a, double *b, double *c, int nx)
{
  int bs=12;
  int i0,j0,i,j,k;
  transpose(n, b, nx);
  for(i0=0;i0<n;i0+=bs)
    for(j0=0;j0<n;j0+=bs)
      for(i=i0;i<i0+bs && i<n;i++)
	for(j=j0;j<j0+bs && j<n;j++)
	  {
	    double s = 0.0;
	    for(k=0;k<n;k++) s += a[i*nx+k] * b[j*nx+k];
	    c[i*nx+j] = s;
	  }
}

void
diffmat(int n, double *a, double *b, int nx)
{
  double s=0.0;
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      s += (a[i*nx+j]-b[i*nx+j])*(a[i*nx+j]-b[i*nx+j]);
  fprintf(stderr,"diff: %lf\n", s);
}

/* */


void swap_doubles(double *a, int n){
  /* assuming that long is 32-bit, and double is 64-bit */
  int i;
  /* fprintf(stderr, "swapping...\n"); */
  for(i=0;i<n;i++){
    unsigned long *xp = (unsigned long *)(a+i);
    unsigned long x0 = xp[0], x1 = xp[1];
    unsigned long y0 = x0, y1 = x1;
    y0 = (y0 << 8) | ((x0 >>= 8) & 0xff);
    y0 = (y0 << 8) | ((x0 >>= 8) & 0xff);
    y0 = (y0 << 8) | ((x0 >> 8));
    y1 = (y1 << 8) | ((x1 >>= 8) & 0xff);
    y1 = (y1 << 8) | ((x1 >>= 8) & 0xff);
    y1 = (y1 << 8) | ((x1 >> 8));
    xp[1] = y0; xp[0] = y1;
  }
  /* fprintf(stderr, "swapping...done\n"); */
}

void input_mat(double *a, int n1, int n2, int nx){
  int i, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();
  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz < n2)
      sz += fread(a+(i*nx+sz), sizeof(double), n2-sz, stdin);
  }
  scanf(" )");
  if((int)x.c[0] != e)
    for(i=0;i<n1;i++)
      swap_doubles(a+(i*nx), n2);
}


void output_mat(double *a, int n1, int n2, int nx){
  int i;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  printf(" (\n %d %d %d\n", 
	 sizeof(double)*n1*n2, sizeof(double), (int)x.c[0]);
  for(i=0;i<n1;i++)
    {
      size_t sz = 0;
      while(sz < n2)
	sz += fwrite(a+(i*nx+sz), sizeof(double), n2-sz, stdout);
    }
  printf(" )");
}

void input_mat_l(double *a, int n1, int nx){
  int i, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();
  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz <= i)
      sz += fread(a+(i*nx+sz), sizeof(double), (i+1)-sz, stdin);
  }
  scanf(" )");
  if((int)x.c[0] != e)
    for(i=0;i<n1;i++)
      swap_doubles(a+(i*nx), i+1);
}

void output_mat_l(double *a, int n1, int nx){
  int i;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  printf(" (\n %d %d %d\n", 
	 sizeof(double)*n1*(n1+1)/2, sizeof(double), (int)x.c[0]);
  for(i=0;i<n1;i++)
    {
      size_t sz = 0;
      while(sz <= i)
	sz += fwrite(a+(i*nx+sz), sizeof(double), (i+1)-sz, stdout);
    }
  printf(" )");
}

void input_mat_u(double *a, int n1, int nx){
  int i, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;

  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();

  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz < n1-i-1)
      sz += fread(a+(i*nx+i+1+sz), sizeof(double), (n1-i-1)-sz, stdin);
  }
  scanf(" )");
  if((int)x.c[0] != e)
    for(i=0;i<n1;i++)
      swap_doubles(a+(i*nx+i+1), n1-i-1);
}

void output_mat_u(double *a, int n1, int nx){
  int i;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  printf(" (\n %d %d %d\n", 
	 sizeof(double)*n1*(n1-1)/2, sizeof(double), (int)x.c[0]);
  for(i=0;i<n1;i++)
    {
      size_t sz = 0;
      while(sz < n1-i-1)
	sz += fwrite(a+(i*nx+i+1+sz), sizeof(double), (n1-i-1)-sz, stdout);
    }
  printf(" )");
}

#if 0 
void input_merge_mat(double *a, int n1, int n2, int nx){
  int i, j, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();
  double *tmp = (double *)malloc(sizeof(double)*n2);
  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz < n2)
      sz += fread(tmp+sz, sizeof(double), n2-sz, stdin);
    if((int)x.c[0] != e)
      swap_doubles(tmp, n2);
    for(j=0;j<n2;j++)
      a[i*nx+j] += tmp[j];
  }
  scanf(" )");
  free(tmp);
}
#endif

void zero_mat(double *a, int n1, int n2, int nx){
  int i, j;
  for(i=0;i<n1;i++)
    for(j=0;j<n2;j++)
      a[i*nx+j] = 0.0;
}

void send_task_body(struct thread_data *thr, void *x0){
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int nx=x->nx;
  outp_int(x->tt); 
  switch(x->tt){
  case TASK_LU: 
    outp_int(n1);
    output_mat(a, n1, n1, nx);
    break;
  case TASK_R:
    outp_int(n1);
    outp_int(n2);
    output_mat_l(b1, n1, nx);
    output_mat(a, n1, n2, nx);
    break;
  case TASK_D:
    outp_int(n1);
    outp_int(n2);
    output_mat_u(b1, n1, nx);
    output_mat(a, n2, n1, nx);
    break;
  case TASK_RD:
    outp_int(n1);
    outp_int(n2);
    outp_int(n3);
    output_mat(b1, n2, n1, nx);
    output_mat(b2, n1, n3, nx);
    break;
  }
}

void *recv_task_body(struct thread_data *thr){
  struct task_lu *x = (struct task_lu *) malloc(sizeof(struct task_lu));
  task_tp tt; int n1; int n2; int n3;
  double *b1; double *b2; double *a; int nx;
  static double ans;
  tt = inp_int(); 
  switch(tt){
  case TASK_CMD:
    n1 = inp_int();
    n2 = inp_int();
    n3 = inp_int();
    a = &ans;
    break;
  case TASK_LU: 
    n1 = inp_int();
    nx = n1;
    a = (double *)malloc(sizeof(double)*n1*n1);
    input_mat(a, n1, n1, nx);
    break;
  case TASK_R:
    n1 = inp_int();
    n2 = inp_int();
    nx = n1 + n2;
    b1 = (double *)malloc(sizeof(double)*n1*nx);
    input_mat_l(b1, n1, nx);
    a = b1 + n1;
    input_mat(a, n1, n2, nx);
    break;
  case TASK_D:
    n1 = inp_int();
    n2 = inp_int();
    nx = n1;
    b1 = (double *)malloc(sizeof(double)*(n1+n2)*nx);
    input_mat_u(b1, n1, nx);
    a = b1 + (n1*nx);
    input_mat(a, n2, n1, nx);
    break;
  case TASK_RD:
    n1 = inp_int();
    n2 = inp_int();
    n3 = inp_int();
    nx = n1 + n3;
    a = (double *)malloc(sizeof(double)*(n1+n2)*nx);
    b1 = a + (n1*nx);
    b2 = a + n1;
    a += ((n1*nx)+n1);
    input_mat(b1, n2, n1, nx);
    input_mat(b2, n1, n3, nx);
    zero_mat(a, n2, n3, nx);
    break;
  }
  x->tt = tt; x->n1 = n1; x->n2 = n2; x->n3 = n3;
  x->b1 = b1; x->b2 = b2; x->a = a;
  x->nx = nx;
  return x;
}

void proc_rslt(double *);

void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int nx=x->nx;
  switch(x->tt){
  case TASK_CMD:
    proc_rslt(a);
    break;
  case TASK_LU:
    output_mat(a, n1, n1, nx);
    free(a);
    break;
  case TASK_R:
    output_mat(a, n1, n2, nx);
    free(b1);
    break;
  case TASK_D:
    output_mat(a, n2, n1, nx);
    free(b1);
    break;
  case TASK_RD:
    output_mat(a, n2, n3, nx);
    a -= ((n1*nx)+n1);
    free(a);
    break;
  }
  free(x);
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int nx=x->nx;
  switch(x->tt){
  case TASK_LU:
    input_mat(a, n1, n1, nx);
    break;
  case TASK_R:
    input_mat(a, n1, n2, nx);
    break;
  case TASK_D:
    input_mat(a, n2, n1, nx);
    break;
  case TASK_RD:
    x->a = (double *)malloc(sizeof(double)*n2*n3);
    input_mat(x->a, n2, n3, n3);
    break;
  }
}

void proc_cmd(FF, int, int, int, double *);

void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int nx=x->nx;
  fprintf(stderr, "start %d\n", x->tt);

  switch(x->tt){
  case TASK_CMD: proc_cmd(FA, n1, n2, n3, a); break;
  case TASK_LU:  decomp_lu(FA, n1, a, nx); break;
  case TASK_R:   decomp_r(FA, n1, n2, b1, a, nx); break;
  case TASK_D:   decomp_d(FA, n1, n2, b1, a, nx); break;
  case TASK_RD:  decomp_rd(FA, n1, n2, n3, b1, b2, a, nx); break;
  }
}

void proc_cmd(FF, int n1, int n2, int n3, double *ansp){
  struct timeval tp[2];
  int n = n1, al = n2, d = n3, nx;
  double *a, *l, *u, *c;
  nx = n;
  a = (double *)malloc(sizeof(double)*n*n);
  genmat(n, a, nx);
  if(d>1) printmat(n, a, nx);

  gettimeofday(tp, 0);
  switch(al){
  case 1: decomp_lu_0(n, a, nx); break;
  case 2: decomp_lu(FA, n, a, nx); break;
  case 3: decomp_lu_1(FA, n, a, nx); break;
  }
  gettimeofday(tp+1, 0);
  *ansp = elapsed_time(tp);
  fprintf(stderr, "%lf\n", *ansp);
  if(d>0){
    l = (double *)malloc(sizeof(double)*n*n);
    u = (double *)malloc(sizeof(double)*n*n);
    c = (double *)malloc(sizeof(double)*n*n);
    if(d>1)printmat(n, a, nx);
    copy_l(n, a, l, nx);
    copy_u(n, a, u, nx);
    if(d>1)printmat(n, l, nx);
    if(d>1)printmat(n, u, nx);
    matmul(n, l, u, c, nx);
    if(d>1)printmat(n, c, nx);
    genmat(n, a, nx);
    diffmat(n, a, c, nx);
  }
}

void proc_rslt(double *a){ printf("%lf", *a); }
