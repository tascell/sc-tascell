#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include<getopt.h>

int             connect_to(char *hostname, unsigned short port);

void            close_socket(int socket);

int             send_char(char, int);

int             send_string(char *str, int socket);

int             send_fmt_string(int socket, char *fmt_string,...);

int             send_binary(void *src, unsigned long elm_size, unsigned long n_elm, int socket);

int             receive_char(int socket);

char           *receive_line(char *buf, int maxlen, int socket);

int             receive_binary(void *dst, unsigned long elm_size, unsigned long n_elm, int socket);
enum node {
OUTSIDE = -1, INSIDE = -2, ANY = -3, PARENT = -4, TERM = -5};
enum command {
  TASK, RSLT, TREQ, NONE, RACK, EXIT, WRNG
};
extern char    *cmd_strings[];

struct cmd {
  enum command    w;
  int             c;
  enum node       node;
  enum node       v[4][16];
};

struct cmd_list {
  struct cmd      cmd;
  void           *body;
  int             task_no;
  struct cmd_list *next;
};

void            proto_error(char const *str, struct cmd *pcmd);

void            read_to_eol(void);

void            recv_rslt(struct cmd *, void *);

void            recv_task(struct cmd *, void *);

void            recv_treq(struct cmd *);

void            recv_rack(struct cmd *);

void            recv_none(struct cmd *);
struct task;
struct thread_data;
void            (*task_doers[256]) (struct thread_data *, void *);
void            (*task_senders[256]) (void *);
void           *(*task_receivers[256]) ();
void            (*rslt_senders[256]) (void *);
void            (*rslt_receivers[256]) (void *);
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
  int             task_no;
  void           *body;
  int             ndiv;
  enum node       rslt_to;
  enum node       rslt_head[16];
};

struct task_home {
  enum task_home_stat stat;
  int             id;
  int             task_no;
  enum node       req_from;
  struct task_home *next;
  void           *body;
  enum node       task_head[16];
};

struct thread_data {
  struct task_home *req;
  int             id;
  int             w_rack;
  int             w_none;
  int             ndiv;
  struct task    *task_free;
  struct task    *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t  cond;
  pthread_cond_t  cond_r;
};

void            make_and_send_task(struct thread_data *thr, int task_no, void *body);

void           *wait_rslt(struct thread_data *thr);

int             serialize_cmdname(char *buf, enum command w);

int             deserialize_cmdname(enum command *buf, char *str);

int             serialize_arg(char *buf, enum node *arg);

enum node       deserialize_node(char *str);

int             deserialize_arg(enum node *buf, char *str);

int             serialize_cmd(char *buf, struct cmd *pcmd);

int             deserialize_cmd(struct cmd *pcmd, char *str);

int             copy_address(enum node *dst, enum node *src);

int             systhr_create(void *(*start_func) (void *), void *arg){
  int             status = 0;
  pthread_t       tid;
  pthread_attr_t  attr;

  pthread_attr_init(&attr);
  status = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0)
    status = pthread_create(&tid, &attr, start_func, arg);
  else
    status = pthread_create(&tid, 0, start_func, arg);
  return status;
}

void            mem_error(char const *str)
{
  fputs(str, stderr);
  fputc('\n', stderr);
}

void            proto_error(char const *str, struct cmd *pcmd)
{
  int             i;
  char            buf[1280];

  serialize_cmd(buf, pcmd);
  fputs(str, stderr);
  fputc('>', stderr);
  fputc(' ', stderr);
  fputs(buf, stderr);
  fputc('\n', stderr);
}
pthread_mutex_t send_mut;
int             sv_socket;

void            read_to_eol(void)
{
  int             c;

  while (EOF != (c = receive_char(sv_socket))) {
    if (c == '\n')
      break;
  }
}

void            write_eol(void)
{
  send_char('\n', sv_socket);
}

void            flush_send(void)
{
  fflush(stdout);
}
char            buf[1280];
struct cmd      cmd_buf;

struct cmd     *read_command()
{
  char            p;
  char            c;
  char           *b = buf;
  int             cmdc;

  receive_line(b, 1280, sv_socket);
  cmd_buf.node = OUTSIDE;
  deserialize_cmd(&cmd_buf, b);
  return &cmd_buf;
}

void            send_out_command(struct cmd *pcmd, void *body, int task_no)
{
  char            send_buf[1280];
  int             ret;
  enum command    w;

  w = pcmd->w;
  serialize_cmd(buf, pcmd);
  pthread_mutex_lock(&send_mut);
  send_string(buf, sv_socket);
  write_eol();
  if (body)
    if (w == TASK) {
      (task_senders[task_no]) (body);
    } else if (w == RSLT) {
      (rslt_senders[task_no]) (body);
    }
  pthread_mutex_unlock(&send_mut);
}

void            proc_cmd(struct cmd *pcmd, void *body)
{
  enum command    w;

  w = pcmd->w;
  if (w == TASK) {
    recv_task(pcmd, body);
  } else if (w == RSLT) {
    recv_rslt(pcmd, body);
  } else if (w == TREQ) {
    recv_treq(pcmd);
  } else if (w == NONE) {
    recv_none(pcmd);
  } else if (w == RACK) {
    recv_rack(pcmd);
  } else if (w == EXIT) {
    exit(0);
  } else {
    proto_error("wrong cmd", pcmd);
  }
}

void            send_command(struct cmd *pcmd, void *body, int task_no)
{
  if (pcmd->node == OUTSIDE)
    send_out_command(pcmd, body, task_no);
  else
    proc_cmd(pcmd, body);
}
struct thread_data threads[64];
unsigned int    num_thrs;

void            flush_treq_with_none(struct thread_data *thr)
{
  struct cmd      rcmd;
  struct task_home *hx;

  rcmd.c = 1;
  rcmd.w = NONE;
  while (hx = thr->treq_top) {
    rcmd.node = hx->req_from;
    copy_address((rcmd.v)[0], hx->task_head);
    send_command(&rcmd, 0, 0);
    thr->treq_top = hx->next;
    hx->next = thr->treq_free;
    thr->treq_free = hx;
  }
}

void            recv_exec_send(struct thread_data *thr, enum node *treq_head, enum node req_to)
{
  struct cmd      rcmd;
  struct task    *tx;
  long            delay;
  int             old_ndiv;

  while (thr->w_none > 0) {
    pthread_cond_wait(&thr->cond, &thr->mut);
    if (thr->sub && thr->sub->stat == TASK_HOME_DONE)
      return;
  }
  tx = thr->task_free;
  tx->stat = TASK_ALLOCATED;
  if (!tx)
    mem_error("Not enough task memory");
  thr->task_top = tx;
  thr->task_free = tx->prev;
  delay = 2 * 1000 * 1000;
  rcmd.c = 2;
  if (num_thrs > 1)
    rcmd.node = req_to;
  else
    rcmd.node = OUTSIDE;
  rcmd.w = TREQ;
  (rcmd.v)[0][0] = thr->id;
  (rcmd.v)[0][1] = TERM;
  copy_address((rcmd.v)[1], treq_head);
  do {
    flush_treq_with_none(thr);
    tx->stat = TASK_ALLOCATED;
    {
      pthread_mutex_unlock(&thr->mut);
      send_command(&rcmd, 0, 0);
      pthread_mutex_lock(&thr->mut);
    }
    while (1) {
      if (tx->stat != TASK_INITIALIZED && thr->sub && thr->sub->stat == TASK_HOME_DONE) {
        if (tx->stat != TASK_NONE)
          (thr->w_none)++;
        goto Lnone;
      }
      if (tx->stat != TASK_ALLOCATED)
        break;
      pthread_cond_wait(&thr->cond, &thr->mut);
    }
    if (tx->stat == TASK_NONE) {
      struct timespec t1;
      struct timeval  now;
      long            nsec;

      gettimeofday(&now, 0);
      nsec = now.tv_usec * 1000;
      nsec += delay;
      delay += delay;
      if (delay > 40 * 1000 * 1000)
        delay = 40 * 1000 * 1000;
      t1.tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
      t1.tv_sec = now.tv_sec + (nsec > 999999999 ? 1 : 0);
      pthread_cond_timedwait(&thr->cond_r, &thr->mut, &t1);
      if (thr->sub && thr->sub->stat == TASK_HOME_DONE)
        goto Lnone;
    }
  } while (tx->stat != TASK_INITIALIZED);
  tx->stat = TASK_STARTED;
  old_ndiv = thr->ndiv;
  thr->ndiv = tx->ndiv;
  pthread_mutex_unlock(&thr->mut);
  (task_doers[tx->task_no]) (thr, tx->body);
  pthread_mutex_lock(&thr->mut);
  rcmd.w = RSLT;
  rcmd.c = 1;
  rcmd.node = tx->rslt_to;
  copy_address((rcmd.v)[0], tx->rslt_head);
  send_command(&rcmd, tx->body, tx->task_no);
  flush_treq_with_none(thr);
  pthread_mutex_lock(&thr->rack_mut);
  (thr->w_rack)++;
  pthread_mutex_unlock(&thr->rack_mut);
  thr->ndiv = old_ndiv;
Lnone:{
    thr->task_free = tx;
    thr->task_top = tx->next;
  }
}

void           *worker(void *arg)
{
  struct thread_data *thr = arg;

  pthread_mutex_lock(&thr->mut);
  while (1) {
    recv_exec_send(thr, (enum node[2]){
      ANY, TERM
    }, ANY);
  }
  pthread_mutex_unlock(&thr->mut);
}

void            recv_task(struct cmd *pcmd, void *body)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int    id;
  int             task_no;
  size_t          len;

  if (pcmd->c < 4)
    proto_error("wrong-task", pcmd);
  task_no = (pcmd->v)[3][0];
  if (pcmd->node == OUTSIDE) {
    body = (task_receivers[task_no]) ();
    read_to_eol();
  }
  id = (pcmd->v)[2][0];
  if (!(id < num_thrs))
    proto_error("wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&thr->mut);
  tx = thr->task_top;
  tx->rslt_to = pcmd->node;
  copy_address(tx->rslt_head, (pcmd->v)[1]);
  tx->ndiv = (pcmd->v)[0][0];
  tx->task_no = task_no;
  tx->body = body;
  tx->stat = TASK_INITIALIZED;
  pthread_mutex_unlock(&thr->mut);
  pthread_cond_signal(&thr->cond);
}

void            recv_none(struct cmd *pcmd)
{
  struct thread_data *thr;
  unsigned int    id;
  size_t          len;

  if (pcmd->c < 1)
    proto_error("Wrong none", pcmd);
  id = (pcmd->v)[0][0];
  if (!(id < num_thrs))
    proto_error("Wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&thr->mut);
  if (thr->w_none > 0)
    (thr->w_none)--;
  else
    thr->task_top->stat = TASK_NONE;
  pthread_mutex_unlock(&thr->mut);
  pthread_cond_signal(&thr->cond);
}

void            recv_rslt(struct cmd *pcmd, void *body)
{
  struct cmd      rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int    tid;
  unsigned int    sid;

  if (pcmd->c < 1)
    proto_error("Wrong rslt", pcmd);
  tid = (pcmd->v)[0][0];
  if (!(tid < num_thrs))
    proto_error("wrong rslt-head", pcmd);
  sid = (pcmd->v)[0][1];
  if (TERM == sid)
    proto_error("Wrong rslt-head (no task-id)", pcmd);
  thr = threads + tid;
  pthread_mutex_lock(&thr->mut);
  hx = thr->sub;
  while (hx && hx->id != sid) {
    hx = hx->next;
  }
  if (!hx)
    proto_error("Wrong rslt-head (specified task not exists)", pcmd);
  if (pcmd->node == OUTSIDE) {
    (rslt_receivers[hx->task_no]) (hx->body);
    read_to_eol();
  } else if (pcmd->node == INSIDE) {
    hx->body = body;
  } else {
    proto_error("Wrong cmd.node", pcmd);
  }
  rcmd.c = 1;
  rcmd.node = pcmd->node;
  rcmd.w = RACK;
  copy_address((rcmd.v)[0], hx->task_head);
  hx->stat = TASK_HOME_DONE;
  if (hx == thr->sub) {
    pthread_cond_signal(&thr->cond_r);
    pthread_cond_signal(&thr->cond);
  }
  pthread_mutex_unlock(&thr->mut);
  send_command(&rcmd, 0, 0);
}

int             try_treq(struct cmd *pcmd, unsigned int id)
{
  struct task_home *hx;
  struct thread_data *thr;
  int             avail = 0;

  thr = threads + id;
  pthread_mutex_lock(&thr->mut);
  pthread_mutex_lock(&thr->rack_mut);
  if (thr->task_top && (thr->task_top->stat == TASK_STARTED || thr->task_top->stat == TASK_INITIALIZED) && thr->w_rack == 0)
    avail = 1;
  pthread_mutex_unlock(&thr->rack_mut);
  if (avail) {
    hx = thr->treq_free;
    if (!hx)
      mem_error("Not enough task-home memory");
    thr->treq_free = hx->next;
    hx->next = thr->treq_top;
    hx->stat = TASK_HOME_ALLOCATED;
    copy_address(hx->task_head, (pcmd->v)[0]);
    if (pcmd->node != OUTSIDE)
      hx->req_from = INSIDE;
    else
      hx->req_from = OUTSIDE;
    thr->treq_top = hx;
    thr->req = hx;
  }
  pthread_mutex_unlock(&thr->mut);
  return avail;
}

void            recv_treq(struct cmd *pcmd)
{
  struct cmd      rcmd;
  unsigned int    id;

  if (pcmd->c < 2)
    proto_error("Wrong treq", pcmd);
  if ((pcmd->v)[1][0] == ANY) {
    int             myid;
    int             d;

    myid = (pcmd->v)[0][0];
    for (d = 0; d < num_thrs; d++) {
      id = (1 + d + myid) % num_thrs;
      if (pcmd->node != OUTSIDE && id == myid)
        continue;
      if (try_treq(pcmd, id))
        break;
    }
    if (d != num_thrs)
      return;
  } else {
    id = (pcmd->v)[1][0];
    if (!(id < num_thrs))
      proto_error("Wrong task-head", pcmd);
    if (try_treq(pcmd, id))
      return;
  }
  if (pcmd->node == ANY)
    if ((pcmd->v)[0][0] == 0) {
      pcmd->node = OUTSIDE;
      send_command(pcmd, 0, 0);
      return;
    } else
      pcmd->node = INSIDE;
  rcmd.c = 1;
  rcmd.node = pcmd->node;
  rcmd.w = NONE;
  copy_address((rcmd.v)[0], (pcmd->v)[0]);
  send_command(&rcmd, 0, 0);
}

void            recv_rack(struct cmd *pcmd)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int    id;
  size_t          len;

  if (pcmd->c < 1)
    proto_error("Wrong rack", pcmd);
  id = (pcmd->v)[0][0];
  if (!(id < num_thrs))
    proto_error("Wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&thr->rack_mut);
  (thr->w_rack)--;
  pthread_mutex_unlock(&thr->rack_mut);
}

void            handle_req(int (*_bk) (void), struct thread_data *_thr){
  pthread_mutex_lock(&_thr->mut);
  if (_thr->req) {
    _bk();
    _thr->req = _thr->treq_top;
  }
  pthread_mutex_unlock(&_thr->mut);
}

void            make_and_send_task(struct thread_data *thr, int task_no, void *body)
{
  struct cmd      tcmd;
  struct task_home *hx = thr->treq_top;

  thr->treq_top = hx->next;
  hx->next = thr->sub;
  thr->sub = hx;
  hx->task_no = task_no;
  hx->body = body;
  hx->id = hx->next ? hx->next->id + 1 : 0;
  hx->stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = hx->req_from;
  tcmd.w = TASK;
  (tcmd.v)[0][0] = ++thr->ndiv;
  (tcmd.v)[0][1] = TERM;
  (tcmd.v)[1][0] = thr->id;
  (tcmd.v)[1][1] = hx->id;
  (tcmd.v)[1][2] = TERM;
  copy_address((tcmd.v)[2], hx->task_head);
  (tcmd.v)[3][0] = task_no;
  (tcmd.v)[3][1] = TERM;
  send_command(&tcmd, body, task_no);
}

void           *wait_rslt(struct thread_data *thr)
{
  void           *body;
  struct timespec t1;
  struct timeval  now;
  long            nsec;
  struct task_home *sub;

  pthread_mutex_lock(&thr->mut);
  sub = thr->sub;
  while (sub->stat != TASK_HOME_DONE) {
    gettimeofday(&now, 0);
    nsec = 1000 * now.tv_usec;
    nsec += 5 * 1000 * 1000;
    t1.tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
    t1.tv_sec = now.tv_sec + (nsec > 999999999 ? 1 : 0);
    pthread_cond_timedwait(&thr->cond_r, &thr->mut, &t1);
    thr->task_top->stat = TASK_STARTED;
    if (sub->stat == TASK_HOME_DONE)
      break;
    recv_exec_send(thr, sub->task_head, sub->req_from);
  }
  body = sub->body;
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  pthread_mutex_unlock(&thr->mut);
  return body;
}

struct runtime_option {
  int             num_thrs;
  char            sv_hostname[256];
  unsigned short  port;
};
static struct runtime_option option;

void            usage(int argc, char **argv)
{
  fprintf(stderr, "Usage: %s [-s hostname] [-p port-num] [-n n-threads]\n", argv[0]);
  exit(1);
}

void            set_option(int argc, char **argv)
{
  int             i;
  int             ch;

  option.num_thrs = 1;
  (option.sv_hostname)[0] = '\x0';
  option.port = 8888;
  while (-1 != (ch = getopt(argc, argv, "n:s:p:"))) {
    for (i = 0; i < argc; i++) {
      switch (ch) {
      case 'n':
        option.num_thrs = atoi(optarg);
        break;
      case 's':
        if (strcmp("stdout", optarg)) {
          strncpy(option.sv_hostname, optarg, 256);
          (option.sv_hostname)[256 - 1] = 0;
        } else
          (option.sv_hostname)[0] = '\x0';
        break;
      case 'p':
        option.port = atoi(optarg);
        break;
      case 'h':
        usage(argc, argv);
        break;
      default:
        fprintf(stderr, "Unknown option: %c\n", ch);
        usage(argc, argv);
        break;
      }
    }
  }
  return;
}

int             main(int argc, char **argv)
{
  int             i;
  int             j;
  struct cmd     *pcmd;

  set_option(argc, argv);
  sv_socket = '\x0' == (option.sv_hostname)[0] ? -1 : connect_to(option.sv_hostname, option.port);
  pthread_mutex_init(&send_mut, 0);
  num_thrs = (option.num_thrs);
  for (i = 0; i < num_thrs; i++) {
    {
      struct thread_data *thr = threads + i;
      struct task    *tx;
      struct task_home *hx;

      thr->req = 0;
      thr->id = i;
      thr->w_rack = 0;
      thr->w_none = 0;
      thr->ndiv = 0;
      pthread_mutex_init(&thr->mut, 0);
      pthread_mutex_init(&thr->rack_mut, 0);
      pthread_cond_init(&thr->cond, 0);
      pthread_cond_init(&thr->cond_r, 0);
      tx = (struct task *)malloc(sizeof(struct task) * 256);
      thr->task_top = 0;
      thr->task_free = tx;
      for (j = 0; j < 256 - 1; j++) {
        (tx[j]).prev = &tx[j + 1];
        (tx[j + 1]).next = &tx[j];
      }
      (tx[0]).next = 0;
      (tx[256 - 1]).prev = 0;
      hx = (struct task_home *)malloc(sizeof(struct task_home) * 256);
      thr->treq_top = 0;
      thr->treq_free = hx;
      thr->sub = 0;
      for (j = 0; j < 256 - 1; j++) {
        (hx[j]).next = &hx[j + 1];
      }
      (hx[256 - 1]).next = 0;
    }
  }
  for (i = 0; i < num_thrs; i++) {
    {
      struct thread_data *thr = threads + i;

      systhr_create(worker, thr);
    }
  }
  while (1) {
    pcmd = read_command();
    proc_cmd(pcmd, 0);
  }
  exit(0);
}
