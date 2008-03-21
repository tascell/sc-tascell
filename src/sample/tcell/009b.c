#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#define BUFSIZE 1024
#define MAXCMDC 4

// #define TEST_PART
// #define TEST_MSG
// #define TEST_MSG_SEND
// #define TEST_MSG_LOCK_THREAD
// #define TEST_MSG_LOCK_SEND
// #define TEST_MSG_LOCK_QUEUE
// #define TEST_MSG_ALLOC
// #define PRINT_TIME

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

enum node {OUTSIDE, INSIDE, ANY};

struct cmd {
  int  c;
  enum node node;
  char *v[MAXCMDC];
};

struct cmd_list {
  struct cmd cmd;
  void *body;
  struct cmd_list *next;
};

struct cmd_list cmd_queue[512];
char cmd_v_buf[512][4][256];
struct cmd_list *cmd_in;
struct cmd_list *cmd_out;

char buf[BUFSIZE];

int divisibility_flag = 0;

/* input -> struc cmd */
struct cmd recv_command(){
  struct cmd r;
  char p, c, *b = buf;
  fgets(b, BUFSIZE, stdin);
  r.c = 0;
  r.node = OUTSIDE;
  for(p = 0, c = *b ; c ; p = c, c= *++b)
    if(c == ' ' || c == '\n')
      *b = c = 0;
    else if (p == 0 && r.c < MAXCMDC)
      r.v[r.c++] = b;
  return r;
}

void recv_rslt(struct cmd, void *);
void recv_task(struct cmd, void *);
void recv_treq(struct cmd);
void recv_rack(struct cmd);
void recv_none(struct cmd);

pthread_mutex_t send_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mut = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_q = PTHREAD_COND_INITIALIZER;

/* struc cmd -> output */
void send_command(struct cmd cmd){
  int i;
#ifdef TEST_MSG_SEND
      fprintf(stderr, "send %s\n", cmd.v[0]);
#endif
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

void enqueue_command(struct cmd cmd, void *body){
  int i;
  size_t len;
  struct cmd_list *q;

#ifdef TEST_MSG
  fprintf(stderr, "node=%d v[0]=%s enqueue_cmd\n",cmd.node, cmd.v[0]);  
#endif
  /* cmd_queue はlock済 */
  q = cmd_in;
  if (cmd_in->next == cmd_out){
    perror("cmd_queue overflow\n");
    exit(0);
  }
  cmd_in = cmd_in->next;
  (*q).cmd.c = cmd.c;
  (*q).cmd.node = cmd.node;
  for(i=0; i<cmd.c; i++){
    len = strlen(cmd.v[i]);
    if(len > 254)
      proto_error("too long cmd", cmd);
    strncpy(q->cmd.v[i], cmd.v[i], len+1);
  }
  q->body = body;
}

void *exec_queue_cmd(void *arg){
  struct cmd cmd;
  void *body;
  for(;;){
    pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "lock queue\n");
#endif
    while (cmd_in == cmd_out){
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "unlock wait queue\n");
#endif
      pthread_cond_wait(&cond_q, &queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "lock wait queue\n");
#endif
    }
    cmd = (*cmd_out).cmd;
    body = cmd_out->body;
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "unlock queue\n");
#endif
    pthread_mutex_unlock(&queue_mut);
#ifdef TEST_MSG
    fprintf(stderr, "node=%d v[0]=%s exec_queue_cmd\n",cmd.node, cmd.v[0]);
#endif
    if(strcmp(cmd.v[0],"task") == 0)
      recv_task(cmd, body);
    else if(strcmp(cmd.v[0],"rslt") == 0)
      recv_rslt(cmd, body);
    else if(strcmp(cmd.v[0],"treq") == 0)
      recv_treq(cmd);
    else if(strcmp(cmd.v[0],"none") == 0)
      recv_none(cmd);
    else if(strcmp(cmd.v[0],"rack") == 0)
      recv_rack(cmd);
    else {
      proto_error("wrong cmd", cmd);
      exit(0);
    }
    cmd_out = cmd_out->next;
  }
}

void read_to_eol(){
  int c;
  while((c = getc(stdin)) != EOF) if (c == '\n') break;
}
void write_eol(){ putc('\n', stdout); }

void flush_send(){
#ifdef TEST_MSG_SEND
  fprintf(stderr, "flush send\n");
#endif
  fflush(stdout);
}

pthread_mutex_t snr_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_snr = PTHREAD_COND_INITIALIZER;
struct cmd_list snr_queue[32];
char snr_v_buf[32][2][256];
struct cmd_list *snr_in;
struct cmd_list *snr_out;
/* snr: send none or rack */

void enqueue_snr(struct cmd cmd){
  int i;
  size_t len;
  struct cmd_list *q;

  q = snr_in;
  if (snr_in->next == snr_out){
    perror("snr_queue overflow\n");
    exit(0);
  }
  snr_in = snr_in->next;
  (*q).cmd.c = cmd.c;
  (*q).cmd.node = cmd.node;
  for(i=0; i<cmd.c; i++){
    len = strlen(cmd.v[i]);
    if(len > 254)
      proto_error("too long cmd", cmd);
    strncpy(q->cmd.v[i], cmd.v[i], len+1);
  }
}

void *send_none_rack(void *arg){
  struct cmd cmd;
  for(;;){
    pthread_mutex_lock(&snr_mut);
    while (snr_in == snr_out){
      pthread_cond_wait(&cond_snr, &snr_mut);
    }
    cmd = (*snr_out).cmd;
    pthread_mutex_unlock(&snr_mut);
    pthread_mutex_lock(&send_mut);
    send_command(cmd);
    flush_send();
    if (strcmp(cmd.v[0],"none") == 0) divisibility_flag = 1;
    pthread_mutex_unlock(&send_mut);
    snr_out = snr_out->next;
  }
}
    
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
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int id;
  enum node req_from;
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
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char ndiv_buf[32];
  char id_str[32];
  char buf[BUFSIZE];
};

struct thread_data thread_data[64];
unsigned int num_thrs = 1;

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
    rcmd.node = hx->req_from;
    rcmd.v[0] = "none";
    rcmd.v[1] = hx->task_head;
    if (rcmd.node == INSIDE){
      pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "lock queue id=%d\n", thr->id);
#endif
      enqueue_command(rcmd, NULL);
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "signal queue id=%d\n", thr->id);
      fprintf(stderr, "unlock queue id=%d\n", thr->id);
#endif
      pthread_cond_signal(&cond_q);
      pthread_mutex_unlock(&queue_mut);
    }
    else if(rcmd.node == OUTSIDE){
      send_command(rcmd);
      divisibility_flag = 1;
    }
    else{
      perror("invalid rcmd.node in flush_treq_with_none_1\n");
      fprintf(stderr, "%d\n", rcmd.node);
      exit(0);
    }
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
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "lock send id=%d\n", thr->id);
#endif
    flush_treq_with_none_1(thr);
    flush_send();
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "unlock send id=%d\n", thr->id);
#endif
    pthread_mutex_unlock(&send_mut);
  }
}

void recv_exec_send(struct thread_data *thr, char *treq_head, int req_to){
  /* thr->mut は lock 済 */
  struct cmd rcmd;
  struct task *tx;
  long delay;
  int old_ndiv;
#ifdef PRINT_TIME
  struct timeval tp[2];
#endif

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
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "unlock wait none in read_exec_send id=%d\n", thr->id);
#endif
    pthread_cond_wait(&thr->cond, &thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "lock after wait none in read_exec_send id=%d\n", thr->id);
#endif
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
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "unlock id=%d in recv_exec_send\n", thr->id);
#endif
    pthread_mutex_unlock(&thr->mut);
    rcmd.c = 3;
    if (num_thrs >1)
      rcmd.node = req_to;
    else
      rcmd.node = OUTSIDE;
    rcmd.v[0] = "treq";
    rcmd.v[1] = thr->id_str;
    rcmd.v[2] = treq_head;
    
    if (rcmd.node != OUTSIDE){
      pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "lock queue id=%d\n", thr->id);
#endif
      enqueue_command(rcmd, NULL);
#ifdef TEST_MSG_LOCK_QUEUE
      fprintf(stderr, "signal queue id=%d\n", thr->id);
      fprintf(stderr, "unlock queue id=%d\n", thr->id);
#endif      
      pthread_cond_signal(&cond_q);
      pthread_mutex_unlock(&queue_mut);
    }
    else{
      pthread_mutex_lock(&send_mut);
#ifdef TEST_MSG_LOCK_SEND
      fprintf(stderr, "lock send id=%d\n", thr->id);
#endif
      /* ↓のsend_commandで出力が詰まる(バッファがいっぱいになる)と
	 デッドロックの可能性がある?
	 ただし出力文字数は少ないので詰まる確率は低い */
      send_command(rcmd);
      flush_send();
      divisibility_flag = 1;
#ifdef TEST_MSG_LOCK_SEND
      fprintf(stderr, "unlock send id=%d\n", thr->id);
#endif
      pthread_mutex_unlock(&send_mut);
    }
    pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "lock id=%d in recv_exec_send\n",thr->id);
#endif  
    /* recv_task で初期化されるのを待つ */
    do{
      /* rslt が到着していたら 特別に先にさせる */
      if(tx->stat != TASK_INITIALIZED && thr->sub && thr->sub->stat == TASK_HOME_DONE){
	if(tx->stat == TASK_NONE)
	  goto Lnone;
	thr->w_none++;
	goto Lnone;
      }
      if (tx->stat != TASK_ALLOCATED) break;
#ifdef TEST_MSG_LOCK_THREAD
      fprintf(stderr, "unlock wait task init id=%d\n", thr->id);
#endif
      pthread_cond_wait(&thr->cond, &thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
      fprintf(stderr, "lock after wait task init id=%d\n", thr->id);
#endif
    } while(1);

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
#ifdef TEST_MSG_LOCK_THREAD
      fprintf(stderr, "unlock wait id=%d\n", thr->id);
#endif
      pthread_cond_timedwait(&thr->cond_r, &thr->mut, &t1);
#ifdef TEST_MSG_LOCK_THREAD
      fprintf(stderr, "lock id=%d\n", thr->id);
#endif
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
#ifdef TEST_MSG
  fprintf(stderr, "id=%d TASK_STARTED\n",thr->id);
#endif
  /* ここに セマフォ? */
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "unlock id=%d before do_task_body\n",thr->id);
#endif
  pthread_mutex_unlock(&thr->mut);
#ifdef PRINT_TIME
  if (tx->ndiv == 0)
    gettimeofday(tp, 0);
#endif
  do_task_body(thr, tx->body);
#ifdef PRINT_TIME
  if (tx->ndiv == 0){
    gettimeofday(tp+1, 0);
    fprintf(stderr, "%lf\n", tp[1].tv_sec-tp[0].tv_sec+1e-6*(tp[1].tv_usec-tp[0].tv_usec));
  }
#endif
  pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d after do_task_body\n",thr->id);
#endif

  /* task の処理完了後は，その task_home に send_rslt する */
  rcmd.c = 2;
  rcmd.node = tx->rslt_to;
  rcmd.v[0] = "rslt";
  rcmd.v[1] = tx->rslt_head;
  if(rcmd.node == INSIDE){
    pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "lock queue id=%d\n", thr->id);
#endif
    enqueue_command(rcmd, tx->body);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "signal queue id=%d\n", thr->id);
    fprintf(stderr, "unlock queue id=%d\n", thr->id);
#endif   
    pthread_cond_signal(&cond_q); 
    pthread_mutex_unlock(&queue_mut);
    pthread_mutex_lock(&send_mut); /* flush_treq のため */
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "lock send id=%d\n", thr->id);
#endif
  }
  else if(rcmd.node == OUTSIDE){
    pthread_mutex_lock(&send_mut);
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "lock send id=%d\n", thr->id);
#endif
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "unlock before send id=%d\n", thr->id);
#endif
    pthread_mutex_unlock(&thr->mut);
    send_command(rcmd);
    /* body ではなく，do_task_body の return value にするのはどうか? */
#ifdef TEST_MSG_SEND
    fprintf(stderr, "id=%d send rslt body\n", thr->id);
#endif
    send_rslt_body(thr, tx->body);
    /* または，x->body.h.class->send_rslt_body(x->body); */
    write_eol();
    pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "lock after send id=%d\n", thr->id);
#endif
  }
  else{
    perror("invalid rcmd.node in recv_exec_send\n");
    fprintf(stderr, "%d\n", rcmd.node);
    exit(0);
  }
  /* 最後にも treq がたまっていたら，none を送る */
  flush_treq_with_none_1(thr);
  flush_send();
#ifdef TEST_MSG_LOCK_SEND
  fprintf(stderr, "unlock send id=%d\n", thr->id);
#endif
  pthread_mutex_unlock(&send_mut);
  pthread_mutex_lock(&thr->rack_mut);
  thr->w_rack++;
  pthread_mutex_unlock(&thr->rack_mut);
  thr->ndiv = old_ndiv;
 Lnone:
  thr->task_free = tx;
  thr->task_top = tx->next;
}

void *worker(void *arg){
  struct thread_data *thr = arg;
  pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d start worker\n",thr->id);
#endif
  for(;;)
    recv_exec_send(thr, "any", ANY);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "unlock id=%d finish worker\n",thr->id);
#endif
  pthread_mutex_unlock(&thr->mut);
}



void recv_task(struct cmd cmd, void *body){
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  size_t len;
  if(cmd.c < 4)
    proto_error("wrong task", cmd);
  /* id を <task_head> に含める */
  id = atoi(cmd.v[3]);
#ifdef TEST_MSG
  fprintf(stderr, "id=%d recv_task\n",id);
#endif  
  if(! (id<num_thrs))
    proto_error("wrong task_head", cmd);
  thr = thread_data+id;
  pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d recv_task\n",thr->id);
#endif
  tx = thr->task_top;
  tx->rslt_to = cmd.node;
  len = strlen(cmd.v[2]);
  if(len > 254)
    proto_error("too long rslt_head for task", cmd);
  strncpy(tx->rslt_head, cmd.v[2], len+1);
  tx->ndiv = atoi(cmd.v[1]);
  if (cmd.node == INSIDE){
    tx->body = body;
  }
  else if(cmd.node == OUTSIDE){
    tx->body =
      recv_task_body(thr); /* 読み出しはロック獲得の前のほうがいいかも */
    read_to_eol();
  }
  else {
    perror("invalid cmd.node in recv_task\n");
    fprintf(stderr, "%d\n", cmd.node);
    exit(0);
  }
  /* task を受け取った後，ノード内に待っているスレッド(ワーカ)を起こす */

  tx->stat = TASK_INITIALIZED; /* そもそもロックが必要なのはここだけかも */
  /* treqしてからtaskを受け取るまでの間はstatしかアクセスされない気がする */
  
  /* スレッド数の上限を超えるなら，後で(どれかのスレッドが終わるとき)
     signal するべきか?
     それよりは，別にセマフォで上限を管理するほうが簡単  */
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "pthread_cond_broadcast id=%d\n",thr->id);
  fprintf(stderr, "unlock id=%d recv_task fin\n",thr->id);
#endif
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
#ifdef TEST_MSG
  fprintf(stderr, "id=%d recv_none\n",thr->id);
#endif
  pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d recv_none\n",thr->id);
#endif
  if(thr->w_none > 0)
    thr->w_none--;
  else
    thr->task_top->stat = TASK_NONE;
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "pthread_cond_broadcast id=%d\n",thr->id);
  fprintf(stderr, "unlock id=%d recv_none fin\n",thr->id);
#endif
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
void recv_rslt(struct cmd cmd, void *rbody){
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int tid, sid;
  char *b;
  char h_buf[256];

  if(cmd.c < 2)
    proto_error("wrong rslt", cmd);
  b = cmd.v[1];
  tid = atoi(b);
  b = strchr(b, ':');
  if(!b)
    proto_error("wrong rslt_head", cmd);
  sid = atoi(b+1);
  thr = thread_data+tid;
#ifdef TEST_MSG
  fprintf(stderr, "id=%d recv_rslt\n",thr->id);
#endif
  pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d recv_rslt\n",thr->id);
#endif
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
  if(cmd.node == INSIDE){
    hx->body = rbody;
  }
  else if (cmd.node == OUTSIDE){
    recv_rslt_body(thr, hx->body);
    read_to_eol();
  }
  else {
    perror("invalid cmd.node in recv_rslt\n");
    fprintf(stderr, "%d\n", cmd.node);
    exit(0);
  }
  /* rack を返す もっと後のほうがよい? */
  rcmd.c = 2;
  rcmd.node = cmd.node;
  rcmd.v[0] = "rack";
  strncpy(h_buf, hx->task_head, strlen(hx->task_head)+1);
  rcmd.v[1] = h_buf;
  /* hx 中に記録された task_head に rack を後で送るなら，
     ここではないが，まだ free されたくないので，つなぎなおすかも  */
  hx->stat = TASK_HOME_DONE;
  if(hx == thr->sub){
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "pthread_cond_broadcast id=%d\n",thr->id);
#endif
    pthread_cond_broadcast(&thr->cond_r);
    pthread_cond_broadcast(&thr->cond);
  }
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "unlock id=%d recv_rslt fin\n",thr->id);
#endif
  pthread_mutex_unlock(&thr->mut);
  if (cmd.node){
    pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "lock queue in recv_rslt\n");
#endif
    enqueue_command(rcmd, NULL);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "signal queue in recv_rslt\n");
    fprintf(stderr, "unlock queue in recv_rslt\n");
#endif    
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
  }
  else{
#if 0
    pthread_mutex_lock(&send_mut);
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "lock send in recv_rslt\n");
#endif
    send_command(rcmd);
    flush_send();
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "unlock send in recv_rslt\n", thr->id);
#endif
    pthread_mutex_unlock(&send_mut);
#endif
    /* デッドロック防止のため読み出しをするスレッドで書き込みをしないようにしている */
    pthread_mutex_lock(&snr_mut);
    enqueue_snr(rcmd);
    pthread_cond_signal(&cond_snr);
    pthread_mutex_unlock(&snr_mut);
  }
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
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d try_treq\n",thr->id);
#endif
  pthread_mutex_lock(&thr->rack_mut);
  if(thr->task_top &&
     (thr->task_top->stat == TASK_STARTED || thr->task_top->stat == TASK_INITIALIZED)
     && thr->w_rack == 0)
    avail = 1;
  pthread_mutex_unlock(&thr->rack_mut);
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
    if (cmd.node != OUTSIDE)
      hx->req_from = INSIDE;
    else
      hx->req_from = OUTSIDE;
    thr->treq_top = hx;
    thr->req = hx;
  }
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "unlock id=%d try_treq fin\n",thr->id);
#endif
  pthread_mutex_unlock(&thr->mut);
#ifdef TEST_MSG
  fprintf(stderr, "id=%d try_treq %d\n",id,avail);
#endif
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
#ifdef TEST_MSG
  fprintf(stderr, "recv_treq\n");
#endif
  if(cmd.c < 3)
    proto_error("wrong treq", cmd);
  /* id を <task_head> に含める */
  if(strcmp(cmd.v[2], "any") == 0){
    unsigned int myid = atoi(cmd.v[1]);
    for(id=0;id<num_thrs;id++){
      if(cmd.node != OUTSIDE && (id == myid)) continue;
      if(try_treq(cmd, id)) break;
    }
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

  if (cmd.node == ANY){
    if (atoi(cmd.v[1]) == 0){
      pthread_mutex_lock(&send_mut);
#ifdef TEST_MSG_LOCK_SEND
      fprintf(stderr, "lock send in recv_treq\n");
#endif
      send_command(cmd);
      flush_send();
      divisibility_flag = 1;
#ifdef TEST_MSG_LOCK_SEND
      fprintf(stderr, "unlock send in recv_treq\n");
#endif
      pthread_mutex_unlock(&send_mut);
      return;
    }
    else{
      cmd.node = INSIDE;
    }
  }
  
  /* none を返す */
  rcmd.c = 2;
  rcmd.node = cmd.node;
  rcmd.v[0] = "none"; rcmd.v[1] = cmd.v[1];
  if(rcmd.node == INSIDE){
    pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "lock queue in recv_treq\n");
#endif
    enqueue_command(rcmd, NULL);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "signal queue in recv_treq\n");
    fprintf(stderr, "unlock queue in recv_treq\n");
#endif    
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
  }
  else if (rcmd.node == OUTSIDE){
#if 0
    pthread_mutex_lock(&send_mut);
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "lock send in recv_treq\n");
#endif
    send_command(rcmd);
    flush_send();
    divisibility_flag = 1;
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "unlock send in recv_treq\n");
#endif
    pthread_mutex_unlock(&send_mut);
#endif
    /* デッドロック防止のため読み出しをするスレッドで書き込みをしないようにしている */
    pthread_mutex_lock(&snr_mut);
    enqueue_snr(rcmd);
    pthread_cond_signal(&cond_snr);
    pthread_mutex_unlock(&snr_mut);
  }
  else{
    perror("invalid rcmd.node in recv_treq\n");
    fprintf(stderr, "%d\n", rcmd.node);
    exit(0);
  }
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
#ifdef TEST_MSG
  fprintf(stderr, "id=%d recv_rack\n",id);
#endif
  if(! (id<num_thrs))
    proto_error("wrong task_head", cmd);
  thr = thread_data+id;
  pthread_mutex_lock(&thr->rack_mut);
  thr->w_rack--;
  pthread_mutex_unlock(&thr->rack_mut);
}

main(int argc, char *argv[]){
  int i, j;
  void *dummy;
  struct cmd cmd;

  /* cmd_queue の初期化 */
  for(i=0; i<511; i++){
    cmd_queue[i].next = &cmd_queue[i+1];
    for(j=0; j<4; j++)
      cmd_queue[i].cmd.v[j] = cmd_v_buf[i][j];
  }
  cmd_queue[511].next = &cmd_queue[0];
  for(j=0; j<4; j++)
    cmd_queue[511].cmd.v[j] = cmd_v_buf[511][j];
  cmd_in = &cmd_queue[0];
  cmd_out = &cmd_queue[0];

  systhr_create(exec_queue_cmd, NULL);

  /* snr_queue の初期化 */
  for(i=0; i<31; i++){
    snr_queue[i].next = &snr_queue[i+1];
    for(j=0; j<2; j++)
      snr_queue[i].cmd.v[j] = snr_v_buf[i][j];
  }
  snr_queue[31].next = &snr_queue[0];
  for(j=0; j<2; j++)
    snr_queue[31].cmd.v[j] = snr_v_buf[31][j];
  snr_in = &snr_queue[0];
  snr_out = &snr_queue[0];

  systhr_create(send_none_rack, NULL);
  
  if (argc > 1){
    num_thrs = atoi(argv[1]);
  }
  /* ここで thread_data の初期化, task の 双方向list も */
  /* num_thrs = 1;*/
  for(i=0; i<num_thrs; i++){
    struct thread_data *thr = thread_data+i;
    struct task *tx;
    struct task_home *hx;
    thr->id = i;
    thr->w_rack = 0;
    thr->w_none = 0;
    thr->ndiv = 0;
    pthread_mutex_init(&thr->mut, 0);
    pthread_mutex_init(&thr->rack_mut, 0);
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
    if(strcmp(cmd.v[0],"task") == 0){
      recv_task(cmd, dummy);
    }
    else if(strcmp(cmd.v[0],"rslt") == 0){
      recv_rslt(cmd, dummy);
    }
    else if(strcmp(cmd.v[0],"treq") == 0){
      recv_treq(cmd);
    }
    else if(strcmp(cmd.v[0],"none") == 0){
      recv_none(cmd);
    }
    else if(strcmp(cmd.v[0],"rack") == 0){
      recv_rack(cmd);
      divisibility_flag = 1;
    }
    else {
      proto_error("wrong cmd", cmd);
    }
  exit(0);
}

void send_divisible(){
  struct cmd cmd;
  if (pthread_mutex_trylock(&send_mut)) return;
  divisibility_flag = 0;
  cmd.c = 1;
  cmd.v[0] = "dvbl";
  send_command(cmd);
  flush_send();
  pthread_mutex_unlock(&send_mut);
}
  

#define FF int (*_bk)(), struct thread_data *_thr
#define FA _bk, _thr

#define DO_TWO(type1, struct1, work1, work2, put_task1, get_rslt1)	\
do{									\
  type1 struct1[1];							\
  int spawned = 0;							\
  if (divisibility_flag == 1) send_divisible();                         \
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
  if (divisibility_flag == 1) send_divisible(); \
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
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d hundle_req\n",_thr->id);
#endif
#ifdef TEST_MSG
  fprintf(stderr, "id=%d hundle_req\n",_thr->id);
#endif
  if(_thr->req){
    _bk();
    /* ここで 残っていたら，send none する代わりに，
       STARTED でなくなったら none する手も．
       _thr->req  != 0 のままにする */
    _thr->req = _thr->treq_top;
  }
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "unlock id=%d hundle_req fin\n",_thr->id);
#endif
  pthread_mutex_unlock(&_thr->mut);
}

void make_and_send_task(struct thread_data *thr, void *body){
  struct cmd tcmd;
  struct task_home *hx=thr->treq_top;
#ifdef TEST_MSG
  fprintf(stderr, "id=%d make_and_send_task\n",thr->id);
#endif
  thr->treq_top = hx->next;
  hx->next = thr->sub;
  thr->sub = hx;
  hx->body = body;
  hx->id = hx->next ? hx->next->id + 1 : 0;
  hx->stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = hx->req_from;
  tcmd.v[0] = "task";
  /* ズルなので注意 */
  sprintf(thr->ndiv_buf, "%d", ++thr->ndiv);
  sprintf(thr->buf, "%s:%d", thr->id_str, hx->id);
  tcmd.v[1] = thr->ndiv_buf;
  tcmd.v[2] = thr->buf;
  tcmd.v[3] = hx->task_head;
  
  if(tcmd.node == INSIDE){
    pthread_mutex_lock(&queue_mut);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "lock queue id=%d\n", thr->id);
#endif
    enqueue_command(tcmd, body);
#ifdef TEST_MSG_LOCK_QUEUE
    fprintf(stderr, "signal queue id=%d\n", thr->id);
    fprintf(stderr, "unlock queue id=%d\n", thr->id);
#endif
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
  }
  else if(tcmd.node == OUTSIDE){
    pthread_mutex_lock(&send_mut);
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "lock send id=%d\n",thr->id);
#endif
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "unlock before send id=%d\n",thr->id);
#endif
    pthread_mutex_unlock(&thr->mut);
    send_command(tcmd);
#ifdef TEST_MSG_SEND
    fprintf(stderr, "id=%d send task body\n", thr->id);
#endif
    send_task_body(thr, body);
    write_eol();
    flush_send();
#ifdef TEST_MSG_LOCK_SEND
    fprintf(stderr, "unlock send id=%d\n",thr->id);
#endif
    pthread_mutex_unlock(&send_mut);
    pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "lock after send id=%d\n",thr->id);
#endif
  }
  else {
    perror("invalid tcmd.node in make_and_send_task\n");
    fprintf(stderr, "%d\n", tcmd.node);
    exit(0);
  }
}

void *wait_rslt(struct thread_data *thr){
  void *body;
  struct timespec t1;
  struct timeval now;
  long nsec;
  struct task_home *sub;
  pthread_mutex_lock(&thr->mut);
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "lock id=%d wait_rslt\n",thr->id);
#endif
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
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "unlock wait id=%d\n",thr->id);
#endif
    pthread_cond_timedwait(&thr->cond_r, &thr->mut, &t1);
#ifdef TEST_MSG_LOCK_THREAD
    fprintf(stderr, "lock after wait in wait_rslt id=%d\n",thr->id);
#endif
#if 0
    thr->task_top->stat = TASK_STARTED;
#endif
    if(sub->stat == TASK_HOME_DONE)
      break;
#endif
    /* fprintf(stderr, "sub %d\n", sub); */

    recv_exec_send(thr, sub->task_head, sub->req_from);
  }
  body = sub->body;
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  /* fprintf(stderr, "nsub %d\n", thr->sub); */
#ifdef TEST_MSG_LOCK_THREAD
  fprintf(stderr, "unlock id=%d wait_rslt fin\n",thr->id);
#endif
  pthread_mutex_unlock(&thr->mut);
  return body;
}

/*  ********************************************  */
