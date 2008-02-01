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

void handle_req (int (*)(void), struct thread_data *);
#include<sys/time.h>

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

double
elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

int fib (int (*_bk) (void), struct thread_data *_thr, int n);

struct fib
{
  int n;
  int r;
};

void
do_fib_task (struct thread_data *_thr, struct fib *pthis)
{

  int _bk (void)
  {
    return 0;
  }
  (*pthis).r = fib (_bk, _thr, (*pthis).n);
}

struct fib_start
{
  int n;
  int r;
};

void
do_fib_start_task (struct thread_data *_thr, struct fib_start *pthis)
{

  int _bk (void)
  {
    return 0;
  }
  struct timeval tp[2];
  fprintf (stderr, "start fib(%d)\n", (*pthis).n);
  gettimeofday (tp, 0);
  (*pthis).r = fib (_bk, _thr, (*pthis).n);
  gettimeofday (tp + 1, 0);
  fprintf (stderr, "time: %lf\n", elapsed_time (tp));
}

int
fib (int (*_bk) (void), struct thread_data *_thr, int n)
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
            _bk ();
            if ((*_thr).treq_top)
              {
                (*pthis).n = n - 2;
                spawned = 1;
                make_and_send_task (_thr, 0, pthis);
                return 1;
              }
            return 0;
          }
          if ((*_thr).req)
            handle_req (do_two_bk, _thr);
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

void
send_fib_start_task (struct fib_start *pthis)
{
  send_int ((*pthis).n);
}

struct fib_start *
recv_fib_start_task ()
{
  struct fib_start *pthis = malloc (sizeof (struct fib_start));
  (*pthis).n = recv_int ();
  return pthis;
}

void
send_fib_start_rslt (struct fib_start *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void
recv_fib_start_rslt (struct fib_start *pthis)
{
  (*pthis).r = recv_int ();
}

void
send_fib_task (struct fib *pthis)
{
  send_int ((*pthis).n);
}

struct fib *
recv_fib_task ()
{
  struct fib *pthis = malloc (sizeof (struct fib));
  (*pthis).n = recv_int ();
  return pthis;
}

void
send_fib_rslt (struct fib *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void
recv_fib_rslt (struct fib *pthis)
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
