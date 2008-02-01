#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define BUFSIZE 1024
#define MAXCMDC 16



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

void proto_error(const char *str){
  fputs(str, stderr);
  exit(1);
}

void mem_error(const char *str){
  fputs(str, stderr);
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
    if(c == ' ' || c = '\n')
      *b = c = 0;
    else if (p == 0 && r.c < MAXCMDC)
      r.v[r.c++] = b;
  return r;
}

pthread_mutex_t send_mut = PTHREAD_MUTEX_INITIALIZER;

/* struc cmd -> output */
struct cmd send_command(struct cmd cmd){
  int i;
  for(i=0; i<cmd.c-1; i++){
    fputs(cmd.v[i], stdout); fputc(' ', stdout);
  }
  if(cmd.c > 0){
    fputs(cmd.v[cmd.c-1], stdout); fputc('\n', stdout);
  }
}

void read_to_eol(){
  int c;
  while((c = getc(stdin)) != EOF) if (c == '\n') break;
}
void write_eol(){ putc('\n', stdout); }
viod flush_send(){ fflush(stdout); }

enum task_stat {
  TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE
};

enum task_home_stat {
  TASK_HOME_STARTED, TASK_HOME_DONE
};

#define FF int (*_bk)(), struct task *_x
#define FA _bk, _x
#define TASK_BODY(tp) (*((TP *)_x->body))

/* 未変更  まだ SP を自分でやるようになっていない */
#define DO_TWO(work1, work2, output_work2, input_work2)			\
do{									\
  int spawned = 0;							\
  {									\
    int (*_bk2)() = _bk;						\
    int _bk(){								\
      if(_bk2()) return 1;						\
      if(!spawned){							\
	spawned = 1;							\
	puts("task");							\
	(output_work2);							\
	putchar('\n');							\
	fflush(stdout);							\
	return 1;							\
      }									\
      return 0;								\
    }									\
    if(req){ req = 0; if(!_bk()){ printf("none\n"); fflush(stdout); }}	\
    (work1);								\
  }									\
  if(spawned){								\
    char *buf;								\
    printf("rqst\n"); fflush(stdout);					\
    while((buf= recv_command(), strncmp(buf,"rslt",4) != 0)){		\
      if(strncmp(buf,"task",4) == 0)					\
	read_exec_print(1, 0);						\
      else if(strncmp(buf,"subt",4) == 0)				\
	read_exec_print(1, 1);						\
    }									\
    (input_work2); read_to_eol();					\
  } else {								\
    (work2);								\
  }									\
}while(0)

struct thread_data;

struct task {
  volatile int req;
  enum task_stat stat;
  struct task* next;
  struct task* prev;
  struct thread_data *thr;
  void *body;
  int ndiv;
  char rslt_head[256];
};

struct task_home {
  // pthread_mutex_t mut;
  enum task_home_stat stat;
  int id;
  struct task_home* next;
  void *body;
  int ndiv;
};

struct thread_data {
  int id;
  char id_str[32];
  struct task_home* sub;
  struct task *task_top;
  pthread_mutex_t mut;
  pthread_cond_t cond;
  char buf[BUFSIZE];
};

struct thread_data thread_data[64];

/*
  リモートに treq してもらった task (copy)
    - task_root からのリストとなり，id は重複しないように付ける．
      (freeリストに戻すとき，id をそのままに，再利用すればよい?)
    - treq まえに allocate
    x ときどき，リストから DONE となったものを除く
      (それなら，rack を得るまで DONE にしないようにする
       active なスレッド数は別管理すればよい?)
    -  ときどきでなくて，rack を得たときでよい?

  分割して作った task (home/orig) => task_home
    x 分割元task の sub からのリストとなり，id は重複しないように付ける．
      (分割元taskは，リモートに treq してもらった task (copy) のみ)
    o thread_data の sub からのリストとなり，id は重複しないように付ける．
    - 最初から，STARTED，すぐに treq 元に送る．
    - rslt がきたら，DONE にして，rack を返す
    - DONE になっていたら，分割元task がマージして消去
 */



/*
  task の処理完了後は，その task_home に send_rslt する
  ?? task_home をそのまま同じノードで処理するケースもあるのか??

  task_home の初期化後は req 元 に send_task するのか?
 */
/*
    treq <task_head> <treq_head>
      <task_head>  タスク返答先
      <treq_head>  要求送信先

    task <ndiv> <rslt_head> <task_head>
      <ndiv>       分割回数 負荷のサイズの目安 (sp2が子の判断に使う)
      <rslt_head>  結果返答先
      <task_head>  タスク送信先

    rslt <rack_head> <rslt_head>
      <rack_head>  rack返答先  ; <task_head> でよい
      <rslt_head>  結果送信先

    rack <rack_head>
      <rack_head>  rack送信先

    treq の送り先が特定のtask であるなら，その task がすでになく
    なっていたら，none をかえすとできるので，rack は不要?
    => No, task の id がもし同じなら区別できない...
    rack は必要だが，rack がもどってくるまで，その id の task 
    を消さなければ，id が同じになってしまうことはない．
    -> 結局は w_rack カウンタを使うべき
 */

/*
  (st.head.nextt = _x->head.subt),
  (st.head.sid = _x->head.sid + 1),
  (_x->head.subt = &st),
 */

void recv_exec_send(struct thread_data *thr, char *treq_head){
  struct cmd cmd;
  struct task *x;
  int bk(){ return 0; }
  /* 最初にxをとる

     タスクをとろうとしている間に，rslt が届いたら工夫するのか?
     単に，rslt 待ちについては，wait しないで，定期的にみる．
     みたときに，なければ，task 待ちにしてしまうというのが簡単?
     だって task 待ちにしたら treq をだすのだし．
     ここでは，task 待ちのする場合

     task については，treq 時点で場所を作っておく => ノード内並列にも

     同時スレッド数は超えないようには，別途セマフォで
  */
  pthread_mutex_lock(&thr->mut);
  x = thr->task_top = thr->task_top->prev;
  x->stat = TASK_ALLOCATED;
  if(!x)
    mem_error("not enough task memory");
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
  } while(x->stat != TASK_INITIALIZED);
  x->stat = TASK_STARTED;
  pthread_mutex_unlock(&thr->mut);

  /* ここに セマフォ? */

  do_work(bk, x);
  /* rslt */

  /* 以下で thr の lock がいるか? */
  rcmd.c = 3;
  rcmd.v[0] = "rslt";
  rcmd.v[1] = thr->id_str;
  rcmd.v[2] = x->rslt_head;
  pthread_mutex_lock(&send_mut);
  send_command(rcmd);

  send_rslt_body(x->body);
  /* x->body.h.class->send_rslt_body(x->body); */

  write_eol();
  flush_send();
  pthread_mutex_unlock(&send_mut);
}

void *worker(void *arg){
  for(;;)
    recv_exec_send(arg, "any");
}

unsinged int num_thrs = 1;

void recv_task(struct cmd cmd){
  struct task *tx;
  struct thread_data *thr;
  unsinged int id;
  size_t len;
  if(cmd.c < 4)
    proto_error("wrong task");
  /* id を <task_head> に含める */
  id = atoi(cmd.v[3]);
  if(! id<num_thrs)
    proto_error("wrong task_head");
  thr=thread_data+id;
  pthread_mutex_lock(&thr->mut);
  tx = thr->task_top;
  tx->req = 0;
  len = strlen(cmd.v[2])
  if(len > 254)
    proto_error("too long rslt_head for task");
  strncpy(tx->rslt_head, cmd.v[2], len+1);
  tx->ndiv = atoi(cmd.v[1]);
  tx->body = recv_task_body();
  read_to_eol();
  /* task を受け取った後，ノード内に待っているスレッド(ワーカ)を起こす */
  tx->stat = TASK_INITIALIZED;
  /* スレッド数の上限を超えるなら，後で(どれかのスレッドが終わるとき)
     signal するべきか?
     それよりは，別にセマフォで上限を管理するほうがら楽では?  */
  pthread_cond_broadcast(&thr->cond); /* pthread_cond_signal? */
  pthread_mutex_unlock(&thr->mut);
}

/* 以下はまだ，task から thread への変更が終わっていない */
void recv_rslt(struct cmd cmd){
  struct cmd rcmd;
  struct task *tx;
  struct task_home *hx;
  int tid, sid;
  char *b;
  if(cmd.c < 3)
    proto_error("wrong rslt");
  b = cmd.v[2]
  tid = atoi(b);
  b = strchr(b, ':');
  if(!b)
    proto_error("wrong rslt_head");
  sid = atoi(b);
  pthread_mutex_lock(&task_mut);
  tx = task_root;
  pthread_mutex_unlock(&task_mut);
  /* id を <task_head> に含める */
  while(tx && tx->id != tid)
    tx = tx->next;
  if(!tx)
    proto_error("wrong rslt_head");
  hx = tx->sub;
  while(hx && hx->id != sid)
    hx = hx->next;
  if(!hx)
    proto_error("wrong rslt_head");
  recv_rslt_body(hx->body);
  read_to_eol();
  /* mut は task_mut にしてもよい? */
  pthread_mutex_lock(&hx->mut);
  hx->stat = TASK_HOME_DONE;
  pthread_mutex_unlock(&hx->mut);
  /* rack を返す もっと後のほうがよい? */
  rcmd.c = 2;
  rcmd.v[0] = "rack"; rcmd.v[1] = cmd.v[1];
  pthread_mutex_lock(&send_mut);
  send_command(rcmd);
  flush_send();
  pthread_mutex_unlock(&send_mut);
}

void recv_treq(struct cmd cmd){
  struct task *tx;
  int id;
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


}

main(int argc, char *argv[]){
  struct cmd cmd;

  /* ここで thread_data の初期化, task の 双方向list も */

  /* ここでワーカをfork 
     同じノード内で(共有メモリで)でもやりくりできるか?  */
  systhr_create(worker, thread_data+0);
  
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
      proto_error("wrong cmd");
    }
  exit(0);
}

/* 共通? */

void send_int(int n){ printf(" %d", n); }
void recv_int(int *n){ scanf(" %ld", n); }

#define TASK_COMMON  struct { int req; int task_type; struct task *t; } h; 

struct task_common {
  TASK_COMMON
};

/* ここから下だけを app では書く */

/* 
  (def (task task-fib)
    (def n int)
    (def r int)
    (def (send-task) n) ?
    (def (send-rslt) r) ?
    (def (recv-rslt) r) ?

  (def (task task-fib)
    (def n int :in)
    (def r int :out))
 */

struct task_fib {
  TASK_COMMON
  int n;
  int r;
};

void *recv_task_body(){
  struct task_fib *x = (struct task_fib *) malloc(sizeof(struct task_fib));
  recv_int(&x->n);
  return x;
}

void send_rslt_body(void *x0){
  struct task_fib *x = x0;
  send_int(x->r);
}

void recv_rslt_body(void *x0){
  struct task_fib *x = x0;
  recv_int(&x->r);
}

void send_task_body(void *x0){
  struct task_fib *x = x0;
  send_int(x->n);
}

/*
  (def-WF (fib n) (fn int int) ....)
  (def (fib n) (w-fn int int) ....)

  (def (fib _bk _x n) (fn int (ptr (fn int)) (prt (struct task)) int) ....)
 */

int fib(FF, int n){
  if(n <= 2){
    return 1;
  }else{
    int s1, s2;
    DO_TWO(struct task_fib, st,
           (s1 = fib(FA, n-1)),
	   (s2 = fib(FA, n-2)),
	   (st.n = n-2),
	   (s2 = st.r));
    return s1+s2;
  }
}

void do_work(FF)
{
  int n = TASK_BODY(struct task_fib).n;
  int r = fib(FA, n);
  TASK_BODY(struct task_fib).r = r;
}
