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

struct nqueens
{
  int r;
  int n;
  int k;
  int i1;
  int i2;
  int a[20];
  int lb[40];
  int rb[40];
  char _dummy_[1000];
};

int nqueens (int n, int k, int ix, int iy, struct nqueens *tsk)
{
  int s = 0;

  {
    int i = ix;
    int I_end = iy;

    for (; i < I_end; i++)
      {
        int ai = ((*tsk).a)[i];

        if (!(((*tsk).lb)[n + -1 + -ai + k] || ((*tsk).rb)[ai + k]))
          if (k == n - 1)
            s++;
          else
            {
              {
                ((*tsk).lb)[n + -1 + -ai + k] = 1;
                ((*tsk).rb)[ai + k] = 1;
                ((*tsk).a)[i] = ((*tsk).a)[k];
                ((*tsk).a)[k] = ai;
              }
              {
                {
                  s += nqueens (n, k + 1, k + 1, n, tsk);
                }
                {
                  ((*tsk).lb)[n + -1 + -ai + k] = 0;
                  ((*tsk).rb)[ai + k] = 0;
                  ai = ((*tsk).a)[k];
                  ((*tsk).a)[k] = ((*tsk).a)[i];
                  ((*tsk).a)[i] = ai;
                }
              }
            }
        else;
      }
  }
  return s;
}

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0E-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

void do_nqueens_task (struct thread_data *_thr, struct nqueens *pthis)
{
  int i;
  int n = (*pthis).n;
  int k = (*pthis).k;
  struct timeval tp[2];

  if (option.verbose > 0)
    fprintf (stderr, "start %d %d %d %d\n", n, k, (*pthis).i1, (*pthis).i2);
  else;
  if (k < 0)
    {
      {
        i = 0;
        for (; i < n; i++)
          {
            ((*pthis).a)[i] = i;
          }
      }
      {
        i = 0;
        for (; i < 2 * n - 1; i++)
          {
            ((*pthis).lb)[i] = 0;
            ((*pthis).rb)[i] = 0;
          }
      }
      gettimeofday (tp, 0);
      (*pthis).r = nqueens (n, 0, 0, n, &(*pthis));
      gettimeofday (tp + 1, 0);
      fprintf (stderr, "time: %lf\n", elapsed_time (tp));
    }
  else
    (*pthis).r = nqueens (n, k, (*pthis).i1, (*pthis).i2, &(*pthis));
  {
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

void send_nqueens_task (struct nqueens *pthis)
{
  send_int ((*pthis).n);
  send_int ((*pthis).k);
  int i = 0;

  if ((*pthis).k >= 0)
    {
      send_int ((*pthis).i1);
      send_int ((*pthis).i2);
      {
        i = 0;
        for (; i < (*pthis).n; i++)
          {
            send_int (((*pthis).a)[i]);
          }
      }
      {
        i = 0;
        for (; i < 2 * (*pthis).n - 1; i++)
          {
            send_int (((*pthis).lb)[i]);
          }
      }
      {
        i = 0;
        for (; i < 2 * (*pthis).n - 1; i++)
          {
            send_int (((*pthis).rb)[i]);
          }
      }
    }
  else;
}

struct nqueens *recv_nqueens_task ()
{
  struct nqueens *pthis = malloc (sizeof (struct nqueens));

  (*pthis).n = recv_int ();
  (*pthis).k = recv_int ();
  int i = 0;

  if ((*pthis).k >= 0)
    {
      (*pthis).i1 = recv_int ();
      (*pthis).i2 = recv_int ();
      {
        i = 0;
        for (; i < (*pthis).n; i++)
          {
            ((*pthis).a)[i] = recv_int ();
          }
      }
      {
        i = 0;
        for (; i < 2 * (*pthis).n - 1; i++)
          {
            ((*pthis).lb)[i] = recv_int ();
          }
      }
      {
        i = 0;
        for (; i < 2 * (*pthis).n - 1; i++)
          {
            ((*pthis).rb)[i] = recv_int ();
          }
      }
    }
  else;
  return pthis;
}

void send_nqueens_rslt (struct nqueens *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void recv_nqueens_rslt (struct nqueens *pthis)
{
  (*pthis).r = recv_int ();
}

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_nqueens_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_nqueens_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_nqueens_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_nqueens_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_nqueens_rslt};

struct worker_data
{
};

void worker_init (struct thread_data *_thr)
{
}
