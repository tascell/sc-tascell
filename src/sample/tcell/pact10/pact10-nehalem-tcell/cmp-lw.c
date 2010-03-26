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
struct do_cmp_task_frame;
struct _bk_in_do_cmp_task_frame;
struct cmp_1_frame;
struct do_two_bk_in_cmp_1_frame;
struct do_two_bk2_in_cmp_1_frame;

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
#include<stdint.h>

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

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0E-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

struct cmp
{
  int r;
  int n1;
  int n2;
  int *d1;
  int *d2;
  char _dummy_[1000];
};

int cmp_1 (char *esp, closure_t * _bk, struct thread_data *_thr, int n1, int n2,
           int *d1, int *d2);

struct _bk_in_do_cmp_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_cmp_task_frame *xfp;
};

struct do_cmp_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int j;
  int i;
  struct timeval tp[2];
  int tmp2;
  int tmp;
  struct cmp *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_cmp_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_cmp_task_frame *efp;
  struct do_cmp_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

LGOTO:;
  efp = (struct _bk_in_do_cmp_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_cmp_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_cmp_task (char *esp, struct thread_data *_thr, struct cmp *pthis)
{
  int tmp;
  int tmp2;
  int i;
  int j;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct do_cmp_task_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_cmp_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_cmp_task_frame) + sizeof (Align_t) +
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
  efp = (struct do_cmp_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_cmp_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
  }
  if (0 > (*pthis).n2)
    {
      (*pthis).d1 = (int *) malloc (sizeof (int) * (*pthis).n1);
      (*pthis).n2 = (*pthis).n1;
      (*pthis).d2 = (int *) malloc (sizeof (int) * (*pthis).n2);
      {
        i = 0;
        for (; i < (*pthis).n1; i++)
          {
            ((*pthis).d1)[i] = i;
          }
      }
      {
        i = 0;
        for (; i < (*pthis).n2; i++)
          {
            ((*pthis).d2)[i] = -i;
          }
      }
      gettimeofday (efp->tp, 0);
      {
        {
          new_esp = esp;
          while (__builtin_expect
                 ((tmp =
                   cmp_1 (new_esp, &efp->_bk, _thr, (*pthis).n1, (*pthis).n2,
                          (*pthis).d1, (*pthis).d2)) == (int) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              efp->j = j;
              efp->i = i;
              efp->tmp2 = tmp2;
              efp->tmp = tmp;
              efp->pthis = pthis;
              efp->_thr = _thr;
              efp->_bk.fun = _bk_in_do_cmp_task;
              efp->_bk.fr = (void *) efp;
              efp->call_id = 0;
              return;
            L_CALL:;
              j = efp->j;
              i = efp->i;
              tmp2 = efp->tmp2;
              tmp = efp->tmp;
              pthis = efp->pthis;
              _thr = efp->_thr;
              new_esp = esp + 1;
            }
        }
        (*pthis).r = tmp;
      }
      gettimeofday (efp->tp + 1, 0);
      fprintf (stderr, "time: %lf\n", elapsed_time (efp->tp));
    }
  else
    {
      {
        new_esp = esp;
        while (__builtin_expect
               ((tmp2 =
                 cmp_1 (new_esp, &efp->_bk, _thr, (*pthis).n1, (*pthis).n2,
                        (*pthis).d1, (*pthis).d2)) == (int) 0 - 1, 0)
               && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 1))
          {
            efp->j = j;
            efp->i = i;
            efp->tmp2 = tmp2;
            efp->tmp = tmp;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_cmp_task;
            efp->_bk.fr = (void *) efp;
            efp->call_id = 1;
            return;
          L_CALL2:;
            j = efp->j;
            i = efp->i;
            tmp2 = efp->tmp2;
            tmp = efp->tmp;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp + 1;
          }
      }
      (*pthis).r = tmp2;
    }
}

int cmp_2 (int n1, int n2, int *d1, int *d2)
{
  int i;
  int j;
  int s;

  s = 0;
  {
    i = 0;
    for (; i < n1; i++)
      {
        j = 0;
        for (; j < n2; j++)
          {
            if ((d1[i] ^ d2[j]) == -1)
              s++;
            else;
          }
      }
  }
  return s;
}

struct do_two_bk_in_cmp_1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct cmp_1_frame *xfp;
};

struct do_two_bk2_in_cmp_1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct cmp_1_frame *xfp;
};

struct cmp_1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned2;
  struct cmp *pthis2;
  struct cmp st2;
  int n2_2;
  int n2_1;
  int spawned;
  struct cmp *pthis;
  struct cmp st;
  int n1_2;
  int n1_1;
  int s2;
  int s1;
  int *d2;
  int *d1;
  int n2;
  int n1;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_two_bk2;
  closure_t do_two_bk;
};

char *do_two_bk_in_cmp_1 (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk_in_cmp_1_frame *efp;
  struct cmp_1_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_two_bk_in_cmp_1_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_two_bk_in_cmp_1_frame) + sizeof (Align_t) +
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
      (*xfp->pthis).n1 = xfp->n1_2;
      (*xfp->pthis).n2 = xfp->n2;
      (*xfp->pthis).d1 = xfp->d1 + xfp->n1_1;
      (*xfp->pthis).d2 = xfp->d2;
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

char *do_two_bk2_in_cmp_1 (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk2_in_cmp_1_frame *efp;
  struct cmp_1_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_two_bk2_in_cmp_1_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_two_bk2_in_cmp_1_frame) + sizeof (Align_t) +
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
      (*xfp->pthis2).n1 = xfp->n1;
      (*xfp->pthis2).n2 = xfp->n2_2;
      (*xfp->pthis2).d1 = xfp->d1;
      (*xfp->pthis2).d2 = xfp->d2 + xfp->n2_1;
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

int cmp_1 (char *esp, closure_t * _bk, struct thread_data *_thr, int n1, int n2,
           int *d1, int *d2)
{
  int s1;
  int s2;
  int n1_1;
  int n1_2;
  struct cmp *pthis;
  int spawned;
  int n2_1;
  int n2_2;
  struct cmp *pthis2;
  int spawned2;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct cmp_1_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct cmp_1_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct cmp_1_frame) + sizeof (Align_t) +
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
        case 3:
          goto L_CALL6;
        case 4:
          goto L_CALL7;
        case 5:
          goto L_CALL8;
        case 6:
          goto L_CALL9;
        case 7:
          goto L_CALL10;
        }
      goto L_CALL3;
    }
  else;
  efp = (struct cmp_1_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct cmp_1_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (n1 < 5)
    return cmp_2 (n1, n2, d1, d2);
  else;
  if (n1 > n2)
    {
      n1_1 = n1 / 2;
      n1_2 = n1 - n1_1;
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
                  efp->n2_2 = n2_2;
                  efp->n2_1 = n2_1;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->n1_2 = n1_2;
                  efp->n1_1 = n1_1;
                  efp->s2 = s2;
                  efp->s1 = s1;
                  efp->d2 = d2;
                  efp->d1 = d1;
                  efp->n2 = n2;
                  efp->n1 = n1;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 0;
                  return (int) 0 - 1;
                L_CALL3:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  n2_2 = efp->n2_2;
                  n2_1 = efp->n2_1;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  n1_2 = efp->n1_2;
                  n1_1 = efp->n1_1;
                  s2 = efp->s2;
                  s1 = efp->s1;
                  d2 = efp->d2;
                  d1 = efp->d1;
                  n2 = efp->n2;
                  n1 = efp->n1;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
          else;
          {
            new_esp = esp;
            while (__builtin_expect
                   ((s1 =
                     cmp_1 (new_esp, &efp->do_two_bk, _thr, n1_1, n2, d1,
                            d2)) == (int) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->spawned2 = spawned2;
                efp->pthis2 = pthis2;
                efp->n2_2 = n2_2;
                efp->n2_1 = n2_1;
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->n1_2 = n1_2;
                efp->n1_1 = n1_1;
                efp->s2 = s2;
                efp->s1 = s1;
                efp->d2 = d2;
                efp->d1 = d1;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                efp->do_two_bk2.fr = (void *) efp;
                efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                efp->do_two_bk.fr = (void *) efp;
                efp->call_id = 1;
                return (int) 0 - 1;
              L_CALL4:;
                spawned2 = efp->spawned2;
                pthis2 = efp->pthis2;
                n2_2 = efp->n2_2;
                n2_1 = efp->n2_1;
                spawned = efp->spawned;
                pthis = efp->pthis;
                n1_2 = efp->n1_2;
                n1_1 = efp->n1_1;
                s2 = efp->s2;
                s1 = efp->s1;
                d2 = efp->d2;
                d1 = efp->d1;
                n2 = efp->n2;
                n1 = efp->n1;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
        }
        if (spawned)
          {
            {
              new_esp = esp;
              while (wait_rslt (new_esp, _thr),
                     __builtin_expect ((efp->tmp_esp =
                                        *((char **) esp)) != 0, 0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->n2_2 = n2_2;
                  efp->n2_1 = n2_1;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->n1_2 = n1_2;
                  efp->n1_1 = n1_1;
                  efp->s2 = s2;
                  efp->s1 = s1;
                  efp->d2 = d2;
                  efp->d1 = d1;
                  efp->n2 = n2;
                  efp->n1 = n1;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 2;
                  return (int) 0 - 1;
                L_CALL5:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  n2_2 = efp->n2_2;
                  n2_1 = efp->n2_1;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  n1_2 = efp->n1_2;
                  n1_1 = efp->n1_1;
                  s2 = efp->s2;
                  s1 = efp->s1;
                  d2 = efp->d2;
                  d1 = efp->d1;
                  n2 = efp->n2;
                  n1 = efp->n1;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
            s2 = (*pthis).r;
          }
        else
          {
            new_esp = esp;
            while (__builtin_expect
                   ((s2 =
                     cmp_1 (new_esp, _bk, _thr, n1_2, n2, d1 + n1_1,
                            d2)) == (int) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->spawned2 = spawned2;
                efp->pthis2 = pthis2;
                efp->n2_2 = n2_2;
                efp->n2_1 = n2_1;
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->n1_2 = n1_2;
                efp->n1_1 = n1_1;
                efp->s2 = s2;
                efp->s1 = s1;
                efp->d2 = d2;
                efp->d1 = d1;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                efp->do_two_bk2.fr = (void *) efp;
                efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                efp->do_two_bk.fr = (void *) efp;
                efp->call_id = 3;
                return (int) 0 - 1;
              L_CALL6:;
                spawned2 = efp->spawned2;
                pthis2 = efp->pthis2;
                n2_2 = efp->n2_2;
                n2_1 = efp->n2_1;
                spawned = efp->spawned;
                pthis = efp->pthis;
                n1_2 = efp->n1_2;
                n1_1 = efp->n1_1;
                s2 = efp->s2;
                s1 = efp->s1;
                d2 = efp->d2;
                d1 = efp->d1;
                n2 = efp->n2;
                n1 = efp->n1;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
      }
    }
  else
    {
      n2_1 = n2 / 2;
      n2_2 = n2 - n2_1;
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
                  efp->n2_2 = n2_2;
                  efp->n2_1 = n2_1;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->n1_2 = n1_2;
                  efp->n1_1 = n1_1;
                  efp->s2 = s2;
                  efp->s1 = s1;
                  efp->d2 = d2;
                  efp->d1 = d1;
                  efp->n2 = n2;
                  efp->n1 = n1;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 4;
                  return (int) 0 - 1;
                L_CALL7:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  n2_2 = efp->n2_2;
                  n2_1 = efp->n2_1;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  n1_2 = efp->n1_2;
                  n1_1 = efp->n1_1;
                  s2 = efp->s2;
                  s1 = efp->s1;
                  d2 = efp->d2;
                  d1 = efp->d1;
                  n2 = efp->n2;
                  n1 = efp->n1;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
          else;
          {
            new_esp = esp;
            while (__builtin_expect
                   ((s1 =
                     cmp_1 (new_esp, &efp->do_two_bk2, _thr, n1, n2_1, d1,
                            d2)) == (int) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->spawned2 = spawned2;
                efp->pthis2 = pthis2;
                efp->n2_2 = n2_2;
                efp->n2_1 = n2_1;
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->n1_2 = n1_2;
                efp->n1_1 = n1_1;
                efp->s2 = s2;
                efp->s1 = s1;
                efp->d2 = d2;
                efp->d1 = d1;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                efp->do_two_bk2.fr = (void *) efp;
                efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                efp->do_two_bk.fr = (void *) efp;
                efp->call_id = 5;
                return (int) 0 - 1;
              L_CALL8:;
                spawned2 = efp->spawned2;
                pthis2 = efp->pthis2;
                n2_2 = efp->n2_2;
                n2_1 = efp->n2_1;
                spawned = efp->spawned;
                pthis = efp->pthis;
                n1_2 = efp->n1_2;
                n1_1 = efp->n1_1;
                s2 = efp->s2;
                s1 = efp->s1;
                d2 = efp->d2;
                d1 = efp->d1;
                n2 = efp->n2;
                n1 = efp->n1;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
        }
        if (spawned2)
          {
            {
              new_esp = esp;
              while (wait_rslt (new_esp, _thr),
                     __builtin_expect ((efp->tmp_esp =
                                        *((char **) esp)) != 0, 0))
                {
                  efp->spawned2 = spawned2;
                  efp->pthis2 = pthis2;
                  efp->n2_2 = n2_2;
                  efp->n2_1 = n2_1;
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->n1_2 = n1_2;
                  efp->n1_1 = n1_1;
                  efp->s2 = s2;
                  efp->s1 = s1;
                  efp->d2 = d2;
                  efp->d1 = d1;
                  efp->n2 = n2;
                  efp->n1 = n1;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                  efp->do_two_bk2.fr = (void *) efp;
                  efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                  efp->do_two_bk.fr = (void *) efp;
                  efp->call_id = 6;
                  return (int) 0 - 1;
                L_CALL9:;
                  spawned2 = efp->spawned2;
                  pthis2 = efp->pthis2;
                  n2_2 = efp->n2_2;
                  n2_1 = efp->n2_1;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  n1_2 = efp->n1_2;
                  n1_1 = efp->n1_1;
                  s2 = efp->s2;
                  s1 = efp->s1;
                  d2 = efp->d2;
                  d1 = efp->d1;
                  n2 = efp->n2;
                  n1 = efp->n1;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp + 1;
                }
            }
            s2 = (*pthis2).r;
          }
        else
          {
            new_esp = esp;
            while (__builtin_expect
                   ((s2 =
                     cmp_1 (new_esp, _bk, _thr, n1, n2_2, d1,
                            d2 + n2_1)) == (int) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->spawned2 = spawned2;
                efp->pthis2 = pthis2;
                efp->n2_2 = n2_2;
                efp->n2_1 = n2_1;
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->n1_2 = n1_2;
                efp->n1_1 = n1_1;
                efp->s2 = s2;
                efp->s1 = s1;
                efp->d2 = d2;
                efp->d1 = d1;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk2.fun = do_two_bk2_in_cmp_1;
                efp->do_two_bk2.fr = (void *) efp;
                efp->do_two_bk.fun = do_two_bk_in_cmp_1;
                efp->do_two_bk.fr = (void *) efp;
                efp->call_id = 7;
                return (int) 0 - 1;
              L_CALL10:;
                spawned2 = efp->spawned2;
                pthis2 = efp->pthis2;
                n2_2 = efp->n2_2;
                n2_1 = efp->n2_1;
                spawned = efp->spawned;
                pthis = efp->pthis;
                n1_2 = efp->n1_2;
                n1_1 = efp->n1_1;
                s2 = efp->s2;
                s1 = efp->s1;
                d2 = efp->d2;
                d1 = efp->d1;
                n2 = efp->n2;
                n1 = efp->n1;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
      }
    }
  return s1 + s2;
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

void send_cmp_task (struct cmp *pthis)
{
  send_int ((*pthis).n1);
  send_int ((*pthis).n2);
  if (0 > (*pthis).n2)
    return;
  else;
  send_int32s ((*pthis).d1, (*pthis).n1);
  send_int32s ((*pthis).d2, (*pthis).n2);
}

struct cmp *recv_cmp_task ()
{
  struct cmp *pthis;
  int i;
  pthis = malloc (sizeof (struct cmp));
  (*pthis).n1 = recv_int ();
  (*pthis).n2 = recv_int ();
  if (!(0 > (*pthis).n2))
    {
      (*pthis).d1 = (int *) malloc (sizeof (int) * (*pthis).n1);
      (*pthis).d2 = (int *) malloc (sizeof (int) * (*pthis).n2);
      recv_int32s ((*pthis).d1, (*pthis).n1);
      recv_int32s ((*pthis).d2, (*pthis).n2);
    }
  else;
  return pthis;
}

void send_cmp_rslt (struct cmp *pthis)
{
  send_int ((*pthis).r);
  free ((*pthis).d1);
  free ((*pthis).d2);
  free (pthis);
}

void recv_cmp_rslt (struct cmp *pthis)
{
  (*pthis).r = recv_int ();
}

void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_cmp_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_cmp_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_cmp_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_cmp_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_cmp_rslt};

struct worker_data
{
};

void worker_init (struct thread_data *_thr)
{
}
