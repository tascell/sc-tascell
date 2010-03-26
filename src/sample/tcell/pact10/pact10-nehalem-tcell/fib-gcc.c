#include <pthread.h>
#include <stdio.h>

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

void handle_req (int (*)(void), struct thread_data *);

#include<sys/time.h>

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

double sqrt (double);

double fabs (double);

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0E-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

struct worker_data
{
  int n_task;
  char _dummy_[1000];
};

void worker_init (struct thread_data *_thr)
{
  (*((struct worker_data *) (*_thr).wdptr)).n_task = 0;
}

int fib (int (*_bk) (void), struct thread_data *_thr, int n);

struct fib
{
  int n;
  int r;
  char _dummy_[1000];
};

void do_fib_task (struct thread_data *_thr, struct fib *pthis)
{

  int _bk (void)
  {
    return 0;
  }
  ++(*((struct worker_data *) (*_thr).wdptr)).n_task;
  fprintf (stderr, "%d starts %d-th task.\n", (*_thr).id,
           (*((struct worker_data *) (*_thr).wdptr)).n_task);
  (*pthis).r = fib (_bk, _thr, (*pthis).n);
}

struct fib_start
{
  int n;
  int r;
  char _dummy_[1000];
};

void do_fib_start_task (struct thread_data *_thr, struct fib_start *pthis)
{

  int _bk (void)
  {
    return 0;
  }
  struct timeval tp[2];

  fprintf (stderr, "start fib(%d)\n", (*pthis).n);
  ++(*((struct worker_data *) (*_thr).wdptr)).n_task;
  fprintf (stderr, "%d starts %d-th task.\n", (*_thr).id,
           (*((struct worker_data *) (*_thr).wdptr)).n_task);
  gettimeofday (tp, 0);
  (*pthis).r = fib (_bk, _thr, (*pthis).n);
  gettimeofday (tp + 1, 0);
  fprintf (stderr, "time: %lf\n", elapsed_time (tp));
}

int fib (int (*_bk) (void), struct thread_data *_thr, int n)
{
  if (n <= 2)
    return 1;
  else
    {
      int s1;
      int s2;

      {
        struct fib st;
        struct fib *pthis = &st;
        int spawned = 0;

        {

          int do_two_bk (void)
          {
            if (spawned)
              return 0;
            else;
            _bk ();
            if ((*_thr).treq_top)
              {
                (*pthis).n = n - 2;
                spawned = 1;
                make_and_send_task (_thr, 0, pthis);
                return 1;
              }
            else;
            return 0;
          }
          if ((*_thr).req)
            handle_req (do_two_bk, _thr);
          else;
          {
            s1 = fib (do_two_bk, _thr, n - 1);
          }
        }
        if (spawned)
          {
            wait_rslt (_thr);
            s2 = (*pthis).r;
          }
        else
          {
            s2 = fib (_bk, _thr, n - 2);
          }
      }
      return s1 + s2;
    }
}

void data_allocate (int n1)
{
}

void data_send (int n1, int n2)
{
}

void data_receive (int n1, int n2)
{
}

void send_fib_start_task (struct fib_start *pthis)
{
  send_int ((*pthis).n);
}

struct fib_start *recv_fib_start_task ()
{
  struct fib_start *pthis = malloc (sizeof (struct fib_start));

  (*pthis).n = recv_int ();
  return pthis;
}

void send_fib_start_rslt (struct fib_start *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void recv_fib_start_rslt (struct fib_start *pthis)
{
  (*pthis).r = recv_int ();
}

void send_fib_task (struct fib *pthis)
{
  send_int ((*pthis).n);
}

struct fib *recv_fib_task ()
{
  struct fib *pthis = malloc (sizeof (struct fib));

  (*pthis).n = recv_int ();
  return pthis;
}

void send_fib_rslt (struct fib *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void recv_fib_rslt (struct fib *pthis)
{
  (*pthis).r = recv_int ();
}

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_fib_task,
    (void (*)(struct thread_data *, void *)) do_fib_start_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_fib_task, (void (*)(void *)) send_fib_start_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_fib_task, (void *(*)()) recv_fib_start_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_fib_rslt, (void (*)(void *)) send_fib_start_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_fib_rslt, (void (*)(void *)) recv_fib_start_rslt};
