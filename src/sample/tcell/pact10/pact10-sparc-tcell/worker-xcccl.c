#include <sys/types.h>
#include <sys/processor.h>
#include <sys/procset.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<pthread.h>
#include<sys/time.h>
#include<getopt.h>

int connect_to (char *hostname, unsigned short port);

void close_socket (int socket);

int send_char (char, int);

int send_string (char *str, int socket);

int send_fmt_string (int socket, char *fmt_string, ...);

int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket);

int receive_char (int socket);

char *receive_line (char *buf, int maxlen, int socket);

int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket);
extern char *receive_buf;
extern char *receive_buf_p;
enum node
{ OUTSIDE = -1, INSIDE = -2, ANY = -3, PARENT = -4, FORWARD = -5, TERM = -99 };
enum command
{ TASK, RSLT, TREQ, NONE, BACK, RACK, DREQ, DATA, STAT, VERB, EXIT, WRNG };
extern char *cmd_strings[];
enum choose
{ CHS_RANDOM, CHS_ORDER };

struct cmd
{
  enum command w;
  int c;
  enum node node;
  enum node v[4][16];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  int task_no;
  struct cmd_list *next;
};
struct task;
struct thread_data;
void (*task_doers[256]) (struct thread_data *, void *);
void (*task_senders[256]) (void *);
void *(*task_receivers[256]) ();
void (*rslt_senders[256]) (void *);
void (*rslt_receivers[256]) (void *);

void data_allocate (int);

void data_send (int, int);

void data_receive (int, int);
struct worker_data;

void worker_init (struct thread_data *);

void _setup_data (int);

void _request_data (struct thread_data *, int, int);

void _wait_data (int, int);

void _set_exist_flag (int, int);
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
  enum node rslt_head[16];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  struct task *owner;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  enum node task_head[16];
};

struct thread_data
{
  struct task_home *req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  int last_treq;
  enum choose last_choose;
  double random_seed1;
  double random_seed2;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  void *wdptr;
  char dummy[1000];
};
enum DATA_FLAG
{ DATA_NONE, DATA_REQUESTING, DATA_EXIST };

struct dhandler_arg
{
  enum node data_to;
  enum node head[16];
  struct cmd dreq_cmd;
  struct cmd dreq_cmd_fwd;
  int start;
  int end;
};

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (struct thread_data *thr);

void proto_error (char const *str, struct cmd *pcmd);

void read_to_eol (void);

void init_data_flag (int);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_dreq (struct cmd *);

void recv_data (struct cmd *);

void recv_none (struct cmd *);

void recv_back (struct cmd *);

void print_task_list (struct task *task_top, char *name);

void print_task_home_list (struct task_home *treq_top, char *name);

void print_thread_status (struct thread_data *thr);

void print_status (struct cmd *);

void set_verbose_level (struct cmd *);

void recv_exit (struct cmd *);

int serialize_cmdname (char *buf, enum command w);

int deserialize_cmdname (enum command *buf, char *str);

int serialize_arg (char *buf, enum node *arg);

enum node deserialize_node (char *str);

int deserialize_arg (enum node *buf, char *str);

int serialize_cmd (char *buf, struct cmd *pcmd);

int deserialize_cmd (struct cmd *pcmd, char *str);

int copy_address (enum node *dst, enum node *src);

int address_equal (enum node *adr1, enum node *adr2);

struct runtime_option
{
  int num_thrs;
  char sv_hostname[256];
  unsigned short port;
  char *initial_task;
  int auto_exit;
  int affinity;
  int prefetch;
  int verbose;
};
extern struct runtime_option option;

int connect_to (char *hostname, unsigned short port);

void close_socket (int socket);

int send_char (char, int);

int send_string (char *str, int socket);

int send_fmt_string (int socket, char *fmt_string, ...);

int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket);

int receive_char (int socket);

char *receive_line (char *buf, int maxlen, int socket);

int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket);
extern char *receive_buf;
extern char *receive_buf_p;
struct runtime_option option;

int systhr_create (void *(*start_func) (void *), void *arg)
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

void mem_error (char const *str)
{
  fputs (str, stderr);
  fputc ('\n', stderr);
  exit (1);
}

int get_universal_real_time ()
{
  struct timeval now;

  gettimeofday (&now, 0);
  return 1000 * 1000 * now.tv_sec + now.tv_usec;
}

void proto_error (char const *str, struct cmd *pcmd)
{
  int i;
  char buf[1280];

  serialize_cmd (buf, pcmd);
  fprintf (stderr, "(%d): %s> %s\n", get_universal_real_time (), str, buf);
}

pthread_mutex_t send_mut;
int sv_socket;

void read_to_eol (void)
{
  int c;

  while (EOF != (c = receive_char (sv_socket)))
    {
      if (c == '\n')
        break;
      else;
    }
}

void write_eol (void)
{
  send_char ('\n', sv_socket);
}

void flush_send (void)
{
  if (sv_socket < 0)
    fflush (stdout);
  else;
}

char buf[1280];
struct cmd cmd_buf;

struct cmd *read_command ()
{
  char p;
  char c;
  char *b = buf;
  int cmdc;

  receive_line (b, 1280, sv_socket);
  cmd_buf.node = OUTSIDE;
  {
  }
  deserialize_cmd (&cmd_buf, b);
  return &cmd_buf;
}

char send_buf[1280];

void send_out_command (struct cmd *pcmd, void *body, int task_no)
{
  int ret;
  enum command w;

  w = pcmd->w;
  pthread_mutex_lock (&send_mut);
  serialize_cmd (send_buf, pcmd);
  send_string (send_buf, sv_socket);
  write_eol ();
  if (body)
    if (w == TASK)
      {
        (task_senders[task_no]) (body);
        write_eol ();
      }
    else if (w == RSLT)
      {
        (rslt_senders[task_no]) (body);
        write_eol ();
      }
    else;
  else if (w == DATA)
    {
      data_send ((pcmd->v)[1][0], (pcmd->v)[1][1]);
      write_eol ();
    }
  else;
  flush_send ();
  pthread_mutex_unlock (&send_mut);
  if (w == RSLT && option.auto_exit)
    exit (0);
  else;
}

void proc_cmd (struct cmd *pcmd, void *body)
{
  enum command w;

  w = pcmd->w;
  switch (w)
    {
    case TASK:
      recv_task (pcmd, body);
      break;
    case RSLT:
      recv_rslt (pcmd, body);
      break;
    case TREQ:
      recv_treq (pcmd);
      break;
    case NONE:
      recv_none (pcmd);
      break;
    case BACK:
      recv_back (pcmd);
      break;
    case RACK:
      recv_rack (pcmd);
      break;
    case DREQ:
      recv_dreq (pcmd);
      break;
    case DATA:
      recv_data (pcmd);
      break;
    case STAT:
      print_status (pcmd);
      break;
    case VERB:
      set_verbose_level (pcmd);
      break;
    case EXIT:
      recv_exit (pcmd);
      break;
    default:
      proto_error ("wrong cmd", pcmd);
      break;
    }
}

void send_command (struct cmd *pcmd, void *body, int task_no)
{
  if (pcmd->node == OUTSIDE)
    {
      {
      }
      send_out_command (pcmd, body, task_no);
    }
  else
    {
      {
      }
      proc_cmd (pcmd, body);
    }
}
struct thread_data threads[128];
struct thread_data *prefetch_thr;
int prefetch_thr_id;
unsigned int num_thrs;

void flush_treq_with_none (struct thread_data *thr)
{
  struct cmd rcmd;
  struct task_home *hx;

  rcmd.c = 1;
  rcmd.w = NONE;
  while (hx = thr->treq_top)
    {
      rcmd.node = hx->req_from;
      copy_address ((rcmd.v)[0], hx->task_head);
      send_command (&rcmd, 0, 0);
      thr->treq_top = hx->next;
      hx->next = thr->treq_free;
      thr->treq_free = hx;
    }
}

struct task *allocate_task (struct thread_data *thr)
{
  struct task *tx;

  tx = thr->task_free;
  tx->stat = TASK_ALLOCATED;
  if (!tx)
    mem_error ("Not enough task memory");
  else;
  thr->task_top = tx;
  thr->task_free = tx->prev;
  return tx;
}

void deallocate_task (struct thread_data *thr)
{
  struct task *tx = thr->task_top;

  thr->task_free = tx;
  thr->task_top = tx->next;
  return;
}

void timeval_plus_nsec_to_timespec (struct timespec *pts_dst,
                                    struct timeval *ptv_src, long diff)
{
  long nsec = diff + 1000 * ptv_src->tv_usec;

  pts_dst->tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
  pts_dst->tv_sec = ptv_src->tv_sec + (nsec > 999999999 ? 1 : 0);
}

int send_treq_to_initialize_task (struct thread_data *thr, enum node *treq_head,
                                  enum node req_to)
{
  struct cmd rcmd;
  long delay = 1000;
  struct task *tx = thr->task_top;

  rcmd.c = 2;
  rcmd.node = req_to;
  rcmd.w = TREQ;
  (rcmd.v)[0][0] = thr->id;
  if (req_to != ANY && thr->sub)
    {
      (rcmd.v)[0][1] = thr->sub->id;
      (rcmd.v)[0][2] = TERM;
    }
  else
    (rcmd.v)[0][1] = TERM;
  copy_address ((rcmd.v)[1], treq_head);
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
              else;
              return 0;
            }
          else;
          if (tx->stat != TASK_ALLOCATED)
            break;
          else;
          pthread_cond_wait (&thr->cond, &thr->mut);
        }
      if (tx->stat == TASK_NONE)
        {
          if (thr->sub)
            {
              struct timespec t_until;
              struct timeval now;

              gettimeofday (&now, 0);
              timeval_plus_nsec_to_timespec (&t_until, &now, delay);
              pthread_cond_timedwait (&thr->cond_r, &thr->mut, &t_until);
              delay += delay;
              if (delay > 1 * 1000 * 1000 * 1000)
                delay = 1 * 1000 * 1000 * 1000;
              else;
            }
          else;
          if (thr->sub && thr->sub->stat == TASK_HOME_DONE)
            return 0;
          else;
        }
      else;
    }
  while (tx->stat != TASK_INITIALIZED);
  return 1;
}

void recv_exec_send (struct thread_data *thr, enum node *treq_head,
                     enum node req_to)
{
  struct task *tx;
  int old_ndiv;
  struct cmd rcmd;

  while (thr->w_none > 0)
    {
      pthread_cond_wait (&thr->cond, &thr->mut);
      if (thr->sub && thr->sub->stat == TASK_HOME_DONE)
        return;
      else;
    }
  tx = allocate_task (thr);
  if (send_treq_to_initialize_task (thr, treq_head, req_to))
    {
      tx->stat = TASK_STARTED;
      old_ndiv = thr->ndiv;
      thr->ndiv = tx->ndiv;
      pthread_mutex_unlock (&thr->mut);
      {
      }
      (task_doers[tx->task_no]) (thr, tx->body);
      {
      }
      rcmd.w = RSLT;
      rcmd.c = 1;
      rcmd.node = tx->rslt_to;
      copy_address ((rcmd.v)[0], tx->rslt_head);
      send_command (&rcmd, tx->body, tx->task_no);
      (thr->w_rack)++;
      pthread_mutex_unlock (&thr->rack_mut);
      pthread_mutex_lock (&thr->mut);
      flush_treq_with_none (thr);
      thr->ndiv = old_ndiv;
    }
  else;
  tx->stat = TASK_DONE;
  deallocate_task (thr);
}

void worker_setaffinity (int pe)
{
  int pe0 = pe;
  int p;

  if (pe >= sysconf (_SC_NPROCESSORS_ONLN))
    {
      fprintf (stderr, "Error in worker-setaffnity: too large pe\n");
      exit (-1);
    }
  else;
  for (p = 0; p < 65536; p++)
    {
      if (P_ONLINE == p_online (p, P_STATUS) && 0 == pe--)
        break;
      else;
    }
  if (0 != processor_bind (P_LWPID, P_MYID, p, 0))
    {
      perror ("Failed to set CPU affinity");
      exit (-1);
    }
  else;
  return;
}

void *worker (void *arg)
{
  struct thread_data *thr = arg;
  thr->wdptr = malloc (sizeof (struct thread_data));
  if (option.affinity)
    worker_setaffinity (thr->id);
  else;
  worker_init (thr);
  pthread_mutex_lock (&thr->mut);
  while (1)
    {
      recv_exec_send (thr, (enum node[2])
                      {
                      ANY, TERM}, ANY);
    }
  pthread_mutex_unlock (&thr->mut);
}

void *prefetcher (void *thr0)
{
  enum node treq_head[2];
  enum node req_to = OUTSIDE;
  struct thread_data *thr = (struct thread_data *) thr0;

  treq_head[0] = ANY;
  treq_head[1] = TERM;
  pthread_mutex_lock (&thr->mut);
  while (1)
    {
      while (thr->task_top)
        {
          pthread_cond_wait (&thr->cond, &thr->mut);
        }
      allocate_task (thr);
      while (!send_treq_to_initialize_task (thr, treq_head, req_to))
        {
        }
      pthread_cond_broadcast (&thr->cond);
    }
  pthread_mutex_unlock (&thr->mut);
  return 0;
}

int pop_prefetched_task (struct thread_data *thr)
{
  struct task *tx_dst;
  struct task *tx_src;

  pthread_mutex_lock (&prefetch_thr->mut);
  while (!
         (prefetch_thr->task_top
          && prefetch_thr->task_top->stat == TASK_INITIALIZED))
    {
      pthread_cond_wait (&prefetch_thr->cond, &prefetch_thr->mut);
    }
  pthread_mutex_lock (&thr->mut);
  tx_dst = thr->task_top;
  tx_src = prefetch_thr->task_top;
  tx_dst->task_no = tx_src->task_no;
  tx_dst->body = tx_src->body;
  tx_dst->ndiv = tx_src->ndiv;
  tx_dst->rslt_to = tx_src->rslt_to;
  copy_address (tx_dst->rslt_head, tx_src->rslt_head);
  tx_dst->stat = TASK_INITIALIZED;
  deallocate_task (prefetch_thr);
  pthread_cond_broadcast (&thr->cond);
  pthread_cond_broadcast (&prefetch_thr->cond);
  pthread_mutex_unlock (&thr->mut);
  pthread_mutex_unlock (&prefetch_thr->mut);
  return 1;
}

int try_take_back_prefetched_task (enum node *treq_head)
{
  struct task *tx;
  int retval;

  pthread_mutex_lock (&prefetch_thr->mut);
  if ((tx = prefetch_thr->task_top) && TASK_INITIALIZED == tx->stat
      && address_equal (tx->rslt_head, treq_head))
    {
      struct cmd rcmd;

      rcmd.w = BACK;
      rcmd.c = 1;
      rcmd.node = OUTSIDE;
      copy_address ((rcmd.v)[0], treq_head);
      send_command (&rcmd, 0, 0);
      deallocate_task (prefetch_thr);
      pthread_cond_broadcast (&prefetch_thr->cond);
      retval = 1;
    }
  else
    retval = 0;
  pthread_mutex_unlock (&prefetch_thr->mut);
  return retval;
}

void recv_task (struct cmd *pcmd, void *body)
{
  struct task *tx;
  struct thread_data *thr;
  enum node id;
  int task_no;
  size_t len;

  if (pcmd->c < 4)
    proto_error ("wrong-task", pcmd);
  else;
  task_no = (pcmd->v)[3][0];
  if (pcmd->node == OUTSIDE)
    {
      body = (task_receivers[task_no]) ();
      read_to_eol ();
    }
  else;
  id = (pcmd->v)[2][0];
  if (!(id < num_thrs + (option.prefetch ? 1 : 0)))
    proto_error ("wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  tx = thr->task_top;
  tx->rslt_to = pcmd->node;
  copy_address (tx->rslt_head, (pcmd->v)[1]);
  tx->ndiv = (pcmd->v)[0][0];
  tx->task_no = task_no;
  tx->body = body;
  tx->stat = TASK_INITIALIZED;
  pthread_mutex_unlock (&thr->mut);
  pthread_cond_broadcast (&thr->cond);
}

void recv_none (struct cmd *pcmd)
{
  struct thread_data *thr;
  enum node id;
  size_t len;

  if (pcmd->c < 1)
    proto_error ("Wrong none", pcmd);
  else;
  id = (pcmd->v)[0][0];
  if (!(id < num_thrs || option.prefetch && id == num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  if (thr->w_none > 0)
    (thr->w_none)--;
  else
    thr->task_top->stat = TASK_NONE;
  pthread_cond_broadcast (&thr->cond);
  pthread_mutex_unlock (&thr->mut);
}

struct task_home *search_task_home_by_id (int id, struct task_home *hx)
{
  while (hx && hx->id != id)
    {
      hx = hx->next;
    }
  return hx;
}

void recv_back (struct cmd *pcmd)
{
  struct thread_data *thr;
  struct task *tx;
  struct task_home *hx;
  enum node thr_id;
  int tsk_id;

  if (pcmd->c < 1)
    proto_error ("Wrong back", pcmd);
  else;
  thr_id = (pcmd->v)[0][0];
  tsk_id = (pcmd->v)[0][1];
  if (!(thr_id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + thr_id;
  pthread_mutex_lock (&thr->mut);
  if (!(hx = search_task_home_by_id (tsk_id, thr->sub)))
    {
      proto_error ("Wrong rslt-head (specified task not exists)", pcmd);
      print_status (0);
      exit (1);
    }
  else;
  tx = thr->task_top;
  hx = thr->sub;
  tx->task_no = hx->task_no;
  tx->body = hx->body;
  tx->ndiv = thr->ndiv;
  tx->rslt_to = INSIDE;
  (tx->rslt_head)[0] = thr_id;
  (tx->rslt_head)[1] = hx->id;
  (tx->rslt_head)[2] = TERM;
  hx->req_from = INSIDE;
  (hx->task_head)[0] = thr_id;
  (hx->task_head)[1] = TERM;
  tx->stat = TASK_INITIALIZED;
  pthread_mutex_unlock (&thr->mut);
  pthread_cond_broadcast (&thr->cond);
}

void recv_rslt (struct cmd *pcmd, void *body)
{
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  enum node tid;
  int sid;

  if (pcmd->c < 1)
    proto_error ("Wrong rslt", pcmd);
  else;
  tid = (pcmd->v)[0][0];
  if (!(tid < num_thrs))
    proto_error ("wrong rslt-head", pcmd);
  else;
  sid = (pcmd->v)[0][1];
  if (TERM == sid)
    proto_error ("Wrong rslt-head (no task-home-id)", pcmd);
  else;
  thr = threads + tid;
  pthread_mutex_lock (&thr->mut);
  if (!(hx = search_task_home_by_id (sid, thr->sub)))
    proto_error ("Wrong rslt-head (specified task not exists)", pcmd);
  else;
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
  rcmd.c = 1;
  rcmd.node = pcmd->node;
  rcmd.w = RACK;
  copy_address ((rcmd.v)[0], hx->task_head);
  hx->stat = TASK_HOME_DONE;
  if (hx == thr->sub)
    {
      pthread_cond_broadcast (&thr->cond_r);
      pthread_cond_broadcast (&thr->cond);
    }
  else;
  pthread_mutex_unlock (&thr->mut);
  send_command (&rcmd, 0, 0);
}

char *task_stat_strings[];

int try_treq (struct cmd *pcmd, enum node id, enum node *from_addr)
{
  struct task_home *hx;
  struct thread_data *thr;
  int avail = 0;
  enum node from_head = from_addr[0];

  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  pthread_mutex_lock (&thr->rack_mut);
  if (thr->w_rack == 0
      && (thr->task_top
          && (thr->task_top->stat == TASK_STARTED
              || thr->task_top->stat == TASK_INITIALIZED)
          && ((pcmd->v)[1][0] == ANY
              || address_equal (thr->task_top->rslt_head, (pcmd->v)[1]))))
    avail = 1;
  else
    {
    }
  pthread_mutex_unlock (&thr->rack_mut);
  if (avail)
    {
      hx = thr->treq_free;
      if (!hx)
        mem_error ("Not enough task-home memory");
      else;
      thr->treq_free = hx->next;
      hx->next = thr->treq_top;
      hx->stat = TASK_HOME_ALLOCATED;
      copy_address (hx->task_head, (pcmd->v)[0]);
      if (pcmd->node != OUTSIDE)
        hx->req_from = INSIDE;
      else
        hx->req_from = OUTSIDE;
      thr->treq_top = hx;
      thr->req = hx;
    }
  else;
  pthread_mutex_unlock (&thr->mut);
  return avail;
}

double random_seed1 = 0.2403703;
double random_seed2 = 3.638732;

int my_random (int max, double *pseed1, double *pseed2)
{
  *pseed1 = *pseed1 * 3.0 + *pseed2;
  *pseed1 -= (int) *pseed1;
  return max ** pseed1;
}

int choose_treq (enum node from_node)
{
  if (0 <= from_node)
    {
      struct thread_data *thr = threads + from_node;

      thr->last_choose = (1 + thr->last_choose) % 2;
      if (CHS_RANDOM == thr->last_choose)
        return my_random (num_thrs, &thr->random_seed1, &thr->random_seed2);
      else if (CHS_ORDER == thr->last_choose)
        {
          thr->last_treq = (1 + thr->last_treq) % num_thrs;
          return thr->last_treq;
        }
      else
        return 0;
    }
  else if (PARENT == from_node)
    return my_random (num_thrs, &random_seed1, &random_seed2);
  else
    return 0;
}

void recv_treq (struct cmd *pcmd)
{
  struct cmd rcmd;
  enum node dst0;

  if (pcmd->c < 2)
    proto_error ("Wrong treq", pcmd);
  else;
  dst0 = (pcmd->v)[1][0];
  if (dst0 == ANY)
    {
      int myid;
      int start_id;
      int d;
      int id;

      myid = (pcmd->v)[0][0];
      start_id = choose_treq (myid);
      for (d = 0; d < num_thrs; d++)
        {
          id = (d + start_id) % num_thrs;
          if (pcmd->node != OUTSIDE && id == myid)
            continue;
          else;
          if (try_treq (pcmd, id, (pcmd->v)[0]))
            {
              {
              }
              break;
            }
          else;
          {
          }
        }
      if (d != num_thrs)
        return;
      else;
    }
  else if (option.prefetch && dst0 == prefetch_thr_id)
    if (pcmd->node == OUTSIDE)
      if (try_take_back_prefetched_task ((pcmd->v)[0]))
        return;
      else
        {
          (pcmd->v)[1][0] = 0;
          recv_treq (pcmd);
          return;
        }
    else
      {
        {
        }
        if (pop_prefetched_task (threads + (pcmd->v)[0][0]))
          {
            {
            }
            return;
          }
        else;
      }
  else
    {
      if (!(0 <= dst0 && dst0 < num_thrs))
        proto_error ("Wrong task-head", pcmd);
      else;
      if (try_treq (pcmd, dst0, (pcmd->v)[0]))
        {
          {
          }
          return;
        }
      else;
      {
      }
    }
  if (pcmd->node == ANY && (pcmd->v)[0][0] == 0)
    if (option.prefetch)
      {
        pcmd->node = INSIDE;
        (pcmd->v)[1][0] = prefetch_thr_id;
        (pcmd->v)[1][1] = TERM;
        send_command (pcmd, 0, 0);
        return;
      }
    else
      {
        pcmd->node = OUTSIDE;
        send_command (pcmd, 0, 0);
        return;
      }
  else;
  rcmd.c = 1;
  rcmd.node = pcmd->node == ANY ? INSIDE : pcmd->node;
  rcmd.w = NONE;
  copy_address ((rcmd.v)[0], (pcmd->v)[0]);
  send_command (&rcmd, 0, 0);
}

void recv_rack (struct cmd *pcmd)
{
  struct task *tx;
  struct thread_data *thr;
  enum node id;
  size_t len;

  if (pcmd->c < 1)
    proto_error ("Wrong rack", pcmd);
  else;
  id = (pcmd->v)[0][0];
  if (option.prefetch && prefetch_thr_id == id)
    id = 0;
  else;
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&thr->rack_mut);
  (thr->w_rack)--;
  pthread_mutex_unlock (&thr->rack_mut);
}
enum DATA_FLAG *data_flags = 0;
pthread_mutex_t data_mutex;
pthread_cond_t data_cond;

void _setup_data (int n)
{
  int i;
  enum DATA_FLAG *tmp;

  if (data_flags)
    return;
  else;
  pthread_mutex_lock (&data_mutex);
  if (!data_flags)
    {
      tmp = (enum DATA_FLAG *) malloc (n * sizeof (enum DATA_FLAG));
      for (i = 0; i < n; i++)
        {
          tmp[i] = DATA_NONE;
        }
      data_flags = tmp;
    }
  else;
  data_allocate (n);
  pthread_mutex_unlock (&data_mutex);
  return;
}

void send_dreq_for_required_range (int start, int end, struct cmd *pcmd,
                                   struct cmd *pcmd_fwd)
{
  int i;
  int j;

  pthread_mutex_lock (&data_mutex);
  for (i = start; i < end; i++)
    {
      if (data_flags[i] == DATA_NONE)
        {
          data_flags[i] = DATA_REQUESTING;
          for (j = i + 1; j < end && data_flags[j] == DATA_NONE; j++)
            {
              data_flags[j] = DATA_REQUESTING;
            }
          if (pcmd)
            {
              (pcmd->v)[2][0] = i;
              (pcmd->v)[2][1] = j;
              (pcmd->v)[2][2] = TERM;
              send_command (pcmd, 0, 0);
            }
          else;
          i = j - 1;
        }
      else if (data_flags[i] == DATA_REQUESTING)
        {
          for (j = i + 1; j < end && data_flags[j] == DATA_REQUESTING; j++)
            {
            }
          if (pcmd_fwd)
            {
              (pcmd_fwd->v)[2][0] = i;
              (pcmd_fwd->v)[2][1] = j;
              (pcmd_fwd->v)[2][2] = TERM;
              send_command (pcmd_fwd, 0, 0);
            }
          else;
          i = j - 1;
        }
      else;
    }
  pthread_mutex_unlock (&data_mutex);
}

int get_first_outside_ancestor_task_address (enum node *head, int tid, int sid)
{
  struct thread_data *thr;
  struct task_home *hx;
  int ok;

  while (1)
    {
      thr = threads + tid;
      pthread_mutex_lock (&thr->mut);
      if (!(hx = search_task_home_by_id (sid, thr->sub)))
        fprintf (stderr,
                 "Error in get-first-outside-ancestor-task-address (specified task not exists)\n");
      else;
      pthread_mutex_unlock (&thr->mut);
      if (!hx->owner)
        {
          fprintf (stderr,
                   "error in get-first-outside-ancestor-task-address: no owner found.\n");
          print_status (0);
          exit (1);
        }
      else if (hx->owner->rslt_to == OUTSIDE)
        {
          copy_address (head, hx->owner->rslt_head);
          break;
        }
      else;
      tid = (hx->owner->rslt_head)[0];
      sid = (hx->owner->rslt_head)[1];
    }
  return ok;
}

void _request_data (struct thread_data *thr, int start, int end)
{
  struct cmd cmd;
  struct task *tx;

  {
  }
  pthread_mutex_lock (&thr->mut);
  tx = thr->task_top;
  pthread_mutex_unlock (&thr->mut);
  cmd.w = DREQ;
  cmd.c = 3;
  (cmd.v)[0][0] = 0;
  (cmd.v)[0][1] = TERM;
  if (OUTSIDE == tx->rslt_to)
    copy_address ((cmd.v)[1], tx->rslt_head);
  else
    get_first_outside_ancestor_task_address ((cmd.v)[1], (tx->rslt_head)[0],
                                             (tx->rslt_head)[1]);
  cmd.node = OUTSIDE;
  {
  }
  send_dreq_for_required_range (start, end, &cmd, 0);
  {
  }
  return;
}

void _wait_data (int start, int end)
{
  int i;

  {
  }
  pthread_mutex_lock (&data_mutex);
  for (i = start; i < end; i++)
    {
      while (data_flags[i] != DATA_EXIST)
        {
          pthread_cond_wait (&data_cond, &data_mutex);
        }
    }
  pthread_mutex_unlock (&data_mutex);
  {
  }
}

void _set_exist_flag (int start, int end)
{
  int i;

  pthread_mutex_lock (&data_mutex);
  for (i = start; i < end; i++)
    {
      data_flags[i] = DATA_EXIST;
    }
  pthread_mutex_unlock (&data_mutex);
}

void *dreq_handler (void *parg0)
{
  struct dhandler_arg *parg = parg0;
  int start = parg->start;
  int end = parg->end;
  struct cmd *pcmd = &parg->dreq_cmd;
  struct cmd *pcmd_fwd = &parg->dreq_cmd_fwd;
  struct cmd data_cmd;
  int i;
  int j;

  send_dreq_for_required_range (start, end, pcmd, pcmd_fwd);
  if (parg->data_to == INSIDE)
    return;
  else;
  data_cmd.w = DATA;
  data_cmd.c = 2;
  data_cmd.node = parg->data_to;
  copy_address ((data_cmd.v)[0], parg->head);
  pthread_mutex_lock (&data_mutex);
  for (i = start; i < end; i++)
    {
      while (data_flags[i] != DATA_EXIST)
        {
          pthread_cond_wait (&data_cond, &data_mutex);
        }
      for (j = i + 1; j < end && data_flags[j] == DATA_EXIST; j++)
        {
        }
      assert (i < j);
      (data_cmd.v)[1][0] = i;
      (data_cmd.v)[1][1] = j;
      (data_cmd.v)[1][2] = TERM;
      send_command (&data_cmd, 0, 0);
      i = j - 1;
    }
  pthread_mutex_unlock (&data_mutex);
  free (parg);
  return;
}

void recv_dreq (struct cmd *pcmd)
{
  struct task *tx;
  enum node tid;
  int sid;
  struct dhandler_arg *parg;
  size_t len;

  if (pcmd->c < 3)
    proto_error ("Wrong dreq", pcmd);
  else;
  parg = (struct dhandler_arg *) malloc (sizeof (struct dhandler_arg));
  parg->data_to = pcmd->node;
  copy_address (parg->head, (pcmd->v)[0]);
  parg->dreq_cmd.w = DREQ;
  parg->dreq_cmd.c = 3;
  (parg->dreq_cmd.v)[0][0] = 0;
  (parg->dreq_cmd.v)[0][1] = TERM;
  {
    tid = (pcmd->v)[1][0];
    if (!(tid < num_thrs))
      proto_error ("wrong dreq-head", pcmd);
    else;
    sid = (pcmd->v)[1][1];
    if (TERM == sid)
      proto_error ("Wrong dreq-head (no task-home-id)", pcmd);
    else;
    get_first_outside_ancestor_task_address ((parg->dreq_cmd.v)[1], tid, sid);
  }
  parg->dreq_cmd.node = OUTSIDE;
  parg->dreq_cmd_fwd.w = DREQ;
  parg->dreq_cmd_fwd.c = 3;
  (parg->dreq_cmd_fwd.v)[0][0] = FORWARD;
  copy_address (&(parg->dreq_cmd_fwd.v)[0][1], (pcmd->v)[0]);
  copy_address ((parg->dreq_cmd_fwd.v)[1], (parg->dreq_cmd.v)[1]);
  parg->dreq_cmd_fwd.node = OUTSIDE;
  parg->start = (pcmd->v)[2][0];
  parg->end = (pcmd->v)[2][1];
  {
    pthread_t tid;

    pthread_create (&tid, 0, dreq_handler, parg);
  }
  return;
}

void recv_data (struct cmd *pcmd)
{
  int i;
  int start = (pcmd->v)[1][0];
  int end = (pcmd->v)[1][1];

  if (pcmd->c < 2)
    proto_error ("Wrong data", pcmd);
  else;
  if (pcmd->node == INSIDE)
    return;
  else;
  pthread_mutex_lock (&data_mutex);
  data_receive (start, end);
  read_to_eol ();
  for (i = start; i < end; i++)
    {
      data_flags[i] = DATA_EXIST;
    }
  pthread_cond_broadcast (&data_cond);
  pthread_mutex_unlock (&data_mutex);
  return;
}

void request_data (struct thread_data *, int, int);

void wait_data (int, int);
char *task_stat_strings[] =
  { "TASK-ALLOCTED", "TASK-INITIALIZED", "TASK-STARTED", "TASK-DONE",
"TASK-NONE", "TASK-SUSPENDED" };

void node_to_string (char *buf, enum node node)
{
  switch (node)
    {
    case OUTSIDE:
      sprintf (buf, "OUTSIDE");
      break;
    case INSIDE:
      sprintf (buf, "INSIDE");
      break;
    case ANY:
      sprintf (buf, "ANY");
      break;
    case PARENT:
      sprintf (buf, "PARENT");
      break;
    case TERM:
      sprintf (buf, "TERM");
      break;
    default:
      sprintf (buf, "%d", node);
      break;
    }
}

void print_task_list (struct task *task_top, char *name)
{
  struct task *cur;
  char buf1[1280];
  char buf2[1280];

  fprintf (stderr, "%s= {", name);
  for (cur = task_top; cur; cur = cur->next)
    {
      fprintf (stderr,
               "{stat=%s, task-no=%d, body=%p, ndiv=%d, rslt-to=%s, rslt-head=%s}, ",
               task_stat_strings[cur->stat], cur->task_no, cur->body, cur->ndiv,
               (node_to_string (buf1, cur->rslt_to), buf1),
               (serialize_arg (buf2, cur->rslt_head), buf2));
    }
  fprintf (stderr, "}, ");
  return;
}
char *task_home_stat_strings[] =
  { "TASK-HOME-ALLOCATED", "TASK-HOME-INITIALIZED", "TASK-HOME-DONE" };

void print_task_home_list (struct task_home *treq_top, char *name)
{
  struct task_home *cur;
  char buf1[1280];
  char buf2[1280];

  fprintf (stderr, "%s= {", name);
  for (cur = treq_top; cur; cur = cur->next)
    {
      fprintf (stderr,
               "{stat=%s, id=%d, owner=%p, task-no=%d, body=%p, req-from=%s, task-head=%s}, ",
               task_home_stat_strings[cur->stat], cur->id, cur->owner,
               cur->task_no, cur->body, (node_to_string (buf1, cur->req_from),
                                         buf1), (serialize_arg (buf2,
                                                                cur->task_head),
                                                 buf2));
    }
  fprintf (stderr, "}, ");
  return;
}
char *choose_strings[] = { "CHS-RANDOM", "CHS-ORDER" };

void print_thread_status (struct thread_data *thr)
{
  fprintf (stderr, "<Thread %d>\n", thr->id);
  fprintf (stderr, "req=%p, ", thr->req);
  fprintf (stderr, "w-rack=%d, ", thr->w_rack);
  fprintf (stderr, "w-none=%d, ", thr->w_none);
  fprintf (stderr, "ndiv=%d, ", thr->ndiv);
  fprintf (stderr, "last-treq=%d, ", thr->last_treq);
  fprintf (stderr, "last-choose=%s, ", choose_strings[thr->last_choose]);
  fprintf (stderr, "random-seed(1,2)=(%f,%f), ", thr->random_seed1,
           thr->random_seed2);
  print_task_list (thr->task_top, "tasks");
  print_task_home_list (thr->treq_top, "treq-top");
  print_task_home_list (thr->sub, "sub");
  fprintf (stderr, "\n");
  return;
}

void print_status (struct cmd *pcmd)
{
  int i;

  fprintf (stderr, "num-thrs: %d\n", num_thrs);
  fprintf (stderr, "prefetches: %d\n", option.prefetch);
  fprintf (stderr, "verbose-level: %d\n", option.verbose);
  if (option.prefetch)
    {
      print_task_list (prefetch_thr->task_top, "prefetched tasks");
      fputc ('\n', stderr);
    }
  else;
  for (i = 0; i < num_thrs; i++)
    {
      print_thread_status (&threads[i]);
    }
  return;
}

void set_verbose_level (struct cmd *pcmd)
{
  if (pcmd->c < 1)
    proto_error ("Wrong verb", pcmd);
  else;
  option.verbose = (pcmd->v)[0][0];
  return;
}

void recv_exit (struct cmd *pcmd)
{
  fprintf (stderr, "Received \"exit\"... terminate.\n");
  exit (0);
}

void handle_req (int (*_bk) closure (void), struct thread_data *_thr)
{
  pthread_mutex_lock (&_thr->mut);
  if (_thr->req)
    {
      _bk ();
      _thr->req = _thr->treq_top;
    }
  else;
  pthread_mutex_unlock (&_thr->mut);
}

void make_and_send_task (struct thread_data *thr, int task_no, void *body)
{
  struct cmd tcmd;
  struct task_home *hx = thr->treq_top;

  thr->treq_top = hx->next;
  hx->next = thr->sub;
  thr->sub = hx;
  hx->task_no = task_no;
  hx->body = body;
  hx->id = hx->next ? hx->next->id + 1 : 0;
  hx->owner = thr->task_top;
  hx->stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = hx->req_from;
  tcmd.w = TASK;
  (tcmd.v)[0][0] = ++thr->ndiv;
  (tcmd.v)[0][1] = TERM;
  (tcmd.v)[1][0] = thr->id;
  (tcmd.v)[1][1] = hx->id;
  (tcmd.v)[1][2] = TERM;
  copy_address ((tcmd.v)[2], hx->task_head);
  (tcmd.v)[3][0] = task_no;
  (tcmd.v)[3][1] = TERM;
  send_command (&tcmd, body, task_no);
}

void *wait_rslt (struct thread_data *thr)
{
  void *body;
  struct task_home *sub;

  pthread_mutex_lock (&thr->mut);
  sub = thr->sub;
  while (sub->stat != TASK_HOME_DONE)
    {
      if (PARENT == (sub->task_head)[0])
        {
          struct timeval now;
          struct timespec t_until;

          gettimeofday (&now, 0);
          timeval_plus_nsec_to_timespec (&t_until, &now, 1000);
          pthread_cond_timedwait (&thr->cond_r, &thr->mut, &t_until);
        }
      else;
      thr->task_top->stat = TASK_STARTED;
      if (sub->stat == TASK_HOME_DONE)
        break;
      else;
      recv_exec_send (thr, sub->task_head, sub->req_from);
    }
  body = sub->body;
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  pthread_mutex_unlock (&thr->mut);
  return body;
}

void usage (int argc, char **argv)
{
  fprintf (stderr,
           "Usage: %s [-s hostname] [-p port-num] [-n n-threads] [-i initial-task-parms] [-a] [-P n-prefetches] [-v verbosity]\n",
           argv[0]);
  exit (1);
}

void set_option (int argc, char **argv)
{
  int i;
  int ch;

  (option.sv_hostname)[0] = '\x0';
  option.port = 8888;
  option.num_thrs = 1;
  option.initial_task = 0;
  option.auto_exit = 0;
  option.affinity = 0;
  option.prefetch = 0;
  option.verbose = 0;
  while (-1 != (ch = getopt (argc, argv, "n:s:p:i:xaP:v:h")))
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
        case 'i':
          if (option.initial_task)
            free (option.initial_task);
          else;
          option.initial_task =
            (char *) malloc ((1 + strlen (optarg)) * sizeof (char));
          strcpy (option.initial_task, optarg);
          option.auto_exit = 1;
          break;
        case 'x':
          option.auto_exit = 1;
          break;
        case 'a':
          if (0 == option.affinity)
            fprintf (stderr, "setaffinity enabled.\n");
          else;
          option.affinity = 1;
          break;
        case 'P':
          option.prefetch = atoi (optarg);
          break;
        case 'v':
          option.verbose = atoi (optarg);
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
  return;
}

void initialize_task_list (struct task *tlist, int len, struct task **p_top,
                           struct task **p_free)
{
  int i;

  *p_top = 0;
  *p_free = tlist;
  for (i = 0; i < len - 1; i++)
    {
      (tlist[i]).prev = &tlist[i + 1];
      (tlist[i + 1]).next = &tlist[i];
    }
  (tlist[0]).next = 0;
  (tlist[len - 1]).prev = 0;
  for (i = 0; i < len; i++)
    {
      (tlist[i]).rslt_to = TERM;
      ((tlist[i]).rslt_head)[0] = TERM;
    }
  return;
}

void initialize_task_home_list (struct task_home *hlist, int len,
                                struct task_home **p_top,
                                struct task_home **p_free)
{
  int i;

  *p_top = 0;
  *p_free = hlist;
  for (i = 0; i < len - 1; i++)
    {
      (hlist[i]).next = &hlist[i + 1];
      (hlist[len - 1]).next = 0;
    }
  return;
}

int main (int argc, char **argv)
{
  int i;
  int j;
  struct cmd *pcmd;

  set_option (argc, argv);
  sv_socket =
    '\x0' == (option.sv_hostname)[0] ? -1 : connect_to (option.sv_hostname,
                                                        option.port);
  pthread_mutex_init (&send_mut, 0);
  pthread_mutex_init (&data_mutex, 0);
  pthread_cond_init (&data_cond, 0);
  num_thrs = option.num_thrs;
  for (i = 0; i < num_thrs + (option.prefetch ? 1 : 0); i++)
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
        thr->last_treq = i;
        thr->last_choose = CHS_RANDOM;
        {
          double r;
          double q;

          r = sqrt (0.5 + i);
          q = sqrt (r + i);
          r -= (int) r;
          thr->random_seed1 = r;
          thr->random_seed2 = q;
        }
        pthread_mutex_init (&thr->mut, 0);
        pthread_mutex_init (&thr->rack_mut, 0);
        pthread_cond_init (&thr->cond, 0);
        pthread_cond_init (&thr->cond_r, 0);
        tx = (struct task *) malloc (sizeof (struct task) * 1024);
        initialize_task_list (tx, 1024, &thr->task_top, &thr->task_free);
        hx = (struct task_home *) malloc (sizeof (struct task_home) * 1024);
        initialize_task_home_list (hx, 1024, &thr->treq_top, &thr->treq_free);
        thr->sub = 0;
      }
    }
  if (option.prefetch)
    {
      prefetch_thr = threads + num_thrs;
      prefetch_thr_id = num_thrs;
    }
  else;
  for (i = 0; i < num_thrs; i++)
    {
      systhr_create (worker, threads + i);
    }
  if (option.prefetch)
    systhr_create (prefetcher, prefetch_thr);
  else;
  if (option.initial_task)
    {
      char *p_src;
      char *p_dst;
      char header[30];

      strcpy (header, "task 0 0 0 ");
      receive_buf =
        (char *) malloc ((3 + strlen (option.initial_task) + strlen (header)) *
                         sizeof (char));
      receive_buf_p = receive_buf;
      strcpy (receive_buf, header);
      for ((p_src = option.initial_task, p_dst = receive_buf + strlen (header));
           *p_src; p_src++, p_dst++)
        {
          *p_dst = ' ' == *p_src ? '\n' : *p_src;
        }
      *p_dst++ = '\n';
      *p_dst++ = '\n';
      *p_dst = 0;
      sleep (1);
      if (option.verbose >= 1)
        fprintf (stderr, "%s", receive_buf);
      else;
    }
  else;
  while (1)
    {
      pcmd = read_command ();
      proc_cmd (pcmd, 0);
    }
  exit (0);
}
