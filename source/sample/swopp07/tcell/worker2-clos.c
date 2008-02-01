#include <sys/time.h>
#define EVALTIME

typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef unsigned int size_t;
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
static int	debug = 1;

struct systhr_create_frame {
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
  struct systhr_create_frame *efp;
  struct func_arg *farg;
  struct func_arg *farg2;

LGOTO:
  ;
  efp = (struct systhr_create_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct systhr_create_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->start_func = start_func;
  efp->arg = arg;
  efp->status = 0;
  pthread_attr_init(&efp->attr);
  efp->status = pthread_attr_setscope(&efp->attr, PTHREAD_SCOPE_SYSTEM);
  if (efp->status == 0) {
    efp->status = (farg = (struct func_arg *)malloc(sizeof(struct func_arg)), farg->func = start_func, farg->arg = arg, pthread_create(&efp->tid, &efp->attr, thread_origin, farg));
  } else {
    efp->status = (farg2 = (struct func_arg *)malloc(sizeof(struct func_arg)), farg2->func = start_func, farg2->arg = arg, pthread_create(&efp->tid, 0, thread_origin, farg2));
  }
  return efp->status;
}

void		mem_error (char const *str)
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

struct cmd	recv_command()
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

void		send_command(struct cmd cmd)
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

void		proto_error(char const *str, struct cmd cmd)
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

void		enqueue_command(struct cmd cmd, void *body)
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

struct exec_queue_cmd_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  void           *body;
  struct cmd	  cmd;
  void           *arg;
};

void           *exec_queue_cmd(char *esp, void *arg)
{
  struct exec_queue_cmd_frame *efp;

LGOTO:
  ;
  efp = (struct exec_queue_cmd_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct exec_queue_cmd_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->arg = arg;
  while (1) {
    pthread_mutex_lock(&queue_mut);
    while (cmd_in == cmd_out) {
      pthread_cond_wait(&cond_q, &queue_mut);
    }
    efp->cmd = (*cmd_out).cmd;
    efp->body = (*cmd_out).body;
    cmd_out = (*cmd_out).next;
    pthread_mutex_unlock(&queue_mut);
    if (0 == strcmp(((efp->cmd).v)[0], "task")) {
      recv_task(efp->cmd, efp->body);
    } else if (0 == strcmp(((efp->cmd).v)[0], "rslt")) {
      recv_rslt(efp->cmd, efp->body);
    } else if (0 == strcmp(((efp->cmd).v)[0], "treq")) {
      recv_treq(efp->cmd);
    } else if (0 == strcmp(((efp->cmd).v)[0], "none")) {
      recv_none(efp->cmd);
    } else if (0 == strcmp(((efp->cmd).v)[0], "rack")) {
      recv_rack(efp->cmd);
    } else {
      proto_error("wrong cmd", efp->cmd);
      exit(0);
    }
  }
}

void		read_to_eol(void)
{
  int		  c;

  while (EOF != (c = getc(stdin))) {
    if (c == '\n')
      break;
  }
}

void		write_eol (void)
{
  putc('\n', stdout);
}

void		flush_send(void)
{
  fflush(stdout);
}
pthread_mutex_t	snr_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	cond_snr = PTHREAD_COND_INITIALIZER;
struct cmd_list	snr_queue[32];
char		snr_v_buf [32][2][256];
struct cmd_list *snr_in;
struct cmd_list *snr_out;

void		enqueue_snr(struct cmd cmd)
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

struct send_none_rack_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct cmd	  cmd;
  void           *arg;
};

void           *send_none_rack(char *esp, void *arg)
{
  struct send_none_rack_frame *efp;

LGOTO:
  ;
  efp = (struct send_none_rack_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct send_none_rack_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->arg = arg;
  while (1) {
    pthread_mutex_lock(&snr_mut);
    while (snr_in == snr_out) {
      pthread_cond_wait(&cond_snr, &snr_mut);
    }
    efp->cmd = (*snr_out).cmd;
    pthread_mutex_unlock(&snr_mut);
    pthread_mutex_lock(&send_mut);
    send_command(efp->cmd);
    flush_send();
    if (0 == strcmp(((efp->cmd).v)[0], "none")) {
      divisibility_flag = 1;
    }
    pthread_mutex_unlock(&send_mut);
    snr_out = (*snr_out).next;
  }
}
struct thread_data threads[64];
unsigned int	num_thrs;

void		flush_treq_with_none_1(struct thread_data *thr)
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

void		flush_treq_with_none(struct thread_data *thr)
{
  if ((*thr).treq_top) {
    pthread_mutex_lock(&send_mut);
    flush_treq_with_none_1(thr);
    flush_send();
    pthread_mutex_unlock(&send_mut);
  }
}

struct recv_exec_send_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
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
};

void		recv_exec_send(char *esp, struct thread_data *thr, char *treq_head, int req_to)
{
  struct recv_exec_send_frame *efp;

LGOTO:
  ;
  efp = (struct recv_exec_send_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct recv_exec_send_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->thr = thr;
  efp->treq_head = treq_head;
  efp->req_to = req_to;
  while ((*efp->thr).w_none > 0) {
    pthread_cond_wait(&(*efp->thr).cond, &(*efp->thr).mut);
    if ((*efp->thr).sub ? ((*(*efp->thr).sub).stat == TASK_HOME_DONE ? 1 : 0) : 0)
      return;
  }
  efp->tx = (*efp->thr).task_free;
  if (!efp->tx) {
    mem_error("Not enough task memory");
  }
  (*efp->thr).task_top = efp->tx;
  (*efp->thr).task_free = (*efp->tx).prev;
  efp->delay = 2 * 1000 * 1000;
  do {
    flush_treq_with_none(efp->thr);
    (*efp->tx).stat = TASK_ALLOCATED;
    pthread_mutex_unlock(&(*efp->thr).mut);
    (efp->rcmd).c = 3;
    if (num_thrs > 1) {
      (efp->rcmd).node = efp->req_to;
    } else {
      (efp->rcmd).node = OUTSIDE;
    }
    ((efp->rcmd).v)[0] = "treq";
    ((efp->rcmd).v)[1] = (*efp->thr).id_str;
    ((efp->rcmd).v)[2] = efp->treq_head;
    if ((efp->rcmd).node != OUTSIDE) {
      pthread_mutex_lock(&queue_mut);
      enqueue_command(efp->rcmd, NULL);
      pthread_cond_signal(&cond_q);
      pthread_mutex_unlock(&queue_mut);
    } else {
      pthread_mutex_lock(&send_mut);
      send_command(efp->rcmd);
      flush_send();
      divisibility_flag = 1;
      pthread_mutex_unlock(&send_mut);
    }
    pthread_mutex_lock(&(*efp->thr).mut);
    while (1) {
      if ((*efp->tx).stat != TASK_INITIALIZED ? ((*efp->thr).sub && (*(*efp->thr).sub).stat == TASK_HOME_DONE ? 1 : 0) : 0) {
	if ((*efp->tx).stat == TASK_NONE)
	  goto Lnone;
	((*efp->thr).w_none)++;
	goto Lnone;
      }
      if ((*efp->tx).stat != TASK_ALLOCATED)
	break;
      pthread_cond_wait(&(*efp->thr).cond, &(*efp->thr).mut);
    }
    if ((*efp->tx).stat == TASK_NONE) {
      gettimeofday(&efp->now, 0);
      efp->nsec = (efp->now).tv_usec * 1000;
      efp->nsec += efp->delay;
      efp->delay += efp->delay;
      if (efp->delay > 40 * 1000 * 1000) {
	efp->delay = 40 * 1000 * 1000;
      }
      (efp->t1).tv_nsec = efp->nsec > 999999999 ? efp->nsec - 999999999 : efp->nsec;
      (efp->t1).tv_sec = (efp->now).tv_sec + (efp->nsec > 999999999 ? 1 : 0);
      pthread_cond_timedwait(&(*efp->thr).cond_r, &(*efp->thr).mut, &efp->t1);
      if ((*efp->thr).sub ? ((*(*efp->thr).sub).stat == TASK_HOME_DONE ? 1 : 0) : 0)
	goto Lnone;
    }
  } while ((*efp->tx).stat != TASK_INITIALIZED);
  (*efp->tx).stat = TASK_STARTED;
  efp->old_ndiv = (*efp->thr).ndiv;
  (*efp->thr).ndiv = (*efp->tx).ndiv;
  pthread_mutex_unlock(&(*efp->thr).mut);
#ifdef EVALTIME
  {
    struct timeval _tp1, _tp2;
    double _ttime;
    gettimeofday(&_tp1, 0); 
#endif
    while ((task_doers[(*efp->tx).task_no]) (esp, efp->thr, (*efp->tx).body), (efp->tmp_esp = *((char **)esp)) != 0) {
      if ((char *)efp->tmp_esp < (char *)efp)
	return;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
#ifdef EVALTIME
    gettimeofday(&_tp2, 0);
    _ttime = 
      (_tp2.tv_sec - _tp1.tv_sec) + (_tp2.tv_usec - _tp1.tv_usec) * 0.000001;
    printf("time: %f\n", _ttime);
  }
#endif
  pthread_mutex_lock(&(*efp->thr).mut);
  (efp->rcmd).c = 2;
  (efp->rcmd).node = (*efp->tx).rslt_to;
  ((efp->rcmd).v)[0] = "rslt";
  ((efp->rcmd).v)[1] = (*efp->tx).rslt_head;
  if ((efp->rcmd).node == INSIDE) {
    pthread_mutex_lock(&queue_mut);
    enqueue_command(efp->rcmd, (*efp->tx).body);
    pthread_cond_signal(&cond_q);
    pthread_mutex_unlock(&queue_mut);
    pthread_mutex_lock(&send_mut);
  } else if ((efp->rcmd).node == OUTSIDE) {
    pthread_mutex_lock(&send_mut);
    pthread_mutex_unlock(&(*efp->thr).mut);
    send_command(efp->rcmd);
    (rslt_senders[(*efp->tx).task_no]) (efp->thr, (*efp->tx).body);
    write_eol();
    pthread_mutex_lock(&(*efp->thr).mut);
  } else {
    perror("Invalid rcmd.node in recv-exec-send\n");
    fprintf(stderr, "%d\n", (efp->rcmd).node);
    exit(0);
  }
  flush_treq_with_none_1(efp->thr);
  flush_send();
  pthread_mutex_unlock(&send_mut);
  pthread_mutex_lock(&(*efp->thr).rack_mut);
  ((*efp->thr).w_rack)++;
  pthread_mutex_unlock(&(*efp->thr).rack_mut);
  (*efp->thr).ndiv = efp->old_ndiv;
Lnone:
  ;
  {
    (*efp->thr).task_free = efp->tx;
    (*efp->thr).task_top = (*efp->tx).next;
  }
}

struct worker_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct thread_data *thr;
  void           *arg;
};

void           *worker(char *esp, void *arg)
{
  struct worker_frame *efp;

LGOTO:
  ;
  efp = (struct worker_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct worker_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->arg = arg;
  efp->thr = efp->arg;
  pthread_mutex_lock(&(*efp->thr).mut);
  while (1) {
    while (recv_exec_send(esp, efp->thr, "any", ANY), (efp->tmp_esp = *((char **)esp)) != 0) {
      if ((char *)efp->tmp_esp < (char *)efp)
	return (void *)0 - 1;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  }
  pthread_mutex_unlock(&(*efp->thr).mut);
}

void		recv_task (struct cmd cmd, void *body)
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
  pthread_mutex_unlock(&(*thr).mut);
  pthread_cond_broadcast(&(*thr).cond);
}

void		recv_none (struct cmd cmd)
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

void		recv_rslt (struct cmd cmd, void *rbody)
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

int		try_treq   (struct cmd cmd, unsigned int id)
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

void		recv_treq (struct cmd cmd)
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

void		recv_rack (struct cmd cmd)
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

void		send_divisible(void)
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

struct handle_req_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct thread_data *_thr;
  closure_t      *_bk;
};

void		handle_req(char *esp, closure_t * _bk, struct thread_data *_thr)
{
  char           *argp;
  struct handle_req_frame *efp;

LGOTO:
  ;
  efp = (struct handle_req_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct handle_req_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  pthread_mutex_lock(&(*efp->_thr).mut);
  if ((*efp->_thr).req) {
    {
      char           *goto_fr;

      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = efp->_bk;
      goto_fr = lw_call(argp);
      if (goto_fr)
	if ((char *)goto_fr < (char *)efp) {
	  efp->tmp_esp = goto_fr;
	  return;
	} else {
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
    }
    (*efp->_thr).req = (*efp->_thr).treq_top;
  }
  pthread_mutex_unlock(&(*efp->_thr).mut);
}

void		make_and_send_task(struct thread_data *thr, int task_no, void *body)
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

struct wait_rslt_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct task_home *sub;
  long		  nsec;
  struct timeval  now;
  struct timespec t1;
  void           *body;
  struct thread_data *thr;
};

void           *wait_rslt(char *esp, struct thread_data *thr)
{
  struct wait_rslt_frame *efp;

LGOTO:
  ;
  efp = (struct wait_rslt_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct wait_rslt_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->thr = thr;
  pthread_mutex_lock(&(*efp->thr).mut);
  efp->sub = (*efp->thr).sub;
  while ((*efp->sub).stat != TASK_HOME_DONE) {
    gettimeofday(&efp->now, 0);
    efp->nsec = 1000 * (efp->now).tv_usec;
    efp->nsec += 5 * 1000 * 1000;
    (efp->t1).tv_nsec = efp->nsec > 999999999 ? efp->nsec - 999999999 : efp->nsec;
    (efp->t1).tv_sec = (efp->now).tv_sec + (efp->nsec > 999999999 ? 1 : 0);
    pthread_cond_timedwait(&(*efp->thr).cond_r, &(*efp->thr).mut, &efp->t1);
    if ((*efp->sub).stat == TASK_HOME_DONE)
      break;
    while (recv_exec_send(esp, efp->thr, (*efp->sub).task_head, (*efp->sub).req_from), (efp->tmp_esp = *((char **)esp)) != 0) {
      if ((char *)efp->tmp_esp < (char *)efp)
	return (void *)0 - 1;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  }
  efp->body = (*efp->sub).body;
  (*efp->thr).sub = (*efp->sub).next;
  (*efp->sub).next = (*efp->thr).treq_free;
  (*efp->thr).treq_free = efp->sub;
  pthread_mutex_unlock(&(*efp->thr).mut);
  return efp->body;
}

struct main_frame {
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

int		main       (int argc, char **argv)
{
  struct main_frame *efp;
  char		  estack  [8192];
  char           *esp = estack;

LGOTO:
  ;
  efp = (struct main_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->argc = argc;
  efp->argv = argv;
  {
    efp->i = 0;
    while (efp->i < 512) {
      (cmd_queue[efp->i]).next = &cmd_queue[(efp->i + 1) % 512];
      {
	efp->j = 0;
	while (efp->j < 5) {
	  ((cmd_queue[efp->i]).cmd.v)[efp->j] = cmd_v_buf[efp->i][efp->j];
	  (efp->j)++;
	}
      }
      (efp->i)++;
    }
  }
  cmd_in = &cmd_queue[0];
  cmd_out = &cmd_queue[0];
  while (systhr_create(esp, exec_queue_cmd, NULL), (efp->tmp_esp = *((char **)esp)) != 0) {
    if ((char *)efp->tmp_esp < (char *)efp)
      return (int)0 - 1;
    efp->tmp_esp = 0;
    goto LGOTO;
  }
  {
    efp->i = 0;
    while (efp->i < 32) {
      (snr_queue[efp->i]).next = &snr_queue[(efp->i + 1) % 32];
      {
	efp->j = 0;
	while (efp->j < 2) {
	  ((snr_queue[efp->i]).cmd.v)[efp->j] = snr_v_buf[efp->i][efp->j];
	  (efp->j)++;
	}
      }
      (efp->i)++;
    }
  }
  snr_in = &snr_queue[0];
  snr_out = &snr_queue[0];
  while (systhr_create(esp, send_none_rack, NULL), (efp->tmp_esp = *((char **)esp)) != 0) {
    if ((char *)efp->tmp_esp < (char *)efp)
      return (int)0 - 1;
    efp->tmp_esp = 0;
    goto LGOTO;
  }
  num_thrs = efp->argc > 1 ? atoi((efp->argv)[1]) : 1;
  {
    efp->i = 0;
    while (efp->i < num_thrs) {
      {
	efp->thr = threads + efp->i;
	(*efp->thr).id = efp->i;
	(*efp->thr).w_rack = 0;
	(*efp->thr).w_none = 0;
	(*efp->thr).ndiv = 0;
	pthread_mutex_init(&(*efp->thr).mut, 0);
	pthread_mutex_init(&(*efp->thr).rack_mut, 0);
	pthread_cond_init(&(*efp->thr).cond, 0);
	pthread_cond_init(&(*efp->thr).cond_r, 0);
	sprintf((*efp->thr).id_str, "%d", efp->i);
	efp->tx = (struct task *)malloc(sizeof(struct task) * 512);
	(*efp->thr).task_top = 0;
	(*efp->thr).task_free = efp->tx;
	{
	  efp->j = 0;
	  while (efp->j < 511) {
	    ((efp->tx)[efp->j]).prev = &(efp->tx)[efp->j + 1];
	    ((efp->tx)[efp->j + 1]).next = &(efp->tx)[efp->j];
	    (efp->j)++;
	  }
	}
	((efp->tx)[0]).next = 0;
	((efp->tx)[511]).prev = 0;
	efp->hx = (struct task_home *)malloc(sizeof(struct task_home) * 512);
	(*efp->thr).treq_top = 0;
	(*efp->thr).treq_free = efp->hx;
	(*efp->thr).sub = 0;
	{
	  efp->j = 0;
	  while (efp->j < 511) {
	    ((efp->hx)[efp->j]).next = &(efp->hx)[efp->j + 1];
	    (efp->j)++;
	  }
	}
	((efp->hx)[511]).next = 0;
	while (systhr_create(esp, worker, efp->thr), (efp->tmp_esp = *((char **)esp)) != 0) {
	  if ((char *)efp->tmp_esp < (char *)efp)
	    return (int)0 - 1;
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
      }
      (efp->i)++;
    }
  }
  efp->cmd = recv_command();
  while ((efp->cmd).c > 0 ? (0 != strcmp(((efp->cmd).v)[0], "exit") ? 1 : 0) : 0) {
    if (0 == strcmp(((efp->cmd).v)[0], "task")) {
      recv_task(efp->cmd, efp->dummy);
    } else if (0 == strcmp(((efp->cmd).v)[0], "rslt")) {
      recv_rslt(efp->cmd, efp->dummy);
    } else if (0 == strcmp(((efp->cmd).v)[0], "treq")) {
      recv_treq(efp->cmd);
    } else if (0 == strcmp(((efp->cmd).v)[0], "none")) {
      recv_none(efp->cmd);
    } else if (0 == strcmp(((efp->cmd).v)[0], "rack")) {
      recv_rack(efp->cmd);
    } else {
      proto_error("wrong cmd", efp->cmd);
    }
    efp->cmd = recv_command();
  }
  exit(0);
}
