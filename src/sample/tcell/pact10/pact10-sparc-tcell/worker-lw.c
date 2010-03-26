#include<stdio.h>
typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);

struct func_arg
{
  void *(*func) (char *, void *);
  void *arg;
};

void *thread_origin (void *farg);
struct systhr_create_frame;
struct recv_exec_send_frame;
struct worker_frame;
struct prefetcher_frame;
struct handle_req_frame;
struct wait_rslt_frame;
struct main_frame;

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
void (*task_doers[256]) (char *, struct thread_data *, void *);
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

void *wait_rslt (char *esp, struct thread_data *thr);

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

struct systhr_create_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  pthread_attr_t attr;
  pthread_t tid;
  int status;
  void *arg;
  void *(*start_func) (char *, void *);
};

int systhr_create (char *esp, void *(*start_func) (char *, void *), void *arg)
{
  struct func_arg *farg;
  struct func_arg *farg2;
  int status;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct systhr_create_frame *efp;

LGOTO:;
  efp = (struct systhr_create_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct systhr_create_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  status = 0;
  pthread_attr_init (&efp->attr);
  status = pthread_attr_setscope (&efp->attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0)
    status = (farg =
              (struct func_arg *) malloc (sizeof (struct func_arg)),
              farg->func = start_func, farg->arg =
              arg, pthread_create (&efp->tid, &efp->attr, thread_origin, farg));
  else
    status = (farg2 =
              (struct func_arg *) malloc (sizeof (struct func_arg)),
              farg2->func = start_func, farg2->arg =
              arg, pthread_create (&efp->tid, 0, thread_origin, farg2));
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
  char *b;
  int cmdc;

  b = buf;
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

  w = (*pcmd).w;
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
      data_send (((*pcmd).v)[1][0], ((*pcmd).v)[1][1]);
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

  w = (*pcmd).w;
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
  if ((*pcmd).node == OUTSIDE)
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
  while (hx = (*thr).treq_top)
    {
      rcmd.node = (*hx).req_from;
      copy_address ((rcmd.v)[0], (*hx).task_head);
      send_command (&rcmd, 0, 0);
      (*thr).treq_top = (*hx).next;
      (*hx).next = (*thr).treq_free;
      (*thr).treq_free = hx;
    }
}

struct task *allocate_task (struct thread_data *thr)
{
  struct task *tx;

  tx = (*thr).task_free;
  (*tx).stat = TASK_ALLOCATED;
  if (!tx)
    mem_error ("Not enough task memory");
  else;
  (*thr).task_top = tx;
  (*thr).task_free = (*tx).prev;
  return tx;
}

void deallocate_task (struct thread_data *thr)
{
  struct task *tx;

  tx = (*thr).task_top;
  (*thr).task_free = tx;
  (*thr).task_top = (*tx).next;
  return;
}

void timeval_plus_nsec_to_timespec (struct timespec *pts_dst,
                                    struct timeval *ptv_src, long diff)
{
  long nsec;

  nsec = diff + 1000 * (*ptv_src).tv_usec;
  (*pts_dst).tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
  (*pts_dst).tv_sec = (*ptv_src).tv_sec + (nsec > 999999999 ? 1 : 0);
}

int send_treq_to_initialize_task (struct thread_data *thr, enum node *treq_head,
                                  enum node req_to)
{
  struct cmd rcmd;
  long delay;
  struct task *tx;
  struct timespec t_until;
  struct timeval now;

  delay = 1000;
  tx = (*thr).task_top;
  rcmd.c = 2;
  rcmd.node = req_to;
  rcmd.w = TREQ;
  (rcmd.v)[0][0] = (*thr).id;
  if (req_to != ANY && (*thr).sub)
    {
      (rcmd.v)[0][1] = (*(*thr).sub).id;
      (rcmd.v)[0][2] = TERM;
    }
  else
    (rcmd.v)[0][1] = TERM;
  copy_address ((rcmd.v)[1], treq_head);
  do
    {
      flush_treq_with_none (thr);
      (*tx).stat = TASK_ALLOCATED;
      {
        pthread_mutex_unlock (&(*thr).mut);
        send_command (&rcmd, 0, 0);
        pthread_mutex_lock (&(*thr).mut);
      }
      while (1)
        {
          if ((*tx).stat != TASK_INITIALIZED && (*thr).sub
              && (*(*thr).sub).stat == TASK_HOME_DONE)
            {
              if ((*tx).stat != TASK_NONE)
                ((*thr).w_none)++;
              else;
              return 0;
            }
          else;
          if ((*tx).stat != TASK_ALLOCATED)
            break;
          else;
          pthread_cond_wait (&(*thr).cond, &(*thr).mut);
        }
      if ((*tx).stat == TASK_NONE)
        {
          if ((*thr).sub)
            {
              gettimeofday (&now, 0);
              timeval_plus_nsec_to_timespec (&t_until, &now, delay);
              pthread_cond_timedwait (&(*thr).cond_r, &(*thr).mut, &t_until);
              delay += delay;
              if (delay > 1 * 1000 * 1000 * 1000)
                delay = 1 * 1000 * 1000 * 1000;
              else;
            }
          else;
          if ((*thr).sub && (*(*thr).sub).stat == TASK_HOME_DONE)
            return 0;
          else;
        }
      else;
    }
  while ((*tx).stat != TASK_INITIALIZED);
  return 1;
}

struct recv_exec_send_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct cmd rcmd;
  int old_ndiv;
  struct task *tx;
  int ifexp_result;
  int ifexp_result2;
  enum node req_to;
  enum node *treq_head;
  struct thread_data *thr;
};

void recv_exec_send (char *esp, struct thread_data *thr, enum node *treq_head,
                     enum node req_to)
{
  int ifexp_result2;
  int ifexp_result;
  struct task *tx;
  int old_ndiv;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct recv_exec_send_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct recv_exec_send_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct recv_exec_send_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL;
        }
      goto L_CALL;
    }
  else;
  efp = (struct recv_exec_send_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct recv_exec_send_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  while ((*thr).w_none > 0)
    {
      pthread_cond_wait (&(*thr).cond, &(*thr).mut);
      {
        if ((*thr).sub)
          {
            if ((*(*thr).sub).stat == TASK_HOME_DONE)
              ifexp_result2 = 1;
            else
              ifexp_result2 = 0;
            ifexp_result = ifexp_result2;
          }
        else
          ifexp_result = 0;
        if (ifexp_result)
          return;
        else;
      }
    }
  tx = allocate_task (thr);
  if (send_treq_to_initialize_task (thr, treq_head, req_to))
    {
      (*tx).stat = TASK_STARTED;
      old_ndiv = (*thr).ndiv;
      (*thr).ndiv = (*tx).ndiv;
      pthread_mutex_unlock (&(*thr).mut);
      {
      }
      {
        new_esp = esp;
        while ((task_doers[(*tx).task_no]) (new_esp, thr, (*tx).body),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->old_ndiv = old_ndiv;
            efp->tx = tx;
            efp->ifexp_result = ifexp_result;
            efp->ifexp_result2 = ifexp_result2;
            efp->req_to = req_to;
            efp->treq_head = treq_head;
            efp->thr = thr;
            efp->call_id = 0;
            return;
          L_CALL:;
            old_ndiv = efp->old_ndiv;
            tx = efp->tx;
            ifexp_result = efp->ifexp_result;
            ifexp_result2 = efp->ifexp_result2;
            req_to = efp->req_to;
            treq_head = efp->treq_head;
            thr = efp->thr;
            new_esp = esp + 1;
          }
      }
      {
      }
      (efp->rcmd).w = RSLT;
      (efp->rcmd).c = 1;
      (efp->rcmd).node = (*tx).rslt_to;
      copy_address (((efp->rcmd).v)[0], (*tx).rslt_head);
      send_command (&efp->rcmd, (*tx).body, (*tx).task_no);
      ((*thr).w_rack)++;
      pthread_mutex_unlock (&(*thr).rack_mut);
      pthread_mutex_lock (&(*thr).mut);
      flush_treq_with_none (thr);
      (*thr).ndiv = old_ndiv;
    }
  else;
  (*tx).stat = TASK_DONE;
  deallocate_task (thr);
}

void worker_setaffinity (int pe)
{
  int pe0;
  int p;

  pe0 = pe;
  if (pe >= sysconf (_SC_NPROCESSORS_ONLN))
    {
      fprintf (stderr, "Error in worker-setaffnity: too large pe\n");
      exit (-1);
    }
  else;
  {
    p = 0;
    for (; p < 65536; p++)
      {
        if (P_ONLINE == p_online (p, P_STATUS) && 0 == pe--)
          break;
        else;
      }
  }
  if (0 != processor_bind (P_LWPID, P_MYID, p, 0))
    {
      perror ("Failed to set CPU affinity");
      exit (-1);
    }
  else;
  return;
}

struct worker_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct thread_data *thr;
  void *arg;
};

void *worker (char *esp, void *arg)
{
  struct thread_data *thr;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct worker_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct worker_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct worker_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL2;
        }
      goto L_CALL2;
    }
  else;
  efp = (struct worker_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct worker_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  thr = arg;
  (*thr).wdptr = malloc (sizeof (struct thread_data));
  if (option.affinity)
    worker_setaffinity ((*thr).id);
  else;
  worker_init (thr);
  pthread_mutex_lock (&(*thr).mut);
  while (1)
    {
      new_esp = esp;
      while (recv_exec_send (new_esp, thr, (enum node[2])
                             {
                             ANY, TERM}, ANY), __builtin_expect ((efp->tmp_esp =
                                                                  *((char **)
                                                                    esp)) != 0,
                                                                 0))
        {
          efp->thr = thr;
          efp->arg = arg;
          efp->call_id = 0;
          return (void *) 0 - 1;
        L_CALL2:;
          thr = efp->thr;
          arg = efp->arg;
          new_esp = esp + 1;
        }
    }
  pthread_mutex_unlock (&(*thr).mut);
}

struct prefetcher_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct thread_data *thr;
  enum node req_to;
  enum node treq_head[2];
  void *thr0;
};

void *prefetcher (char *esp, void *thr0)
{
  enum node req_to;
  struct thread_data *thr;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct prefetcher_frame *efp;

LGOTO:;
  efp = (struct prefetcher_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct prefetcher_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  req_to = OUTSIDE;
  thr = (struct thread_data *) thr0;
  (efp->treq_head)[0] = ANY;
  (efp->treq_head)[1] = TERM;
  pthread_mutex_lock (&(*thr).mut);
  while (1)
    {
      while ((*thr).task_top)
        {
          pthread_cond_wait (&(*thr).cond, &(*thr).mut);
        }
      allocate_task (thr);
      while (!send_treq_to_initialize_task (thr, efp->treq_head, req_to))
        {
        }
      pthread_cond_broadcast (&(*thr).cond);
    }
  pthread_mutex_unlock (&(*thr).mut);
  return 0;
}

int pop_prefetched_task (struct thread_data *thr)
{
  struct task *tx_dst;
  struct task *tx_src;

  pthread_mutex_lock (&(*prefetch_thr).mut);
  while (!
         ((*prefetch_thr).task_top
          && (*(*prefetch_thr).task_top).stat == TASK_INITIALIZED))
    {
      pthread_cond_wait (&(*prefetch_thr).cond, &(*prefetch_thr).mut);
    }
  pthread_mutex_lock (&(*thr).mut);
  tx_dst = (*thr).task_top;
  tx_src = (*prefetch_thr).task_top;
  (*tx_dst).task_no = (*tx_src).task_no;
  (*tx_dst).body = (*tx_src).body;
  (*tx_dst).ndiv = (*tx_src).ndiv;
  (*tx_dst).rslt_to = (*tx_src).rslt_to;
  copy_address ((*tx_dst).rslt_head, (*tx_src).rslt_head);
  (*tx_dst).stat = TASK_INITIALIZED;
  deallocate_task (prefetch_thr);
  pthread_cond_broadcast (&(*thr).cond);
  pthread_cond_broadcast (&(*prefetch_thr).cond);
  pthread_mutex_unlock (&(*thr).mut);
  pthread_mutex_unlock (&(*prefetch_thr).mut);
  return 1;
}

int try_take_back_prefetched_task (enum node *treq_head)
{
  struct task *tx;
  int retval;
  struct cmd rcmd;

  pthread_mutex_lock (&(*prefetch_thr).mut);
  if ((tx = (*prefetch_thr).task_top) && TASK_INITIALIZED == (*tx).stat
      && address_equal ((*tx).rslt_head, treq_head))
    {
      rcmd.w = BACK;
      rcmd.c = 1;
      rcmd.node = OUTSIDE;
      copy_address ((rcmd.v)[0], treq_head);
      send_command (&rcmd, 0, 0);
      deallocate_task (prefetch_thr);
      pthread_cond_broadcast (&(*prefetch_thr).cond);
      retval = 1;
    }
  else
    retval = 0;
  pthread_mutex_unlock (&(*prefetch_thr).mut);
  return retval;
}

void recv_task (struct cmd *pcmd, void *body)
{
  struct task *tx;
  struct thread_data *thr;
  enum node id;
  int task_no;
  size_t len;

  if ((*pcmd).c < 4)
    proto_error ("wrong-task", pcmd);
  else;
  task_no = ((*pcmd).v)[3][0];
  if ((*pcmd).node == OUTSIDE)
    {
      body = (task_receivers[task_no]) ();
      read_to_eol ();
    }
  else;
  id = ((*pcmd).v)[2][0];
  if (!(id < num_thrs + (option.prefetch ? 1 : 0)))
    proto_error ("wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&(*thr).mut);
  tx = (*thr).task_top;
  (*tx).rslt_to = (*pcmd).node;
  copy_address ((*tx).rslt_head, ((*pcmd).v)[1]);
  (*tx).ndiv = ((*pcmd).v)[0][0];
  (*tx).task_no = task_no;
  (*tx).body = body;
  (*tx).stat = TASK_INITIALIZED;
  pthread_mutex_unlock (&(*thr).mut);
  pthread_cond_broadcast (&(*thr).cond);
}

void recv_none (struct cmd *pcmd)
{
  struct thread_data *thr;
  enum node id;
  size_t len;

  if ((*pcmd).c < 1)
    proto_error ("Wrong none", pcmd);
  else;
  id = ((*pcmd).v)[0][0];
  if (!(id < num_thrs || option.prefetch && id == num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&(*thr).mut);
  if ((*thr).w_none > 0)
    ((*thr).w_none)--;
  else
    (*(*thr).task_top).stat = TASK_NONE;
  pthread_cond_broadcast (&(*thr).cond);
  pthread_mutex_unlock (&(*thr).mut);
}

struct task_home *search_task_home_by_id (int id, struct task_home *hx)
{
  while (hx && (*hx).id != id)
    {
      hx = (*hx).next;
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

  if ((*pcmd).c < 1)
    proto_error ("Wrong back", pcmd);
  else;
  thr_id = ((*pcmd).v)[0][0];
  tsk_id = ((*pcmd).v)[0][1];
  if (!(thr_id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + thr_id;
  pthread_mutex_lock (&(*thr).mut);
  if (!(hx = search_task_home_by_id (tsk_id, (*thr).sub)))
    {
      proto_error ("Wrong rslt-head (specified task not exists)", pcmd);
      print_status (0);
      exit (1);
    }
  else;
  tx = (*thr).task_top;
  hx = (*thr).sub;
  (*tx).task_no = (*hx).task_no;
  (*tx).body = (*hx).body;
  (*tx).ndiv = (*thr).ndiv;
  (*tx).rslt_to = INSIDE;
  ((*tx).rslt_head)[0] = thr_id;
  ((*tx).rslt_head)[1] = (*hx).id;
  ((*tx).rslt_head)[2] = TERM;
  (*hx).req_from = INSIDE;
  ((*hx).task_head)[0] = thr_id;
  ((*hx).task_head)[1] = TERM;
  (*tx).stat = TASK_INITIALIZED;
  pthread_mutex_unlock (&(*thr).mut);
  pthread_cond_broadcast (&(*thr).cond);
}

void recv_rslt (struct cmd *pcmd, void *body)
{
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  enum node tid;
  int sid;

  if ((*pcmd).c < 1)
    proto_error ("Wrong rslt", pcmd);
  else;
  tid = ((*pcmd).v)[0][0];
  if (!(tid < num_thrs))
    proto_error ("wrong rslt-head", pcmd);
  else;
  sid = ((*pcmd).v)[0][1];
  if (TERM == sid)
    proto_error ("Wrong rslt-head (no task-home-id)", pcmd);
  else;
  thr = threads + tid;
  pthread_mutex_lock (&(*thr).mut);
  if (!(hx = search_task_home_by_id (sid, (*thr).sub)))
    proto_error ("Wrong rslt-head (specified task not exists)", pcmd);
  else;
  if ((*pcmd).node == OUTSIDE)
    {
      (rslt_receivers[(*hx).task_no]) ((*hx).body);
      read_to_eol ();
    }
  else if ((*pcmd).node == INSIDE)
    {
      (*hx).body = body;
    }
  else
    {
      proto_error ("Wrong cmd.node", pcmd);
    }
  rcmd.c = 1;
  rcmd.node = (*pcmd).node;
  rcmd.w = RACK;
  copy_address ((rcmd.v)[0], (*hx).task_head);
  (*hx).stat = TASK_HOME_DONE;
  if (hx == (*thr).sub)
    {
      pthread_cond_broadcast (&(*thr).cond_r);
      pthread_cond_broadcast (&(*thr).cond);
    }
  else;
  pthread_mutex_unlock (&(*thr).mut);
  send_command (&rcmd, 0, 0);
}

char *task_stat_strings[];

int try_treq (struct cmd *pcmd, enum node id, enum node *from_addr)
{
  struct task_home *hx;
  struct thread_data *thr;
  int avail;
  enum node from_head;

  avail = 0;
  from_head = from_addr[0];
  thr = threads + id;
  pthread_mutex_lock (&(*thr).mut);
  pthread_mutex_lock (&(*thr).rack_mut);
  if ((*thr).w_rack == 0
      && ((*thr).task_top
          && ((*(*thr).task_top).stat == TASK_STARTED
              || (*(*thr).task_top).stat == TASK_INITIALIZED)
          && (((*pcmd).v)[1][0] == ANY
              || address_equal ((*(*thr).task_top).rslt_head, ((*pcmd).v)[1]))))
    avail = 1;
  else
    {
    }
  pthread_mutex_unlock (&(*thr).rack_mut);
  if (avail)
    {
      hx = (*thr).treq_free;
      if (!hx)
        mem_error ("Not enough task-home memory");
      else;
      (*thr).treq_free = (*hx).next;
      (*hx).next = (*thr).treq_top;
      (*hx).stat = TASK_HOME_ALLOCATED;
      copy_address ((*hx).task_head, ((*pcmd).v)[0]);
      if ((*pcmd).node != OUTSIDE)
        (*hx).req_from = INSIDE;
      else
        (*hx).req_from = OUTSIDE;
      (*thr).treq_top = hx;
      (*thr).req = hx;
    }
  else;
  pthread_mutex_unlock (&(*thr).mut);
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
  struct thread_data *thr;

  if (0 <= from_node)
    {
      thr = threads + from_node;
      (*thr).last_choose = (1 + (*thr).last_choose) % 2;
      if (CHS_RANDOM == (*thr).last_choose)
        return my_random (num_thrs, &(*thr).random_seed1, &(*thr).random_seed2);
      else if (CHS_ORDER == (*thr).last_choose)
        {
          (*thr).last_treq = (1 + (*thr).last_treq) % num_thrs;
          return (*thr).last_treq;
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
  int myid;
  int start_id;
  int d;
  int id;

  if ((*pcmd).c < 2)
    proto_error ("Wrong treq", pcmd);
  else;
  dst0 = ((*pcmd).v)[1][0];
  if (dst0 == ANY)
    {
      myid = ((*pcmd).v)[0][0];
      start_id = choose_treq (myid);
      {
        d = 0;
        for (; d < num_thrs; d++)
          {
            id = (d + start_id) % num_thrs;
            if ((*pcmd).node != OUTSIDE && id == myid)
              continue;
            else;
            if (try_treq (pcmd, id, ((*pcmd).v)[0]))
              {
                {
                }
                break;
              }
            else;
            {
            }
          }
      }
      if (d != num_thrs)
        return;
      else;
    }
  else if (option.prefetch && dst0 == prefetch_thr_id)
    if ((*pcmd).node == OUTSIDE)
      if (try_take_back_prefetched_task (((*pcmd).v)[0]))
        return;
      else
        {
          ((*pcmd).v)[1][0] = 0;
          recv_treq (pcmd);
          return;
        }
    else
      {
        {
        }
        if (pop_prefetched_task (threads + ((*pcmd).v)[0][0]))
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
      if (try_treq (pcmd, dst0, ((*pcmd).v)[0]))
        {
          {
          }
          return;
        }
      else;
      {
      }
    }
  if ((*pcmd).node == ANY && ((*pcmd).v)[0][0] == 0)
    if (option.prefetch)
      {
        (*pcmd).node = INSIDE;
        ((*pcmd).v)[1][0] = prefetch_thr_id;
        ((*pcmd).v)[1][1] = TERM;
        send_command (pcmd, 0, 0);
        return;
      }
    else
      {
        (*pcmd).node = OUTSIDE;
        send_command (pcmd, 0, 0);
        return;
      }
  else;
  rcmd.c = 1;
  rcmd.node = (*pcmd).node == ANY ? INSIDE : (*pcmd).node;
  rcmd.w = NONE;
  copy_address ((rcmd.v)[0], ((*pcmd).v)[0]);
  send_command (&rcmd, 0, 0);
}

void recv_rack (struct cmd *pcmd)
{
  struct task *tx;
  struct thread_data *thr;
  enum node id;
  size_t len;

  if ((*pcmd).c < 1)
    proto_error ("Wrong rack", pcmd);
  else;
  id = ((*pcmd).v)[0][0];
  if (option.prefetch && prefetch_thr_id == id)
    id = 0;
  else;
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&(*thr).rack_mut);
  ((*thr).w_rack)--;
  pthread_mutex_unlock (&(*thr).rack_mut);
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
      {
        i = 0;
        for (; i < n; i++)
          {
            tmp[i] = DATA_NONE;
          }
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
  {
    i = start;
    for (; i < end; i++)
      {
        if (data_flags[i] == DATA_NONE)
          {
            data_flags[i] = DATA_REQUESTING;
            {
              j = i + 1;
              for (; j < end && data_flags[j] == DATA_NONE; j++)
                {
                  data_flags[j] = DATA_REQUESTING;
                }
            }
            if (pcmd)
              {
                ((*pcmd).v)[2][0] = i;
                ((*pcmd).v)[2][1] = j;
                ((*pcmd).v)[2][2] = TERM;
                send_command (pcmd, 0, 0);
              }
            else;
            i = j - 1;
          }
        else if (data_flags[i] == DATA_REQUESTING)
          {
            {
              j = i + 1;
              for (; j < end && data_flags[j] == DATA_REQUESTING; j++)
                {
                }
            }
            if (pcmd_fwd)
              {
                ((*pcmd_fwd).v)[2][0] = i;
                ((*pcmd_fwd).v)[2][1] = j;
                ((*pcmd_fwd).v)[2][2] = TERM;
                send_command (pcmd_fwd, 0, 0);
              }
            else;
            i = j - 1;
          }
        else;
      }
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
      pthread_mutex_lock (&(*thr).mut);
      if (!(hx = search_task_home_by_id (sid, (*thr).sub)))
        fprintf (stderr,
                 "Error in get-first-outside-ancestor-task-address (specified task not exists)\n");
      else;
      pthread_mutex_unlock (&(*thr).mut);
      if (!(*hx).owner)
        {
          fprintf (stderr,
                   "error in get-first-outside-ancestor-task-address: no owner found.\n");
          print_status (0);
          exit (1);
        }
      else if ((*(*hx).owner).rslt_to == OUTSIDE)
        {
          copy_address (head, (*(*hx).owner).rslt_head);
          break;
        }
      else;
      tid = ((*(*hx).owner).rslt_head)[0];
      sid = ((*(*hx).owner).rslt_head)[1];
    }
  return ok;
}

void _request_data (struct thread_data *thr, int start, int end)
{
  struct cmd cmd2;
  struct task *tx;

  {
  }
  pthread_mutex_lock (&(*thr).mut);
  tx = (*thr).task_top;
  pthread_mutex_unlock (&(*thr).mut);
  cmd2.w = DREQ;
  cmd2.c = 3;
  (cmd2.v)[0][0] = 0;
  (cmd2.v)[0][1] = TERM;
  if (OUTSIDE == (*tx).rslt_to)
    copy_address ((cmd2.v)[1], (*tx).rslt_head);
  else
    get_first_outside_ancestor_task_address ((cmd2.v)[1], ((*tx).rslt_head)[0],
                                             ((*tx).rslt_head)[1]);
  cmd2.node = OUTSIDE;
  {
  }
  send_dreq_for_required_range (start, end, &cmd2, 0);
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
  {
    i = start;
    for (; i < end; i++)
      {
        while (data_flags[i] != DATA_EXIST)
          {
            pthread_cond_wait (&data_cond, &data_mutex);
          }
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
  {
    i = start;
    for (; i < end; i++)
      {
        data_flags[i] = DATA_EXIST;
      }
  }
  pthread_mutex_unlock (&data_mutex);
}

void *dreq_handler (void *parg0)
{
  struct dhandler_arg *parg;
  int start;
  int end;
  struct cmd *pcmd;
  struct cmd *pcmd_fwd;
  struct cmd data_cmd;
  int i;
  int j;

  parg = parg0;
  start = (*parg).start;
  end = (*parg).end;
  pcmd = &(*parg).dreq_cmd;
  pcmd_fwd = &(*parg).dreq_cmd_fwd;
  send_dreq_for_required_range (start, end, pcmd, pcmd_fwd);
  if ((*parg).data_to == INSIDE)
    return;
  else;
  data_cmd.w = DATA;
  data_cmd.c = 2;
  data_cmd.node = (*parg).data_to;
  copy_address ((data_cmd.v)[0], (*parg).head);
  pthread_mutex_lock (&data_mutex);
  {
    i = start;
    for (; i < end; i++)
      {
        while (data_flags[i] != DATA_EXIST)
          {
            pthread_cond_wait (&data_cond, &data_mutex);
          }
        {
          j = i + 1;
          for (; j < end && data_flags[j] == DATA_EXIST; j++)
            {
            }
        }
        assert (i < j);
        (data_cmd.v)[1][0] = i;
        (data_cmd.v)[1][1] = j;
        (data_cmd.v)[1][2] = TERM;
        send_command (&data_cmd, 0, 0);
        i = j - 1;
      }
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
  pthread_t tid2;

  if ((*pcmd).c < 3)
    proto_error ("Wrong dreq", pcmd);
  else;
  parg = (struct dhandler_arg *) malloc (sizeof (struct dhandler_arg));
  (*parg).data_to = (*pcmd).node;
  copy_address ((*parg).head, ((*pcmd).v)[0]);
  (*parg).dreq_cmd.w = DREQ;
  (*parg).dreq_cmd.c = 3;
  ((*parg).dreq_cmd.v)[0][0] = 0;
  ((*parg).dreq_cmd.v)[0][1] = TERM;
  {
    tid = ((*pcmd).v)[1][0];
    if (!(tid < num_thrs))
      proto_error ("wrong dreq-head", pcmd);
    else;
    sid = ((*pcmd).v)[1][1];
    if (TERM == sid)
      proto_error ("Wrong dreq-head (no task-home-id)", pcmd);
    else;
    get_first_outside_ancestor_task_address (((*parg).dreq_cmd.v)[1], tid, sid);
  }
  (*parg).dreq_cmd.node = OUTSIDE;
  (*parg).dreq_cmd_fwd.w = DREQ;
  (*parg).dreq_cmd_fwd.c = 3;
  ((*parg).dreq_cmd_fwd.v)[0][0] = FORWARD;
  copy_address (&((*parg).dreq_cmd_fwd.v)[0][1], ((*pcmd).v)[0]);
  copy_address (((*parg).dreq_cmd_fwd.v)[1], ((*parg).dreq_cmd.v)[1]);
  (*parg).dreq_cmd_fwd.node = OUTSIDE;
  (*parg).start = ((*pcmd).v)[2][0];
  (*parg).end = ((*pcmd).v)[2][1];
  {
    pthread_create (&tid2, 0, dreq_handler, parg);
  }
  return;
}

void recv_data (struct cmd *pcmd)
{
  int i;
  int start;
  int end;

  start = ((*pcmd).v)[1][0];
  end = ((*pcmd).v)[1][1];
  if ((*pcmd).c < 2)
    proto_error ("Wrong data", pcmd);
  else;
  if ((*pcmd).node == INSIDE)
    return;
  else;
  pthread_mutex_lock (&data_mutex);
  data_receive (start, end);
  read_to_eol ();
  {
    i = start;
    for (; i < end; i++)
      {
        data_flags[i] = DATA_EXIST;
      }
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

void node_to_string (char *buf3, enum node node2)
{
  switch (node2)
    {
    case OUTSIDE:
      sprintf (buf3, "OUTSIDE");
      break;
    case INSIDE:
      sprintf (buf3, "INSIDE");
      break;
    case ANY:
      sprintf (buf3, "ANY");
      break;
    case PARENT:
      sprintf (buf3, "PARENT");
      break;
    case TERM:
      sprintf (buf3, "TERM");
      break;
    default:
      sprintf (buf3, "%d", node2);
      break;
    }
}

void print_task_list (struct task *task_top, char *name)
{
  struct task *cur;
  char buf1[1280];
  char buf2[1280];

  fprintf (stderr, "%s= {", name);
  {
    cur = task_top;
    for (; cur; cur = (*cur).next)
      {
        fprintf (stderr,
                 "{stat=%s, task-no=%d, body=%p, ndiv=%d, rslt-to=%s, rslt-head=%s}, ",
                 task_stat_strings[(*cur).stat], (*cur).task_no, (*cur).body,
                 (*cur).ndiv, (node_to_string (buf1, (*cur).rslt_to), buf1),
                 (serialize_arg (buf2, (*cur).rslt_head), buf2));
      }
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
  {
    cur = treq_top;
    for (; cur; cur = (*cur).next)
      {
        fprintf (stderr,
                 "{stat=%s, id=%d, owner=%p, task-no=%d, body=%p, req-from=%s, task-head=%s}, ",
                 task_home_stat_strings[(*cur).stat], (*cur).id, (*cur).owner,
                 (*cur).task_no, (*cur).body,
                 (node_to_string (buf1, (*cur).req_from), buf1),
                 (serialize_arg (buf2, (*cur).task_head), buf2));
      }
  }
  fprintf (stderr, "}, ");
  return;
}
char *choose_strings[] = { "CHS-RANDOM", "CHS-ORDER" };

void print_thread_status (struct thread_data *thr)
{
  fprintf (stderr, "<Thread %d>\n", (*thr).id);
  fprintf (stderr, "req=%p, ", (*thr).req);
  fprintf (stderr, "w-rack=%d, ", (*thr).w_rack);
  fprintf (stderr, "w-none=%d, ", (*thr).w_none);
  fprintf (stderr, "ndiv=%d, ", (*thr).ndiv);
  fprintf (stderr, "last-treq=%d, ", (*thr).last_treq);
  fprintf (stderr, "last-choose=%s, ", choose_strings[(*thr).last_choose]);
  fprintf (stderr, "random-seed(1,2)=(%f,%f), ", (*thr).random_seed1,
           (*thr).random_seed2);
  print_task_list ((*thr).task_top, "tasks");
  print_task_home_list ((*thr).treq_top, "treq-top");
  print_task_home_list ((*thr).sub, "sub");
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
      print_task_list ((*prefetch_thr).task_top, "prefetched tasks");
      fputc ('\n', stderr);
    }
  else;
  {
    i = 0;
    for (; i < num_thrs; i++)
      {
        print_thread_status (&threads[i]);
      }
  }
  return;
}

void set_verbose_level (struct cmd *pcmd)
{
  if ((*pcmd).c < 1)
    proto_error ("Wrong verb", pcmd);
  else;
  option.verbose = ((*pcmd).v)[0][0];
  return;
}

void recv_exit (struct cmd *pcmd)
{
  fprintf (stderr, "Received \"exit\"... terminate.\n");
  exit (0);
}

struct handle_req_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct thread_data *_thr;
  closure_t *_bk;
};

void handle_req (char *esp, closure_t * _bk, struct thread_data *_thr)
{
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct handle_req_frame *efp;
  char *argp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct handle_req_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct handle_req_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL3;
        }
      goto L_CALL3;
    }
  else;
  efp = (struct handle_req_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct handle_req_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  pthread_mutex_lock (&(*_thr).mut);
  if ((*_thr).req)
    {
      {
        argp =
          (char *) ((Align_t *) esp +
                    (sizeof (char *) + sizeof (Align_t) +
                     -1) / sizeof (Align_t));
        *((closure_t **) argp) = _bk;
        efp->_thr = _thr;
        efp->_bk = _bk;
        efp->argp = argp;
        efp->tmp_esp = argp;
        efp->call_id = 0;
        return;
      L_CALL3:;
        _thr = efp->_thr;
        _bk = efp->_bk;
      }
      (*_thr).req = (*_thr).treq_top;
    }
  else;
  pthread_mutex_unlock (&(*_thr).mut);
}

void make_and_send_task (struct thread_data *thr, int task_no, void *body)
{
  struct cmd tcmd;
  struct task_home *hx;

  hx = (*thr).treq_top;
  (*thr).treq_top = (*hx).next;
  (*hx).next = (*thr).sub;
  (*thr).sub = hx;
  (*hx).task_no = task_no;
  (*hx).body = body;
  (*hx).id = (*hx).next ? (*(*hx).next).id + 1 : 0;
  (*hx).owner = (*thr).task_top;
  (*hx).stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = (*hx).req_from;
  tcmd.w = TASK;
  (tcmd.v)[0][0] = ++(*thr).ndiv;
  (tcmd.v)[0][1] = TERM;
  (tcmd.v)[1][0] = (*thr).id;
  (tcmd.v)[1][1] = (*hx).id;
  (tcmd.v)[1][2] = TERM;
  copy_address ((tcmd.v)[2], (*hx).task_head);
  (tcmd.v)[3][0] = task_no;
  (tcmd.v)[3][1] = TERM;
  send_command (&tcmd, body, task_no);
}

struct wait_rslt_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct timespec t_until;
  struct timeval now;
  struct task_home *sub;
  void *body;
  struct thread_data *thr;
};

void *wait_rslt (char *esp, struct thread_data *thr)
{
  void *body;
  struct task_home *sub;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct wait_rslt_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct wait_rslt_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct wait_rslt_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL4;
        }
      goto L_CALL4;
    }
  else;
  efp = (struct wait_rslt_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct wait_rslt_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  pthread_mutex_lock (&(*thr).mut);
  sub = (*thr).sub;
  while ((*sub).stat != TASK_HOME_DONE)
    {
      if (PARENT == ((*sub).task_head)[0])
        {
          gettimeofday (&efp->now, 0);
          timeval_plus_nsec_to_timespec (&efp->t_until, &efp->now, 1000);
          pthread_cond_timedwait (&(*thr).cond_r, &(*thr).mut, &efp->t_until);
        }
      else;
      (*(*thr).task_top).stat = TASK_STARTED;
      if ((*sub).stat == TASK_HOME_DONE)
        break;
      else;
      {
        new_esp = esp;
        while (recv_exec_send (new_esp, thr, (*sub).task_head, (*sub).req_from),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->sub = sub;
            efp->body = body;
            efp->thr = thr;
            efp->call_id = 0;
            return (void *) 0 - 1;
          L_CALL4:;
            sub = efp->sub;
            body = efp->body;
            thr = efp->thr;
            new_esp = esp + 1;
          }
      }
    }
  body = (*sub).body;
  (*thr).sub = (*sub).next;
  (*sub).next = (*thr).treq_free;
  (*thr).treq_free = sub;
  pthread_mutex_unlock (&(*thr).mut);
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
  {
    i = 0;
    for (; i < len - 1; i++)
      {
        (tlist[i]).prev = &tlist[i + 1];
        (tlist[i + 1]).next = &tlist[i];
      }
  }
  (tlist[0]).next = 0;
  (tlist[len - 1]).prev = 0;
  {
    i = 0;
    for (; i < len; i++)
      {
        (tlist[i]).rslt_to = TERM;
        ((tlist[i]).rslt_head)[0] = TERM;
      }
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
  {
    i = 0;
    for (; i < len - 1; i++)
      {
        (hlist[i]).next = &hlist[i + 1];
        (hlist[len - 1]).next = 0;
      }
  }
  return;
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  char header[30];
  char *p_dst;
  char *p_src;
  double q;
  double r;
  struct task_home *hx;
  struct task *tx;
  struct thread_data *thr;
  struct cmd *pcmd;
  int j;
  int i;
  int ifexp_result3;
  int ifexp_result5;
  int ifexp_result4;
  int ifexp_result6;
  char **argv;
  int argc;
};

int main (int argc, char **argv)
{
  int ifexp_result6;
  int ifexp_result4;
  int ifexp_result5;
  int ifexp_result3;
  int i;
  int j;
  struct cmd *pcmd;
  struct thread_data *thr;
  struct task *tx;
  struct task_home *hx;
  double r;
  double q;
  char *p_src;
  char *p_dst;
  char estack[262144];
  char *esp = estack;
  char *new_esp;
  struct main_frame *efp;

LGOTO:;
  efp = (struct main_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct main_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  set_option (argc, argv);
  {
    if ('\x0' == (option.sv_hostname)[0])
      ifexp_result3 = -1;
    else
      ifexp_result3 = connect_to (option.sv_hostname, option.port);
    sv_socket = ifexp_result3;
  }
  pthread_mutex_init (&send_mut, 0);
  pthread_mutex_init (&data_mutex, 0);
  pthread_cond_init (&data_cond, 0);
  num_thrs = option.num_thrs;
  {
    i = 0;
    {
      if (option.prefetch)
        ifexp_result4 = 1;
      else
        ifexp_result4 = 0;
      if (i < num_thrs + ifexp_result4)
        {
          goto loop_start;
          while (1)
            {
              if (option.prefetch)
                ifexp_result5 = 1;
              else
                ifexp_result5 = 0;
              if (!(i++, i < num_thrs + ifexp_result5))
                goto loop_end;
              else;
            loop_start:;
              {
                thr = threads + i;
                (*thr).req = 0;
                (*thr).id = i;
                (*thr).w_rack = 0;
                (*thr).w_none = 0;
                (*thr).ndiv = 0;
                (*thr).last_treq = i;
                (*thr).last_choose = CHS_RANDOM;
                {
                  r = sqrt (0.5 + i);
                  q = sqrt (r + i);
                  r -= (int) r;
                  (*thr).random_seed1 = r;
                  (*thr).random_seed2 = q;
                }
                pthread_mutex_init (&(*thr).mut, 0);
                pthread_mutex_init (&(*thr).rack_mut, 0);
                pthread_cond_init (&(*thr).cond, 0);
                pthread_cond_init (&(*thr).cond_r, 0);
                tx = (struct task *) malloc (sizeof (struct task) * 1024);
                initialize_task_list (tx, 1024, &(*thr).task_top,
                                      &(*thr).task_free);
                hx =
                  (struct task_home *) malloc (sizeof (struct task_home) *
                                               1024);
                initialize_task_home_list (hx, 1024, &(*thr).treq_top,
                                           &(*thr).treq_free);
                (*thr).sub = 0;
              }
            }
        loop_end:;
        }
      else;
    }
  }
  if (option.prefetch)
    {
      prefetch_thr = threads + num_thrs;
      prefetch_thr_id = num_thrs;
    }
  else;
  {
    i = 0;
    for (; i < num_thrs; i++)
      {
        new_esp = esp;
        while (systhr_create (new_esp, worker, threads + i),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            char *goto_fr;

            *((char **) esp) = 0;
            efp->p_dst = p_dst;
            efp->p_src = p_src;
            efp->q = q;
            efp->r = r;
            efp->hx = hx;
            efp->tx = tx;
            efp->thr = thr;
            efp->pcmd = pcmd;
            efp->j = j;
            efp->i = i;
            efp->ifexp_result3 = ifexp_result3;
            efp->ifexp_result5 = ifexp_result5;
            efp->ifexp_result4 = ifexp_result4;
            efp->ifexp_result6 = ifexp_result6;
            efp->argv = argv;
            efp->argc = argc;
            goto_fr = lw_call (efp->tmp_esp);
            if ((char *) goto_fr == (char *) efp)
              goto LGOTO;
            else;
            p_dst = efp->p_dst;
            p_src = efp->p_src;
            q = efp->q;
            r = efp->r;
            hx = efp->hx;
            tx = efp->tx;
            thr = efp->thr;
            pcmd = efp->pcmd;
            j = efp->j;
            i = efp->i;
            ifexp_result3 = efp->ifexp_result3;
            ifexp_result5 = efp->ifexp_result5;
            ifexp_result4 = efp->ifexp_result4;
            ifexp_result6 = efp->ifexp_result6;
            argv = efp->argv;
            argc = efp->argc;
            new_esp = esp + 1;
          }
      }
  }
  if (option.prefetch)
    {
      new_esp = esp;
      while (systhr_create (new_esp, prefetcher, prefetch_thr),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          char *goto_fr;

          *((char **) esp) = 0;
          efp->p_dst = p_dst;
          efp->p_src = p_src;
          efp->q = q;
          efp->r = r;
          efp->hx = hx;
          efp->tx = tx;
          efp->thr = thr;
          efp->pcmd = pcmd;
          efp->j = j;
          efp->i = i;
          efp->ifexp_result3 = ifexp_result3;
          efp->ifexp_result5 = ifexp_result5;
          efp->ifexp_result4 = ifexp_result4;
          efp->ifexp_result6 = ifexp_result6;
          efp->argv = argv;
          efp->argc = argc;
          goto_fr = lw_call (efp->tmp_esp);
          if ((char *) goto_fr == (char *) efp)
            goto LGOTO;
          else;
          p_dst = efp->p_dst;
          p_src = efp->p_src;
          q = efp->q;
          r = efp->r;
          hx = efp->hx;
          tx = efp->tx;
          thr = efp->thr;
          pcmd = efp->pcmd;
          j = efp->j;
          i = efp->i;
          ifexp_result3 = efp->ifexp_result3;
          ifexp_result5 = efp->ifexp_result5;
          ifexp_result4 = efp->ifexp_result4;
          ifexp_result6 = efp->ifexp_result6;
          argv = efp->argv;
          argc = efp->argc;
          new_esp = esp + 1;
        }
    }
  else;
  if (option.initial_task)
    {
      strcpy (efp->header, "task 0 0 0 ");
      receive_buf =
        (char *)
        malloc ((3 + strlen (option.initial_task) +
                 strlen (efp->header)) * sizeof (char));
      receive_buf_p = receive_buf;
      strcpy (receive_buf, efp->header);
      {
        p_src = option.initial_task, p_dst = receive_buf + strlen (efp->header);
        for (; *p_src; (p_src++, p_dst++))
          {
            if (' ' == *p_src)
              ifexp_result6 = '\n';
            else
              ifexp_result6 = *p_src;
            *p_dst = ifexp_result6;
          }
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
