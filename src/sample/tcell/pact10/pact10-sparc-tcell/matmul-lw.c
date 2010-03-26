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
struct do_tmatmul_task_frame;
struct _bk_in_do_tmatmul_task_frame;
struct do_matmul_start_task_frame;
struct _bk_in_do_matmul_start_task_frame;
struct matmul_s_frame;
struct flush_subtask_queue_frame;
struct matmul_1_frame;
struct do_two_bk_in_matmul_1_frame;
struct do_two_bk2_in_matmul_1_frame;

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

void handle_req (char *esp, closure_t *, struct thread_data *);

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
  double d;
  double s1;
  double s2;

  d = 0.0;
  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
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

void matmul_1 (char *esp, closure_t * _bk, struct thread_data *_thr, int, int,
               int, int, int, double *, double *, double *, int,
               struct tmatmul *);

void matmul_2 (int, int, int, int, int, int, double *, double *, double *, int);

void flush_subtask_queue (char *esp, closure_t * _bk, struct thread_data *_thr,
                          int, double *, double *, double *, int,
                          struct tmatmul *);

struct _bk_in_do_tmatmul_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_tmatmul_task_frame *xfp;
};

struct do_tmatmul_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  struct tmatmul *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_tmatmul_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_tmatmul_task_frame *efp;
  struct do_tmatmul_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

LGOTO:;
  efp = (struct _bk_in_do_tmatmul_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_tmatmul_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_tmatmul_task (char *esp, struct thread_data *_thr,
                      struct tmatmul *pthis)
{
  int i;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct do_tmatmul_task_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_tmatmul_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_tmatmul_task_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL;
        case 1:
          goto L_CALL2;
        }
      goto L_CALL;
    }
  else;
  efp = (struct do_tmatmul_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_tmatmul_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
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
  {
    new_esp = esp;
    while (matmul_1
           (new_esp, &efp->_bk, _thr, (*pthis).i0, (*pthis).i1, (*pthis).j0,
            (*pthis).j1, (*pthis).n, A, B, C, (*pthis).n, &(*pthis)),
           __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
      {
        efp->i = i;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_tmatmul_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 0;
        return;
      L_CALL:;
        i = efp->i;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
  }
  {
    new_esp = esp;
    while (flush_subtask_queue
           (new_esp, &efp->_bk, _thr, (*pthis).n, A, B, C, (*pthis).n,
            &(*pthis)), __builtin_expect ((efp->tmp_esp =
                                           *((char **) esp)) != 0, 0))
      {
        efp->i = i;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_tmatmul_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 1;
        return;
      L_CALL2:;
        i = efp->i;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
  }
}

struct matmul_start
{
  int n;
  char _dummy_[1000];
};

struct _bk_in_do_matmul_start_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_matmul_start_task_frame *xfp;
};

struct do_matmul_start_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  struct tmatmul mtask;
  struct timeval tp[2];
  struct matmul_start *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_matmul_start_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_matmul_start_task_frame *efp;
  struct do_matmul_start_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

LGOTO:;
  efp = (struct _bk_in_do_matmul_start_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_matmul_start_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_matmul_start_task (char *esp, struct thread_data *_thr,
                           struct matmul_start *pthis)
{
  int i;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct do_matmul_start_task_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_matmul_start_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_matmul_start_task_frame) +
                   sizeof (Align_t) + -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL3;
        case 1:
          goto L_CALL4;
        }
      goto L_CALL3;
    }
  else;
  efp = (struct do_matmul_start_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_matmul_start_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  fprintf (stderr, "start matmul(%d)\n", (*pthis).n);
  _setup_data (2 * (*pthis).n);
  {
    genmat ((*pthis).n, A, (*pthis).n);
    genmat ((*pthis).n, B, (*pthis).n);
    _set_exist_flag (0, 2 * (*pthis).n);
  }
  (efp->mtask).n = (*pthis).n;
  (efp->mtask).i0 = 0;
  (efp->mtask).i1 = (*pthis).n;
  (efp->mtask).j0 = 0;
  (efp->mtask).j1 = (*pthis).n;
  (efp->mtask).head = 0;
  {
    i = 0;
    for (; i < 4; i++)
      {
        (((efp->mtask).queue)[i]).fl = 0;
      }
  }
  gettimeofday (efp->tp, 0);
  {
    new_esp = esp;
    while (matmul_1
           (new_esp, &efp->_bk, _thr, (efp->mtask).i0, (efp->mtask).i1,
            (efp->mtask).j0, (efp->mtask).j1, (efp->mtask).n, A, B, C,
            (efp->mtask).n, &efp->mtask), __builtin_expect ((efp->tmp_esp =
                                                             *((char **) esp))
                                                            != 0, 0))
      {
        efp->i = i;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_matmul_start_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 0;
        return;
      L_CALL3:;
        i = efp->i;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
  }
  {
    new_esp = esp;
    while (flush_subtask_queue
           (new_esp, &efp->_bk, _thr, (efp->mtask).n, A, B, C, (efp->mtask).n,
            &efp->mtask), __builtin_expect ((efp->tmp_esp =
                                             *((char **) esp)) != 0, 0))
      {
        efp->i = i;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_matmul_start_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 1;
        return;
      L_CALL4:;
        i = efp->i;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
  }
  gettimeofday (efp->tp + 1, 0);
  fprintf (stderr, "time: %lf\n", elapsed_time (efp->tp));
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
  int half;

  {
  }
  {
  }
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

struct matmul_s_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int h_j1;
  int h_j0;
  int h_i1;
  int h_i0;
  int head;
  struct mm_subtask *sq;
  struct tmatmul *tsk;
  int nx;
  double *c;
  double *b;
  double *a;
  int n;
  int j1;
  int j0;
  int i1;
  int i0;
  struct thread_data *_thr;
  closure_t *_bk;
};

void matmul_s (char *esp, closure_t * _bk, struct thread_data *_thr, int i0,
               int i1, int j0, int j1, int n, double *a, double *b, double *c,
               int nx, struct tmatmul *tsk)
{
  struct mm_subtask *sq;
  int head;
  int h_i0;
  int h_i1;
  int h_j0;
  int h_j1;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct matmul_s_frame *efp;

LGOTO:;
  efp = (struct matmul_s_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct matmul_s_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  sq = (*tsk).queue;
  head = (*tsk).head;
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

struct flush_subtask_queue_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int h_j1;
  int h_j0;
  int h_i1;
  int h_i0;
  struct mm_subtask *sq;
  int head;
  int m;
  struct tmatmul *tsk;
  int nx;
  double *c;
  double *b;
  double *a;
  int n;
  struct thread_data *_thr;
  closure_t *_bk;
};

void flush_subtask_queue (char *esp, closure_t * _bk, struct thread_data *_thr,
                          int n, double *a, double *b, double *c, int nx,
                          struct tmatmul *tsk)
{
  int m;
  int head;
  struct mm_subtask *sq;
  int h_i0;
  int h_i1;
  int h_j0;
  int h_j1;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct flush_subtask_queue_frame *efp;

LGOTO:;
  efp = (struct flush_subtask_queue_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct flush_subtask_queue_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  head = (*tsk).head;
  sq = (*tsk).queue;
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

struct do_two_bk_in_matmul_1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct matmul_1_frame *xfp;
};

struct do_two_bk2_in_matmul_1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct matmul_1_frame *xfp;
};

struct matmul_1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned2;
  struct tmatmul *pthis2;
  struct tmatmul st2;
  int spawned;
  struct tmatmul *pthis;
  struct tmatmul st;
  int half;
  int ifexp_result;
  int ifexp_result4;
  int ifexp_result2;
  int ifexp_result3;
  struct tmatmul *tsk;
  int nx;
  double *c;
  double *b;
  double *a;
  int n;
  int j1;
  int j0;
  int i1;
  int i0;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_two_bk2;
  closure_t do_two_bk;
};

char *do_two_bk_in_matmul_1 (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk_in_matmul_1_frame *efp;
  struct matmul_1_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_two_bk_in_matmul_1_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_two_bk_in_matmul_1_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (xfp->spawned)
    {
      *((int *) efp) = 0;
      return 0;
    }
  else;
  {
    char *goto_fr;
    argp =
      (char *) ((Align_t *) esp +
                (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = xfp->_bk;
    efp->xfp = xfp;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
        return goto_fr;
      else
        {
          efp->tmp_esp = 0;
          goto LGOTO;
        }
    else;
    xfp = efp->xfp;
  }
  if ((*xfp->_thr).treq_top)
    {
      (*xfp->pthis).n = xfp->n;
      (*xfp->pthis).i0 = xfp->half;
      (*xfp->pthis).i1 = xfp->i1;
      (*xfp->pthis).j0 = xfp->j0;
      (*xfp->pthis).j1 = xfp->j1;
      xfp->spawned = 1;
      make_and_send_task (xfp->_thr, 0, xfp->pthis);
      {
        *((int *) efp) = 1;
        return 0;
      }
    }
  else;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

char *do_two_bk2_in_matmul_1 (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk2_in_matmul_1_frame *efp;
  struct matmul_1_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_two_bk2_in_matmul_1_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_two_bk2_in_matmul_1_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (xfp->spawned2)
    {
      *((int *) efp) = 0;
      return 0;
    }
  else;
  {
    char *goto_fr;
    argp =
      (char *) ((Align_t *) esp +
                (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = xfp->_bk;
    efp->xfp = xfp;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
        return goto_fr;
      else
        {
          efp->tmp_esp = 0;
          goto LGOTO;
        }
    else;
    xfp = efp->xfp;
  }
  if ((*xfp->_thr).treq_top)
    {
      (*xfp->pthis2).n = xfp->n;
      (*xfp->pthis2).i0 = xfp->i0;
      (*xfp->pthis2).i1 = xfp->i1;
      (*xfp->pthis2).j0 = xfp->half;
      (*xfp->pthis2).j1 = xfp->j1;
      xfp->spawned2 = 1;
      make_and_send_task (xfp->_thr, 0, xfp->pthis2);
      {
        *((int *) efp) = 1;
        return 0;
      }
    }
  else;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void matmul_1 (char *esp, closure_t * _bk, struct thread_data *_thr, int i0,
               int i1, int j0, int j1, int n, double *a, double *b, double *c,
               int nx, struct tmatmul *tsk)
{
  int ifexp_result3;
  int ifexp_result2;
  int ifexp_result4;
  int ifexp_result;
  int half;
  struct tmatmul *pthis;
  int spawned;
  struct tmatmul *pthis2;
  int spawned2;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct matmul_1_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct matmul_1_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct matmul_1_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL5;
        case 1:
          goto L_CALL6;
        case 2:
          goto L_CALL7;
        case 3:
          goto L_CALL8;
        case 4:
          goto L_CALL9;
        case 5:
          goto L_CALL10;
        case 6:
          goto L_CALL11;
        case 7:
          goto L_CALL12;
        case 8:
          goto L_CALL13;
        }
      goto L_CALL5;
    }
  else;
  efp = (struct matmul_1_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct matmul_1_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
  }
  {
    {
      if (i1 - i0 <= 12)
        {
          if (j1 - j0 <= 12)
            ifexp_result3 = 1;
          else
            ifexp_result3 = 0;
          ifexp_result2 = ifexp_result3;
        }
      else
        ifexp_result2 = 0;
      if (ifexp_result2)
        ifexp_result = 1;
      else
        {
          if (n >> 13 > j1 - j0)
            ifexp_result4 = 1;
          else
            ifexp_result4 = 0;
          ifexp_result = ifexp_result4;
        }
    }
    if (ifexp_result)
      {
        new_esp = esp;
        while (matmul_s
               (new_esp, _bk, _thr, i0, i1, j0, j1, n, a, b, c, nx, tsk),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->spawned2 = spawned2;
            efp->pthis2 = pthis2;
            efp->spawned = spawned;
            efp->pthis = pthis;
            efp->half = half;
            efp->ifexp_result = ifexp_result;
            efp->ifexp_result4 = ifexp_result4;
            efp->ifexp_result2 = ifexp_result2;
            efp->ifexp_result3 = ifexp_result3;
            efp->tsk = tsk;
            efp->nx = nx;
            efp->c = c;
            efp->b = b;
            efp->a = a;
            efp->n = n;
            efp->j1 = j1;
            efp->j0 = j0;
            efp->i1 = i1;
            efp->i0 = i0;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
            efp->do_two_bk2.fr = (void *) efp;
            efp->do_two_bk.fun = do_two_bk_in_matmul_1;
            efp->do_two_bk.fr = (void *) efp;
            efp->call_id = 0;
            return;
          L_CALL5:;
            spawned2 = efp->spawned2;
            pthis2 = efp->pthis2;
            spawned = efp->spawned;
            pthis = efp->pthis;
            half = efp->half;
            ifexp_result = efp->ifexp_result;
            ifexp_result4 = efp->ifexp_result4;
            ifexp_result2 = efp->ifexp_result2;
            ifexp_result3 = efp->ifexp_result3;
            tsk = efp->tsk;
            nx = efp->nx;
            c = efp->c;
            b = efp->b;
            a = efp->a;
            n = efp->n;
            j1 = efp->j1;
            j0 = efp->j0;
            i1 = efp->i1;
            i0 = efp->i0;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    else if (i1 - i0 > j1 - j0)
      {
        half = (i0 + i1) / 2;
        {
          pthis = &efp->st;
          spawned = 0;
          {
            if ((*_thr).req)
              {
                new_esp = esp;
                while (handle_req (new_esp, &efp->do_two_bk, _thr),
                       __builtin_expect ((efp->tmp_esp =
                                          *((char **) esp)) != 0, 0))
                  {
                    efp->spawned2 = spawned2;
                    efp->pthis2 = pthis2;
                    efp->spawned = spawned;
                    efp->pthis = pthis;
                    efp->half = half;
                    efp->ifexp_result = ifexp_result;
                    efp->ifexp_result4 = ifexp_result4;
                    efp->ifexp_result2 = ifexp_result2;
                    efp->ifexp_result3 = ifexp_result3;
                    efp->tsk = tsk;
                    efp->nx = nx;
                    efp->c = c;
                    efp->b = b;
                    efp->a = a;
                    efp->n = n;
                    efp->j1 = j1;
                    efp->j0 = j0;
                    efp->i1 = i1;
                    efp->i0 = i0;
                    efp->_thr = _thr;
                    efp->_bk = _bk;
                    efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                    efp->do_two_bk2.fr = (void *) efp;
                    efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                    efp->do_two_bk.fr = (void *) efp;
                    efp->call_id = 1;
                    return;
                  L_CALL6:;
                    spawned2 = efp->spawned2;
                    pthis2 = efp->pthis2;
                    spawned = efp->spawned;
                    pthis = efp->pthis;
                    half = efp->half;
                    ifexp_result = efp->ifexp_result;
                    ifexp_result4 = efp->ifexp_result4;
                    ifexp_result2 = efp->ifexp_result2;
                    ifexp_result3 = efp->ifexp_result3;
                    tsk = efp->tsk;
                    nx = efp->nx;
                    c = efp->c;
                    b = efp->b;
                    a = efp->a;
                    n = efp->n;
                    j1 = efp->j1;
                    j0 = efp->j0;
                    i1 = efp->i1;
                    i0 = efp->i0;
                    _thr = efp->_thr;
                    _bk = efp->_bk;
                    new_esp = esp + 1;
                  }
              }
            else;
            {
              new_esp = esp;
              while (matmul_1
                     (new_esp, &efp->do_two_bk, _thr, i0, half, j0, j1, n, a, b,
                      c, nx, tsk), __builtin_expect ((efp->tmp_esp =
                                                      *((char **) esp)) != 0,
                                                     0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->half = half;
                  efp->ifexp_result = ifexp_result;
                  efp->ifexp_result4 = ifexp_result4;
                  efp->ifexp_result2 = ifexp_result2;
                  efp->ifexp_result3 = ifexp_result3;
                  efp->tsk = tsk;
                  efp->nx = nx;
                  efp->c = c;
                  efp->b = b;
                  efp->a = a;
                  efp->n = n;
                  efp->j1 = j1;
                  efp->j0 = j0;
                  efp->i1 = i1;
                  efp->i0 = i0;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 2;
                  return;
                L_CALL7:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  half = efp->half;
                  ifexp_result = efp->ifexp_result;
                  ifexp_result4 = efp->ifexp_result4;
                  ifexp_result2 = efp->ifexp_result2;
                  ifexp_result3 = efp->ifexp_result3;
                  tsk = efp->tsk;
                  nx = efp->nx;
                  c = efp->c;
                  b = efp->b;
                  a = efp->a;
                  n = efp->n;
                  j1 = efp->j1;
                  j0 = efp->j0;
                  i1 = efp->i1;
                  i0 = efp->i0;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
          }
          if (spawned)
            {
              new_esp = esp;
              while (wait_rslt (new_esp, _thr),
                     __builtin_expect ((efp->tmp_esp =
                                        *((char **) esp)) != 0, 0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->half = half;
                  efp->ifexp_result = ifexp_result;
                  efp->ifexp_result4 = ifexp_result4;
                  efp->ifexp_result2 = ifexp_result2;
                  efp->ifexp_result3 = ifexp_result3;
                  efp->tsk = tsk;
                  efp->nx = nx;
                  efp->c = c;
                  efp->b = b;
                  efp->a = a;
                  efp->n = n;
                  efp->j1 = j1;
                  efp->j0 = j0;
                  efp->i1 = i1;
                  efp->i0 = i0;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 3;
                  return;
                L_CALL8:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  half = efp->half;
                  ifexp_result = efp->ifexp_result;
                  ifexp_result4 = efp->ifexp_result4;
                  ifexp_result2 = efp->ifexp_result2;
                  ifexp_result3 = efp->ifexp_result3;
                  tsk = efp->tsk;
                  nx = efp->nx;
                  c = efp->c;
                  b = efp->b;
                  a = efp->a;
                  n = efp->n;
                  j1 = efp->j1;
                  j0 = efp->j0;
                  i1 = efp->i1;
                  i0 = efp->i0;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
          else
            {
              new_esp = esp;
              while (matmul_1
                     (new_esp, _bk, _thr, half, i1, j0, j1, n, a, b, c, nx,
                      tsk), __builtin_expect ((efp->tmp_esp =
                                               *((char **) esp)) != 0, 0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->half = half;
                  efp->ifexp_result = ifexp_result;
                  efp->ifexp_result4 = ifexp_result4;
                  efp->ifexp_result2 = ifexp_result2;
                  efp->ifexp_result3 = ifexp_result3;
                  efp->tsk = tsk;
                  efp->nx = nx;
                  efp->c = c;
                  efp->b = b;
                  efp->a = a;
                  efp->n = n;
                  efp->j1 = j1;
                  efp->j0 = j0;
                  efp->i1 = i1;
                  efp->i0 = i0;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 4;
                  return;
                L_CALL9:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  half = efp->half;
                  ifexp_result = efp->ifexp_result;
                  ifexp_result4 = efp->ifexp_result4;
                  ifexp_result2 = efp->ifexp_result2;
                  ifexp_result3 = efp->ifexp_result3;
                  tsk = efp->tsk;
                  nx = efp->nx;
                  c = efp->c;
                  b = efp->b;
                  a = efp->a;
                  n = efp->n;
                  j1 = efp->j1;
                  j0 = efp->j0;
                  i1 = efp->i1;
                  i0 = efp->i0;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
        }
      }
    else
      {
        half = (j0 + j1) / 2;
        {
          pthis2 = &efp->st2;
          spawned2 = 0;
          {
            if ((*_thr).req)
              {
                new_esp = esp;
                while (handle_req (new_esp, &efp->do_two_bk2, _thr),
                       __builtin_expect ((efp->tmp_esp =
                                          *((char **) esp)) != 0, 0))
                  {
                    efp->spawned2 = spawned2;
                    efp->pthis2 = pthis2;
                    efp->spawned = spawned;
                    efp->pthis = pthis;
                    efp->half = half;
                    efp->ifexp_result = ifexp_result;
                    efp->ifexp_result4 = ifexp_result4;
                    efp->ifexp_result2 = ifexp_result2;
                    efp->ifexp_result3 = ifexp_result3;
                    efp->tsk = tsk;
                    efp->nx = nx;
                    efp->c = c;
                    efp->b = b;
                    efp->a = a;
                    efp->n = n;
                    efp->j1 = j1;
                    efp->j0 = j0;
                    efp->i1 = i1;
                    efp->i0 = i0;
                    efp->_thr = _thr;
                    efp->_bk = _bk;
                    efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                    efp->do_two_bk2.fr = (void *) efp;
                    efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                    efp->do_two_bk.fr = (void *) efp;
                    efp->call_id = 5;
                    return;
                  L_CALL10:;
                    spawned2 = efp->spawned2;
                    pthis2 = efp->pthis2;
                    spawned = efp->spawned;
                    pthis = efp->pthis;
                    half = efp->half;
                    ifexp_result = efp->ifexp_result;
                    ifexp_result4 = efp->ifexp_result4;
                    ifexp_result2 = efp->ifexp_result2;
                    ifexp_result3 = efp->ifexp_result3;
                    tsk = efp->tsk;
                    nx = efp->nx;
                    c = efp->c;
                    b = efp->b;
                    a = efp->a;
                    n = efp->n;
                    j1 = efp->j1;
                    j0 = efp->j0;
                    i1 = efp->i1;
                    i0 = efp->i0;
                    _thr = efp->_thr;
                    _bk = efp->_bk;
                    new_esp = esp + 1;
                  }
              }
            else;
            {
              new_esp = esp;
              while (matmul_1
                     (new_esp, &efp->do_two_bk2, _thr, i0, i1, j0, half, n, a,
                      b, c, nx, tsk), __builtin_expect ((efp->tmp_esp =
                                                         *((char **) esp)) != 0,
                                                        0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->half = half;
                  efp->ifexp_result = ifexp_result;
                  efp->ifexp_result4 = ifexp_result4;
                  efp->ifexp_result2 = ifexp_result2;
                  efp->ifexp_result3 = ifexp_result3;
                  efp->tsk = tsk;
                  efp->nx = nx;
                  efp->c = c;
                  efp->b = b;
                  efp->a = a;
                  efp->n = n;
                  efp->j1 = j1;
                  efp->j0 = j0;
                  efp->i1 = i1;
                  efp->i0 = i0;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 6;
                  return;
                L_CALL11:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  half = efp->half;
                  ifexp_result = efp->ifexp_result;
                  ifexp_result4 = efp->ifexp_result4;
                  ifexp_result2 = efp->ifexp_result2;
                  ifexp_result3 = efp->ifexp_result3;
                  tsk = efp->tsk;
                  nx = efp->nx;
                  c = efp->c;
                  b = efp->b;
                  a = efp->a;
                  n = efp->n;
                  j1 = efp->j1;
                  j0 = efp->j0;
                  i1 = efp->i1;
                  i0 = efp->i0;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
          }
          if (spawned2)
            {
              new_esp = esp;
              while (wait_rslt (new_esp, _thr),
                     __builtin_expect ((efp->tmp_esp =
                                        *((char **) esp)) != 0, 0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->half = half;
                  efp->ifexp_result = ifexp_result;
                  efp->ifexp_result4 = ifexp_result4;
                  efp->ifexp_result2 = ifexp_result2;
                  efp->ifexp_result3 = ifexp_result3;
                  efp->tsk = tsk;
                  efp->nx = nx;
                  efp->c = c;
                  efp->b = b;
                  efp->a = a;
                  efp->n = n;
                  efp->j1 = j1;
                  efp->j0 = j0;
                  efp->i1 = i1;
                  efp->i0 = i0;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 7;
                  return;
                L_CALL12:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  half = efp->half;
                  ifexp_result = efp->ifexp_result;
                  ifexp_result4 = efp->ifexp_result4;
                  ifexp_result2 = efp->ifexp_result2;
                  ifexp_result3 = efp->ifexp_result3;
                  tsk = efp->tsk;
                  nx = efp->nx;
                  c = efp->c;
                  b = efp->b;
                  a = efp->a;
                  n = efp->n;
                  j1 = efp->j1;
                  j0 = efp->j0;
                  i1 = efp->i1;
                  i0 = efp->i0;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
          else
            {
              new_esp = esp;
              while (matmul_1
                     (new_esp, _bk, _thr, i0, i1, half, j1, n, a, b, c, nx,
                      tsk), __builtin_expect ((efp->tmp_esp =
                                               *((char **) esp)) != 0, 0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->half = half;
                  efp->ifexp_result = ifexp_result;
                  efp->ifexp_result4 = ifexp_result4;
                  efp->ifexp_result2 = ifexp_result2;
                  efp->ifexp_result3 = ifexp_result3;
                  efp->tsk = tsk;
                  efp->nx = nx;
                  efp->c = c;
                  efp->b = b;
                  efp->a = a;
                  efp->n = n;
                  efp->j1 = j1;
                  efp->j0 = j0;
                  efp->i1 = i1;
                  efp->i0 = i0;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_matmul_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_matmul_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 8;
                  return;
                L_CALL13:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  half = efp->half;
                  ifexp_result = efp->ifexp_result;
                  ifexp_result4 = efp->ifexp_result4;
                  ifexp_result2 = efp->ifexp_result2;
                  ifexp_result3 = efp->ifexp_result3;
                  tsk = efp->tsk;
                  nx = efp->nx;
                  c = efp->c;
                  b = efp->b;
                  a = efp->a;
                  n = efp->n;
                  j1 = efp->j1;
                  j0 = efp->j0;
                  i1 = efp->i1;
                  i0 = efp->i0;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
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
  struct matmul_start *pthis;
  pthis = malloc (sizeof (struct matmul_start));
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
  struct tmatmul *pthis;
  pthis = malloc (sizeof (struct tmatmul));
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
void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_tmatmul_task,
    (void (*)(char *, struct thread_data *, void *)) do_matmul_start_task};
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
