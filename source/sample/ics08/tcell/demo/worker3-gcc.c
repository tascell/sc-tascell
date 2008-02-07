#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include<getopt.h>
#include<gtk/gtk.h>

int connect_to (char *hostname, unsigned short port);

void close_socket (int socket);

int send_char (char, int);

int send_string (char *str, int socket);

int send_fmt_string (int socket, char *fmt_string, ...);

int send_binary (void *src, int elm_size, int n_elm, int socket);

int receive_char (int socket);

char *receive_line (char *buf, int maxlen, int socket);

int receive_binary (void *dst, int elm_size, int n_elm, int socket);
enum node
{ OUTSIDE, INSIDE, ANY };

struct cmd
{
  int c;
  enum node node;
  char *v[5];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  int task_no;
  struct cmd_list *next;
};

void read_to_eol (void);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_none (struct cmd *);
struct task;
struct thread_data;
void (*task_doers[256]) (struct thread_data *, void *);
void (*task_senders[256]) (void *);
void *(*task_receivers[256]) ();
void (*rslt_senders[256]) (void *);
void (*rslt_receivers[256]) (void *);
enum task_stat
{ TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE,
    TASK_SUSPENDED };
enum task_home_stat
{ TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE };

struct task
{
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  char task_head[256];
};

struct thread_data
{
  struct task_home *req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char ndiv_buf[32];
  char tno_buf[8];
  char id_str[32];
  char buf[256];
};
extern int divisibility_flag;

void send_divisible (void);

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (struct thread_data *thr);

void send_int (int n);

int recv_int (void);

int
systhr_create (void *(*start_func) (void *), void *arg)
{
  int status = 0;
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init (&attr);
  status = pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0)
    status = pthread_create (&tid, &attr, start_func, arg);
  else
    status = pthread_create (&tid, 0, start_func, arg);
  return status;
}

void
mem_error (char const *str)
{
  fputs (str, stderr);
  fputc ('\n', stderr);
}

void
proto_error (char const *str, struct cmd *pcmd)
{
  int i;
  fputs (str, stderr);
  fputc ('>', stderr);
  fputc (' ', stderr);
  for (i = 0; i < pcmd->c - 1; i++)
    {
      fputs ((pcmd->v)[i], stderr);
      fputc (' ', stderr);
    }
  if (pcmd->c > 0)
    {
      fputs ((pcmd->v)[pcmd->c - 1], stderr);
      fputc ('\n', stderr);
    }
}

struct cmd_queue
{
  int len;
  struct cmd_list *queue;
  char *arg_buf;
  int arg_size_max;
  struct cmd_list *in;
  struct cmd_list *out;
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

struct cmd_queue *
make_cmd_queue (int length, int maxcmdc, int arg_size_max)
{
  struct cmd_queue *p_newq;
  int i;
  int j;
  p_newq = (struct cmd_queue *) malloc (sizeof (struct cmd_queue));
  p_newq->len = length;
  p_newq->queue =
    (struct cmd_list *) malloc (length * sizeof (struct cmd_list));
  p_newq->arg_buf =
    (char *) malloc (length * maxcmdc * arg_size_max * sizeof (char));
  p_newq->arg_size_max = arg_size_max;
  p_newq->in = p_newq->queue;
  p_newq->out = p_newq->queue;
  pthread_mutex_init (&p_newq->mut, 0);
  pthread_cond_init (&p_newq->cond, 0);
  for (i = 0; i < length; i++)
    {
      ((p_newq->queue)[i]).next = &(p_newq->queue)[(i + 1) % length];
      for (j = 0; j < maxcmdc; j++)
        {
          (((p_newq->queue)[i]).cmd.v)[j] =
            p_newq->arg_buf + maxcmdc * arg_size_max * i + arg_size_max * j;
        }
    }
  return p_newq;
}

void
enqueue_command (struct cmd *pcmd, void *body, int task_no,
                 struct cmd_queue *pq)
{
  int i;
  size_t len;
  struct cmd_list *q;
  int len_max = pq->arg_size_max;
  pthread_mutex_lock (&pq->mut);
  q = pq->in;
  if (q->next == pq->out)
    {
      perror ("queue overflow\n");
      exit (1);
    }
  pq->in = q->next;
  q->cmd.c = pcmd->c;
  q->cmd.node = pcmd->node;
  for (i = 0; i < pcmd->c; i++)
    {
      len = strlen ((pcmd->v)[i]);
      if (len >= len_max)
        proto_error ("too long cmd", pcmd);
      strncpy ((q->cmd.v)[i], (pcmd->v)[i], len + 1);
    }
  q->body = body;
  q->task_no = task_no;
  pthread_mutex_unlock (&pq->mut);
  pthread_cond_signal (&pq->cond);
}
struct cmd_queue *inms_queue;
struct cmd_queue *send_queue;
int divisibility_flag = 0;
char buf[1280];
struct cmd cmd_buf;
int sv_socket;

struct cmd *
recv_command ()
{
  char p;
  char c;
  char *b = buf;
  int cmdc;
  receive_line (b, 1280, sv_socket);
  cmd_buf.node = OUTSIDE;
  cmdc = 0;
  for ((p = '\x0', c = *b); c; p = c, c = *(++b))
    {
      if (c == ' ' || c == '\n')
        {
          c = '\x0';
          *b = '\x0';
        }
      else if (p == '\x0' && cmdc < 5)
        (cmd_buf.v)[cmdc++] = b;
    }
  cmd_buf.c = cmdc;
  return &cmd_buf;
}

void
send_out_command (struct cmd *pcmd)
{
  int i;
  int narg = pcmd->c;
  for (i = 0; i < narg - 1; i++)
    {
      send_string ((pcmd->v)[i], sv_socket);
      send_char (' ', sv_socket);
    }
  if (narg > 0)
    {
      send_string ((pcmd->v)[narg - 1], sv_socket);
      send_char ('\n', sv_socket);
    }
}

void
send_command (struct cmd *pcmd, void *body, int task_no)
{
  if (pcmd->node == OUTSIDE)
    enqueue_command (pcmd, body, task_no, send_queue);
  else
    enqueue_command (pcmd, body, task_no, inms_queue);
}

void *
exec_queue_cmd (void *arg)
{
  struct cmd *pcmd;
  void *body;
  pthread_mutex_t *p_mut = &inms_queue->mut;
  pthread_cond_t *p_cond = &inms_queue->cond;
  while (1)
    {
      pthread_mutex_lock (p_mut);
      while (inms_queue->in == inms_queue->out)
        {
          pthread_cond_wait (p_cond, p_mut);
        }
      pcmd = &inms_queue->out->cmd;
      body = inms_queue->out->body;
      pthread_mutex_unlock (p_mut);
      if (0 == strcmp ((pcmd->v)[0], "task"))
        {
          proto_error ("INSIDE", pcmd);
          recv_task (pcmd, body);
        }
      else if (0 == strcmp ((pcmd->v)[0], "rslt"))
        {
          proto_error ("INSIDE", pcmd);
          recv_rslt (pcmd, body);
        }
      else if (0 == strcmp ((pcmd->v)[0], "treq"))
        {
          recv_treq (pcmd);
        }
      else if (0 == strcmp ((pcmd->v)[0], "none"))
        {
          recv_none (pcmd);
        }
      else if (0 == strcmp ((pcmd->v)[0], "rack"))
        {
          recv_rack (pcmd);
        }
      else
        {
          proto_error ("wrong cmd", pcmd);
          exit (0);
        }
      pthread_mutex_lock (p_mut);
      inms_queue->out = inms_queue->out->next;
      pthread_mutex_unlock (p_mut);
    }
}

void
read_to_eol (void)
{
  int c;
  while (EOF != (c = receive_char (sv_socket)))
    {
      if (c == '\n')
        break;
    }
}

void
write_eol (void)
{
  send_char ('\n', sv_socket);
}

void
flush_send (void)
{
  fflush (stdout);
}

void *
sender_loop (void *arg)
{
  struct cmd *pcmd;
  pthread_mutex_t *p_mut = &send_queue->mut;
  pthread_cond_t *p_cond = &send_queue->cond;
  void *body;
  while (1)
    {
      pthread_mutex_lock (p_mut);
      while (send_queue->in == send_queue->out)
        {
          pthread_cond_wait (p_cond, p_mut);
        }
      pcmd = &send_queue->out->cmd;
      pthread_mutex_unlock (p_mut);
      send_out_command (pcmd);
      body = send_queue->out->body;
      if (body)
        {
          if (0 == strcmp ((pcmd->v)[0], "task"))
            {
              (task_senders[send_queue->out->task_no]) (body);
              write_eol ();
            }
          else if (0 == strcmp ((pcmd->v)[0], "rslt"))
            {
              (rslt_senders[send_queue->out->task_no]) (body);
              write_eol ();
            }
          send_queue->out->body = 0;
        }
      flush_send ();
      if (0 == strcmp ((pcmd->v)[0], "none"))
        divisibility_flag = 1;
      pthread_mutex_lock (p_mut);
      send_queue->out = send_queue->out->next;
      pthread_mutex_unlock (p_mut);
      flush_send ();
    }
}
struct thread_data threads[64];
unsigned int num_thrs;

void
flush_treq_with_none (struct thread_data *thr)
{
  struct cmd rcmd;
  struct task_home *hx;
  rcmd.c = 2;
  (rcmd.v)[0] = "none";
  while (hx = thr->treq_top)
    {
      rcmd.node = hx->req_from;
      (rcmd.v)[1] = hx->task_head;
      send_command (&rcmd, 0, 0);
      thr->treq_top = hx->next;
      hx->next = thr->treq_free;
      thr->treq_free = hx;
    }
}

void
recv_exec_send (struct thread_data *thr, char *treq_head, int req_to)
{
  struct cmd rcmd;
  struct task *tx;
  long delay;
  int old_ndiv;
  while (thr->w_none > 0)
    {
      pthread_cond_wait (&thr->cond, &thr->mut);
      if (thr->sub && thr->sub->stat == TASK_HOME_DONE)
        return;
    }
  tx = thr->task_free;
  tx->stat = TASK_ALLOCATED;
  if (!tx)
    mem_error ("Not enough task memory");
  thr->task_top = tx;
  thr->task_free = tx->prev;
  delay = 2 * 1000 * 1000;
  rcmd.c = 3;
  if (num_thrs > 1)
    rcmd.node = req_to;
  else
    rcmd.node = OUTSIDE;
  (rcmd.v)[0] = "treq";
  (rcmd.v)[1] = thr->id_str;
  (rcmd.v)[2] = treq_head;
  do
    {
      flush_treq_with_none (thr);
      tx->stat = TASK_ALLOCATED;
      {
        pthread_mutex_unlock (&thr->mut);
        send_command (&rcmd, 0, 0);
        pthread_mutex_lock (&thr->mut);
      }
      while (1)
        {
          if (tx->stat != TASK_INITIALIZED && thr->sub
              && thr->sub->stat == TASK_HOME_DONE)
            {
              if (tx->stat != TASK_NONE)
                (thr->w_none)++;
              goto Lnone;
            }
          if (tx->stat != TASK_ALLOCATED)
            break;
          pthread_cond_wait (&thr->cond, &thr->mut);
        }
      if (tx->stat == TASK_NONE)
        {
          struct timespec t1;
          struct timeval now;
          long nsec;

          gettimeofday (&now, 0);
          nsec = now.tv_usec * 1000;
          nsec += delay;
          delay += delay;
          if (delay > 40 * 1000 * 1000)
            delay = 40 * 1000 * 1000;
          t1.tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
          t1.tv_sec = now.tv_sec + (nsec > 999999999 ? 1 : 0);
          pthread_cond_timedwait (&thr->cond_r, &thr->mut, &t1);
          if (thr->sub && thr->sub->stat == TASK_HOME_DONE)
            goto Lnone;
        }
    }
  while (tx->stat != TASK_INITIALIZED);
  tx->stat = TASK_STARTED;
  old_ndiv = thr->ndiv;
  thr->ndiv = tx->ndiv;
  pthread_mutex_unlock (&thr->mut);
  (task_doers[tx->task_no]) (thr, tx->body);
  pthread_mutex_lock (&thr->mut);
  rcmd.c = 2;
  rcmd.node = tx->rslt_to;
  (rcmd.v)[0] = "rslt";
  (rcmd.v)[1] = tx->rslt_head;
  send_command (&rcmd, tx->body, tx->task_no);
  flush_treq_with_none (thr);
  pthread_mutex_lock (&thr->rack_mut);
  ((*thr).w_rack)++;
  pthread_mutex_unlock (&thr->rack_mut);
  thr->ndiv = old_ndiv;
Lnone:{
    thr->task_free = tx;
    thr->task_top = tx->next;
  }
}

void *
worker (void *arg)
{
  struct thread_data *thr = arg;
  pthread_mutex_lock (&thr->mut);
  while (1)
    {
      recv_exec_send (thr, "any", ANY);
    }
  pthread_mutex_unlock (&thr->mut);
}

void
recv_task (struct cmd *pcmd, void *body)
{
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  int task_no;
  size_t len;
  if (pcmd->c < 5)
    proto_error ("wrong-task", pcmd);
  task_no = atoi ((pcmd->v)[4]);
  if (pcmd->node == OUTSIDE)
    {
      body = (task_receivers[task_no]) ();
      read_to_eol ();
    }
  id = atoi ((pcmd->v)[3]);
  if (!(id < num_thrs))
    proto_error ("wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  tx = thr->task_top;
  tx->rslt_to = pcmd->node;
  len = strlen ((pcmd->v)[2]);
  if (len >= 256)
    proto_error ("Too long rslt-head for task", pcmd);
  strncpy (tx->rslt_head, (pcmd->v)[2], len + 1);
  tx->ndiv = atoi ((pcmd->v)[1]);
  tx->task_no = task_no;
  tx->body = body;
  tx->stat = TASK_INITIALIZED;
  pthread_mutex_unlock (&thr->mut);
  pthread_cond_signal (&thr->cond);
}

void
recv_none (struct cmd *pcmd)
{
  struct thread_data *thr;
  unsigned int id;
  size_t len;
  if (pcmd->c < 2)
    proto_error ("Wrong none", pcmd);
  id = atoi ((pcmd->v)[1]);
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  if (thr->w_none > 0)
    (thr->w_none)--;
  else
    thr->task_top->stat = TASK_NONE;
  pthread_mutex_unlock (&thr->mut);
  pthread_cond_signal (&thr->cond);
}

void
recv_rslt (struct cmd *pcmd, void *body)
{
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int tid;
  unsigned int sid;
  char *b;
  char h_buf[256];
  if (pcmd->c < 2)
    proto_error ("Wrong rslt", pcmd);
  b = (pcmd->v)[1];
  tid = atoi (b);
  if (!(tid < num_thrs))
    proto_error ("wrong rslt-head", pcmd);
  b = strchr (b, ':');
  if (!b)
    proto_error ("Wrong rslt-head", pcmd);
  sid = atoi (b + 1);
  thr = threads + tid;
  pthread_mutex_lock (&thr->mut);
  hx = thr->sub;
  while (hx && hx->id != sid)
    {
      hx = hx->next;
    }
  if (!hx)
    proto_error ("Wrong rslt-head", pcmd);
  if (pcmd->node == OUTSIDE)
    {
      (rslt_receivers[hx->task_no]) (hx->body);
      read_to_eol ();
    }
  else if (pcmd->node == INSIDE)
    {
      hx->body = body;
    }
  else
    {
      proto_error ("Wrong cmd.node", pcmd);
    }
  rcmd.c = 2;
  rcmd.node = pcmd->node;
  (rcmd.v)[0] = "rack";
  strncpy (h_buf, hx->task_head, 256);
  (rcmd.v)[1] = h_buf;
  hx->stat = TASK_HOME_DONE;
  if (hx == thr->sub)
    {
      pthread_cond_signal (&thr->cond_r);
      pthread_cond_signal (&thr->cond);
    }
  pthread_mutex_unlock (&thr->mut);
  send_command (&rcmd, 0, 0);
}

int
try_treq (struct cmd *pcmd, unsigned int id)
{
  struct task_home *hx;
  struct thread_data *thr;
  size_t len;
  int avail = 0;
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  pthread_mutex_lock (&thr->rack_mut);
  if (thr->task_top
      && (thr->task_top->stat == TASK_STARTED
          || thr->task_top->stat == TASK_INITIALIZED) && thr->w_rack == 0)
    avail = 1;
  pthread_mutex_unlock (&thr->rack_mut);
  if (avail)
    {
      hx = thr->treq_free;
      if (!hx)
        mem_error ("Not enough task-home memory");
      thr->treq_free = hx->next;
      hx->next = thr->treq_top;
      hx->stat = TASK_HOME_ALLOCATED;
      len = strlen ((pcmd->v)[1]);
      if (len >= 256)
        proto_error ("Too long task-head for treq", pcmd);
      strncpy (hx->task_head, (pcmd->v)[1], len + 1);
      if (pcmd->node != OUTSIDE)
        hx->req_from = INSIDE;
      else
        hx->req_from = OUTSIDE;
      thr->treq_top = hx;
      thr->req = hx;
    }
  pthread_mutex_unlock (&thr->mut);
  return avail;
}

void
recv_treq (struct cmd *pcmd)
{
  struct cmd rcmd;
  unsigned int id;
  if (pcmd->c < 3)
    proto_error ("Wrong treq", pcmd);
  if (strcmp ((pcmd->v)[2], "any") == 0)
    {
      int myid;

      myid = atoi ((pcmd->v)[1]);
      for (id = 0; id < num_thrs; id++)
        {
          if (pcmd->node != OUTSIDE && id == myid)
            continue;
          if (try_treq (pcmd, id))
            break;
        }
      if (id != num_thrs)
        return;
    }
  else
    {
      id = atoi ((pcmd->v)[2]);
      if (!(id < num_thrs))
        proto_error ("Wrong task-head", pcmd);
      if (try_treq (pcmd, id))
        return;
    }
  if (pcmd->node == ANY)
    if (atoi ((pcmd->v)[1]) == 0)
      {
        pcmd->node = OUTSIDE;
        send_command (pcmd, 0, 0);
        divisibility_flag = 1;
        return;
      }
    else
      pcmd->node = INSIDE;
  rcmd.c = 2;
  rcmd.node = pcmd->node;
  (rcmd.v)[0] = "none";
  (rcmd.v)[1] = (pcmd->v)[1];
  send_command (&rcmd, 0, 0);
}

void
recv_rack (struct cmd *pcmd)
{
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  size_t len;
  if (pcmd->c < 2)
    proto_error ("Wrong rack", pcmd);
  id = atoi ((pcmd->v)[1]);
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock (&thr->rack_mut);
  (thr->w_rack)--;
  pthread_mutex_unlock (&thr->rack_mut);
}

void
send_divisible (void)
{
  struct cmd cmd;
  divisibility_flag = 0;
  cmd.c = 1;
  cmd.node = OUTSIDE;
  (cmd.v)[0] = "dvbl";
  send_command (&cmd, 0, 0);
}

void
handle_req (int (*_bk) (void), struct thread_data *_thr)
{
  pthread_mutex_lock (&_thr->mut);
  if (_thr->req)
    {
      _bk ();
      _thr->req = _thr->treq_top;
    }
  pthread_mutex_unlock (&_thr->mut);
}

void
make_and_send_task (struct thread_data *thr, int task_no, void *body)
{
  struct cmd tcmd;
  struct task_home *hx = thr->treq_top;
  thr->treq_top = hx->next;
  hx->next = thr->sub;
  thr->sub = hx;
  hx->task_no = task_no;
  hx->body = body;
  hx->id = hx->next ? hx->next->id + 1 : 0;
  hx->stat = TASK_HOME_INITIALIZED;
  tcmd.c = 5;
  tcmd.node = hx->req_from;
  (tcmd.v)[0] = "task";
  sprintf (thr->ndiv_buf, "%d", ++thr->ndiv);
  sprintf (thr->buf, "%s:%d", thr->id_str, hx->id);
  sprintf (thr->tno_buf, "%d", task_no);
  (tcmd.v)[1] = thr->ndiv_buf;
  (tcmd.v)[2] = thr->buf;
  (tcmd.v)[3] = hx->task_head;
  (tcmd.v)[4] = thr->tno_buf;
  send_command (&tcmd, body, task_no);
}

void *
wait_rslt (struct thread_data *thr)
{
  void *body;
  struct timespec t1;
  struct timeval now;
  long nsec;
  struct task_home *sub;
  pthread_mutex_lock (&thr->mut);
  sub = thr->sub;
  while (sub->stat != TASK_HOME_DONE)
    {
      gettimeofday (&now, 0);
      nsec = 1000 * now.tv_usec;
      nsec += 5 * 1000 * 1000;
      t1.tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
      t1.tv_sec = now.tv_sec + (nsec > 999999999 ? 1 : 0);
      pthread_cond_timedwait (&thr->cond_r, &thr->mut, &t1);
      thr->task_top->stat = TASK_STARTED;
      if (sub->stat == TASK_HOME_DONE)
        break;
      recv_exec_send (thr, sub->task_head, sub->req_from);
    }
  body = sub->body;
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  pthread_mutex_unlock (&thr->mut);
  return body;
}

struct runtime_option
{
  int num_thrs;
  char sv_hostname[256];
  unsigned short port;
};
static struct runtime_option option;

void
usage (int argc, char **argv)
{
  fprintf (stderr, "Usage: %s [-s hostname] [-p port-num] [-n n-threads]\n",
           argv[0]);
  exit (1);
}

void
set_option (int argc, char **argv)
{
  int i;
  int ch;
  option.num_thrs = 1;
  (option.sv_hostname)[0] = '\x0';
  option.port = 8888;
  while (-1 != (ch = getopt (argc, argv, "n:s:p:")))
    {
      for (i = 0; i < argc; i++)
        {
          switch (ch)
            {
            case 'n':
              option.num_thrs = atoi (optarg);
              break;
            case 's':
              if (strcmp ("stdout", optarg))
                {
                  strncpy (option.sv_hostname, optarg, 256);
                  (option.sv_hostname)[256 - 1] = 0;
                }
              else
                (option.sv_hostname)[0] = '\x0';
              break;
            case 'p':
              option.port = atoi (optarg);
              break;
            case 'h':
              usage (argc, argv);
              break;
            default:
              fprintf (stderr, "Unknown option: %c\n", ch);
              usage (argc, argv);
              break;
            }
        }
    }
  return;
}

GtkWidget *window;
GtkWidget *darea;
GdkGC *gc = 0;
GdkPixmap *pixmap = 0;

GdkGC *
set_color (gushort r, gushort g, gushort b)
{
  GdkColor color;
  color.red = r;
  color.green = g;
  color.blue = b;
  gdk_color_alloc (gdk_colormap_get_system (), &color);
  gdk_gc_set_foreground (gc, &color);
  return gc;
}

void
configure_event (GtkWidget * widget, GdkEventConfigure * event, gpointer data)
{
  if (pixmap)
    gdk_pixmap_unref (pixmap);
  pixmap =
    gdk_pixmap_new ((*widget).window, ((*widget).allocation).width,
                    ((*widget).allocation).height, -1);
}

void
expose_event (GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
  gdk_draw_pixmap (widget->window,
                   (widget->style->fg_gc)[GTK_WIDGET_STATE (widget)], pixmap,
                   ((*event).area).x, ((*event).area).y, ((*event).area).x,
                   ((*event).area).y, ((*event).area).width,
                   ((*event).area).height);
}

extern gint repaint (gpointer);

int
main (int argc, char **argv)
{
  int i;
  int j;
  void *dummy;
  struct cmd *pcmd;
  gtk_init (&argc, &argv);
  set_option (argc, argv);
  {
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_show (window);
    darea = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (darea), 240*option.num_thrs, 300);
    gtk_container_add (GTK_CONTAINER (window), darea);
    gc = gdk_gc_new (window->window);
    gtk_signal_connect (GTK_OBJECT (darea), "configure_event",
                        GTK_SIGNAL_FUNC (configure_event), 0);
    gtk_signal_connect (GTK_OBJECT (darea), "expose_event",
                        GTK_SIGNAL_FUNC (expose_event), 0);
    gtk_timeout_add (33, repaint, (gpointer) darea);
    gtk_widget_show_all (window);
    systhr_create (gtk_main, 0);
  }
  sv_socket =
    '\x0' == (option.sv_hostname)[0] ? -1 : connect_to (option.sv_hostname,
                                                        option.port);
  inms_queue = make_cmd_queue (512, 5, 256);
  systhr_create (exec_queue_cmd, NULL);
  send_queue = make_cmd_queue (512, 5, 256);
  systhr_create (sender_loop, NULL);
  num_thrs = (option.num_thrs);
  for (i = 0; i < num_thrs; i++)
    {
      {
        struct thread_data *thr = threads + i;
        struct task *tx;
        struct task_home *hx;

        thr->req = 0;
        thr->id = i;
        thr->w_rack = 0;
        thr->w_none = 0;
        thr->ndiv = 0;
        pthread_mutex_init (&thr->mut, 0);
        pthread_mutex_init (&thr->rack_mut, 0);
        pthread_cond_init (&thr->cond, 0);
        pthread_cond_init (&thr->cond_r, 0);
        sprintf (thr->id_str, "%d", i);
        tx = (struct task *) malloc (sizeof (struct task) * 256);
        thr->task_top = 0;
        thr->task_free = tx;
        for (j = 0; j < 256 - 1; j++)
          {
            (tx[j]).prev = &tx[j + 1];
            (tx[j + 1]).next = &tx[j];
          }
        (tx[0]).next = 0;
        (tx[256 - 1]).prev = 0;
        hx = (struct task_home *) malloc (sizeof (struct task_home) * 256);
        thr->treq_top = 0;
        thr->treq_free = hx;
        thr->sub = 0;
        for (j = 0; j < 256 - 1; j++)
          {
            (hx[j]).next = &hx[j + 1];
          }
        (hx[256 - 1]).next = 0;
      }
    }
  for (i = 0; i < num_thrs; i++)
    {
      {
        struct thread_data *thr = threads + i;

        systhr_create (worker, thr);
      }
    }
  while (pcmd = recv_command (), pcmd->c > 0
         && 0 != strcmp ((pcmd->v)[0], "exit"))
    {
      if (0 == strcmp ((pcmd->v)[0], "task"))
        {
          recv_task (pcmd, dummy);
        }
      else if (0 == strcmp ((pcmd->v)[0], "rslt"))
        {
          recv_rslt (pcmd, dummy);
        }
      else if (0 == strcmp ((pcmd->v)[0], "treq"))
        {
          recv_treq (pcmd);
        }
      else if (0 == strcmp ((pcmd->v)[0], "none"))
        {
          recv_none (pcmd);
        }
      else if (0 == strcmp ((pcmd->v)[0], "rack"))
        {
          recv_rack (pcmd);
        }
      else
        {
          proto_error ("wrong cmd", pcmd);
        }
    }
  exit (0);
}
