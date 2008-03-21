#include<sys/time.h>
#define EVALTIME

typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef double	Align_t;

char           *lw_call(char *esp);

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

void           *thread_origin(void *farg);

#include<stdio.h>
#include<pthread.h>
enum node {
  OUTSIDE, INSIDE, ANY
};

struct cmd {
  int		  c;
  enum node	  node;
  char           *v[5];
};

struct cmd_list {
  struct cmd	  cmd;
  void           *body;
  struct cmd_list *next;
};

void		read_to_eol(void);

void		recv_rslt (struct cmd, void *);

void		recv_task (struct cmd, void *);

void		recv_treq (struct cmd);

void		recv_rack (struct cmd);

void		recv_none (struct cmd);
struct task;
struct thread_data;
void            (*task_doers[256]) (char *, struct thread_data *, void *);
void            (*task_senders[256]) (struct thread_data *, void *);
void           *(*task_receivers[256]) (struct thread_data *);
void            (*rslt_senders[256]) (struct thread_data *, void *);
void            (*rslt_receivers[256]) (struct thread_data *, void *);
enum task_stat {
  TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE, TASK_SUSPENDED
};
enum task_home_stat {
  TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE
};

struct task {
  enum task_stat  stat;
  struct task    *next;
  struct task    *prev;
  int		  task_no;
  void           *body;
  int		  ndiv;
  enum node	  rslt_to;
  char		  rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int		  id;
  int		  task_no;
  enum node	  req_from;
  struct task_home *next;
  void           *body;
  char		  task_head[256];
};

struct thread_data {
  struct task_home *volatile req;
  int		  id;
  int		  w_rack;
  int		  w_none;
  int		  ndiv;
  struct task    *task_free;
  struct task    *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t  cond;
  pthread_cond_t  cond_r;
  char		  ndiv_buf[32];
  char		  tno_buf [8];
  char		  id_str  [32];
  char		  buf     [1024];
};
extern int	divisibility_flag;

void		send_divisible(void);

void		make_and_send_task(struct thread_data *thr, int task_no, void *body);

void           *wait_rslt(char *esp, struct thread_data *thr);

void		send_int  (int n);

int		recv_int   (void);

struct systhr_create_frame17 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  pthread_attr_t  attr;
  pthread_t	  tid;
  int		  status;
  void           *arg;
  void           *(*start_func) (char *, void *);
};

int		systhr_create(char *esp, void *(*start_func) (char *, void *), void *arg){
  struct systhr_create_frame17 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  int		  status;
  struct func_arg *farg18;
  struct func_arg *farg19;

LGOTO:
  ;
  efp = (struct systhr_create_frame17 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct systhr_create_frame17) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  status = 0;
  pthread_attr_init(&efp->attr);
  status = pthread_attr_setscope(&efp->attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0) {
    status = (farg18 = (struct func_arg *)malloc(sizeof(struct func_arg)), farg18->func = start_func, farg18->arg = arg, pthread_create(&efp->tid, &efp->attr, thread_origin, farg18));
  } else {
    status = (farg19 = (struct func_arg *)malloc(sizeof(struct func_arg)), farg19->func = start_func, farg19->arg = arg, pthread_create(&efp->tid, 0, thread_origin, farg19));
  }
  return status;
}

void 
mem_error(char const *str)
{
  fputs(str, stderr);
  fputc('\n', stderr);
}
struct cmd_list	cmd_queue[512];
char		cmd_v_buf [512][5][256];
struct cmd_list *cmd_in;
struct cmd_list *cmd_out;
char		buf       [1024];
int		divisibility_flag = 0;

struct cmd 
recv_command()
{
  struct cmd	  r;
  char		  p;
  char		  c;
  char           *b = buf;

  fgets(b, 1024, stdin);
  r.c = 0;
  r.node = OUTSIDE;
  {
    p = '\x0', c = *b;
    for (; c; (p = c, c = *(++b))) {
      if (c == ' ' || c == '\n') {
	c = '\x0';
	*b = '\x0';
      } else if (p == '\x0' && r.c < 5)
	(r.v)[(r.c)++] = b;
    }
  }
  return r;
}
pthread_mutex_t	send_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	queue_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	cond_q = PTHREAD_COND_INITIALIZER;

void 
send_command(struct cmd cmd)
{
  int		  i;

  {
    i = 0;
    for (; i < cmd.c - 1; i++) {
      fputs((cmd.v)[i], stdout);
      fputc(' ', stdout);
    }
  }
  if (cmd.c > 0) {
    fputs((cmd.v)[cmd.c - 1], stdout);
    fputc('\n', stdout);
  }
}

void 
proto_error(char const *str, struct cmd cmd)
{
  int		  i;

  fputs(str, stderr);
  fputc('\n', stderr);
  {
    i = 0;
    for (; i < cmd.c - 1; i++) {
      fputs((cmd.v)[i], stderr);
      fputc('\n', stderr);
    }
  }
  if (cmd.c > 0) {
    fputs((cmd.v)[cmd.c - 1], stderr);
    fputc(' ', stderr);
  }
}

void 
enqueue_command(struct cmd cmd, void *body)
{
  int		  i;
  size_t	  len;
  struct cmd_list *q;

  q = cmd_in;
  if ((*cmd_in).next == cmd_out) {
    perror("cmd-queue overflow\n");
    exit(0);
  }
  cmd_in = (*cmd_in).next;
  (*q).cmd.c = cmd.c;
  (*q).cmd.node = cmd.node;
  {
    i = 0;
    for (; i < cmd.c; i++) {
      len = strlen((cmd.v)[i]);
      if (len > 254)
	proto_error("too long cmd", cmd);
      strncpy(((*q).cmd.v)[i], (cmd.v)[i], len + 1);
    }
  }
  (*q).body = body;
}

struct exec_queue_cmd_frame20 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  void           *body;
  struct cmd	  cmd;
  void           *arg;
};

void           *
exec_queue_cmd(char *esp, void *arg)
{
  struct exec_queue_cmd_frame20 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  struct cmd	  cmd;
  void           *body;

LGOTO:
  ;
  efp = (struct exec_queue_cmd_frame20 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct exec_queue_cmd_frame20) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  while (1) {
    pthread_mutex_lock(&queue_mut);
    while (cmd_in == cmd_out) {
      pthread_cond_wait(&cond_q, &queue_mut);
    }
    cmd = (*cmd_out).cmd;
    body = (*cmd_out).body;
    pthread_mutex_unlock(&queue_mut);
    if (0 == strcmp((cmd.v)[0], "task")) {
      recv_task(cmd, body);
    } else if (0 == strcmp((cmd.v)[0], "rslt")) {
      recv_rslt(cmd, body);
    } else if (0 == strcmp((cmd.v)[0], "treq")) {
      recv_treq(cmd);
    } else if (0 == strcmp((cmd.v)[0], "none")) {
      recv_none(cmd);
    } else if (0 == strcmp((cmd.v)[0], "rack")) {
      recv_rack(cmd);
    } else {
      proto_error("wrong cmd", cmd);
      exit(0);
    }
    cmd_out = (*cmd_out).next;
  }
}

void 
read_to_eol(void)
{
  int		  c;

  while (EOF != (c = getc(stdin))) {
    if (c == '\n')
      break;
  }
}

void 
write_eol(void)
{
  putc('\n', stdout);
}

void 
flush_send(void)
{
  fflush(stdout);
}
pthread_mutex_t	snr_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	cond_snr = PTHREAD_COND_INITIALIZER;
struct cmd_list	snr_queue[32];
char		snr_v_buf [32][2][256];
struct cmd_list *snr_in;
struct cmd_list *snr_out;

void 
enqueue_snr(struct cmd cmd)
{
  int		  i;
  size_t	  len;
  struct cmd_list *q;

  q = snr_in;
  if ((*snr_in).next == snr_out) {
    perror("snr-queue overflow\n");
    exit(0);
  }
  snr_in = (*snr_in).next;
  (*q).cmd.c = cmd.c;
  (*q).cmd.node = cmd.node;
  {
    i = 0;
    for (; i < cmd.c; i++) {
      len = strlen((cmd.v)[i]);
      if (len > 254)
	proto_error("too long cmd", cmd);
      strncpy(((*q).cmd.v)[i], (cmd.v)[i], len + 1);
    }
  }
}

struct send_none_rack_frame21 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct cmd	  cmd;
  void           *arg;
};

void           *
send_none_rack(char *esp, void *arg)
{
  struct send_none_rack_frame21 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  struct cmd	  cmd;

LGOTO:
  ;
  efp = (struct send_none_rack_frame21 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct send_none_rack_frame21) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  while (1) {
    pthread_mutex_lock(&snr_mut);
    while (snr_in == snr_out) {
      pthread_cond_wait(&cond_snr, &snr_mut);
    }
    cmd = (*snr_out).cmd;
    pthread_mutex_unlock(&snr_mut);
    pthread_mutex_lock(&send_mut);
    send_command(cmd);
    flush_send();
    if (0 == strcmp((cmd.v)[0], "none")) {
      divisibility_flag = 1;
    }
    pthread_mutex_unlock(&send_mut);
    snr_out = (*snr_out).next;
  }
}
struct thread_data threads[64];
unsigned int	num_thrs;

void 
flush_treq_with_none_1(struct thread_data *thr)
{
  struct cmd	  rcmd;
  struct task_home *hx;

  while (hx = (*thr).treq_top) {
    rcmd.c = 2;
    rcmd.node = (*hx).req_from;
    (rcmd.v)[0] = "none";
    (rcmd.v)[1] = (*hx).task_head;
    if (rcmd.node == INSIDE) {
      pthread_mutex_lock(&queue_mut);
      enqueue_command(rcmd, NULL);
      pthread_cond_signal(&cond_q);
      pthread_mutex_unlock(&queue_mut);
    } else if (rcmd.node == OUTSIDE) {
      send_command(rcmd);
      divisibility_flag = 1;
    } else {
      perror("Invalid rcmd.node in flush-treq-with-none-1\n");
      fprintf(stderr, "%d\n", rcmd.node);
      exit(0);
    }
    (*thr).treq_top = (*hx).next;
    (*hx).next = (*thr).treq_free;
    (*thr).treq_free = hx;
  }
}

void 
flush_treq_with_none(struct thread_data *thr)
{
  if ((*thr).treq_top) {
    pthread_mutex_lock(&send_mut);
    flush_treq_with_none_1(thr);
    flush_send();
    pthread_mutex_unlock(&send_mut);
  }
}

struct recv_exec_send_frame22 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  void            (*tmp_fp23) (char *, struct thread_data *, void *);
  long		  nsec;
  struct timeval  now;
  struct timespec t1;
  int		  old_ndiv;
  long		  delay;
  struct task    *tx;
  struct cmd	  rcmd;
  int		  req_to;
  char           *treq_head;
  struct thread_data *thr;
  void           *tmp_arg24;
};

void 
recv_exec_send(char *esp, struct thread_data *thr, char *treq_head, int req_to)
{
  struct recv_exec_send_frame22 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  struct cmd	  rcmd;
  struct task    *tx;
  long		  delay;
  int		  old_ndiv;
  long		  nsec;
  void            (*tmp_fp23) (char *, struct thread_data *, void *);
  void           *tmp_arg24;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct recv_exec_send_frame22 *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct recv_exec_send_frame22) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL25;
    }
    goto L_CALL25;
  }
  efp = (struct recv_exec_send_frame22 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct recv_exec_send_frame22) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  while ((*thr).w_none > 0) {
    pthread_cond_wait(&(*thr).cond, &(*thr).mut);
    if ((*thr).sub ? ((*(*thr).sub).stat == TASK_HOME_DONE ? 1 : 0) : 0)
      return;
  }
  tx = (*thr).task_free;
  if (!tx) {
    mem_error("Not enough task memory");
  }
  (*thr).task_top = tx;
  (*thr).task_free = (*tx).prev;
  delay = 2 * 1000 * 1000;
  do {
    flush_treq_with_none(thr);
    (*tx).stat = TASK_ALLOCATED;
    pthread_mutex_unlock(&(*thr).mut);
    rcmd.c = 3;
    if (num_thrs > 1) {
      rcmd.node = req_to;
    } else {
      rcmd.node = OUTSIDE;
    }
    (rcmd.v)[0] = "treq";
    (rcmd.v)[1] = (*thr).id_str;
    (rcmd.v)[2] = treq_head;
    if (rcmd.node != OUTSIDE) {
      pthread_mutex_lock(&queue_mut);
      enqueue_command(rcmd, NULL);
      pthread_cond_signal(&cond_q);
      pthread_mutex_unlock(&queue_mut);
    } else {
      pthread_mutex_lock(&send_mut);
      send_command(rcmd);
      flush_send();
      divisibility_flag = 1;
      pthread_mutex_unlock(&send_mut);
    }
    pthread_mutex_lock(&(*thr).mut);
    while (1) {
      if (((*tx).stat != TASK_INITIALIZED ? ((*thr).sub ? 1 : 0) : 0) ? ((*(*thr).sub).stat == TASK_HOME_DONE ? 1 : 0) : 0) {
	if ((*tx).stat == TASK_NONE)
	  goto Lnone;
	((*thr).w_none)++;
	goto Lnone;
      }
      if ((*tx).stat != TASK_ALLOCATED)
	break;
      pthread_cond_wait(&(*thr).cond, &(*thr).mut);
    }
    if ((*tx).stat == TASK_NONE) {
      gettimeofday(&efp->now, 0);
      nsec = (efp->now).tv_usec * 1000;
      nsec += delay;
      delay += delay;
      if (delay > 40 * 1000 * 1000) {
	delay = 40 * 1000 * 1000;
      }
      (efp->t1).tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
      (efp->t1).tv_sec = (efp->now).tv_sec + (nsec > 999999999 ? 1 : 0);
      pthread_cond_timedwait(&(*thr).cond_r, &(*thr).mut, &efp->t1);
      if ((*thr).sub ? ((*(*thr).sub).stat == TASK_HOME_DONE ? 1 : 0) : 0)
	goto Lnone;
    }
  } while ((*tx).stat != TASK_INITIALIZED);
  (*tx).stat = TASK_STARTED;
  old_ndiv = (*thr).ndiv;
  (*thr).ndiv = (*tx).ndiv;
  pthread_mutex_unlock(&(*thr).mut);
#ifdef EVALTIME
  {
    struct timeval _tp1, _tp2;
    double _ttime;
    gettimeofday(&_tp1, 0); 
#endif
    new_esp = esp;
    tmp_fp23 = task_doers[(*tx).task_no];
    tmp_arg24 = (*tx).body;
    while (tmp_fp23(new_esp, thr, tmp_arg24), (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->tmp_fp23 = tmp_fp23;
      efp->nsec = nsec;
      efp->old_ndiv = old_ndiv;
      efp->delay = delay;
      efp->tx = tx;
      efp->rcmd = rcmd;
      efp->req_to = req_to;
      efp->treq_head = treq_head;
      efp->thr = thr;
      efp->call_id = 0;
      return;
    L_CALL25:
      ;
      tmp_fp23 = efp->tmp_fp23;
      nsec = efp->nsec;
      old_ndiv = efp->old_ndiv;
      delay = efp->delay;
      tx = efp->tx;
      rcmd = efp->rcmd;
      req_to = efp->req_to;
      treq_head = efp->treq_head;
      thr = efp->thr;
      new_esp = esp + 1;
    }
#ifdef EVALTIME
    gettimeofday(&_tp2, 0);
    _ttime = 
      (_tp2.tv_sec - _tp1.tv_sec) + (_tp2.tv_usec - _tp1.tv_usec) * 0.000001;
    printf("time: %f\n", _ttime);
  }
#endif
  pthread_mutex_lock(&(*thr).mut);
  rcmd.c = 2;
  rcmd.node = (*tx).rslt_to;
  (rcmd.v)[0] = "rslt";
  (rcmd.v)[1] = (*tx).rslt_head;
  if (rcmd.node == INSIDE) {
    pthread_mutex_lock(&queue_mut);
    enqueue_command(rcmd, (*tx).body);
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
    pthread_mutex_lock(&send_mut);
  } else if (rcmd.node == OUTSIDE) {
    pthread_mutex_lock(&send_mut);
    pthread_mutex_unlock(&(*thr).mut);
    send_command(rcmd);
    (rslt_senders[(*tx).task_no]) (thr, (*tx).body);
    write_eol();
    pthread_mutex_lock(&(*thr).mut);
  } else {
    perror("Invalid rcmd.node in recv-exec-send\n");
    fprintf(stderr, "%d\n", rcmd.node);
    exit(0);
  }
  flush_treq_with_none_1(thr);
  flush_send();
  pthread_mutex_unlock(&send_mut);
  pthread_mutex_lock(&(*thr).rack_mut);
  ((*thr).w_rack)++;
  pthread_mutex_unlock(&(*thr).rack_mut);
  (*thr).ndiv = old_ndiv;
Lnone:
  ;
  {
    (*thr).task_free = tx;
    (*thr).task_top = (*tx).next;
  }
}

struct worker_frame26 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct thread_data *thr;
  void           *arg;
};

void           *
worker(char *esp, void *arg)
{
  struct worker_frame26 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  struct thread_data *thr;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct worker_frame26 *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct worker_frame26) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL27;
    }
    goto L_CALL27;
  }
  efp = (struct worker_frame26 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct worker_frame26) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  thr = arg;
  pthread_mutex_lock(&(*thr).mut);
  while (1) {
    new_esp = esp;
    while (recv_exec_send(new_esp, thr, "any", ANY), (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->thr = thr;
      efp->arg = arg;
      efp->call_id = 0;
      return (void *)0 - 1;
  L_CALL27:
      ;
      thr = efp->thr;
      arg = efp->arg;
      new_esp = esp + 1;
    }
  }
  pthread_mutex_unlock(&(*thr).mut);
}

void 
recv_task(struct cmd cmd, void *body)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int	  id;
  size_t	  len;

  if (cmd.c < 5)
    proto_error("wrong-task", cmd);
  id = atoi((cmd.v)[3]);
  if (!(id < num_thrs))
    proto_error("wrong task-head", cmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  tx = (*thr).task_top;
  (*tx).rslt_to = cmd.node;
  len = strlen((cmd.v)[2]);
  if (len > 254)
    proto_error("Too long rslt-head for task", cmd);
  strncpy((*tx).rslt_head, (cmd.v)[2], len + 1);
  (*tx).ndiv = atoi((cmd.v)[1]);
  (*tx).task_no = atoi((cmd.v)[4]);
  if (cmd.node == INSIDE)
    (*tx).body = body;
  else if (cmd.node == OUTSIDE) {
    (*tx).body = (task_receivers[(*tx).task_no]) (thr);
    read_to_eol();
  } else {
    perror("Invalid cmd.node in recv-task\n");
    fprintf(stderr, "%d\n", cmd.node);
    exit(0);
  }
  (*tx).stat = TASK_INITIALIZED;
  pthread_cond_broadcast(&(*thr).cond);
  pthread_mutex_unlock(&(*thr).mut);
}

void 
recv_none(struct cmd cmd)
{
  struct thread_data *thr;
  unsigned int	  id;
  size_t	  len;

  if (cmd.c < 2)
    proto_error("Wrong none", cmd);
  id = atoi((cmd.v)[1]);
  if (!(id < num_thrs))
    proto_error("Wrong task-head", cmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  if ((*thr).w_none > 0)
    ((*thr).w_none)--;
  else
    (*(*thr).task_top).stat = TASK_NONE;
  pthread_cond_broadcast(&(*thr).cond);
  pthread_mutex_unlock(&(*thr).mut);
}

void 
recv_rslt(struct cmd cmd, void *rbody)
{
  struct cmd	  rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int	  tid;
  unsigned int	  sid;
  char           *b;
  char		  h_buf   [256];

  if (cmd.c < 2)
    proto_error("Wrong rslt", cmd);
  b = (cmd.v)[1];
  tid = atoi(b);
  b = strchr(b, ':');
  if (!b)
    proto_error("Wrong rslt-head", cmd);
  sid = atoi(b + 1);
  thr = threads + tid;
  pthread_mutex_lock(&(*thr).mut);
  hx = (*thr).sub;
  while (hx && (*hx).id != sid) {
    hx = (*hx).next;
  }
  if (!hx)
    proto_error("Wrong rslt-head", cmd);
  if (cmd.node == INSIDE)
    (*hx).body = rbody;
  else if (cmd.node == OUTSIDE) {
    (rslt_receivers[(*hx).task_no]) (thr, (*hx).body);
    read_to_eol();
  } else {
    perror("Invalid cmd.node in recv-rslt\n");
    fprintf(stderr, "%d\n", cmd.node);
    exit(0);
  }
  rcmd.c = 2;
  rcmd.node = cmd.node;
  (rcmd.v)[0] = "rack";
  strncpy(h_buf, (*hx).task_head, 1 + strlen((*hx).task_head));
  (rcmd.v)[1] = h_buf;
  (*hx).stat = TASK_HOME_DONE;
  if (hx == (*thr).sub) {
    pthread_cond_broadcast(&(*thr).cond_r);
    pthread_cond_broadcast(&(*thr).cond);
  }
  pthread_mutex_unlock(&(*thr).mut);
  if (cmd.node) {
    pthread_mutex_lock(&queue_mut);
    enqueue_command(rcmd, NULL);
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
  } else {
    pthread_mutex_lock(&snr_mut);
    enqueue_snr(rcmd);
    pthread_cond_signal(&cond_snr);
    pthread_mutex_unlock(&snr_mut);
  }
}

int 
try_treq(struct cmd cmd, unsigned int id)
{
  struct task_home *hx;
  struct thread_data *thr;
  size_t	  len;
  int		  avail = 0;

  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  pthread_mutex_lock(&(*thr).rack_mut);
  if ((*thr).task_top && ((*(*thr).task_top).stat == TASK_STARTED || (*(*thr).task_top).stat == TASK_INITIALIZED) && (*thr).w_rack == 0)
    avail = 1;
  pthread_mutex_unlock(&(*thr).rack_mut);
  if (avail) {
    hx = (*thr).treq_free;
    if (!hx)
      mem_error("Not enough task-home memory");
    (*thr).treq_free = (*hx).next;
    (*hx).next = (*thr).treq_top;
    (*hx).stat = TASK_HOME_ALLOCATED;
    len = strlen((cmd.v)[1]);
    if (len > 254)
      proto_error("Too long task-head for treq", cmd);
    strncpy((*hx).task_head, (cmd.v)[1], len + 1);
    if (cmd.node != OUTSIDE)
      (*hx).req_from = INSIDE;
    else
      (*hx).req_from = OUTSIDE;
    (*thr).treq_top = hx;
    (*thr).req = hx;
  }
  pthread_mutex_unlock(&(*thr).mut);
  return avail;
}

void 
recv_treq(struct cmd cmd)
{
  struct cmd	  rcmd;
  unsigned int	  id;

  if (cmd.c < 3)
    proto_error("Wrong treq", cmd);
  if (strcmp((cmd.v)[2], "any") == 0) {
    int		    myid;

    myid = atoi((cmd.v)[1]);
    {
      id = 0;
      for (; id < num_thrs; id++) {
	if (cmd.node != OUTSIDE && id == myid)
	  continue;
	if (try_treq(cmd, id))
	  break;
      }
    }
    if (id != num_thrs)
      return;
  } else {
    id = atoi((cmd.v)[2]);
    if (!(id < num_thrs))
      proto_error("Wrong task-head", cmd);
    if (try_treq(cmd, id))
      return;
  }
  if (cmd.node == ANY)
    if (atoi((cmd.v)[1]) == 0) {
      pthread_mutex_lock(&send_mut);
      send_command(cmd);
      flush_send();
      divisibility_flag = 1;
      pthread_mutex_unlock(&send_mut);
      return;
    } else
      cmd.node = INSIDE;
  rcmd.c = 2;
  rcmd.node = cmd.node;
  (rcmd.v)[0] = "none";
  (rcmd.v)[1] = (cmd.v)[1];
  if (rcmd.node == INSIDE) {
    pthread_mutex_lock(&queue_mut);
    enqueue_command(rcmd, NULL);
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
  } else if (rcmd.node == OUTSIDE) {
    pthread_mutex_lock(&snr_mut);
    enqueue_snr(rcmd);
    pthread_cond_signal(&cond_snr);
    pthread_mutex_unlock(&snr_mut);
  } else {
    perror("Invalid rcmd.node in recv-treq\n");
    fprintf(stderr, "%d\n", rcmd.node);
    exit(0);
  }
}

void 
recv_rack(struct cmd cmd)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int	  id;
  size_t	  len;

  if (cmd.c < 2)
    proto_error("Wrong rack", cmd);
  id = atoi((cmd.v)[1]);
  if (!(id < num_thrs))
    proto_error("Wrong task-head", cmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).rack_mut);
  ((*thr).w_rack)--;
  pthread_mutex_unlock(&(*thr).rack_mut);
}

void 
send_divisible(void)
{
  struct cmd	  cmd;

  if (pthread_mutex_trylock(&send_mut))
    return;
  divisibility_flag = 0;
  cmd.c = 1;
  (cmd.v)[0] = "dvbl";
  send_command(cmd);
  flush_send();
  pthread_mutex_unlock(&send_mut);
}

struct handle_req_frame28 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct thread_data *_thr;
  closure_t      *_bk;
};

void 
handle_req(char *esp, closure_t * _bk, struct thread_data *_thr)
{
  char           *argp;
  struct handle_req_frame28 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct handle_req_frame28 *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct handle_req_frame28) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL29;
    }
    goto L_CALL29;
  }
  efp = (struct handle_req_frame28 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct handle_req_frame28) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  pthread_mutex_lock(&(*_thr).mut);
  if ((*_thr).req) {
    {
      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = _bk;
      efp->_thr = _thr;
      efp->_bk = _bk;
      efp->argp = argp;
      efp->tmp_esp = argp;
      efp->call_id = 0;
      return;
  L_CALL29:
      ;
      _thr = efp->_thr;
      _bk = efp->_bk;
    }
    (*_thr).req = (*_thr).treq_top;
  }
  pthread_mutex_unlock(&(*_thr).mut);
}

void 
make_and_send_task(struct thread_data *thr, int task_no, void *body)
{
  struct cmd	  tcmd;
  struct task_home *hx = (*thr).treq_top;

  (*thr).treq_top = (*hx).next;
  (*hx).next = (*thr).sub;
  (*thr).sub = hx;
  (*hx).task_no = task_no;
  (*hx).body = body;
  (*hx).id = (*hx).next ? (*(*hx).next).id + 1 : 0;
  (*hx).stat = TASK_HOME_INITIALIZED;
  tcmd.c = 5;
  tcmd.node = (*hx).req_from;
  (tcmd.v)[0] = "task";
  sprintf((*thr).ndiv_buf, "%d", ++(*thr).ndiv);
  sprintf((*thr).buf, "%s:%d", (*thr).id_str, (*hx).id);
  sprintf((*thr).tno_buf, "%d", task_no);
  (tcmd.v)[1] = (*thr).ndiv_buf;
  (tcmd.v)[2] = (*thr).buf;
  (tcmd.v)[3] = (*hx).task_head;
  (tcmd.v)[4] = (*thr).tno_buf;
  if (tcmd.node == INSIDE) {
    pthread_mutex_lock(&queue_mut);
    enqueue_command(tcmd, body);
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
  } else if (tcmd.node == OUTSIDE) {
    pthread_mutex_lock(&send_mut);
    pthread_mutex_unlock(&(*thr).mut);
    send_command(tcmd);
    (task_senders[task_no]) (thr, body);
    write_eol();
    flush_send();
    pthread_mutex_unlock(&send_mut);
    pthread_mutex_lock(&(*thr).mut);
  } else {
    perror("Invalid tcmd.node in make-and-send-task\n");
    fprintf(stderr, "%d\n", tcmd.node);
    exit(0);
  }
}

struct wait_rslt_frame30 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct task_home *sub;
  long		  nsec;
  struct timeval  now;
  struct timespec t1;
  void           *body;
  struct thread_data *thr;
  enum node	  tmp_arg32;
  char           *tmp_arg31;
};

void           *
wait_rslt(char *esp, struct thread_data *thr)
{
  struct wait_rslt_frame30 *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  void           *body;
  long		  nsec;
  struct task_home *sub;
  char           *tmp_arg31;
  enum node	  tmp_arg32;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct wait_rslt_frame30 *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct wait_rslt_frame30) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL33;
    }
    goto L_CALL33;
  }
  efp = (struct wait_rslt_frame30 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct wait_rslt_frame30) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  pthread_mutex_lock(&(*thr).mut);
  sub = (*thr).sub;
  while ((*sub).stat != TASK_HOME_DONE) {
    gettimeofday(&efp->now, 0);
    nsec = 1000 * (efp->now).tv_usec;
    nsec += 5 * 1000 * 1000;
    (efp->t1).tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
    (efp->t1).tv_sec = (efp->now).tv_sec + (nsec > 999999999 ? 1 : 0);
    pthread_cond_timedwait(&(*thr).cond_r, &(*thr).mut, &efp->t1);
    if ((*sub).stat == TASK_HOME_DONE)
      break;
    new_esp = esp;
    tmp_arg31 = (*sub).task_head;
    tmp_arg32 = (*sub).req_from;
    while (recv_exec_send(new_esp, thr, tmp_arg31, tmp_arg32), (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->sub = sub;
      efp->nsec = nsec;
      efp->body = body;
      efp->thr = thr;
      efp->call_id = 0;
      return (void *)0 - 1;
  L_CALL33:
      ;
      sub = efp->sub;
      nsec = efp->nsec;
      body = efp->body;
      thr = efp->thr;
      new_esp = esp + 1;
    }
  }
  body = (*sub).body;
  (*thr).sub = (*sub).next;
  (*sub).next = (*thr).treq_free;
  (*thr).treq_free = sub;
  pthread_mutex_unlock(&(*thr).mut);
  return body;
}

struct main_frame34 {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct task_home *hx;
  struct task    *tx;
  struct thread_data *thr;
  struct cmd	  cmd;
  void           *dummy;
  int		  j;
  int		  i;
  char          **argv;
  int		  argc;
};

int 
main(int argc, char **argv)
{
  struct main_frame34 *efp;
  char           *new_esp;
  char		  estack  [8192];
  char           *esp = estack;
  int		  i;
  int		  j;
  void           *dummy;
  struct cmd	  cmd;
  struct thread_data *thr;
  struct task    *tx;
  struct task_home *hx;

LGOTO:
  ;
  efp = (struct main_frame34 *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame34) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  {
    i = 0;
    while (i < 512) {
      (cmd_queue[i]).next = &cmd_queue[(i + 1) % 512];
      {
	j = 0;
	while (j < 5) {
	  (((cmd_queue[i]).cmd).v)[j] = (cmd_v_buf[i])[j];
	  j++;
	}
      }
      i++;
    }
  }
  cmd_in = &cmd_queue[0];
  cmd_out = &cmd_queue[0];
  new_esp = esp;
  while (systhr_create(new_esp, exec_queue_cmd, NULL), (efp->tmp_esp = *((char **)esp)) != 0) {
    char           *goto_fr;

    *((char **)esp) = 0;
    efp->cmd = cmd;
    efp->dummy = dummy;
    efp->j = j;
    efp->i = i;
    efp->argv = argv;
    efp->argc = argc;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto LGOTO;
    cmd = efp->cmd;
    dummy = efp->dummy;
    j = efp->j;
    i = efp->i;
    argv = efp->argv;
    argc = efp->argc;
    new_esp = esp + 1;
  }
  {
    i = 0;
    while (i < 32) {
      (snr_queue[i]).next = &snr_queue[(i + 1) % 32];
      {
	j = 0;
	while (j < 2) {
	  (((snr_queue[i]).cmd).v)[j] = (snr_v_buf[i])[j];
	  j++;
	}
      }
      i++;
    }
  }
  snr_in = &snr_queue[0];
  snr_out = &snr_queue[0];
  new_esp = esp;
  while (systhr_create(new_esp, send_none_rack, NULL), (efp->tmp_esp = *((char **)esp)) != 0) {
    char           *goto_fr;

    *((char **)esp) = 0;
    efp->cmd = cmd;
    efp->dummy = dummy;
    efp->j = j;
    efp->i = i;
    efp->argv = argv;
    efp->argc = argc;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto LGOTO;
    cmd = efp->cmd;
    dummy = efp->dummy;
    j = efp->j;
    i = efp->i;
    argv = efp->argv;
    argc = efp->argc;
    new_esp = esp + 1;
  }
  num_thrs = argc > 1 ? atoi(argv[1]) : 1;
  {
    i = 0;
    while (i < num_thrs) {
      {
	thr = threads + i;
	(*thr).id = i;
	(*thr).w_rack = 0;
	(*thr).w_none = 0;
	(*thr).ndiv = 0;
	pthread_mutex_init(&(*thr).mut, 0);
	pthread_mutex_init(&(*thr).rack_mut, 0);
	pthread_cond_init(&(*thr).cond, 0);
	pthread_cond_init(&(*thr).cond_r, 0);
	sprintf((*thr).id_str, "%d", i);
	tx = (struct task *)malloc(sizeof(struct task) * 512);
	(*thr).task_top = 0;
	(*thr).task_free = tx;
	{
	  j = 0;
	  while (j < 511) {
	    (tx[j]).prev = &tx[j + 1];
	    (tx[j + 1]).next = &tx[j];
	    j++;
	  }
	}
	(tx[0]).next = 0;
	(tx[511]).prev = 0;
	hx = (struct task_home *)malloc(sizeof(struct task_home) * 512);
	(*thr).treq_top = 0;
	(*thr).treq_free = hx;
	(*thr).sub = 0;
	{
	  j = 0;
	  while (j < 511) {
	    (hx[j]).next = &hx[j + 1];
	    j++;
	  }
	}
	(hx[511]).next = 0;
	new_esp = esp;
	while (systhr_create(new_esp, worker, thr), (efp->tmp_esp = *((char **)esp)) != 0) {
	  char           *goto_fr;

	  *((char **)esp) = 0;
	  efp->hx = hx;
	  efp->tx = tx;
	  efp->thr = thr;
	  efp->cmd = cmd;
	  efp->dummy = dummy;
	  efp->j = j;
	  efp->i = i;
	  efp->argv = argv;
	  efp->argc = argc;
	  goto_fr = lw_call(efp->tmp_esp);
	  if ((char *)goto_fr == (char *)efp)
	    goto LGOTO;
	  hx = efp->hx;
	  tx = efp->tx;
	  thr = efp->thr;
	  cmd = efp->cmd;
	  dummy = efp->dummy;
	  j = efp->j;
	  i = efp->i;
	  argv = efp->argv;
	  argc = efp->argc;
	  new_esp = esp + 1;
	}
      }
      i++;
    }
  }
  cmd = recv_command();
  while (cmd.c > 0 ? (0 != strcmp((cmd.v)[0], "exit") ? 1 : 0) : 0) {
    if (0 == strcmp((cmd.v)[0], "task")) {
      recv_task(cmd, dummy);
    } else if (0 == strcmp((cmd.v)[0], "rslt")) {
      recv_rslt(cmd, dummy);
    } else if (0 == strcmp((cmd.v)[0], "treq")) {
      recv_treq(cmd);
    } else if (0 == strcmp((cmd.v)[0], "none")) {
      recv_none(cmd);
    } else if (0 == strcmp((cmd.v)[0], "rack")) {
      recv_rack(cmd);
    } else {
      proto_error("wrong cmd", cmd);
    }
    cmd = recv_command();
  }
  exit(0);
}
