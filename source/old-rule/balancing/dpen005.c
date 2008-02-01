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

#define DO_MANY_BEGIN(type1, struct1,			\
inum1, o_inum1, o_inum2, n_inum1, n_inum2)		\
do{							\
  int inum1 = (o_inum1);				\
  int einum1 = (o_inum2);				\
  type1 *struct1;					\
  int spawned = 0;					\
  int (*_bk2)() = _bk;					\
  int _bk(){						\
    if(!spawned)					\
      _bk2();						\
    while(inum1 + 1 < einum1 && _thr->treq_top){	\
      int n_inum2 = einum1;				\
      int n_inum1 = ((inum1+1)+n_inum2)/2;		\
      einum1 = n_inum1;					\
      struct1 = (type1 *)malloc(sizeof (type1));

#define DO_MANY_BODY(struct1, inum1)		\
      spawned++;				\
      make_and_send_task(_thr, struct1);	\
    }						\
    return 0;					\
  }						\
  if(_thr->req)					\
    handle_req(FA);				\
  for (; inum1 < einum1; inum1++)

#define DO_MANY_FINISH(struct1)			\
  while(spawned-- > 0){				\
    struct1 = wait_rslt(_thr);

#define DO_MANY_END(struct1)			\
    free(struct1);				\
  }						\
}while(0)

#define DO_INI_FIN(init1, body1, fin1)		\
do{						\
  { init1 ; }					\
  {						\
    int (*_bk2)() = _bk;			\
    int _bk(){					\
      { fin1 ; }				\
      _bk2();					\
      { init1 ; }				\
      return 0;					\
    }						\
    { body1 ; }					\
  }						\
  { fin1 ; }					\
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

void *wait_rslt(struct thread_data *thr){
  void *body;
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
  body = sub->body;
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  /* fprintf(stderr, "nsub %d\n", thr->sub); */
  pthread_mutex_unlock(&thr->mut);
  return body;
}




/* 共通? */

void send_int(int n){ fprintf(stdout, " %d", n); }
void recv_int(int *n){ fscanf(stdin, " %ld", n); }

/* ここから上をライブラリにする */


/* ここから下だけを app では書く */

struct task_pen {
  int r; 
  int k;
  int i0, i1, i2;
  int a[12];
  int b[70];
};

void send_task_body(struct thread_data *thr, void *x0){
  struct task_pen *x = x0;
  int i;
  send_int(x->k);
  if(x->k < 0) return;
  send_int(x->i0);
  send_int(x->i1);
  send_int(x->i2);
  for(i=0;i < 12; i++)
    send_int(x->a[i]);
  for(i=0;i < 70; i++)
    send_int(x->b[i]);
}

void *recv_task_body(struct thread_data *thr){
  int i;
  struct task_pen *x = (struct task_pen *) malloc(sizeof(struct task_pen));
  recv_int(&x->k);
  if(x->k < 0) return x;
  recv_int(&x->i0);
  recv_int(&x->i1);
  recv_int(&x->i2);
  for(i=0;i < 12; i++)
    recv_int(&x->a[i]);
  for(i=0;i < 70; i++)
    recv_int(&x->b[i]);
  return x;
}

void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_pen *x = x0;
  send_int(x->r);
  free(x);
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_pen *x = x0;
  int r;
  recv_int(&x->r);
}

int ps[63][5] = {
  {1,1,1,1},{7,7,7,7},
  {7,1,1,1},{4,1,1,1},{1,1,1,4},{1,1,1,7},
  {7,7,7,1},{7,7,6,1},{1,6,7,7},{1,7,7,7},
  {6,1,1,1},{5,1,1,1},{1,1,1,5},{1,1,1,6},
  {7,1,6,7},{7,7,1,6},{7,6,1,7},{6,1,7,7},
  {7,7,1,1},{1,1,5,7},{1,1,7,7},{7,5,1,1},
  {2,5,1,1},{1,1,5,2},{1,6,7,1},{1,7,6,1},
  {7,1,1,5},{5,1,1,7},{7,6,1,1},{1,1,6,7},
  {7,1,1,6},{5,1,1,6},{6,1,1,5},{6,1,1,7},
  {1,7,1,6},{7,1,5,1},{1,5,1,7},{6,1,7,1},
  {7,1,1,7},{5,1,1,5},{1,7,7,1},{1,6,6,1},
  {6,1,1,6},
  {1,6,1,1},{1,1,5,1},{1,5,1,1},{1,1,6,1},
  {7,1,6,1},{1,6,1,6},{6,1,6,1},{1,6,1,7},
  {1,4,1,1},{1,1,7,1},{1,7,1,1},{1,1,4,1},
  {7,1,7,7},{7,6,1,6},{6,1,6,7},{7,7,1,7},
  {7,1,7,1},{1,5,1,6},{1,7,1,7},{6,1,5,1}
};

int pos[13] = { 0, 2, 10, 18, 22, 26, 30, 38, 42, 43, 51, 59, 63};

int 
search(FF, int k, int j0, int j1, int j2, struct task_pen *tsk)
{
  int s = 0;
  DO_MANY_BEGIN(struct task_pen, st, p, j1, j2, i1, i2){
    *st = *tsk;
    st->k = k;
    st->i0 = j0;
    st->i1 = i1;
    st->i2 = i2;
  }
  DO_MANY_BODY(st, p){
    int ap = tsk->a[p], i;
    for(i = pos[ap]; i < pos[ap+1]; i++)
      /* examine the "i"-th (piece, direction)
	 at the first empty location "k" */
      {
	int *pss = ps[i];
	int kk=k,l;
	/* room available? */
	for(l=0;l<4;l++) if((kk += pss[l]) >= 70 || tsk->b[kk] !=0) goto Ln;

	DO_INI_FIN({
	  tsk->b[kk=k] = p+'A'; for(l=0;l<4;l++) tsk->b[kk += pss[l]] = p+'A';
	  tsk->a[p] = tsk->a[j0]; tsk->a[j0] = ap;
	},{ 
	  for(kk=k; kk<70; kk++) if( tsk->b[kk] == 0 ) break;
	  if(kk == 70)
	    s += 1;
	  else if((kk+7 >= 70 || tsk->b[kk+7] != 0) && 
		  (tsk->b[kk+1] != 0 || 
		   (kk+8 >= 70 || tsk->b[kk+8] != 0) && tsk->b[kk+2] != 0))
	    ;
	  else
	    s += search(FA, kk, j0 + 1, j0 + 1, 12, tsk);
	},{
	  ap = tsk->a[j0]; tsk->a[j0] = tsk->a[p]; tsk->a[p] = ap; 
	  tsk->b[kk=k] = 0; for(l=0;l<4;l++) tsk->b[kk += pss[l]] = 0;
	});
      Ln:
	continue;
      }
  }
  DO_MANY_FINISH(st){
    s += st->r;
  }
  DO_MANY_END(st);
  return s;
}

void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_pen *x = x0;
  int i;
  int k = x->k;
  int i0 = x->i0;
  int i1 = x->i1;
  int i2 = x->i2;
  fprintf(stderr, "start %d %d %d %d\n", k, i0, i1, i2);
  if(k < 0){
    for(i=0;i<12;i++) x->a[i] = i % 12;
    for(i=0;i<70;i++) x->b[i] = 0;
    for(i=6;i<70;i+=7) x->b[i] = '\n'; /* right side wall */
    x->r = search(FA, 0, 0, 0, 12, x);  
  }else{
    x->r = search(FA, k, i0, i1, i2, x);
    fprintf(stderr, "end %d\n", x->r);
  }
}
