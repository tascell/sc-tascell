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
struct do_gravity_task_frame;
struct _bk_in_do_gravity_task_frame;
struct do_start_gravity_task_frame;
struct _bk_in_do_start_gravity_task_frame;
struct grav_z_frame;
struct do_many_bk_in_grav_z_frame;
struct grav_y_frame;
struct do_many_bk2_in_grav_y_frame;
struct grav_x_frame;
struct do_many_bk3_in_grav_x_frame;

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

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0E-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

struct gravity
{
  double f[3];
  double rf[3];
  int minx;
  int miny;
  int minz;
  int maxx;
  int maxy;
  int maxz;
  double cur[3];
  char _dummy_[1000];
};

void grav_x (char *esp, closure_t * _bk, struct thread_data *_thr, int minx,
             int miny, int minz, int maxx, int maxy, int maxz,
             struct gravity *tsk);

struct _bk_in_do_gravity_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_gravity_task_frame *xfp;
};

struct do_gravity_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  struct gravity *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_gravity_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_gravity_task_frame *efp;
  struct do_gravity_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

LGOTO:;
  efp = (struct _bk_in_do_gravity_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_gravity_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_gravity_task (char *esp, struct thread_data *_thr,
                      struct gravity *pthis)
{
  int i;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct do_gravity_task_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_gravity_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_gravity_task_frame) + sizeof (Align_t) +
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
  efp = (struct do_gravity_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_gravity_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    i = 0;
    for (; i < 3; i++)
      {
        ((*pthis).f)[i] = 0.0;
      }
  }
  {
    i = 0;
    for (; i < 3; i++)
      {
        ((*pthis).rf)[i] = 0.0;
      }
  }
  {
  }
  {
    new_esp = esp;
    while (grav_x
           (new_esp, &efp->_bk, _thr, (*pthis).minx, (*pthis).miny,
            (*pthis).minz, (*pthis).maxx, (*pthis).maxy, (*pthis).maxz,
            &(*pthis)), __builtin_expect ((efp->tmp_esp =
                                           *((char **) esp)) != 0, 0))
      {
        efp->i = i;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_gravity_task;
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
    i = 0;
    for (; i < 3; i++)
      {
        ((*pthis).f)[i] += ((*pthis).rf)[i];
      }
  }
}

struct start_gravity
{
  int minx;
  int miny;
  int minz;
  int maxx;
  int maxy;
  int maxz;
  int curx;
  int cury;
  int curz;
  char _dummy_[1000];
};

struct _bk_in_do_start_gravity_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_start_gravity_task_frame *xfp;
};

struct do_start_gravity_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct timeval tp[2];
  int i;
  struct gravity tsk;
  struct start_gravity *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_start_gravity_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_start_gravity_task_frame *efp;
  struct do_start_gravity_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

LGOTO:;
  efp = (struct _bk_in_do_start_gravity_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_start_gravity_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_start_gravity_task (char *esp, struct thread_data *_thr,
                            struct start_gravity *pthis)
{
  int i;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct do_start_gravity_task_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct do_start_gravity_task_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct do_start_gravity_task_frame) +
                   sizeof (Align_t) + -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL2;
        }
      goto L_CALL2;
    }
  else;
  efp = (struct do_start_gravity_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_start_gravity_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    i = 0;
    for (; i < 3; i++)
      {
        ((efp->tsk).f)[i] = 0.0;
      }
  }
  {
    i = 0;
    for (; i < 3; i++)
      {
        ((efp->tsk).rf)[i] = 0.0;
      }
  }
  (efp->tsk).minx = (*pthis).minx;
  (efp->tsk).miny = (*pthis).miny;
  (efp->tsk).minz = (*pthis).minz;
  (efp->tsk).maxx = (*pthis).maxx;
  (efp->tsk).maxy = (*pthis).maxy;
  (efp->tsk).maxz = (*pthis).maxz;
  ((efp->tsk).cur)[0] = (double) (*pthis).curx / 100;
  ((efp->tsk).cur)[1] = (double) (*pthis).cury / 100;
  ((efp->tsk).cur)[2] = (double) (*pthis).curz / 100;
  fprintf (stderr, "start (%d, %d, %d)--(%d, %d, %d): (%lf, %lf, %lf)\n",
           (efp->tsk).minx, (efp->tsk).miny, (efp->tsk).minz, (efp->tsk).maxx,
           (efp->tsk).maxy, (efp->tsk).maxz, ((efp->tsk).cur)[0],
           ((efp->tsk).cur)[1], ((efp->tsk).cur)[2]);
  gettimeofday (efp->tp, 0);
  {
    new_esp = esp;
    while (grav_x
           (new_esp, &efp->_bk, _thr, (efp->tsk).minx, (efp->tsk).miny,
            (efp->tsk).minz, (efp->tsk).maxx, (efp->tsk).maxy, (efp->tsk).maxz,
            &efp->tsk), __builtin_expect ((efp->tmp_esp =
                                           *((char **) esp)) != 0, 0))
      {
        efp->i = i;
        efp->pthis = pthis;
        efp->_thr = _thr;
        efp->_bk.fun = _bk_in_do_start_gravity_task;
        efp->_bk.fr = (void *) efp;
        efp->call_id = 0;
        return;
      L_CALL2:;
        i = efp->i;
        pthis = efp->pthis;
        _thr = efp->_thr;
        new_esp = esp + 1;
      }
  }
  {
    i = 0;
    for (; i < 3; i++)
      {
        ((efp->tsk).f)[i] += ((efp->tsk).rf)[i];
      }
  }
  gettimeofday (efp->tp + 1, 0);
  fprintf (stderr, "f=(%lf, %lf, %lf)\n", ((efp->tsk).f)[0], ((efp->tsk).f)[1],
           ((efp->tsk).f)[2]);
  fprintf (stderr, "time: %lf\n", elapsed_time (efp->tp));
}

struct do_many_bk_in_grav_z_frame
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
  struct grav_z_frame *xfp;
};

struct grav_z_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i3;
  double one_d3th;
  double one_d2th;
  double dz;
  double dy;
  double dx;
  int spawned;
  struct gravity *pthis;
  int Z_end;
  int z;
  int i;
  int ifexp_result5;
  int ifexp_result6;
  void *tmp;
  int ifexp_result7;
  int ifexp_result8;
  struct gravity *tsk;
  int maxz;
  int minz;
  int y;
  int x;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk;
};

char *do_many_bk_in_grav_z (char *esp, void *xfp0)
{
  int ifexp_result2;
  int ifexp_result;
  int ifexp_result4;
  int ifexp_result3;
  int i2;
  int i1;
  char *new_esp;
  struct do_many_bk_in_grav_z_frame *efp;
  struct grav_z_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_many_bk_in_grav_z_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk_in_grav_z_frame) + sizeof (Align_t) +
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
        if (xfp->z + 1 < xfp->Z_end)
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
                if (xfp->z + 1 < xfp->Z_end)
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
            i2 = xfp->Z_end;
            i1 = (1 + xfp->z + xfp->Z_end) / 2;
            xfp->Z_end = i1;
            xfp->pthis = (struct gravity *) malloc (sizeof (struct gravity));
            (*xfp->pthis).minx = xfp->x;
            (*xfp->pthis).miny = xfp->y;
            (*xfp->pthis).minz = i1;
            (*xfp->pthis).maxx = xfp->x;
            (*xfp->pthis).maxy = xfp->y;
            (*xfp->pthis).maxz = i2 - 1;
            ((*xfp->pthis).cur)[0] = ((*xfp->tsk).cur)[0];
            ((*xfp->pthis).cur)[1] = ((*xfp->tsk).cur)[1];
            ((*xfp->pthis).cur)[2] = ((*xfp->tsk).cur)[2];
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

void grav_z (char *esp, closure_t * _bk, struct thread_data *_thr, int x, int y,
             int minz, int maxz, struct gravity *tsk)
{
  int ifexp_result8;
  int ifexp_result7;
  void *tmp;
  int ifexp_result6;
  int ifexp_result5;
  int i;
  int z;
  int Z_end;
  struct gravity *pthis;
  int spawned;
  double dx;
  double dy;
  double dz;
  double one_d2th;
  double one_d3th;
  int i3;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct grav_z_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct grav_z_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct grav_z_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
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
  efp = (struct grav_z_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct grav_z_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    z = minz;
    Z_end = maxz + 1;
    spawned = 0;
    if ((*_thr).req)
      {
        new_esp = esp;
        while (handle_req (new_esp, &efp->do_many_bk, _thr),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->i3 = i3;
            efp->one_d3th = one_d3th;
            efp->one_d2th = one_d2th;
            efp->dz = dz;
            efp->dy = dy;
            efp->dx = dx;
            efp->spawned = spawned;
            efp->pthis = pthis;
            efp->Z_end = Z_end;
            efp->z = z;
            efp->i = i;
            efp->ifexp_result5 = ifexp_result5;
            efp->ifexp_result6 = ifexp_result6;
            efp->tmp = tmp;
            efp->ifexp_result7 = ifexp_result7;
            efp->ifexp_result8 = ifexp_result8;
            efp->tsk = tsk;
            efp->maxz = maxz;
            efp->minz = minz;
            efp->y = y;
            efp->x = x;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_many_bk.fun = do_many_bk_in_grav_z;
            efp->do_many_bk.fr = (void *) efp;
            efp->call_id = 0;
            return;
          L_CALL3:;
            i3 = efp->i3;
            one_d3th = efp->one_d3th;
            one_d2th = efp->one_d2th;
            dz = efp->dz;
            dy = efp->dy;
            dx = efp->dx;
            spawned = efp->spawned;
            pthis = efp->pthis;
            Z_end = efp->Z_end;
            z = efp->z;
            i = efp->i;
            ifexp_result5 = efp->ifexp_result5;
            ifexp_result6 = efp->ifexp_result6;
            tmp = efp->tmp;
            ifexp_result7 = efp->ifexp_result7;
            ifexp_result8 = efp->ifexp_result8;
            tsk = efp->tsk;
            maxz = efp->maxz;
            minz = efp->minz;
            y = efp->y;
            x = efp->x;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    else;
    for (; z < Z_end; z++)
      {
        dx = (double) x - ((*tsk).cur)[0];
        dy = (double) y - ((*tsk).cur)[1];
        dz = (double) z - ((*tsk).cur)[2];
        one_d2th = 1.0 / (dx * dx + dy * dy + dz * dz);
        one_d3th = sqrt (one_d2th) * one_d2th;
        ((*tsk).rf)[0] += dx * one_d3th;
        ((*tsk).rf)[1] += dy * one_d3th;
        ((*tsk).rf)[2] += dz * one_d3th;
        {
          i = 0;
          for (; i < 3; i++)
            {
              if (((*tsk).rf)[i] > 1.0)
                ifexp_result5 = 1;
              else
                {
                  if (((*tsk).rf)[i] < -1.0)
                    ifexp_result6 = 1;
                  else
                    ifexp_result6 = 0;
                  ifexp_result5 = ifexp_result6;
                }
              if (ifexp_result5)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
              else;
            }
        }
      }
    while (spawned-- > 0)
      {
        {
          {
            new_esp = esp;
            while (__builtin_expect
                   ((tmp = wait_rslt (new_esp, _thr)) == (void *) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->i3 = i3;
                efp->one_d3th = one_d3th;
                efp->one_d2th = one_d2th;
                efp->dz = dz;
                efp->dy = dy;
                efp->dx = dx;
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->Z_end = Z_end;
                efp->z = z;
                efp->i = i;
                efp->ifexp_result5 = ifexp_result5;
                efp->ifexp_result6 = ifexp_result6;
                efp->tmp = tmp;
                efp->ifexp_result7 = ifexp_result7;
                efp->ifexp_result8 = ifexp_result8;
                efp->tsk = tsk;
                efp->maxz = maxz;
                efp->minz = minz;
                efp->y = y;
                efp->x = x;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_many_bk.fun = do_many_bk_in_grav_z;
                efp->do_many_bk.fr = (void *) efp;
                efp->call_id = 1;
                return;
              L_CALL4:;
                i3 = efp->i3;
                one_d3th = efp->one_d3th;
                one_d2th = efp->one_d2th;
                dz = efp->dz;
                dy = efp->dy;
                dx = efp->dx;
                spawned = efp->spawned;
                pthis = efp->pthis;
                Z_end = efp->Z_end;
                z = efp->z;
                i = efp->i;
                ifexp_result5 = efp->ifexp_result5;
                ifexp_result6 = efp->ifexp_result6;
                tmp = efp->tmp;
                ifexp_result7 = efp->ifexp_result7;
                ifexp_result8 = efp->ifexp_result8;
                tsk = efp->tsk;
                maxz = efp->maxz;
                minz = efp->minz;
                y = efp->y;
                x = efp->x;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
          pthis = (struct gravity *) tmp;
        }
        ((*tsk).rf)[0] += ((*pthis).f)[0];
        ((*tsk).rf)[1] += ((*pthis).f)[1];
        ((*tsk).rf)[2] += ((*pthis).f)[2];
        {
          i3 = 0;
          for (; i3 < 3; i3++)
            {
              if (((*tsk).rf)[i3] > 1.0)
                ifexp_result7 = 1;
              else
                {
                  if (((*tsk).rf)[i3] < -1.0)
                    ifexp_result8 = 1;
                  else
                    ifexp_result8 = 0;
                  ifexp_result7 = ifexp_result8;
                }
              if (ifexp_result7)
                {
                  ((*tsk).f)[i3] += ((*tsk).rf)[i3];
                  ((*tsk).rf)[i3] = 0.0;
                }
              else;
            }
        }
        free (pthis);
      }
  }
}

struct do_many_bk2_in_grav_y_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i1;
  int i2;
  int ifexp_result11;
  int ifexp_result12;
  int ifexp_result9;
  int ifexp_result10;
  struct grav_y_frame *xfp;
};

struct grav_y_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  int spawned2;
  struct gravity *pthis;
  int Y_end;
  int y;
  void *tmp2;
  int ifexp_result13;
  int ifexp_result14;
  struct gravity *tsk;
  int maxz;
  int maxy;
  int minz;
  int miny;
  int x;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk2;
};

char *do_many_bk2_in_grav_y (char *esp, void *xfp0)
{
  int ifexp_result10;
  int ifexp_result9;
  int ifexp_result12;
  int ifexp_result11;
  int i2;
  int i1;
  char *new_esp;
  struct do_many_bk2_in_grav_y_frame *efp;
  struct grav_y_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_many_bk2_in_grav_y_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk2_in_grav_y_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (!xfp->spawned2)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      efp->i1 = i1;
      efp->i2 = i2;
      efp->ifexp_result11 = ifexp_result11;
      efp->ifexp_result12 = ifexp_result12;
      efp->ifexp_result9 = ifexp_result9;
      efp->ifexp_result10 = ifexp_result10;
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
      ifexp_result11 = efp->ifexp_result11;
      ifexp_result12 = efp->ifexp_result12;
      ifexp_result9 = efp->ifexp_result9;
      ifexp_result10 = efp->ifexp_result10;
      xfp = efp->xfp;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->y + 1 < xfp->Y_end)
          ifexp_result10 = 1;
        else
          ifexp_result10 = 0;
        ifexp_result9 = ifexp_result10;
      }
    else
      ifexp_result9 = 0;
    if (ifexp_result9)
      {
        goto loop_start2;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->y + 1 < xfp->Y_end)
                  ifexp_result12 = 1;
                else
                  ifexp_result12 = 0;
                ifexp_result11 = ifexp_result12;
              }
            else
              ifexp_result11 = 0;
            if (!ifexp_result11)
              goto loop_end2;
            else;
          loop_start2:;
            i2 = xfp->Y_end;
            i1 = (1 + xfp->y + xfp->Y_end) / 2;
            xfp->Y_end = i1;
            xfp->pthis = (struct gravity *) malloc (sizeof (struct gravity));
            (*xfp->pthis).minx = xfp->x;
            (*xfp->pthis).miny = i1;
            (*xfp->pthis).minz = (*xfp->tsk).minz;
            (*xfp->pthis).maxx = xfp->x;
            (*xfp->pthis).maxy = i2 - 1;
            (*xfp->pthis).maxz = (*xfp->tsk).maxz;
            ((*xfp->pthis).cur)[0] = ((*xfp->tsk).cur)[0];
            ((*xfp->pthis).cur)[1] = ((*xfp->tsk).cur)[1];
            ((*xfp->pthis).cur)[2] = ((*xfp->tsk).cur)[2];
            (xfp->spawned2)++;
            make_and_send_task (xfp->_thr, 0, xfp->pthis);
          }
      loop_end2:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void grav_y (char *esp, closure_t * _bk, struct thread_data *_thr, int x,
             int miny, int minz, int maxy, int maxz, struct gravity *tsk)
{
  int ifexp_result14;
  int ifexp_result13;
  void *tmp2;
  int y;
  int Y_end;
  struct gravity *pthis;
  int spawned2;
  int i;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct grav_y_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct grav_y_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct grav_y_frame) + sizeof (Align_t) +
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
        }
      goto L_CALL5;
    }
  else;
  efp = (struct grav_y_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct grav_y_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    y = miny;
    Y_end = maxy + 1;
    spawned2 = 0;
    if ((*_thr).req)
      {
        new_esp = esp;
        while (handle_req (new_esp, &efp->do_many_bk2, _thr),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->i = i;
            efp->spawned2 = spawned2;
            efp->pthis = pthis;
            efp->Y_end = Y_end;
            efp->y = y;
            efp->tmp2 = tmp2;
            efp->ifexp_result13 = ifexp_result13;
            efp->ifexp_result14 = ifexp_result14;
            efp->tsk = tsk;
            efp->maxz = maxz;
            efp->maxy = maxy;
            efp->minz = minz;
            efp->miny = miny;
            efp->x = x;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_many_bk2.fun = do_many_bk2_in_grav_y;
            efp->do_many_bk2.fr = (void *) efp;
            efp->call_id = 0;
            return;
          L_CALL5:;
            i = efp->i;
            spawned2 = efp->spawned2;
            pthis = efp->pthis;
            Y_end = efp->Y_end;
            y = efp->y;
            tmp2 = efp->tmp2;
            ifexp_result13 = efp->ifexp_result13;
            ifexp_result14 = efp->ifexp_result14;
            tsk = efp->tsk;
            maxz = efp->maxz;
            maxy = efp->maxy;
            minz = efp->minz;
            miny = efp->miny;
            x = efp->x;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    else;
    for (; y < Y_end; y++)
      {
        new_esp = esp;
        while (grav_z (new_esp, &efp->do_many_bk2, _thr, x, y, minz, maxz, tsk),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->i = i;
            efp->spawned2 = spawned2;
            efp->pthis = pthis;
            efp->Y_end = Y_end;
            efp->y = y;
            efp->tmp2 = tmp2;
            efp->ifexp_result13 = ifexp_result13;
            efp->ifexp_result14 = ifexp_result14;
            efp->tsk = tsk;
            efp->maxz = maxz;
            efp->maxy = maxy;
            efp->minz = minz;
            efp->miny = miny;
            efp->x = x;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_many_bk2.fun = do_many_bk2_in_grav_y;
            efp->do_many_bk2.fr = (void *) efp;
            efp->call_id = 1;
            return;
          L_CALL6:;
            i = efp->i;
            spawned2 = efp->spawned2;
            pthis = efp->pthis;
            Y_end = efp->Y_end;
            y = efp->y;
            tmp2 = efp->tmp2;
            ifexp_result13 = efp->ifexp_result13;
            ifexp_result14 = efp->ifexp_result14;
            tsk = efp->tsk;
            maxz = efp->maxz;
            maxy = efp->maxy;
            minz = efp->minz;
            miny = efp->miny;
            x = efp->x;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    while (spawned2-- > 0)
      {
        {
          {
            new_esp = esp;
            while (__builtin_expect
                   ((tmp2 = wait_rslt (new_esp, _thr)) == (void *) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->i = i;
                efp->spawned2 = spawned2;
                efp->pthis = pthis;
                efp->Y_end = Y_end;
                efp->y = y;
                efp->tmp2 = tmp2;
                efp->ifexp_result13 = ifexp_result13;
                efp->ifexp_result14 = ifexp_result14;
                efp->tsk = tsk;
                efp->maxz = maxz;
                efp->maxy = maxy;
                efp->minz = minz;
                efp->miny = miny;
                efp->x = x;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_many_bk2.fun = do_many_bk2_in_grav_y;
                efp->do_many_bk2.fr = (void *) efp;
                efp->call_id = 2;
                return;
              L_CALL7:;
                i = efp->i;
                spawned2 = efp->spawned2;
                pthis = efp->pthis;
                Y_end = efp->Y_end;
                y = efp->y;
                tmp2 = efp->tmp2;
                ifexp_result13 = efp->ifexp_result13;
                ifexp_result14 = efp->ifexp_result14;
                tsk = efp->tsk;
                maxz = efp->maxz;
                maxy = efp->maxy;
                minz = efp->minz;
                miny = efp->miny;
                x = efp->x;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
          pthis = (struct gravity *) tmp2;
        }
        ((*tsk).rf)[0] += ((*pthis).f)[0];
        ((*tsk).rf)[1] += ((*pthis).f)[1];
        ((*tsk).rf)[2] += ((*pthis).f)[2];
        {
          i = 0;
          for (; i < 3; i++)
            {
              if (((*tsk).rf)[i] > 1.0)
                ifexp_result13 = 1;
              else
                {
                  if (((*tsk).rf)[i] < -1.0)
                    ifexp_result14 = 1;
                  else
                    ifexp_result14 = 0;
                  ifexp_result13 = ifexp_result14;
                }
              if (ifexp_result13)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
              else;
            }
        }
        free (pthis);
      }
  }
}

struct do_many_bk3_in_grav_x_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i1;
  int i2;
  int ifexp_result17;
  int ifexp_result18;
  int ifexp_result15;
  int ifexp_result16;
  struct grav_x_frame *xfp;
};

struct grav_x_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  int spawned3;
  struct gravity *pthis;
  int X_end;
  int x;
  void *tmp3;
  int ifexp_result19;
  int ifexp_result20;
  struct gravity *tsk;
  int maxz;
  int maxy;
  int maxx;
  int minz;
  int miny;
  int minx;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk3;
};

char *do_many_bk3_in_grav_x (char *esp, void *xfp0)
{
  int ifexp_result16;
  int ifexp_result15;
  int ifexp_result18;
  int ifexp_result17;
  int i2;
  int i1;
  char *new_esp;
  struct do_many_bk3_in_grav_x_frame *efp;
  struct grav_x_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct do_many_bk3_in_grav_x_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk3_in_grav_x_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (!xfp->spawned3)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      efp->i1 = i1;
      efp->i2 = i2;
      efp->ifexp_result17 = ifexp_result17;
      efp->ifexp_result18 = ifexp_result18;
      efp->ifexp_result15 = ifexp_result15;
      efp->ifexp_result16 = ifexp_result16;
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
      ifexp_result17 = efp->ifexp_result17;
      ifexp_result18 = efp->ifexp_result18;
      ifexp_result15 = efp->ifexp_result15;
      ifexp_result16 = efp->ifexp_result16;
      xfp = efp->xfp;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->x + 1 < xfp->X_end)
          ifexp_result16 = 1;
        else
          ifexp_result16 = 0;
        ifexp_result15 = ifexp_result16;
      }
    else
      ifexp_result15 = 0;
    if (ifexp_result15)
      {
        goto loop_start3;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->x + 1 < xfp->X_end)
                  ifexp_result18 = 1;
                else
                  ifexp_result18 = 0;
                ifexp_result17 = ifexp_result18;
              }
            else
              ifexp_result17 = 0;
            if (!ifexp_result17)
              goto loop_end3;
            else;
          loop_start3:;
            i2 = xfp->X_end;
            i1 = (1 + xfp->x + xfp->X_end) / 2;
            xfp->X_end = i1;
            xfp->pthis = (struct gravity *) malloc (sizeof (struct gravity));
            (*xfp->pthis).minx = i1;
            (*xfp->pthis).miny = (*xfp->tsk).miny;
            (*xfp->pthis).minz = (*xfp->tsk).minz;
            (*xfp->pthis).maxx = i2 - 1;
            (*xfp->pthis).maxy = (*xfp->tsk).maxy;
            (*xfp->pthis).maxz = (*xfp->tsk).maxz;
            ((*xfp->pthis).cur)[0] = ((*xfp->tsk).cur)[0];
            ((*xfp->pthis).cur)[1] = ((*xfp->tsk).cur)[1];
            ((*xfp->pthis).cur)[2] = ((*xfp->tsk).cur)[2];
            (xfp->spawned3)++;
            make_and_send_task (xfp->_thr, 0, xfp->pthis);
          }
      loop_end3:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void grav_x (char *esp, closure_t * _bk, struct thread_data *_thr, int minx,
             int miny, int minz, int maxx, int maxy, int maxz,
             struct gravity *tsk)
{
  int ifexp_result20;
  int ifexp_result19;
  void *tmp3;
  int x;
  int X_end;
  struct gravity *pthis;
  int spawned3;
  int i;
  size_t esp_flag = (size_t) esp & 3;
  char *new_esp;
  struct grav_x_frame *efp;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct grav_x_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct grav_x_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL8;
        case 1:
          goto L_CALL9;
        case 2:
          goto L_CALL10;
        }
      goto L_CALL8;
    }
  else;
  efp = (struct grav_x_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct grav_x_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    x = minx;
    X_end = maxx + 1;
    spawned3 = 0;
    if ((*_thr).req)
      {
        new_esp = esp;
        while (handle_req (new_esp, &efp->do_many_bk3, _thr),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            efp->i = i;
            efp->spawned3 = spawned3;
            efp->pthis = pthis;
            efp->X_end = X_end;
            efp->x = x;
            efp->tmp3 = tmp3;
            efp->ifexp_result19 = ifexp_result19;
            efp->ifexp_result20 = ifexp_result20;
            efp->tsk = tsk;
            efp->maxz = maxz;
            efp->maxy = maxy;
            efp->maxx = maxx;
            efp->minz = minz;
            efp->miny = miny;
            efp->minx = minx;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_many_bk3.fun = do_many_bk3_in_grav_x;
            efp->do_many_bk3.fr = (void *) efp;
            efp->call_id = 0;
            return;
          L_CALL8:;
            i = efp->i;
            spawned3 = efp->spawned3;
            pthis = efp->pthis;
            X_end = efp->X_end;
            x = efp->x;
            tmp3 = efp->tmp3;
            ifexp_result19 = efp->ifexp_result19;
            ifexp_result20 = efp->ifexp_result20;
            tsk = efp->tsk;
            maxz = efp->maxz;
            maxy = efp->maxy;
            maxx = efp->maxx;
            minz = efp->minz;
            miny = efp->miny;
            minx = efp->minx;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    else;
    for (; x < X_end; x++)
      {
        new_esp = esp;
        while (grav_y
               (new_esp, &efp->do_many_bk3, _thr, x, miny, minz, maxy, maxz,
                tsk), __builtin_expect ((efp->tmp_esp =
                                         *((char **) esp)) != 0, 0))
          {
            efp->i = i;
            efp->spawned3 = spawned3;
            efp->pthis = pthis;
            efp->X_end = X_end;
            efp->x = x;
            efp->tmp3 = tmp3;
            efp->ifexp_result19 = ifexp_result19;
            efp->ifexp_result20 = ifexp_result20;
            efp->tsk = tsk;
            efp->maxz = maxz;
            efp->maxy = maxy;
            efp->maxx = maxx;
            efp->minz = minz;
            efp->miny = miny;
            efp->minx = minx;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_many_bk3.fun = do_many_bk3_in_grav_x;
            efp->do_many_bk3.fr = (void *) efp;
            efp->call_id = 1;
            return;
          L_CALL9:;
            i = efp->i;
            spawned3 = efp->spawned3;
            pthis = efp->pthis;
            X_end = efp->X_end;
            x = efp->x;
            tmp3 = efp->tmp3;
            ifexp_result19 = efp->ifexp_result19;
            ifexp_result20 = efp->ifexp_result20;
            tsk = efp->tsk;
            maxz = efp->maxz;
            maxy = efp->maxy;
            maxx = efp->maxx;
            minz = efp->minz;
            miny = efp->miny;
            minx = efp->minx;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp + 1;
          }
      }
    while (spawned3-- > 0)
      {
        {
          {
            new_esp = esp;
            while (__builtin_expect
                   ((tmp3 = wait_rslt (new_esp, _thr)) == (void *) 0 - 1, 0)
                   && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                        1))
              {
                efp->i = i;
                efp->spawned3 = spawned3;
                efp->pthis = pthis;
                efp->X_end = X_end;
                efp->x = x;
                efp->tmp3 = tmp3;
                efp->ifexp_result19 = ifexp_result19;
                efp->ifexp_result20 = ifexp_result20;
                efp->tsk = tsk;
                efp->maxz = maxz;
                efp->maxy = maxy;
                efp->maxx = maxx;
                efp->minz = minz;
                efp->miny = miny;
                efp->minx = minx;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_many_bk3.fun = do_many_bk3_in_grav_x;
                efp->do_many_bk3.fr = (void *) efp;
                efp->call_id = 2;
                return;
              L_CALL10:;
                i = efp->i;
                spawned3 = efp->spawned3;
                pthis = efp->pthis;
                X_end = efp->X_end;
                x = efp->x;
                tmp3 = efp->tmp3;
                ifexp_result19 = efp->ifexp_result19;
                ifexp_result20 = efp->ifexp_result20;
                tsk = efp->tsk;
                maxz = efp->maxz;
                maxy = efp->maxy;
                maxx = efp->maxx;
                minz = efp->minz;
                miny = efp->miny;
                minx = efp->minx;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp + 1;
              }
          }
          pthis = (struct gravity *) tmp3;
        }
        ((*tsk).rf)[0] += ((*pthis).f)[0];
        ((*tsk).rf)[1] += ((*pthis).f)[1];
        ((*tsk).rf)[2] += ((*pthis).f)[2];
        {
          i = 0;
          for (; i < 3; i++)
            {
              if (((*tsk).rf)[i] > 1.0)
                ifexp_result19 = 1;
              else
                {
                  if (((*tsk).rf)[i] < -1.0)
                    ifexp_result20 = 1;
                  else
                    ifexp_result20 = 0;
                  ifexp_result19 = ifexp_result20;
                }
              if (ifexp_result19)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
              else;
            }
        }
        free (pthis);
      }
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

void send_start_gravity_task (struct start_gravity *pthis)
{
  send_int ((*pthis).minx);
  send_int ((*pthis).miny);
  send_int ((*pthis).minz);
  send_int ((*pthis).maxx);
  send_int ((*pthis).maxy);
  send_int ((*pthis).maxz);
  send_int ((*pthis).curx);
  send_int ((*pthis).cury);
  send_int ((*pthis).curz);
}

struct start_gravity *recv_start_gravity_task ()
{
  struct start_gravity *pthis;
  pthis = malloc (sizeof (struct start_gravity));
  (*pthis).minx = recv_int ();
  (*pthis).miny = recv_int ();
  (*pthis).minz = recv_int ();
  (*pthis).maxx = recv_int ();
  (*pthis).maxy = recv_int ();
  (*pthis).maxz = recv_int ();
  (*pthis).curx = recv_int ();
  (*pthis).cury = recv_int ();
  (*pthis).curz = recv_int ();
  return pthis;
}

void send_start_gravity_rslt (struct start_gravity *pthis)
{
  free (pthis);
}

void recv_start_gravity_rslt (struct start_gravity *pthis)
{
}

void send_gravity_task (struct gravity *pthis)
{
  send_int ((*pthis).minx);
  send_int ((*pthis).miny);
  send_int ((*pthis).minz);
  send_int ((*pthis).maxx);
  send_int ((*pthis).maxy);
  send_int ((*pthis).maxz);
  send_doubles ((*pthis).cur, 3);
}

struct gravity *recv_gravity_task ()
{
  struct gravity *pthis;
  pthis = malloc (sizeof (struct gravity));
  (*pthis).minx = recv_int ();
  (*pthis).miny = recv_int ();
  (*pthis).minz = recv_int ();
  (*pthis).maxx = recv_int ();
  (*pthis).maxy = recv_int ();
  (*pthis).maxz = recv_int ();
  recv_doubles ((*pthis).cur, 3);
  return pthis;
}

void send_gravity_rslt (struct gravity *pthis)
{
  send_doubles ((*pthis).f, 3);
  free (pthis);
}

void recv_gravity_rslt (struct gravity *pthis)
{
  recv_doubles ((*pthis).f, 3);
}

void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_gravity_task,
    (void (*)(char *, struct thread_data *, void *)) do_start_gravity_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_gravity_task,
    (void (*)(void *)) send_start_gravity_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_gravity_task, (void *(*)()) recv_start_gravity_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_gravity_rslt,
    (void (*)(void *)) send_start_gravity_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_gravity_rslt,
    (void (*)(void *)) recv_start_gravity_rslt};

struct worker_data
{
};

void worker_init (struct thread_data *_thr)
{
}
