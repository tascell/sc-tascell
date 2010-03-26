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

int cmp_1 (int (*_bk) closure (void), struct thread_data *_thr, int n1, int n2,
           int *d1, int *d2);

void do_cmp_task (struct thread_data *_thr, struct cmp *pthis)
{

  int _bk closure (void)
  {
    return 0;
  }
  struct timeval tp[2];
  int i;
  int j;

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
      gettimeofday (tp, 0);
      (*pthis).r =
        cmp_1 (_bk, _thr, (*pthis).n1, (*pthis).n2, (*pthis).d1, (*pthis).d2);
      gettimeofday (tp + 1, 0);
      fprintf (stderr, "time: %lf\n", elapsed_time (tp));
    }
  else
    (*pthis).r =
      cmp_1 (_bk, _thr, (*pthis).n1, (*pthis).n2, (*pthis).d1, (*pthis).d2);
}

int cmp_2 (int n1, int n2, int *d1, int *d2)
{
  int i;
  int j;
  int s = 0;

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

int cmp_1 (int (*_bk) closure (void), struct thread_data *_thr, int n1, int n2,
           int *d1, int *d2)
{
  int s1;
  int s2;

  if (n1 < 5)
    return cmp_2 (n1, n2, d1, d2);
  else;
  if (n1 > n2)
    {
      int n1_1 = n1 / 2;
      int n1_2 = n1 - n1_1;

      {
        struct cmp st;
        struct cmp *pthis = &st;
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
                (*pthis).n1 = n1_2;
                (*pthis).n2 = n2;
                (*pthis).d1 = d1 + n1_1;
                (*pthis).d2 = d2;
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
            s1 = cmp_1 (do_two_bk, _thr, n1_1, n2, d1, d2);
          }
        }
        if (spawned)
          {
            wait_rslt (_thr);
            s2 = (*pthis).r;
          }
        else
          {
            s2 = cmp_1 (_bk, _thr, n1_2, n2, d1 + n1_1, d2);
          }
      }
    }
  else
    {
      int n2_1 = n2 / 2;
      int n2_2 = n2 - n2_1;

      {
        struct cmp st2;
        struct cmp *pthis = &st2;
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
                (*pthis).n1 = n1;
                (*pthis).n2 = n2_2;
                (*pthis).d1 = d1;
                (*pthis).d2 = d2 + n2_1;
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
            s1 = cmp_1 (do_two_bk2, _thr, n1, n2_1, d1, d2);
          }
        }
        if (spawned2)
          {
            wait_rslt (_thr);
            s2 = (*pthis).r;
          }
        else
          {
            s2 = cmp_1 (_bk, _thr, n1, n2_2, d1, d2 + n2_1);
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
  struct cmp *pthis = malloc (sizeof (struct cmp));

  (*pthis).n1 = recv_int ();
  (*pthis).n2 = recv_int ();
  int i;

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

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_cmp_task};
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
