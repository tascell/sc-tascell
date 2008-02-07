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

double
elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
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
};

void grav_x (int (*_bk) (void), struct thread_data *_thr, int minx, int miny,
             int minz, int maxx, int maxy, int maxz, struct gravity *tsk);

void
do_gravity_task (struct thread_data *_thr, struct gravity *pthis)
{

  int _bk (void)
  {
    return 0;
  }
  struct timeval tp[2];
  int i;
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
  fprintf (stderr, "start (%d, %d, %d)--(%d, %d, %d): (%lf, %lf, %lf)\n",
           (*pthis).minx, (*pthis).miny, (*pthis).minz, (*pthis).maxx,
           (*pthis).maxy, (*pthis).maxz, ((*pthis).cur)[0], ((*pthis).cur)[1],
           ((*pthis).cur)[2]);
  grav_x (_bk, _thr, (*pthis).minx, (*pthis).miny, (*pthis).minz,
          (*pthis).maxx, (*pthis).maxy, (*pthis).maxz, &(*pthis));
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
};

void
do_start_gravity_task (struct thread_data *_thr, struct start_gravity *pthis)
{

  int _bk (void)
  {
    return 0;
  }
  struct gravity tsk;
  int i;
  {
    i = 0;
    for (; i < 3; i++)
      {
        (tsk.f)[i] = 0.0;
      }
  }
  {
    i = 0;
    for (; i < 3; i++)
      {
        (tsk.rf)[i] = 0.0;
      }
  }
  tsk.minx = (*pthis).minx;
  tsk.miny = (*pthis).miny;
  tsk.minz = (*pthis).minz;
  tsk.maxx = (*pthis).maxx;
  tsk.maxy = (*pthis).maxy;
  tsk.maxz = (*pthis).maxz;
  (tsk.cur)[0] = (double) (*pthis).curx / 100;
  (tsk.cur)[1] = (double) (*pthis).cury / 100;
  (tsk.cur)[2] = (double) (*pthis).curz / 100;
  fprintf (stderr, "start (%d, %d, %d)--(%d, %d, %d): (%lf, %lf, %lf)\n",
           tsk.minx, tsk.miny, tsk.minz, tsk.maxx, tsk.maxy, tsk.maxz,
           (tsk.cur)[0], (tsk.cur)[1], (tsk.cur)[2]);
  grav_x (_bk, _thr, tsk.minx, tsk.miny, tsk.minz, tsk.maxx, tsk.maxy,
          tsk.maxz, &tsk);
  {
    i = 0;
    for (; i < 3; i++)
      {
        (tsk.f)[i] += (tsk.rf)[i];
      }
  }
  fprintf (stderr, "f=(%lf, %lf, %lf)\n", (tsk.f)[0], (tsk.f)[1], (tsk.f)[2]);
}

void
grav_z (int (*_bk) (void), struct thread_data *_thr, int x, int y, int minz,
        int maxz, struct gravity *tsk)
{
  int i;
  {
    int z = minz;
    int z_end = maxz + 1;
    struct gravity *pthis;
    int spawned = 0;

    int do_many_bk (void)
    {
      if (!spawned)
        _bk ();
      while ((*_thr).treq_top && z + 1 < z_end)
        {
          int i2 = z_end;
          int i1 = (1 + z + z_end) / 2;
          z_end = i1;
          pthis = (struct gravity *) malloc (sizeof (struct gravity));
          *pthis = *tsk;
          (*pthis).minx = x;
          (*pthis).maxx = x;
          (*pthis).miny = y;
          (*pthis).maxy = y;
          (*pthis).minz = i1;
          (*pthis).maxz = i2 - 1;
          spawned++;
          make_and_send_task (_thr, 0, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk, _thr);
    for (; z < z_end; z++)
      {
        double dx;
        double dy;
        double dz;
        double one_d2th;
        double one_d3th;
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
              if (((*tsk).rf)[i] > 1.0 || ((*tsk).rf)[i] < -1.0)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
            }
        }
      }
    while (spawned-- > 0)
      {
        pthis = (struct gravity *) wait_rslt (_thr);
        int i;
        ((*tsk).rf)[0] += ((*pthis).f)[0];
        ((*tsk).rf)[1] += ((*pthis).f)[1];
        ((*tsk).rf)[2] += ((*pthis).f)[2];
        {
          i = 0;
          for (; i < 3; i++)
            {
              if (((*tsk).rf)[i] > 1.0 || ((*tsk).rf)[i] < -1.0)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
            }
        }
        free (pthis);
      }
  }
}

void
grav_y (int (*_bk) (void), struct thread_data *_thr, int x, int miny,
        int minz, int maxy, int maxz, struct gravity *tsk)
{
  {
    int y = miny;
    int y_end = maxy + 1;
    struct gravity *pthis;
    int spawned2 = 0;

    int do_many_bk2 (void)
    {
      if (!spawned2)
        _bk ();
      while ((*_thr).treq_top && y + 1 < y_end)
        {
          int i2 = y_end;
          int i1 = (1 + y + y_end) / 2;
          y_end = i1;
          pthis = (struct gravity *) malloc (sizeof (struct gravity));
          *pthis = *tsk;
          (*pthis).minx = x;
          (*pthis).maxx = x;
          (*pthis).miny = i1;
          (*pthis).maxy = i2 - 1;
          spawned2++;
          make_and_send_task (_thr, 0, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk2, _thr);
    for (; y < y_end; y++)
      {
        grav_z (do_many_bk2, _thr, x, y, minz, maxz, tsk);
      }
    while (spawned2-- > 0)
      {
        pthis = (struct gravity *) wait_rslt (_thr);
        int i;
        ((*tsk).rf)[0] += ((*pthis).f)[0];
        ((*tsk).rf)[1] += ((*pthis).f)[1];
        ((*tsk).rf)[2] += ((*pthis).f)[2];
        {
          i = 0;
          for (; i < 3; i++)
            {
              if (((*tsk).rf)[i] > 1.0 || ((*tsk).rf)[i] < -1.0)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
            }
        }
        free (pthis);
      }
  }
}

void
grav_x (int (*_bk) (void), struct thread_data *_thr, int minx, int miny,
        int minz, int maxx, int maxy, int maxz, struct gravity *tsk)
{
  {
    int x = minx;
    int x_end = maxx + 1;
    struct gravity *pthis;
    int spawned3 = 0;

    int do_many_bk3 (void)
    {
      if (!spawned3)
        _bk ();
      while ((*_thr).treq_top && x + 1 < x_end)
        {
          int i2 = x_end;
          int i1 = (1 + x + x_end) / 2;
          x_end = i1;
          pthis = (struct gravity *) malloc (sizeof (struct gravity));
          *pthis = *tsk;
          (*pthis).minx = i1;
          (*pthis).maxx = i2 - 1;
          spawned3++;
          make_and_send_task (_thr, 0, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk3, _thr);
    for (; x < x_end; x++)
      {
        grav_y (do_many_bk3, _thr, x, miny, minz, maxy, maxz, tsk);
      }
    while (spawned3-- > 0)
      {
        pthis = (struct gravity *) wait_rslt (_thr);
        int i;
        ((*tsk).rf)[0] += ((*pthis).f)[0];
        ((*tsk).rf)[1] += ((*pthis).f)[1];
        ((*tsk).rf)[2] += ((*pthis).f)[2];
        {
          i = 0;
          for (; i < 3; i++)
            {
              if (((*tsk).rf)[i] > 1.0 || ((*tsk).rf)[i] < -1.0)
                {
                  ((*tsk).f)[i] += ((*tsk).rf)[i];
                  ((*tsk).rf)[i] = 0.0;
                }
            }
        }
        free (pthis);
      }
  }
}

void
send_start_gravity_task (struct start_gravity *pthis)
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

struct start_gravity *
recv_start_gravity_task ()
{
  struct start_gravity *pthis = malloc (sizeof (struct start_gravity));
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

void
send_start_gravity_rslt (struct start_gravity *pthis)
{
  free (pthis);
}

void
recv_start_gravity_rslt (struct start_gravity *pthis)
{
}

void
send_gravity_task (struct gravity *pthis)
{
  send_int ((*pthis).minx);
  send_int ((*pthis).miny);
  send_int ((*pthis).minz);
  send_int ((*pthis).maxx);
  send_int ((*pthis).maxy);
  send_int ((*pthis).maxz);
  send_doubles ((*pthis).cur, 3);
}

struct gravity *
recv_gravity_task ()
{
  struct gravity *pthis = malloc (sizeof (struct gravity));
  (*pthis).minx = recv_int ();
  (*pthis).miny = recv_int ();
  (*pthis).minz = recv_int ();
  (*pthis).maxx = recv_int ();
  (*pthis).maxy = recv_int ();
  (*pthis).maxz = recv_int ();
  recv_doubles ((*pthis).cur, 3);
  return pthis;
}

void
send_gravity_rslt (struct gravity *pthis)
{
  int i;
  send_doubles ((*pthis).f, 3);
  free (pthis);
}

void
recv_gravity_rslt (struct gravity *pthis)
{
  recv_doubles ((*pthis).f, 3);
}

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_gravity_task,
    (void (*)(struct thread_data *, void *)) do_start_gravity_task};
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
