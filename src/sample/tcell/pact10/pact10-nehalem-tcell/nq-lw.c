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
struct nqueens_frame;
struct do_many_bk_in_nqueens_frame;
struct dwind_bk_in_nqueens_frame;
struct do_nqueens_task_frame;
struct _bk_in_do_nqueens_task_frame;

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

struct do_many_bk_in_nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i1;
  int i2;
  int ifexp_result3;
  int ifexp_result4;
  int ifexp_result;
  int ifexp_result2;
  struct nqueens_frame *xfp;
};

struct dwind_bk_in_nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct nqueens_frame *xfp;
};

struct nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int ai;
  int spawned;
  struct nqueens *pthis;
  int I_end;
  int i;
  int s;
  int ifexp_result5;
  int ifexp_result6;
  int tmp;
  void *tmp2;
  struct nqueens *tsk;
  int iy;
  int ix;
  int k;
  int n;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t dwind_bk;
  closure_t do_many_bk;
};

char *do_many_bk_in_nqueens (char *esp, void *xfp0)
{
  int ifexp_result2;
  int ifexp_result;
  int ifexp_result4;
  int ifexp_result3;
  int i2;
  int i1;
  char *new_esp;
  struct do_many_bk_in_nqueens_frame *efp;
  struct nqueens_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_many_bk_in_nqueens_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk_in_nqueens_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (!xfp->spawned)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      efp->i1 = i1;
      efp->i2 = i2;
      efp->ifexp_result3 = ifexp_result3;
      efp->ifexp_result4 = ifexp_result4;
      efp->ifexp_result = ifexp_result;
      efp->ifexp_result2 = ifexp_result2;
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
      i1 = efp->i1;
      i2 = efp->i2;
      ifexp_result3 = efp->ifexp_result3;
      ifexp_result4 = efp->ifexp_result4;
      ifexp_result = efp->ifexp_result;
      ifexp_result2 = efp->ifexp_result2;
      xfp = efp->xfp;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->i + 1 < xfp->I_end)
          ifexp_result2 = 1;
        else
          ifexp_result2 = 0;
        ifexp_result = ifexp_result2;
      }
    else
      ifexp_result = 0;
    if (ifexp_result)
      {
        goto loop_start;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->i + 1 < xfp->I_end)
                  ifexp_result4 = 1;
                else
                  ifexp_result4 = 0;
                ifexp_result3 = ifexp_result4;
              }
            else
              ifexp_result3 = 0;
            if (!ifexp_result3)
              goto loop_end;
            else;
          loop_start:;
            i2 = xfp->I_end;
            i1 = (1 + xfp->i + xfp->I_end) / 2;
            xfp->I_end = i1;
            xfp->pthis = (struct nqueens *) malloc (sizeof (struct nqueens));
            *xfp->pthis = *xfp->tsk;
            (*xfp->pthis).k = xfp->k;
            (*xfp->pthis).i1 = i1;
            (*xfp->pthis).i2 = i2;
            (xfp->spawned)++;
            make_and_send_task (xfp->_thr, 0, xfp->pthis);
          }
      loop_end:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

char *dwind_bk_in_nqueens (char *esp, void *xfp0)
{
  char *new_esp;
  struct dwind_bk_in_nqueens_frame *efp;
  struct nqueens_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct dwind_bk_in_nqueens_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct dwind_bk_in_nqueens_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    ((*xfp->tsk).lb)[xfp->n + -1 + -xfp->ai + xfp->k] = 0;
    ((*xfp->tsk).rb)[xfp->ai + xfp->k] = 0;
    xfp->ai = ((*xfp->tsk).a)[xfp->k];
    ((*xfp->tsk).a)[xfp->k] = ((*xfp->tsk).a)[xfp->i];
    ((*xfp->tsk).a)[xfp->i] = xfp->ai;
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
    else;
    xfp = efp->xfp;
  }
  {
    ((*xfp->tsk).lb)[xfp->n + -1 + -xfp->ai + xfp->k] = 1;
    ((*xfp->tsk).rb)[xfp->ai + xfp->k] = 1;
    ((*xfp->tsk).a)[xfp->i] = ((*xfp->tsk).a)[xfp->k];
    ((*xfp->tsk).a)[xfp->k] = xfp->ai;
  }
  return 0;
}

int nqueens (char *esp, closure_t * _bk, struct thread_data *_thr, int n, int k,
             int ix, int iy, struct nqueens *tsk)
{
  void *tmp2;
  int tmp;
  int ifexp_result6;
  int ifexp_result5;
  int s;
  int i;
  int I_end;
  struct nqueens *pthis;
  int spawned;
  int ai;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct nqueens_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct nqueens_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct nqueens_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL;
        case 1:
          goto L_CALL2;
        case 2:
          goto L_CALL3;
        }
      goto L_CALL;
    }
  else;
  efp = (struct nqueens_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct nqueens_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  s = 0;
  {
    i = ix;
    I_end = iy;
    spawned = 0;
    if ((*_thr).req)
      {
        new_esp = esp;
        while (handle_req (new_esp, &efp->do_many_bk, _thr),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->ai = ai;
            efp->spawned = spawned;
            efp->pthis = pthis;
            efp->I_end = I_end;
            efp->i = i;
            efp->s = s;
            efp->ifexp_result5 = ifexp_result5;
            efp->ifexp_result6 = ifexp_result6;
            efp->tmp = tmp;
            efp->tmp2 = tmp2;
            efp->tsk = tsk;
            efp->iy = iy;
            efp->ix = ix;
            efp->k = k;
            efp->n = n;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->dwind_bk.fun = dwind_bk_in_nqueens;
            efp->dwind_bk.fr = (void *) efp;
            efp->do_many_bk.fun = do_many_bk_in_nqueens;
            efp->do_many_bk.fr = (void *) efp;
            efp->call_id = 0;
            return (int) 0 - 1;
          L_CALL:;
            ai = efp->ai;
            spawned = efp->spawned;
            pthis = efp->pthis;
            I_end = efp->I_end;
            i = efp->i;
            s = efp->s;
            ifexp_result5 = efp->ifexp_result5;
            ifexp_result6 = efp->ifexp_result6;
            tmp = efp->tmp;
            tmp2 = efp->tmp2;
            tsk = efp->tsk;
            iy = efp->iy;
            ix = efp->ix;
            k = efp->k;
            n = efp->n;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    else;
    for (; i < I_end; i++)
      {
        ai = ((*tsk).a)[i];
        {
          if (((*tsk).lb)[n + -1 + -ai + k])
            ifexp_result5 = 1;
          else
            {
              if (((*tsk).rb)[ai + k])
                ifexp_result6 = 1;
              else
                ifexp_result6 = 0;
              ifexp_result5 = ifexp_result6;
            }
          if (!ifexp_result5)
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
                    {
                      new_esp = esp;
                      while (__builtin_expect
                             ((tmp =
                               nqueens (new_esp, &efp->dwind_bk, _thr, n, k + 1,
                                        k + 1, n, tsk)) == (int) 0 - 1, 0)
                             &&
                             __builtin_expect ((efp->tmp_esp =
                                                *((char **) esp)) != 0, 1))
                        {
                          efp->ai = ai;
                          efp->spawned = spawned;
                          efp->pthis = pthis;
                          efp->I_end = I_end;
                          efp->i = i;
                          efp->s = s;
                          efp->ifexp_result5 = ifexp_result5;
                          efp->ifexp_result6 = ifexp_result6;
                          efp->tmp = tmp;
                          efp->tmp2 = tmp2;
                          efp->tsk = tsk;
                          efp->iy = iy;
                          efp->ix = ix;
                          efp->k = k;
                          efp->n = n;
                          efp->_thr = _thr;
                          efp->_bk = _bk;
                          efp->dwind_bk.fun = dwind_bk_in_nqueens;
                          efp->dwind_bk.fr = (void *) efp;
                          efp->do_many_bk.fun = do_many_bk_in_nqueens;
                          efp->do_many_bk.fr = (void *) efp;
                          efp->call_id = 1;
                          return (int) 0 - 1;
                        L_CALL2:;
                          ai = efp->ai;
                          spawned = efp->spawned;
                          pthis = efp->pthis;
                          I_end = efp->I_end;
                          i = efp->i;
                          s = efp->s;
                          ifexp_result5 = efp->ifexp_result5;
                          ifexp_result6 = efp->ifexp_result6;
                          tmp = efp->tmp;
                          tmp2 = efp->tmp2;
                          tsk = efp->tsk;
                          iy = efp->iy;
                          ix = efp->ix;
                          k = efp->k;
                          n = efp->n;
                          _thr = efp->_thr;
                          _bk = efp->_bk;
                          new_esp = esp + 1;
                        }
                    }
                    s += tmp;
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
    while (spawned-- > 0)
      {
        {
          {
            new_esp = esp;
            while (__builtin_expect
                   ((tmp2 = wait_rslt (new_esp, _thr)) == (void *) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->ai = ai;
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->I_end = I_end;
                efp->i = i;
                efp->s = s;
                efp->ifexp_result5 = ifexp_result5;
                efp->ifexp_result6 = ifexp_result6;
                efp->tmp = tmp;
                efp->tmp2 = tmp2;
                efp->tsk = tsk;
                efp->iy = iy;
                efp->ix = ix;
                efp->k = k;
                efp->n = n;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->dwind_bk.fun = dwind_bk_in_nqueens;
                efp->dwind_bk.fr = (void *) efp;
                efp->do_many_bk.fun = do_many_bk_in_nqueens;
                efp->do_many_bk.fr = (void *) efp;
                efp->call_id = 2;
                return (int) 0 - 1;
              L_CALL3:;
                ai = efp->ai;
                spawned = efp->spawned;
                pthis = efp->pthis;
                I_end = efp->I_end;
                i = efp->i;
                s = efp->s;
                ifexp_result5 = efp->ifexp_result5;
                ifexp_result6 = efp->ifexp_result6;
                tmp = efp->tmp;
                tmp2 = efp->tmp2;
                tsk = efp->tsk;
                iy = efp->iy;
                ix = efp->ix;
                k = efp->k;
                n = efp->n;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
          pthis = (struct nqueens *) tmp2;
        }
        s += (*pthis).r;
        free (pthis);
      }
  }
  return s;
}

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0E-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

struct _bk_in_do_nqueens_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_nqueens_task_frame *xfp;
};

struct do_nqueens_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct timeval tp[2];
  int k;
  int n;
  int i;
  int tmp4;
  int tmp3;
  struct nqueens *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_nqueens_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_nqueens_task_frame *efp;
  struct do_nqueens_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

LGOTO:;
  efp = (struct _bk_in_do_nqueens_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_nqueens_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_nqueens_task (char *esp, struct thread_data *_thr,
                      struct nqueens *pthis)
{
  int tmp3;
  int tmp4;
  int i;
  int n;
  int k;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct do_nqueens_task_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_nqueens_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_nqueens_task_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL4;
        case 1:
          goto L_CALL5;
        }
      goto L_CALL4;
    }
  else;
  efp = (struct do_nqueens_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_nqueens_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  n = (*pthis).n;
  k = (*pthis).k;
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
      gettimeofday (efp->tp, 0);
      {
        {
          new_esp = esp;
          while (__builtin_expect
                 ((tmp3 =
                   nqueens (new_esp, &efp->_bk, _thr, n, 0, 0, n,
                            &(*pthis))) == (int) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              efp->k = k;
              efp->n = n;
              efp->i = i;
              efp->tmp4 = tmp4;
              efp->tmp3 = tmp3;
              efp->pthis = pthis;
              efp->_thr = _thr;
              efp->_bk.fun = _bk_in_do_nqueens_task;
              efp->_bk.fr = (void *) efp;
              efp->call_id = 0;
              return;
            L_CALL4:;
              k = efp->k;
              n = efp->n;
              i = efp->i;
              tmp4 = efp->tmp4;
              tmp3 = efp->tmp3;
              pthis = efp->pthis;
              _thr = efp->_thr;
              new_esp = esp + 1;
            }
        }
        (*pthis).r = tmp3;
      }
      gettimeofday (efp->tp + 1, 0);
      fprintf (stderr, "time: %lf\n", elapsed_time (efp->tp));
    }
  else
    {
      {
        new_esp = esp;
        while (__builtin_expect
               ((tmp4 =
                 nqueens (new_esp, &efp->_bk, _thr, n, k, (*pthis).i1,
                          (*pthis).i2, &(*pthis))) == (int) 0 - 1, 0)
               && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 1))
          {
            efp->k = k;
            efp->n = n;
            efp->i = i;
            efp->tmp4 = tmp4;
            efp->tmp3 = tmp3;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_nqueens_task;
            efp->_bk.fr = (void *) efp;
            efp->call_id = 1;
            return;
          L_CALL5:;
            k = efp->k;
            n = efp->n;
            i = efp->i;
            tmp4 = efp->tmp4;
            tmp3 = efp->tmp3;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp + 1;
          }
      }
      (*pthis).r = tmp4;
    }
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
  int i;

  send_int ((*pthis).n);
  send_int ((*pthis).k);
  i = 0;
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
  struct nqueens *pthis;
  int i;
  pthis = malloc (sizeof (struct nqueens));
  (*pthis).n = recv_int ();
  (*pthis).k = recv_int ();
  i = 0;
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

void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_nqueens_task};
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
