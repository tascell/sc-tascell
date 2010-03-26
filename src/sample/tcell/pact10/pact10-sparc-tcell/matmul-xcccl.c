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

void handle_req (int (*)closure (void), struct thread_data *);

#include<sys/time.h>
#include<assert.h>

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

void genmat (int n, double *a, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            a[i * nx + j] = n - (i - j >= 0 ? i - j : j - i);
          }
      }
  }
}

void zero_mat (int n, double *a, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            a[i * nx + j] = 0;
          }
      }
  }
}

double diffmat (int n, double *c1, double *c2, int nx)
{
  int i;
  int j;
  double d = 0.0;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            double s1;
            double s2;

            s1 = c1[i * nx + j];
            s2 = c2[i * nx + j];
            d += s1 > s2 ? s1 - s2 : s2 - s1;
          }
      }
  }
  return d;
}

double *A;
double *B;
double *C;
int N;
int NX;

void data_allocate (int n)
{
  N = n / 2;
  NX = N;
  A = (double *) malloc (N * N * sizeof (double));
  B = (double *) malloc (N * N * sizeof (double));
  C = (double *) malloc (N * N * sizeof (double));
}

void data_send (int n1, int n2)
{
  double *p;

  if (n1 > N)
    {
      n1 -= N;
      n2 -= N;
      p = B;
    }
  else
    p = A;
  assert (n2 > n1);
  send_doubles (p + n1 * N, (n2 - n1) * N);
}

void data_receive (int n1, int n2)
{
  double *p;

  if (n1 > N)
    {
      n1 -= N;
      n2 -= N;
      p = B;
    }
  else
    p = A;
  recv_doubles (p + n1 * N, (n2 - n1) * N);
}

struct mm_subtask
{
  int fl;
  int i0;
  int i1;
  int j0;
  int j1;
};

struct tmatmul
{
  int n;
  int i0;
  int i1;
  int j0;
  int j1;
  int head;
  struct mm_subtask queue[4];
  char _dummy_[1000];
};

void matmul_1 (int (*_bk) closure (void), struct thread_data *_thr, int, int,
               int, int, int, double *, double *, double *, int,
               struct tmatmul *);

void matmul_2 (int, int, int, int, int, int, double *, double *, double *, int);

void flush_subtask_queue (int (*_bk) closure (void), struct thread_data *_thr,
                          int, double *, double *, double *, int,
                          struct tmatmul *);

void do_tmatmul_task (struct thread_data *_thr, struct tmatmul *pthis)
{

  int _bk closure (void)
  {
    return 0;
  }
  int i;

  (*pthis).head = 0;
  _setup_data (2 * (*pthis).n);
  {
    i = 0;
    for (; i < 4; i++)
      {
        (((*pthis).queue)[i]).fl = 0;
      }
  }
  {
  }
  matmul_1 (_bk, _thr, (*pthis).i0, (*pthis).i1, (*pthis).j0, (*pthis).j1,
            (*pthis).n, A, B, C, (*pthis).n, &(*pthis));
  flush_subtask_queue (_bk, _thr, (*pthis).n, A, B, C, (*pthis).n, &(*pthis));
}

struct matmul_start
{
  int n;
  char _dummy_[1000];
};

void do_matmul_start_task (struct thread_data *_thr, struct matmul_start *pthis)
{

  int _bk closure (void)
  {
    return 0;
  }
  struct timeval tp[2];
  struct tmatmul mtask;
  int i;

  fprintf (stderr, "start matmul(%d)\n", (*pthis).n);
  _setup_data (2 * (*pthis).n);
  {
    genmat ((*pthis).n, A, (*pthis).n);
    genmat ((*pthis).n, B, (*pthis).n);
    _set_exist_flag (0, 2 * (*pthis).n);
  }
  mtask.n = (*pthis).n;
  mtask.i0 = 0;
  mtask.i1 = (*pthis).n;
  mtask.j0 = 0;
  mtask.j1 = (*pthis).n;
  mtask.head = 0;
  {
    i = 0;
    for (; i < 4; i++)
      {
        ((mtask.queue)[i]).fl = 0;
      }
  }
  gettimeofday (tp, 0);
  matmul_1 (_bk, _thr, mtask.i0, mtask.i1, mtask.j0, mtask.j1, mtask.n, A, B, C,
            mtask.n, &mtask);
  flush_subtask_queue (_bk, _thr, mtask.n, A, B, C, mtask.n, &mtask);
  gettimeofday (tp + 1, 0);
  fprintf (stderr, "time: %lf\n", elapsed_time (tp));
  {
  }
}

void matmul_2 (int i0, int i1, int j0, int j1, int k0, int k1, double *a,
               double *b, double *c, int nx)
{
  int i;
  int j;
  int k;
  double s;

  {
  }
  {
  }
  {
    i = i0;
    for (; i < i1; i++)
      {
        j = j0;
        for (; j < j1; j++)
          {
            s = 0.0;
            {
              k = k0;
              for (; k < k1; k++)
                {
                  s += a[i * nx + k] * b[j * nx + k];
                  if (k0 == 0)
                    c[i * nx + j] = 0.0;
                  else
                    c[i * nx + j] += s;
                }
            }
          }
      }
  }
  return;
}

void matmul_m (int i0, int i1, int j0, int j1, int k0, int k1, double *a,
               double *b, double *c, int nx)
{
  {
  }
  {
  }
  int half;

  if (i1 - i0 <= 12 && j1 - j0 <= 12)
    {
      matmul_2 (i0, i1, j0, j1, k0, k1, a, b, c, nx);
    }
  else if (k1 - k0 >> 13 > j1 - j0)
    {
      half = (k0 + k1) / 2;
      matmul_m (i0, i1, j0, j1, k0, half, a, b, c, nx);
      matmul_m (i0, i1, j0, j1, half, k1, a, b, c, nx);
    }
  else if (i1 - i0 > j1 - j0)
    {
      half = (i0 + i1) / 2;
      matmul_m (i0, half, j0, j1, k0, k1, a, b, c, nx);
      matmul_m (half, i1, j0, j1, k0, k1, a, b, c, nx);
    }
  else
    {
      half = (j0 + j1) / 2;
      matmul_m (i0, i1, j0, half, k0, k1, a, b, c, nx);
      matmul_m (i0, i1, half, j1, k0, k1, a, b, c, nx);
    }
}

void matmul_s (int (*_bk) closure (void), struct thread_data *_thr, int i0,
               int i1, int j0, int j1, int n, double *a, double *b, double *c,
               int nx, struct tmatmul *tsk)
{
  struct mm_subtask *sq = (*tsk).queue;
  int head = (*tsk).head;
  int h_i0;
  int h_i1;
  int h_j0;
  int h_j1;

  {
  }
  {
  }
  _request_data (_thr, i0, i1);
  _request_data (_thr, j0, j1);
  if ((sq[head]).fl)
    {
      h_i0 = (sq[head]).i0;
      h_i1 = (sq[head]).i1;
      h_j0 = (sq[head]).j0;
      h_j1 = (sq[head]).j1;
      (sq[head]).fl = 0;
      _wait_data (h_i0, h_i1);
      _wait_data (h_j0, h_j1);
      matmul_m (h_i0, h_i1, h_j0, h_j1, 0, n, a, b, c, nx);
    }
  else;
  (sq[head]).i0 = i0;
  (sq[head]).i1 = i1;
  (sq[head]).j0 = j0;
  (sq[head]).j1 = j1;
  (sq[head]).fl = 1;
  head++;
  if (head >= 4)
    head = 0;
  else;
  (*tsk).head = head;
}

void flush_subtask_queue (int (*_bk) closure (void), struct thread_data *_thr,
                          int n, double *a, double *b, double *c, int nx,
                          struct tmatmul *tsk)
{
  int m;
  int head = (*tsk).head;
  struct mm_subtask *sq = (*tsk).queue;
  int h_i0;
  int h_i1;
  int h_j0;
  int h_j1;

  {
    m = 0;
    for (; m < 4; m++)
      {
        if ((sq[head]).fl)
          {
            h_i0 = (sq[head]).i0;
            h_i1 = (sq[head]).i1;
            h_j0 = (sq[head]).j0;
            h_j1 = (sq[head]).j1;
            (sq[head]).fl = 0;
            _wait_data (h_i0, h_i1);
            _wait_data (h_j0, h_j1);
            matmul_m (h_i0, h_i1, h_j0, h_j1, 0, n, a, b, c, nx);
          }
        else;
        head++;
        if (head >= 4)
          head = 0;
        else;
      }
  }
}

void matmul_1 (int (*_bk) closure (void), struct thread_data *_thr, int i0,
               int i1, int j0, int j1, int n, double *a, double *b, double *c,
               int nx, struct tmatmul *tsk)
{
  {
  }
  int half;

  if (i1 - i0 <= 12 && j1 - j0 <= 12 || n >> 13 > j1 - j0)
    {
      matmul_s (_bk, _thr, i0, i1, j0, j1, n, a, b, c, nx, tsk);
    }
  else if (i1 - i0 > j1 - j0)
    {
      half = (i0 + i1) / 2;
      {
        struct tmatmul st;
        struct tmatmul *pthis = &st;
        int spawned = 0;

        {

          int do_two_bk closure (void)
          {
            if (spawned)
              return 0;
            else;
            _bk ();
            if ((*_thr).treq_top)
              {
                (*pthis).n = n;
                (*pthis).i0 = half;
                (*pthis).i1 = i1;
                (*pthis).j0 = j0;
                (*pthis).j1 = j1;
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
            matmul_1 (do_two_bk, _thr, i0, half, j0, j1, n, a, b, c, nx, tsk);
          }
        }
        if (spawned)
          {
            wait_rslt (_thr);
          }
        else
          {
            matmul_1 (_bk, _thr, half, i1, j0, j1, n, a, b, c, nx, tsk);
          }
      }
    }
  else
    {
      half = (j0 + j1) / 2;
      {
        struct tmatmul st2;
        struct tmatmul *pthis = &st2;
        int spawned2 = 0;

        {

          int do_two_bk2 closure (void)
          {
            if (spawned2)
              return 0;
            else;
            _bk ();
            if ((*_thr).treq_top)
              {
                (*pthis).n = n;
                (*pthis).i0 = i0;
                (*pthis).i1 = i1;
                (*pthis).j0 = half;
                (*pthis).j1 = j1;
                spawned2 = 1;
                make_and_send_task (_thr, 0, pthis);
                return 1;
              }
            else;
            return 0;
          }
          if ((*_thr).req)
            handle_req (do_two_bk2, _thr);
          else;
          {
            matmul_1 (do_two_bk2, _thr, i0, i1, j0, half, n, a, b, c, nx, tsk);
          }
        }
        if (spawned2)
          {
            wait_rslt (_thr);
          }
        else
          {
            matmul_1 (_bk, _thr, i0, i1, half, j1, n, a, b, c, nx, tsk);
          }
      }
    }
  return;
}

void send_matmul_start_task (struct matmul_start *pthis)
{
  send_int ((*pthis).n);
}

struct matmul_start *recv_matmul_start_task ()
{
  struct matmul_start *pthis = malloc (sizeof (struct matmul_start));

  (*pthis).n = recv_int ();
  return pthis;
}

void send_matmul_start_rslt (struct matmul_start *pthis)
{
  free (pthis);
}

void recv_matmul_start_rslt (struct matmul_start *pthis)
{
}

void send_tmatmul_task (struct tmatmul *pthis)
{
  send_int ((*pthis).n);
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
  send_int ((*pthis).j0);
  send_int ((*pthis).j1);
}

struct tmatmul *recv_tmatmul_task ()
{
  struct tmatmul *pthis = malloc (sizeof (struct tmatmul));

  (*pthis).n = recv_int ();
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  (*pthis).j0 = recv_int ();
  (*pthis).j1 = recv_int ();
  return pthis;
}

void send_tmatmul_rslt (struct tmatmul *pthis)
{
  int i;
  int j;
  int idiff;
  int jdiff;

  {
  }
  idiff = (*pthis).i1 - (*pthis).i0;
  jdiff = (*pthis).j1 - (*pthis).j0;
  assert (idiff * jdiff > 0);
  send_binary_header (sizeof (double), idiff * jdiff);
  {
    i = (*pthis).i0;
    for (; i < (*pthis).i1; i++)
      {
        send_double_seq (C + i * NX + (*pthis).j0, jdiff);
      }
  }
  send_binary_terminator ();
  {
  }
  free (pthis);
}

void recv_tmatmul_rslt (struct tmatmul *pthis)
{
  int i;
  int idiff;
  int jdiff;
  int swp;

  {
  }
  idiff = (*pthis).i1 - (*pthis).i0;
  jdiff = (*pthis).j1 - (*pthis).j0;
  swp = recv_binary_header (0, 0);
  {
    i = (*pthis).i0;
    for (; i < (*pthis).i1; i++)
      {
        recv_double_seq (C + i * NX + (*pthis).j0, jdiff);
      }
  }
  recv_binary_terminator ();
  if (swp)
    {
      i = (*pthis).i0;
      for (; i < (*pthis).i1; i++)
        {
          swap_doubles (C + i * NX + (*pthis).j0, jdiff);
        }
    }
  else;
  {
  }
}
void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_tmatmul_task,
    (void (*)(struct thread_data *, void *)) do_matmul_start_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_tmatmul_task,
    (void (*)(void *)) send_matmul_start_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_tmatmul_task, (void *(*)()) recv_matmul_start_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_tmatmul_rslt,
    (void (*)(void *)) send_matmul_start_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_tmatmul_rslt,
    (void (*)(void *)) recv_matmul_start_rslt};

struct worker_data
{
};

void worker_init (struct thread_data *_thr)
{
}
