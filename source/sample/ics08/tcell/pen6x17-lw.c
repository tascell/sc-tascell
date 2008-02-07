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
{ OUTSIDE = -1, INSIDE = -2, ANY = -3, PARENT = -4, TERM = -5 };
enum command
{ TASK, RSLT, TREQ, NONE, RACK, EXIT, WRNG };
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

void proto_error (char const *str, struct cmd *pcmd);

void read_to_eol (void);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_none (struct cmd *);
struct task;
struct thread_data;
void (*task_doers[256]) (char *, struct thread_data *, void *);
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
  enum node rslt_head[16];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
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
};

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (char *esp, struct thread_data *thr);

int serialize_cmdname (char *buf, enum command w);

int deserialize_cmdname (enum command *buf, char *str);

int serialize_arg (char *buf, enum node *arg);

enum node deserialize_node (char *str);

int deserialize_arg (enum node *buf, char *str);

int serialize_cmd (char *buf, struct cmd *pcmd);

int deserialize_cmd (struct cmd *pcmd, char *str);

int copy_address (enum node *dst, enum node *src);

void handle_req (char *esp, closure_t *, struct thread_data *);
#include<sys/time.h>

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

double sqrt (double);

double fabs (double);

void send_int (int n);

int recv_int (void);

int send_binary_header (int elmsize, int nelm);

int recv_binary_header (int *pelmsize, int *pnelm);

void send_binary_terminator (void);

void recv_binary_terminator (void);

void swap_doubles (double *a, int n);

int send_double_seq (double *a, int nelm);

int recv_double_seq (double *a, int nelm);

int send_doubles (double *a, int nelm);

int recv_doubles (double *a, int nelm);

struct pentomino
{
  int r;
  int k;
  int i0;
  int i1;
  int i2;
  int maxp;
  int maxk;
  int a[20];
  int b[119];
};

int search (char *esp, closure_t * _bk, struct thread_data *_thr, int k,
            int j0, int j1, int j2, int maxp, int maxk,
            struct pentomino *tsk);

struct do_pentomino_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int maxk;
  int maxp;
  int i2;
  int i1;
  int i0;
  int k;
  int i;
  int tmp;
  struct pentomino *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

struct _bk_in_do_pentomino_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_pentomino_task_frame *xfp;
};

char *
_bk_in_do_pentomino_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_pentomino_task_frame *efp;
  struct do_pentomino_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
LGOTO:;
  efp = (struct _bk_in_do_pentomino_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_pentomino_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  *((int *) efp) = 0;
  return 0;
  return 0;
}

void
do_pentomino_task (char *esp, struct thread_data *_thr,
                   struct pentomino *pthis)
{
  struct do_pentomino_task_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int tmp;
  int i;
  int k;
  int i0;
  int i1;
  int i2;
  int maxp;
  int maxk;
  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_pentomino_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_pentomino_task_frame) +
                   sizeof (Align_t) + -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL;
        }
      goto L_CALL;
    }
  efp = (struct do_pentomino_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_pentomino_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  k = (*pthis).k;
  i0 = (*pthis).i0;
  i1 = (*pthis).i1;
  i2 = (*pthis).i2;
  maxp = (*pthis).maxp;
  maxk = (*pthis).maxk;
  //fprintf (stderr, "start %d %d %d %d %d %d\n", k, i0, i1, i2, maxp, maxk);
  {
    new_esp = esp;
    while (__builtin_expect
           ((tmp =
             search (new_esp, &efp->_bk, _thr, k, i0, i1, i2, maxp, maxk,
                     &(*pthis))) == (int) 0 - 1, 0)
           && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 1))
      {
        efp->maxk = maxk;
        efp->maxp = maxp;
        efp->i2 = i2;
        efp->i1 = i1;
        efp->i0 = i0;
        efp->k = k;
        efp->i = i;
        efp->tmp = tmp;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_pentomino_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 0;
        return;
      L_CALL:;
        maxk = efp->maxk;
        maxp = efp->maxp;
        i2 = efp->i2;
        i1 = efp->i1;
        i0 = efp->i0;
        k = efp->k;
        i = efp->i;
        tmp = efp->tmp;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
    (*pthis).r = tmp;
  }
  //fprintf (stderr, "end %d\n", (*pthis).r);
}

struct start_pentomino
{
  int maxp;
  int r;
};

double
elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

struct do_start_pentomino_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int kk;
  int i;
  int maxp;
  struct pentomino pen;
  struct timeval tp[2];
  int tmp2;
  int ifexp_result;
  int ifexp_result2;
  struct start_pentomino *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

struct _bk_in_do_start_pentomino_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_start_pentomino_task_frame *xfp;
};

char *
_bk_in_do_start_pentomino_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_start_pentomino_task_frame *efp;
  struct do_start_pentomino_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
LGOTO:;
  efp = (struct _bk_in_do_start_pentomino_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_start_pentomino_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  *((int *) efp) = 0;
  return 0;
  return 0;
}

void
do_start_pentomino_task (char *esp, struct thread_data *_thr,
                         struct start_pentomino *pthis)
{
  struct do_start_pentomino_task_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int ifexp_result2;
  int ifexp_result;
  int tmp2;
  int maxp;
  int i;
  int kk;
  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_start_pentomino_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_start_pentomino_task_frame) +
                   sizeof (Align_t) + -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL2;
        }
      goto L_CALL2;
    }
  efp = (struct do_start_pentomino_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_start_pentomino_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  maxp = (*pthis).maxp;
  {
    if (maxp < 0)
      {
        ifexp_result = 1;
      }
    else
      {
        if (maxp > 20)
          {
            ifexp_result2 = 1;
          }
        else
          {
            ifexp_result2 = 0;
          }
        ifexp_result = ifexp_result2;
      }
    if (ifexp_result)
      {
        fprintf (stderr, "maxp=%d: no more than 20 pieces\n", maxp);
        maxp = 12;
      }
  }
  {
    i = 0;
    for (; i < 20; i++)
      {
        ((efp->pen).a)[i] = 0;
      }
  }
  {
    i = 0;
    for (; i < maxp; i++)
      {
        ((efp->pen).a)[i] = i;
      }
  }
  {
    i = 0;
    for (; i < 119; i++)
      {
        ((efp->pen).b)[i] = 0;
      }
  }
  {
    i = 6;
    for (; i < 119; i += 7)
      {
        ((efp->pen).b)[i] = '\n';
      }
  }
  kk = 0;
  {
    i = 0;
    for (; i < maxp * 5; i++)
      {
        kk++;
        for (; kk < 119; kk++)
          {
            if (((efp->pen).b)[kk] == 0)
              break;
          }
      }
  }
  (efp->pen).maxk = kk;
  for (; kk < 119; kk++)
    {
      if (((efp->pen).b)[kk] == 0)
        {
          ((efp->pen).b)[kk] = '*';
        }
    }
  (efp->pen).maxp = maxp;
  gettimeofday (efp->tp, 0);
  {
    new_esp = esp;
    while (__builtin_expect
           ((tmp2 =
             search (new_esp, &efp->_bk, _thr, 0, 0, 0, maxp, maxp,
                     (efp->pen).maxk, &efp->pen)) == (int) 0 - 1, 0)
           && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 1))
      {
        efp->kk = kk;
        efp->i = i;
        efp->maxp = maxp;
        efp->tmp2 = tmp2;
        efp->ifexp_result = ifexp_result;
        efp->ifexp_result2 = ifexp_result2;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_start_pentomino_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 0;
        return;
      L_CALL2:;
        kk = efp->kk;
        i = efp->i;
        maxp = efp->maxp;
        tmp2 = efp->tmp2;
        ifexp_result = efp->ifexp_result;
        ifexp_result2 = efp->ifexp_result2;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
    (*pthis).r = tmp2;
  }
  gettimeofday (efp->tp + 1, 0);
  fprintf (stderr, "time: %lf\n", elapsed_time (efp->tp));
}
int ps[73][5] =
  { {1, 1, 1, 1}, {7, 7, 7, 7}, {4, 1, 1, 1}, {7, 7, 6, 1}, {6, 1, 1, 1}, {5,
                                                                           1,
                                                                           1,
                                                                           1},
  {1, 1, 1, 5}, {1, 1, 1, 6}, {7, 1, 6, 7}, {7, 7, 1, 6}, {7, 6, 1, 7}, {6, 1,
                                                                         7,
                                                                         7},
  {7, 7, 1, 1}, {1, 1, 5, 7}, {1, 1, 7, 7}, {7, 5, 1, 1}, {2, 5, 1, 1}, {1, 1,
                                                                         5,
                                                                         2},
  {1, 6, 7, 1}, {1, 7, 6, 1}, {7, 1, 1, 5}, {5, 1, 1, 7}, {7, 6, 1, 1}, {1, 1,
                                                                         6,
                                                                         7},
  {7, 1, 1, 6}, {5, 1, 1, 6}, {6, 1, 1, 5}, {6, 1, 1, 7}, {1, 7, 1, 6}, {7, 1,
                                                                         5,
                                                                         1},
  {1, 5, 1, 7}, {6, 1, 7, 1}, {7, 1, 1, 7}, {5, 1, 1, 5}, {1, 7, 7, 1}, {1, 6,
                                                                         6,
                                                                         1},
  {6, 1, 1, 6}, {1, 6, 1, 1}, {1, 1, 5, 1}, {1, 5, 1, 1}, {1, 1, 6, 1}, {7, 1,
                                                                         6,
                                                                         1},
  {1, 6, 1, 6}, {6, 1, 6, 1}, {1, 6, 1, 7}, {1, 4, 1, 1}, {1, 1, 7, 1}, {1, 7,
                                                                         1,
                                                                         1},
  {1, 1, 4, 1}, {7, 1, 7, 7}, {7, 6, 1, 6}, {6, 1, 6, 7}, {7, 7, 1, 7}, {7, 1,
                                                                         7,
                                                                         1},
  {1, 5, 1, 6}, {1, 7, 1, 7}, {6, 1, 5, 1}, {1, 1, 1, 1}, {7, 7, 7, 7}, {1, 1,
                                                                         1,
                                                                         1},
  {7, 7, 7, 7}, {1, 1, 1, 1}, {7, 7, 7, 7}, {1, 1, 1, 1}, {7, 7, 7, 7}, {1, 1,
                                                                         1,
                                                                         1},
  {7, 7, 7, 7}, {1, 1, 1, 1}, {7, 7, 7, 7}, {1, 1, 1, 1}, {7, 7, 7, 7}, {1, 1,
                                                                         1,
                                                                         1},
  {7, 7, 7, 7} };
int pos[21] =
  { 0, 2, 4, 12, 16, 20, 24, 32, 36, 37, 45, 53, 57, 59, 61, 63, 65, 67, 69,
71, 73 };

struct search_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp4;
  int tmp3;
  int ifexp_result9;
  int ifexp_result12;
  int ifexp_result13;
  int ifexp_result14;
  int ifexp_result15;
  int ifexp_result18;
  int ifexp_result16;
  int ifexp_result17;
  int ifexp_result10;
  int ifexp_result11;
  int ifexp_result7;
  int ifexp_result8;
  int l;
  int kk;
  int *pss;
  int i;
  int ap;
  int spawned;
  struct pentomino *pthis;
  int p_end;
  int p;
  int s;
  struct pentomino *tsk;
  int maxk;
  int maxp;
  int j2;
  int j1;
  int j0;
  int k;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t dwind_bk;
  closure_t do_many_bk;
};

struct do_many_bk_in_search_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i1;
  int i2;
  int ifexp_result5;
  int ifexp_result6;
  int ifexp_result3;
  int ifexp_result4;
  struct search_frame *xfp;
};

char *
do_many_bk_in_search (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_many_bk_in_search_frame *efp;
  struct search_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;
  int ifexp_result4;
  int ifexp_result3;
  int ifexp_result6;
  int ifexp_result5;
  int i2;
  int i1;
LGOTO:;
  efp = (struct do_many_bk_in_search_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk_in_search_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (!xfp->spawned)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      efp->ifexp_result5 = ifexp_result5;
      efp->ifexp_result6 = ifexp_result6;
      efp->ifexp_result3 = ifexp_result3;
      efp->ifexp_result4 = ifexp_result4;
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
      ifexp_result5 = efp->ifexp_result5;
      ifexp_result6 = efp->ifexp_result6;
      ifexp_result3 = efp->ifexp_result3;
      ifexp_result4 = efp->ifexp_result4;
      xfp = efp->xfp;
    }
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->p + 1 < xfp->p_end)
          {
            ifexp_result4 = 1;
          }
        else
          {
            ifexp_result4 = 0;
          }
        ifexp_result3 = ifexp_result4;
      }
    else
      {
        ifexp_result3 = 0;
      }
    if (ifexp_result3)
      {
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->p + 1 < xfp->p_end)
                  {
                    ifexp_result6 = 1;
                  }
                else
                  {
                    ifexp_result6 = 0;
                  }
                ifexp_result5 = ifexp_result6;
              }
            else
              {
                ifexp_result5 = 0;
              }
            if (!ifexp_result5)
              goto loop_end;
            i2 = xfp->p_end;
            i1 = (1 + xfp->p + xfp->p_end) / 2;
            xfp->p_end = i1;
            xfp->pthis =
              (struct pentomino *) malloc (sizeof (struct pentomino));
            *xfp->pthis = *xfp->tsk;
            (*xfp->pthis).k = xfp->k;
            (*xfp->pthis).i0 = xfp->j0;
            (*xfp->pthis).i1 = i1;
            (*xfp->pthis).i2 = i2;
            (xfp->spawned)++;
            make_and_send_task (xfp->_thr, 0, xfp->pthis);
          }
      loop_end:;
      }
  }
  *((int *) efp) = 0;
  return 0;
  return 0;
}

struct dwind_bk_in_search_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct search_frame *xfp;
};

char *
dwind_bk_in_search (char *esp, void *xfp0)
{
  char *new_esp;
  struct dwind_bk_in_search_frame *efp;
  struct search_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;
LGOTO:;
  efp = (struct dwind_bk_in_search_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct dwind_bk_in_search_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    xfp->ap = ((*xfp->tsk).a)[xfp->j0];
    ((*xfp->tsk).a)[xfp->j0] = ((*xfp->tsk).a)[xfp->p];
    ((*xfp->tsk).a)[xfp->p] = xfp->ap;
    ((*xfp->tsk).b)[xfp->kk = xfp->k] = 0;
    {
      xfp->l = 0;
      for (; xfp->l < 4; (xfp->l)++)
        {
          ((*xfp->tsk).b)[xfp->kk += (xfp->pss)[xfp->l]] = 0;
        }
    }
  }
  {
    char *goto_fr;
    argp =
      (char *) ((Align_t *) esp +
                (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = &xfp->do_many_bk;
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
    xfp = efp->xfp;
  }
  {
    ((*xfp->tsk).b)[xfp->kk = xfp->k] = xfp->p + 'A';
    {
      xfp->l = 0;
      for (; xfp->l < 4; (xfp->l)++)
        {
          ((*xfp->tsk).b)[xfp->kk += (xfp->pss)[xfp->l]] = xfp->p + 'A';
        }
    }
    ((*xfp->tsk).a)[xfp->p] = ((*xfp->tsk).a)[xfp->j0];
    ((*xfp->tsk).a)[xfp->j0] = xfp->ap;
  }
  return 0;
}

int
search (char *esp, closure_t * _bk, struct thread_data *_thr, int k, int j0,
        int j1, int j2, int maxp, int maxk, struct pentomino *tsk)
{
  struct search_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int s;
  int p;
  int p_end;
  struct pentomino *pthis;
  int spawned;
  int ap;
  int i;
  int *pss;
  int kk;
  int l;
  int ifexp_result8;
  int ifexp_result7;
  int ifexp_result11;
  int ifexp_result10;
  int ifexp_result17;
  int ifexp_result16;
  int ifexp_result18;
  int ifexp_result15;
  int ifexp_result14;
  int ifexp_result13;
  int ifexp_result12;
  int ifexp_result9;
  int tmp3;
  void *tmp4;
  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct search_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct search_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL3;
        case 1:
          goto L_CALL4;
        case 2:
          goto L_CALL5;
        }
      goto L_CALL3;
    }
  efp = (struct search_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  s = 0;
  {
    p = j1;
    p_end = j2;
    spawned = 0;
    if ((*_thr).req)
      {
        new_esp = esp;
        while (handle_req (new_esp, &efp->do_many_bk, _thr),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->spawned = spawned;
            efp->pthis = pthis;
            efp->p_end = p_end;
            efp->p = p;
            efp->s = s;
            efp->tsk = tsk;
            efp->maxk = maxk;
            efp->maxp = maxp;
            efp->j2 = j2;
            efp->j1 = j1;
            efp->j0 = j0;
            efp->k = k;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_many_bk.fun = do_many_bk_in_search;
            efp->do_many_bk.fr = (void *) efp;
            efp->call_id = 0;
            return (int) 0 - 1;
          L_CALL3:;
            spawned = efp->spawned;
            pthis = efp->pthis;
            p_end = efp->p_end;
            p = efp->p;
            s = efp->s;
            tsk = efp->tsk;
            maxk = efp->maxk;
            maxp = efp->maxp;
            j2 = efp->j2;
            j1 = efp->j1;
            j0 = efp->j0;
            k = efp->k;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    for (; p < p_end; p++)
      {
        ap = ((*tsk).a)[p];
        {
          i = pos[ap];
          for (; i < pos[ap + 1]; i++)
            {
              pss = ps[i];
              kk = k;
              {
                l = 0;
                for (; l < 4; l++)
                  {
                    if ((kk += pss[l]) >= maxk)
                      {
                        ifexp_result7 = 1;
                      }
                    else
                      {
                        if (((*tsk).b)[kk] != 0)
                          {
                            ifexp_result8 = 1;
                          }
                        else
                          {
                            ifexp_result8 = 0;
                          }
                        ifexp_result7 = ifexp_result8;
                      }
                    if (ifexp_result7)
                      goto Ln;
                  }
              }
              {
                {
                  ((*tsk).b)[kk = k] = p + 'A';
                  {
                    l = 0;
                    for (; l < 4; l++)
                      {
                        ((*tsk).b)[kk += pss[l]] = p + 'A';
                      }
                  }
                  ((*tsk).a)[p] = ((*tsk).a)[j0];
                  ((*tsk).a)[j0] = ap;
                }
                {
                  {
                    {
                      kk = k;
                      for (; kk < maxk; kk++)
                        {
                          if (((*tsk).b)[kk] == 0)
                            break;
                        }
                    }
                    if (kk == maxk)
                      {
                        s += 1;
                      }
                    else
                      {
                        {
                          if (kk + 7 >= maxk)
                            {
                              ifexp_result10 = 1;
                            }
                          else
                            {
                              if (((*tsk).b)[kk + 7] != 0)
                                {
                                  ifexp_result11 = 1;
                                }
                              else
                                {
                                  ifexp_result11 = 0;
                                }
                              ifexp_result10 = ifexp_result11;
                            }
                          if (ifexp_result10)
                            {
                              {
                                if (((*tsk).b)[kk + 1] != 0)
                                  {
                                    ifexp_result13 = 1;
                                  }
                                else
                                  {
                                    {
                                      {
                                        if (kk + 8 >= maxk)
                                          {
                                            ifexp_result16 = 1;
                                          }
                                        else
                                          {
                                            if (((*tsk).b)[kk + 8] != 0)
                                              {
                                                ifexp_result17 = 1;
                                              }
                                            else
                                              {
                                                ifexp_result17 = 0;
                                              }
                                            ifexp_result16 = ifexp_result17;
                                          }
                                        if (ifexp_result16)
                                          {
                                            if (((*tsk).b)[kk + 2] != 0)
                                              {
                                                ifexp_result18 = 1;
                                              }
                                            else
                                              {
                                                ifexp_result18 = 0;
                                              }
                                            ifexp_result15 = ifexp_result18;
                                          }
                                        else
                                          {
                                            ifexp_result15 = 0;
                                          }
                                      }
                                      if (ifexp_result15)
                                        {
                                          ifexp_result14 = 1;
                                        }
                                      else
                                        {
                                          ifexp_result14 = 0;
                                        }
                                    }
                                    ifexp_result13 = ifexp_result14;
                                  }
                                if (ifexp_result13)
                                  {
                                    ifexp_result12 = 1;
                                  }
                                else
                                  {
                                    ifexp_result12 = 0;
                                  }
                              }
                              ifexp_result9 = ifexp_result12;
                            }
                          else
                            {
                              ifexp_result9 = 0;
                            }
                        }
                        if (ifexp_result9)
                          {
                          }
                        else
                          {
                            new_esp = esp;
                            while (__builtin_expect
                                   ((tmp3 =
                                     search (new_esp, &efp->dwind_bk, _thr,
                                             kk, j0 + 1, j0 + 1, maxp, maxp,
                                             maxk, tsk)) == (int) 0 - 1, 0)
                                   &&
                                   __builtin_expect ((efp->tmp_esp =
                                                      *((char **) esp)) != 0,
                                                     1))
                              {
                                efp->tmp3 = tmp3;
                                efp->ifexp_result9 = ifexp_result9;
                                efp->ifexp_result12 = ifexp_result12;
                                efp->ifexp_result13 = ifexp_result13;
                                efp->ifexp_result14 = ifexp_result14;
                                efp->ifexp_result15 = ifexp_result15;
                                efp->ifexp_result18 = ifexp_result18;
                                efp->ifexp_result16 = ifexp_result16;
                                efp->ifexp_result17 = ifexp_result17;
                                efp->ifexp_result10 = ifexp_result10;
                                efp->ifexp_result11 = ifexp_result11;
                                efp->ifexp_result7 = ifexp_result7;
                                efp->ifexp_result8 = ifexp_result8;
                                efp->l = l;
                                efp->kk = kk;
                                efp->pss = pss;
                                efp->i = i;
                                efp->ap = ap;
                                efp->spawned = spawned;
                                efp->pthis = pthis;
                                efp->p_end = p_end;
                                efp->p = p;
                                efp->s = s;
                                efp->tsk = tsk;
                                efp->maxk = maxk;
                                efp->maxp = maxp;
                                efp->j2 = j2;
                                efp->j1 = j1;
                                efp->j0 = j0;
                                efp->k = k;
                                efp->_thr = _thr;
                                efp->_bk = _bk;
                                efp->dwind_bk.fun = dwind_bk_in_search;
                                efp->dwind_bk.fr = (void *) efp;
                                efp->do_many_bk.fun = do_many_bk_in_search;
                                efp->do_many_bk.fr = (void *) efp;
                                efp->call_id = 1;
                                return (int) 0 - 1;
                              L_CALL4:;
                                tmp3 = efp->tmp3;
                                ifexp_result9 = efp->ifexp_result9;
                                ifexp_result12 = efp->ifexp_result12;
                                ifexp_result13 = efp->ifexp_result13;
                                ifexp_result14 = efp->ifexp_result14;
                                ifexp_result15 = efp->ifexp_result15;
                                ifexp_result18 = efp->ifexp_result18;
                                ifexp_result16 = efp->ifexp_result16;
                                ifexp_result17 = efp->ifexp_result17;
                                ifexp_result10 = efp->ifexp_result10;
                                ifexp_result11 = efp->ifexp_result11;
                                ifexp_result7 = efp->ifexp_result7;
                                ifexp_result8 = efp->ifexp_result8;
                                l = efp->l;
                                kk = efp->kk;
                                pss = efp->pss;
                                i = efp->i;
                                ap = efp->ap;
                                spawned = efp->spawned;
                                pthis = efp->pthis;
                                p_end = efp->p_end;
                                p = efp->p;
                                s = efp->s;
                                tsk = efp->tsk;
                                maxk = efp->maxk;
                                maxp = efp->maxp;
                                j2 = efp->j2;
                                j1 = efp->j1;
                                j0 = efp->j0;
                                k = efp->k;
                                _thr = efp->_thr;
                                _bk = efp->_bk;
                                new_esp = esp + 1;
                              }
                            s += tmp3;
                          }
                      }
                  }
                  {
                    ap = ((*tsk).a)[j0];
                    ((*tsk).a)[j0] = ((*tsk).a)[p];
                    ((*tsk).a)[p] = ap;
                    ((*tsk).b)[kk = k] = 0;
                    {
                      l = 0;
                      for (; l < 4; l++)
                        {
                          ((*tsk).b)[kk += pss[l]] = 0;
                        }
                    }
                  }
                }
              }
            Ln:;
              continue;
            }
        }
      }
    while (spawned-- > 0)
      {
        {
          new_esp = esp;
          while (__builtin_expect
                 ((tmp4 = wait_rslt (new_esp, _thr)) == (void *) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              efp->tmp4 = tmp4;
              efp->tmp3 = tmp3;
              efp->ifexp_result9 = ifexp_result9;
              efp->ifexp_result12 = ifexp_result12;
              efp->ifexp_result13 = ifexp_result13;
              efp->ifexp_result14 = ifexp_result14;
              efp->ifexp_result15 = ifexp_result15;
              efp->ifexp_result18 = ifexp_result18;
              efp->ifexp_result16 = ifexp_result16;
              efp->ifexp_result17 = ifexp_result17;
              efp->ifexp_result10 = ifexp_result10;
              efp->ifexp_result11 = ifexp_result11;
              efp->ifexp_result7 = ifexp_result7;
              efp->ifexp_result8 = ifexp_result8;
              efp->l = l;
              efp->kk = kk;
              efp->pss = pss;
              efp->i = i;
              efp->ap = ap;
              efp->spawned = spawned;
              efp->pthis = pthis;
              efp->p_end = p_end;
              efp->p = p;
              efp->s = s;
              efp->tsk = tsk;
              efp->maxk = maxk;
              efp->maxp = maxp;
              efp->j2 = j2;
              efp->j1 = j1;
              efp->j0 = j0;
              efp->k = k;
              efp->_thr = _thr;
              efp->_bk = _bk;
              efp->dwind_bk.fun = dwind_bk_in_search;
              efp->dwind_bk.fr = (void *) efp;
              efp->do_many_bk.fun = do_many_bk_in_search;
              efp->do_many_bk.fr = (void *) efp;
              efp->call_id = 2;
              return (int) 0 - 1;
            L_CALL5:;
              tmp4 = efp->tmp4;
              tmp3 = efp->tmp3;
              ifexp_result9 = efp->ifexp_result9;
              ifexp_result12 = efp->ifexp_result12;
              ifexp_result13 = efp->ifexp_result13;
              ifexp_result14 = efp->ifexp_result14;
              ifexp_result15 = efp->ifexp_result15;
              ifexp_result18 = efp->ifexp_result18;
              ifexp_result16 = efp->ifexp_result16;
              ifexp_result17 = efp->ifexp_result17;
              ifexp_result10 = efp->ifexp_result10;
              ifexp_result11 = efp->ifexp_result11;
              ifexp_result7 = efp->ifexp_result7;
              ifexp_result8 = efp->ifexp_result8;
              l = efp->l;
              kk = efp->kk;
              pss = efp->pss;
              i = efp->i;
              ap = efp->ap;
              spawned = efp->spawned;
              pthis = efp->pthis;
              p_end = efp->p_end;
              p = efp->p;
              s = efp->s;
              tsk = efp->tsk;
              maxk = efp->maxk;
              maxp = efp->maxp;
              j2 = efp->j2;
              j1 = efp->j1;
              j0 = efp->j0;
              k = efp->k;
              _thr = efp->_thr;
              _bk = efp->_bk;
              new_esp = esp + 1;
            }
          pthis = (struct pentomino *) tmp4;
        }
        s += (*pthis).r;
        free (pthis);
      }
  }
  return s;
}

void
send_start_pentomino_task (struct start_pentomino *pthis)
{
  send_int ((*pthis).maxp);
}

struct start_pentomino *
recv_start_pentomino_task ()
{
  struct start_pentomino *pthis = malloc (sizeof (struct start_pentomino));
  (*pthis).maxp = recv_int ();
  return pthis;
}

void
send_start_pentomino_rslt (struct start_pentomino *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void
recv_start_pentomino_rslt (struct start_pentomino *pthis)
{
  (*pthis).r = recv_int ();
}

void
send_pentomino_task (struct pentomino *pthis)
{
  send_int ((*pthis).k);
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
  send_int ((*pthis).i2);
  send_int ((*pthis).maxp);
  send_int ((*pthis).maxk);
  {
    int i3;
    int sz = 20;
    {
      i3 = 0;
      for (; i3 < sz; i3++)
        {
          send_int (((*pthis).a)[i3]);
        }
    }
  }
  {
    int i4;
    int sz2 = 119;
    {
      i4 = 0;
      for (; i4 < sz2; i4++)
        {
          send_int (((*pthis).b)[i4]);
        }
    }
  }
}

struct pentomino *
recv_pentomino_task ()
{
  struct pentomino *pthis = malloc (sizeof (struct pentomino));
  (*pthis).k = recv_int ();
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  (*pthis).i2 = recv_int ();
  (*pthis).maxp = recv_int ();
  (*pthis).maxk = recv_int ();
  {
    int i5;
    int sz3 = 20;
    {
      i5 = 0;
      for (; i5 < sz3; i5++)
        {
          ((*pthis).a)[i5] = recv_int ();
        }
    }
  }
  {
    int i6;
    int sz4 = 119;
    {
      i6 = 0;
      for (; i6 < sz4; i6++)
        {
          ((*pthis).b)[i6] = recv_int ();
        }
    }
  }
  return pthis;
}

void
send_pentomino_rslt (struct pentomino *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void
recv_pentomino_rslt (struct pentomino *pthis)
{
  (*pthis).r = recv_int ();
}

void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_pentomino_task,
    (void (*)(char *, struct thread_data *, void *)) do_start_pentomino_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_pentomino_task,
    (void (*)(void *)) send_start_pentomino_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_pentomino_task, (void *(*)()) recv_start_pentomino_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_pentomino_rslt,
    (void (*)(void *)) send_start_pentomino_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_pentomino_rslt,
    (void (*)(void *)) recv_start_pentomino_rslt};
