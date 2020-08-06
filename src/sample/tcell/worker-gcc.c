#define _GNU_SOURCE
#include<sched.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<pthread.h>
#include<sys/time.h>
#include<unistd.h>
#include "sock.h"
#include <mpi.h>
#include<sys/time.h>
#include "sock.h"
enum addr
{ ANY = -3, PARENT = -4, FORWARD = -5, TERM = -99 };
enum node
{ INSIDE, OUTSIDE };
enum command
{ TASK, RSLT, TREQ, NONE, RACK, BCST, BCAK, STAT, VERB, EXIT, CNCL, WRNG };
static char *cmd_strings[] =
  { "task", "rslt", "treq", "none", "rack", "bcst", "bcak", "stat", "verb",
"exit", "cncl", "wrng", 0 };
enum choose
{ CHS_RANDOM, CHS_ORDER };
static char *choose_strings[] = { "CHS-RANDOM", "CHS-ORDER" };

enum Affinity
{ COMPACT, SCATTER, SHAREDMEMORY };

struct send_block
{
  char *buf;
  int len;
  int size;
  int rank;
};

struct cmd
{
  enum command w;
  int c;
  enum node node;
  enum addr v[4][16];
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
extern void (*task_doers[256]) (struct thread_data *, void *);
extern void (*task_senders[256]) (void *);
extern void *(*task_allocators[256]) ();
extern void (*task_receivers[256]) (void *);
extern void (*rslt_senders[256]) (void *);
extern void (*rslt_receivers[256]) (void *);
struct worker_data;

void worker_init (struct thread_data *);
enum task_stat
{ TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE,
    TASK_SUSPENDED };
static char *task_stat_strings[] =
  { "TASK-ALLOCATED", "TASK-INITIALIZED", "TASK-STARTED", "TASK-DONE",
"TASK-NONE", "TASK-SUSPENDED" };
enum task_home_stat
{ TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE,
    TASK_HOME_EXCEPTION, TASK_HOME_ABORTED };
static char *task_home_stat_strings[] =
  { "TASK-HOME-ALLOCATED", "TASK-HOME-INITIALIZED", "TASK-HOME-DONE",
"TASK-HOME-EXCEPTION", "TASK-HOME-ABORTED" };
enum exiting_rsn
{ EXITING_NORMAL, EXITING_EXCEPTION, EXITING_CANCEL, EXITING_SPAWN };
static char *exiting_rsn_strings[] =
  { "EXITING-NORMAL", "EXITING-EXCEPTION", "EXITING-CANCEL",
"EXITING-SPAWN" };
enum tcounter
{ TCOUNTER_INIT, TCOUNTER_EXEC, TCOUNTER_SPWN, TCOUNTER_WAIT, TCOUNTER_EXCP,
    TCOUNTER_EXCP_WAIT, TCOUNTER_ABRT, TCOUNTER_ABRT_WAIT, TCOUNTER_TREQ_BK,
    TCOUNTER_TREQ_ANY };
static char *tcounter_strings[] =
  { "TCOUNTER-INIT", "TCOUNTER-EXEC", "TCOUNTER-SPWN", "TCOUNTER-WAIT",
"TCOUNTER-EXCP", "TCOUNTER-EXCP-WAIT", "TCOUNTER-ABRT", "TCOUNTER-ABRT-WAIT", "TCOUNTER-TREQ-BK",
"TCOUNTER-TREQ-ANY" };
enum event
{ EV_SEND_TASK_INSIDE, EV_SEND_TASK_OUTSIDE, EV_STRT_TASK_INSIDE,
    EV_STRT_TASK_OUTSIDE, EV_RSLT_TASK_INSIDE, EV_RSLT_TASK_OUTSIDE,
    EV_EXCP_TASK_INSIDE, EV_EXCP_TASK_OUTSIDE, EV_ABRT_TASK_INSIDE,
    EV_ABRT_TASK_OUTSIDE, EV_SEND_CNCL_INSIDE, EV_SEND_CNCL_OUTSIDE };
static char *ev_strings[] =
  { "EV-SEND-TASK-INSIDE", "EV-SEND-TASK-OUTSIDE", "EV-STRT-TASK-INSIDE",
"EV-STRT-TASK-OUTSIDE", "EV-RSLT-TASK-INSIDE", "EV-RSLT-TASK-OUTSIDE", "EV-EXCP-TASK-INSIDE",
"EV-EXCP-TASK-OUTSIDE", "EV-ABRT-TASK-INSIDE", "EV-ABRT-TASK-OUTSIDE", "EV-SEND-CNCL-INSIDE",
"EV-SEND-CNCL-OUTSIDE" };
enum obj_type
{ OBJ_NULL, OBJ_INT, OBJ_ADDR, OBJ_PADDR };

union aux_data_body
{
  long aux_int;
  enum addr aux_addr[16];
  enum addr *aux_paddr;
};

struct aux_data
{
  enum obj_type type;
  union aux_data_body body;
};

struct task
{
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  int cancellation;
  enum node rslt_to;
  enum addr rslt_head[16];
  int volatile progress;
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  int exception_tag;
  int msg_cncl;
  enum addr waiting_head[16];
  struct task *owner;
  struct task_home *eldest;
  int task_no;
  enum node req_from;
  enum addr task_head[16];
  struct task_home *next;
  void *body;
};

struct thread_data
{
  int id;
  pthread_t pthr_id;
  struct task_home *req;
  int req_cncl;
  int w_rack;
  int w_none;
  int ndiv;
  double probability;
  int last_treq;
  enum choose last_choose;
  double random_seed1;
  double random_seed2;
  unsigned short random_seed_probability[3];
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
  int w_bcak;
  enum exiting_rsn exiting;
  int exception_tag;
  enum tcounter tcnt_stat;
  double tcnt[10];
  struct timeval tcnt_tp[10];
  struct aux_data tc_aux;
  int ev_cnt[12];
  FILE *fp_tc;
  char dummy[1111];
};
enum DATA_FLAG
{ DATA_NONE, DATA_REQUESTING, DATA_EXIST };

struct dhandler_arg
{
  enum node data_to;
  enum addr head[16];
  struct cmd dreq_cmd;
  struct cmd dreq_cmd_fwd;
  int start;
  int end;
};

void make_and_send_task (struct thread_data *thr, int task_no, void *body,
                         int eldest_p);

void *wait_rslt (struct thread_data *thr, int stback);

void broadcast_task (struct thread_data *thr, int task_no, void *body);

void proto_error (char const *str, struct cmd *pcmd);

void read_to_eol (void);

void init_data_flag (int);

void guard_task_request (struct thread_data *);

int guard_task_request_prob (struct thread_data *, double);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_none (struct cmd *);

void recv_back (struct cmd *);

void print_task_list (struct task *task_top, char *name);

void print_task_home_list (struct task_home *treq_top, char *name);

void print_thread_status (struct thread_data *thr);

void print_status (struct cmd *);

void set_verbose_level (struct cmd *);

void recv_exit (struct cmd *);

void recv_bcst (struct cmd *);

void recv_bcak (struct cmd *);

void recv_cncl (struct cmd *);

void initialize_tcounter (struct thread_data *);

void tcounter_start (struct thread_data *, enum tcounter);

void tcounter_end (struct thread_data *, enum tcounter);

enum tcounter tcounter_change_state (struct thread_data *, enum tcounter,
                                     enum obj_type, void *);

void initialize_evcounter (struct thread_data *);

int evcounter_count (struct thread_data *, enum event, enum obj_type, void *);

void show_counters ();

void finalize_tcounter ();

int serialize_cmdname (char *buf, enum command w);

int deserialize_cmdname (enum command *buf, char *str);

int serialize_arg (char *buf, enum addr *arg);

enum addr deserialize_addr (char *str);

int deserialize_arg (enum addr *buf, char *str);

int serialize_cmd (char *buf, struct cmd *pcmd);

int deserialize_cmd (struct cmd *pcmd, char *str);

int copy_address (enum addr *dst, enum addr *src);

int address_equal (enum addr *adr1, enum addr *adr2);

struct runtime_option
{
  int num_thrs;
  char *sv_hostname;
  int port;
  char *node_name;
  char *initial_task;
  int auto_exit;
  int affinity;
  int always_flush_accepted_treq;
  enum Affinity thread_affinity;
  int cpu_num;
  int thread_per_cpu;
  int verbose;
  char *timechart_file;
};
extern struct runtime_option option;
#define NDEBUG
#include<assert.h>
struct thread_data threads[128];
unsigned int num_thrs;
int my_rank;
int num_procs;
char *init_task = NULL;
int wid;
double random_seed1 = 0.2403703;
double random_seed2 = 3.638732;
struct timeval tp_strt;

int
my_random (int max, double *pseed1, double *pseed2)
{
  *pseed1 = *pseed1 * 3.0 + *pseed2;
  *pseed1 -= (int) *pseed1;
  return max ** pseed1;
}

double
my_random_double (double *pseed1, double *pseed2)
{
  *pseed1 = *pseed1 * 3.0 + *pseed2;
  *pseed1 -= (int) *pseed1;
  return *pseed1;
}

double
my_random_probability (struct thread_data *thr)
{
  double d = erand48 (thr->random_seed_probability);
  return d;
}

struct runtime_option option;

int
systhr_create (pthread_t * p_tid, void *(*start_func) (void *), void *arg)
{
  int status = 0;
  pthread_t tid;
  pthread_attr_t attr;
  if (!p_tid)
    p_tid = &tid;
  else;
  pthread_attr_init (&attr);
  status = pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0)
    status = pthread_create (p_tid, &attr, start_func, arg);
  else
    status = pthread_create (p_tid, 0, start_func, arg);
  return status;
}

void
mem_error (char const *str)
{
  fputs (str, stderr);
  fputc ('\n', stderr);
  exit (1);
}

int
get_universal_real_time ()
{
  struct timeval now;
  gettimeofday (&now, 0);
  return 1000 * 1000 * now.tv_sec + now.tv_usec;
}

double
diff_timevals (struct timeval *tp1, struct timeval *tp2)
{
  return tp1->tv_sec - tp2->tv_sec + 1.0E-6 * (tp1->tv_usec - tp2->tv_usec);
}

void
proto_error (char const *str, struct cmd *pcmd)
{
  int i;
  char buf[1280];
  serialize_cmd (buf, pcmd);
  fprintf (stderr, "RANK %d: (%d): %s> %s\n", my_rank,
           get_universal_real_time (), str, buf);
}

pthread_mutex_t send_mut;
int sv_socket;

int
choose_rank (void)
{
  int rank;
  rank = my_random (num_procs - 1, &random_seed1, &random_seed2);
  if (rank >= my_rank)
    rank++;
  else;
  return rank;
}

void
read_to_eol (void)
{
  int c;
  while (EOF != (c = receive_char (sv_socket)))
    {
      if (c == '\n')
        break;
      else;
    }
}

void
flush_send (void)
{
  if (sv_socket == 0)
    fflush (stdout);
  else;
}

char buf[1280];
struct cmd cmd_buf;

int
read_command (char *buf, struct cmd *cmd_buf)
{
  char *cp = NULL;
  cp = receive_line (buf, 1280, sv_socket);
  if (cp)
    {
      cmd_buf->node = OUTSIDE;
      if (option.verbose >= 1)
        fprintf (stderr, "RANK %d: (%d): RECEIVED> %s", my_rank,
                 get_universal_real_time (), buf);
      else;
      deserialize_cmd (cmd_buf, buf);
      return 1;
    }
  else
    {
      if (option.verbose >= 1)
        fprintf (stderr, "RANK %d: (%d): RECEIVED> (failed)", my_rank,
                 get_universal_real_time ());
      else;
      return 0;
    }
}

void
set_progress (struct thread_data *thr, int n)
{
  struct task *tx = thr->task_top;
  tx->progress = n;
}

void
wait_progress (struct thread_data *thr, int k)
{
  struct task *tx = thr->task_top;
  while (tx->progress < k)
    {
    }
}

void
get_worker_id (struct thread_data *thr)
{
  wid = thr->id;
}

char send_buf[1280];

void
send_out_command (struct cmd *pcmd, void *body, int task_no)
{
  int ret;
  enum command w;
  struct thread_data *thr;
  int dest_rank;
  w = pcmd->w;
  get_worker_id (thr);
  send_block_start (dest_rank, num_thrs, wid);
  serialize_cmd (sq->buf, pcmd);
  sq->len = strlen (sq->buf);
  send_char ('\n', sv_socket);
  switch (w)
    {
    case TASK:
      dest_rank = (pcmd->v)[2][0];
      break;
    case RSLT:
      dest_rank = (pcmd->v)[0][0];
      break;
    case TREQ:
      dest_rank = (pcmd->v)[1][0];
      break;
    case NONE:
      dest_rank = (pcmd->v)[0][0];
      break;
    case RACK:
      dest_rank = (pcmd->v)[0][0];
      break;
    default:
      fprintf (stderr, "Error: Invalid destination rank.\n");
      break;
    }
  if (w == RSLT && -1 == dest_rank)
    {
      show_mpisend_buf (sv_socket);
      finalize_tcounter ();
      show_counters ();
      MPI_Abort (MPI_COMM_WORLD, 0);
      exit (0);
    }
  else;
  send_block_end (dest_rank);
}

/* Error!
(sc::cond
 (sc::body
  (sc::cond
   ((sc::or (sc::== sc::w sc::TASK) (sc::== sc::w sc::BCST)) ((sc::aref sc::task-senders sc::task-no) sc::body)
    (csym::send-char #\Newline sc::sv-socket))
   ((sc::== sc::w sc::RSLT) ((sc::aref sc::rslt-senders sc::task-no) sc::body)
    (csym::send-char #\Newline sc::sv-socket)))))
*/

void
proc_cmd (struct cmd *pcmd, void *body)
{
  enum command w;
  w = pcmd->w;
  switch (w)
    {
    case TASK:
      recv_task (pcmd, body);
      break;
    case RSLT:
      recv_rslt (pcmd, body);
      break;
    case TREQ:
      recv_treq (pcmd);
      break;
    case NONE:
      recv_none (pcmd);
      break;
    case RACK:
      recv_rack (pcmd);
      break;
    case BCST:
      recv_bcst (pcmd);
      break;
    case CNCL:
      recv_cncl (pcmd);
      break;
    case BCAK:
      recv_bcak (pcmd);
      break;
    case STAT:
      print_status (pcmd);
      break;
    case VERB:
      set_verbose_level (pcmd);
      break;
    case EXIT:
      recv_exit (pcmd);
      break;
    default:
      proto_error ("wrong cmd", pcmd);
      break;
    }
}

void
send_command (struct cmd *pcmd, void *body, int task_no)
{
  if (pcmd->node == INSIDE)
    {
      if (option.verbose >= 3)
        if (option.verbose >= 4 || TREQ != pcmd->w && NONE != pcmd->w)
          proto_error ("INSIDE", pcmd);
        else;
      else;
      proc_cmd (pcmd, body);
    }
  else
    {
      if (option.verbose >= 1)
        {
          proto_error ("OUTSIDE", pcmd);
        }
      else;
      send_out_command (pcmd, body, task_no);
    }
}

void
flush_treq_with_none_1 (struct thread_data *thr, struct task_home **p_hx)
{
  struct task_home *hx = *p_hx;
  struct cmd rcmd = {.c = 1,.w = NONE,.node = hx->req_from };
  copy_address ((rcmd.v)[0], hx->task_head);
  send_command (&rcmd, 0, 0);
  *p_hx = hx->next;
  hx->next = thr->treq_free;
  thr->treq_free = hx;
}

void
guard_task_request (struct thread_data *thr)
{
  flush_treq_with_none_1 (thr, &thr->treq_top);
}

int
guard_task_request_prob (struct thread_data *thr, double prob)
{
  if (prob >= my_random_probability (thr))
    {
      flush_treq_with_none_1 (thr, &thr->treq_top);
      return 1;
    }
  else
    return 0;
}

void
flush_treq_with_none (struct thread_data *thr, enum addr *rslt_head,
                      enum node rslt_to)
{
  struct task_home **pcur_hx = &thr->treq_top;
  struct task_home *hx;
  int flush;
  int ignored = 0;
  int flushed_any_treq = 0;
  enum addr *flushed_stealing_back_head = 0;
  while (hx = *pcur_hx)
    {
      if (option.always_flush_accepted_treq || TERM == (hx->waiting_head)[0])
        {
          if (option.verbose >= 2)
            {
              flushed_any_treq++;
            }
          else;
          flush = 1;
        }
      else if (rslt_head && hx->req_from == rslt_to
               && address_equal (hx->waiting_head, rslt_head))
        {
          if (option.verbose >= 2)
            {
              flushed_stealing_back_head = rslt_head;
              rslt_head = 0;
            }
          else;
          flush = 1;
        }
      else
        {
          if (option.verbose >= 2)
            {
              ignored++;
            }
          else;
          flush = 0;
        }
      if (flush)
        flush_treq_with_none_1 (thr, pcur_hx);
      else
        {
          pcur_hx = &hx->next;
        }
    }
  if (option.verbose >= 2)
    {
      char buf0[1280];
      char buf1[1280];
      if (flushed_any_treq > 0 || ignored > 0 || flushed_stealing_back_head)
        fprintf (stderr,
                 "(%d): (Thread %d) flushed %d any %s and ignored %d stealing-back treqs in flush-treq-with-none\n",
                 get_universal_real_time (), thr->id, flushed_any_treq,
                 flushed_stealing_back_head
                 ? (serialize_arg (buf1, flushed_stealing_back_head),
                    sprintf (buf0, "and stealing-back from %s", buf1),
                    buf0) : "", ignored);
      else;
    }
  else;
}

struct task *
allocate_task (struct thread_data *thr)
{
  struct task *tx;
  tx = thr->task_free;
  tx->stat = TASK_ALLOCATED;
  if (!tx)
    mem_error ("Not enough task memory");
  else;
  thr->task_top = tx;
  thr->task_free = tx->prev;
  return tx;
}

void
deallocate_task (struct thread_data *thr)
{
  struct task *tx = thr->task_top;
  thr->task_free = tx;
  thr->task_top = tx->next;
  return;
}

void
timeval_plus_nsec_to_timespec (struct timespec *pts_dst,
                               struct timeval *ptv_src, long diff)
{
  long nsec = diff + 1000 * ptv_src->tv_usec;
  pts_dst->tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
  pts_dst->tv_sec = ptv_src->tv_sec + (nsec > 999999999 ? 1 : 0);
}

int
send_treq_to_initialize_task (struct thread_data *thr, enum addr *treq_head,
                              enum node req_to)
{
  struct cmd rcmd;
  long delay = 1000;
  struct task *tx = thr->task_top;
  int rank_p = sv_socket < 0 ? 1 : 0;
  rcmd.c = 2;
  rcmd.node = req_to;
  rcmd.w = TREQ;
  if (rank_p)
    (rcmd.v)[0][0] = my_rank;
  else;
  (rcmd.v)[0][rank_p + 0] = thr->id;
  if (req_to != ANY && thr->sub)
    {
      (rcmd.v)[0][rank_p + 1] = thr->sub->id;
      (rcmd.v)[0][rank_p + 2] = TERM;
    }
  else
    (rcmd.v)[0][rank_p + 1] = TERM;
  copy_address ((rcmd.v)[1], treq_head);
  do
    {
      flush_treq_with_none (thr, 0, 0);
      tx->stat = TASK_ALLOCATED;
      {
        pthread_mutex_unlock (&thr->mut);
        if (rank_p && rcmd.node == OUTSIDE && (rcmd.v)[1][1] == ANY)
          (rcmd.v)[1][0] = choose_rank ();
        else;
        send_command (&rcmd, 0, 0);
        pthread_mutex_lock (&thr->mut);
      }
      while (1)
        {
          if (tx->stat != TASK_INITIALIZED && thr->sub
              && (thr->sub->stat == TASK_HOME_DONE
                  || thr->sub->stat == TASK_HOME_EXCEPTION
                  || thr->sub->stat == TASK_HOME_ABORTED))
            {
              if (tx->stat != TASK_NONE)
                (thr->w_none)++;
              else;
              return 0;
            }
          else;
          if (tx->stat != TASK_ALLOCATED)
            break;
          else;
          pthread_cond_wait (&thr->cond, &thr->mut);
        }
      if (tx->stat == TASK_NONE)
        {
          if (1)
            {
              struct timespec t_until;
              struct timeval now;

              gettimeofday (&now, 0);
              timeval_plus_nsec_to_timespec (&t_until, &now, delay);
              pthread_cond_timedwait (&thr->cond_r, &thr->mut, &t_until);
              delay += delay;
              if (delay > 100 * 1000 * 1000)
                delay = 100 * 1000 * 1000;
              else;
            }
          else;
          if (thr->sub
              && (thr->sub->stat == TASK_HOME_DONE
                  || thr->sub->stat == TASK_HOME_EXCEPTION
                  || thr->sub->stat == TASK_HOME_ABORTED))
            return 0;
          else;
        }
      else;
    }
  while (tx->stat != TASK_INITIALIZED);
  return 1;
}

void
recv_exec_send (struct thread_data *thr, enum addr *treq_head,
                enum node req_to)
{
  struct task *tx;
  int old_ndiv;
  double old_probability;
  enum exiting_rsn rsn;
  int reason;
  struct cmd rcmd;
  while (thr->w_none > 0)
    {
      pthread_cond_wait (&thr->cond, &thr->mut);
      if (thr->sub
          && (thr->sub->stat == TASK_HOME_DONE
              || thr->sub->stat == TASK_HOME_EXCEPTION
              || thr->sub->stat == TASK_HOME_ABORTED))
        return;
      else;
    }
  tx = allocate_task (thr);
  if (send_treq_to_initialize_task (thr, treq_head, req_to))
    {
      tcounter_change_state (thr, TCOUNTER_EXEC, OBJ_NULL, 0);
      evcounter_count (thr,
                       tx->rslt_to ==
                       INSIDE ? EV_STRT_TASK_INSIDE : EV_STRT_TASK_OUTSIDE,
                       OBJ_PADDR, tx->rslt_head);
      tx->stat = TASK_STARTED;
      tx->cancellation = 0;
      old_ndiv = thr->ndiv;
      old_probability = thr->probability;
      thr->ndiv = tx->ndiv;
      thr->probability = 1.0;
      pthread_mutex_unlock (&thr->mut);
      if (option.verbose >= 1)
        fprintf (stderr, "(%d): (Thread %d) start %d<%p> (body=%p).\n",
                 get_universal_real_time (), thr->id, tx->task_no, tx,
                 tx->body);
      else;
      (task_doers[tx->task_no]) (thr, tx->body);
      rsn = thr->exiting;
      thr->exiting = EXITING_NORMAL;
      switch (rsn)
        {
        case EXITING_NORMAL:
          if (option.verbose >= 1)
            fprintf (stderr, "(%d): (Thread %d) end %d<%p> (body=%p).\n",
                     get_universal_real_time (), thr->id, tx->task_no, tx,
                     tx->body);
          else;
          evcounter_count (thr,
                           tx->rslt_to ==
                           INSIDE ? EV_RSLT_TASK_INSIDE :
                           EV_RSLT_TASK_OUTSIDE, OBJ_PADDR, tx->rslt_head);
          reason = 0;
          break;
        case EXITING_EXCEPTION:
          if (option.verbose >= 1)
            fprintf (stderr,
                     "(%d): (Thread %d) end %d<%p> (body=%p) with exception %d.\n",
                     get_universal_real_time (), thr->id, tx->task_no, tx,
                     tx->body, thr->exception_tag);
          else;
          evcounter_count (thr,
                           tx->rslt_to ==
                           INSIDE ? EV_EXCP_TASK_INSIDE :
                           EV_EXCP_TASK_OUTSIDE, OBJ_PADDR, tx->rslt_head);
          reason = 1;
          break;
        case EXITING_CANCEL:
          if (option.verbose >= 1)
            fprintf (stderr, "(%d): (Thread %d) aborted %d<%p> (body=%p).\n",
                     get_universal_real_time (), thr->id, tx->task_no, tx,
                     tx->body);
          else;
          evcounter_count (thr,
                           tx->rslt_to ==
                           INSIDE ? EV_ABRT_TASK_INSIDE :
                           EV_ABRT_TASK_OUTSIDE, OBJ_PADDR, tx->rslt_head);
          reason = 2;
          break;
        default:
          fprintf (stderr,
                   "(%d) Warn: Thread %d ended with unexpected reason.\n",
                   get_universal_real_time (), thr->id);
          reason = 0;
        }
      rcmd.w = RSLT;
      rcmd.c = 3;
      rcmd.node = tx->rslt_to;
      copy_address ((rcmd.v)[0], tx->rslt_head);
      (rcmd.v)[1][0] = reason;
      (rcmd.v)[1][1] = TERM;
      (rcmd.v)[2][0] = thr->exception_tag;
      (rcmd.v)[2][1] = TERM;
      send_command (&rcmd, tx->body, tx->task_no);
      pthread_mutex_lock (&thr->rack_mut);
      (thr->w_rack)++;
      pthread_mutex_unlock (&thr->rack_mut);
      pthread_mutex_lock (&thr->mut);
      thr->ndiv = old_ndiv;
      thr->probability = old_probability;
    }
  else;
  tx->stat = TASK_DONE;
  flush_treq_with_none (thr, tx->rslt_head, tx->rslt_to);
  deallocate_task (thr);
}

void
worker_setaffinity (int n)
{
  cpu_set_t mask;
  CPU_ZERO (&mask);
  CPU_SET (n, &mask);
  if (-1 == sched_setaffinity (0, sizeof (mask), &mask))
    {
      perror ("Failed to set CPU affinity");
      exit (-1);
    }
  else;
  if (option.verbose >= 1)
    fprintf (stderr, "Bind worker to core %d\n", n);
  else;
}

void *
worker (void *arg)
{
  struct thread_data *thr = arg;
  thr->wdptr = malloc (sizeof (struct thread_data));
  if (option.affinity)
    switch (option.thread_affinity)
      {
      case SHAREDMEMORY:
        worker_setaffinity (my_rank * num_thrs + thr->id);
        break;
      case COMPACT:
        worker_setaffinity (thr->id);
        break;
      case SCATTER:
        worker_setaffinity (option.thread_per_cpu *
                            (thr->id % option.cpu_num) +
                            thr->id / option.cpu_num);
        break;
      }
  else;
  worker_init (thr);
  pthread_mutex_lock (&thr->mut);
  while (1)
    {
      if (thr->tcnt_stat != TCOUNTER_INIT)
        tcounter_change_state (thr, TCOUNTER_TREQ_ANY, OBJ_NULL, 0);
      else;
      if (sv_socket < 0)
        recv_exec_send (thr, (enum addr[3])
                        {
                        my_rank, ANY, TERM}
                        , INSIDE);
      else
      recv_exec_send (thr, (enum addr[2])
                      {
                      ANY, TERM}, INSIDE);
    }
  pthread_mutex_unlock (&thr->mut);
}

void
mpi_loop ()
{
  sendrecv ();
}

void
proc_msg ()
{
  struct cmd cmd;
  char buf[1280];
  if (option.verbose >= 1)
    fprintf (stderr, "RANK %d: Waiting for an external message.\n", my_rank);
  else;
  if (read_command (buf, &cmd))
    {
      if (option.verbose >= 1)
        fprintf (stderr, "RANK %d: Processing a %s command.\n", my_rank,
                 cmd_strings[cmd.w]);
      else;
      proc_cmd (&cmd, 0);
    }
  else;
}

void
recv_task (struct cmd *pcmd, void *body)
{
  struct task *tx;
  struct thread_data *thr;
  enum addr id;
  int task_no;
  size_t len;
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 4)
    proto_error ("wrong-task", pcmd);
  else;
  task_no = (pcmd->v)[3][0];
  if (pcmd->node == OUTSIDE)
    {
      body = (task_allocators[task_no]) ();
      (task_receivers[task_no]) (body);
      read_to_eol ();
    }
  else;
  id = (pcmd->v)[2][rank_p + 0];
  if (!(id < num_thrs))
    proto_error ("wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  tx = thr->task_top;
  tx->rslt_to = pcmd->node;
  copy_address (tx->rslt_head, (pcmd->v)[1]);
  tx->ndiv = (pcmd->v)[0][0];
  tx->task_no = task_no;
  tx->body = body;
  tx->stat = TASK_INITIALIZED;
  tx->progress = 0;
  pthread_cond_broadcast (&thr->cond);
  pthread_mutex_unlock (&thr->mut);
}

void
recv_none (struct cmd *pcmd)
{
  struct thread_data *thr;
  enum addr id;
  size_t len;
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 1)
    proto_error ("Wrong none", pcmd);
  else;
  id = (pcmd->v)[0][rank_p + 0];
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  if (thr->w_none > 0)
    (thr->w_none)--;
  else
    thr->task_top->stat = TASK_NONE;
  pthread_cond_broadcast (&thr->cond);
  pthread_mutex_unlock (&thr->mut);
}

struct task_home *
search_task_home_by_id (int id, struct task_home *hx)
{
  while (hx && hx->id != id)
    {
      hx = hx->next;
    }
  return hx;
}

int
set_cancelled (struct thread_data *thr, struct task *owner,
               struct task_home *eldest)
{
  struct task_home *cur;
  int count = 0;
  for (cur = thr->sub; cur; cur = cur->next)
    {
      if (cur->owner == owner && cur->stat == TASK_HOME_INITIALIZED
          && cur->msg_cncl == 0)
        {
          cur->msg_cncl = 1;
          count++;
        }
      else;
      if (eldest && eldest == cur->eldest)
        break;
      else;
    }
  if (count)
    thr->req_cncl = 1;
  else;
  return count;
}

void
recv_rslt (struct cmd *pcmd, void *body)
{
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  enum addr tid;
  int sid;
  int reason;
  int exception_tag;
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 2)
    proto_error ("Wrong rslt", pcmd);
  else;
  tid = (pcmd->v)[0][rank_p + 0];
  if (!(tid < num_thrs))
    proto_error ("wrong rslt-head", pcmd);
  else;
  sid = (pcmd->v)[0][rank_p + 1];
  if (TERM == sid)
    proto_error ("Wrong rslt-head (no task-home-id)", pcmd);
  else;
  thr = threads + tid;
  reason = (pcmd->v)[1][0];
  exception_tag = (pcmd->v)[2][0];
  pthread_mutex_lock (&thr->mut);
  if (!(hx = search_task_home_by_id (sid, thr->sub)))
    proto_error ("Wrong rslt-head (specified task not exists)", pcmd);
  else;
  if (pcmd->node == OUTSIDE)
    {
      (rslt_receivers[hx->task_no]) (hx->body);
      read_to_eol ();
    }
  else if (pcmd->node == INSIDE)
    {
      hx->body = body;
    }
  else
    {
      proto_error ("Wrong cmd.node", pcmd);
    }
  rcmd.c = 1;
  rcmd.node = pcmd->node;
  rcmd.w = RACK;
  copy_address ((rcmd.v)[0], hx->task_head);
  if (reason == 0)
    {
      hx->stat = TASK_HOME_DONE;
    }
  else if (reason == 1)
    {
      hx->stat = TASK_HOME_EXCEPTION;
      hx->exception_tag = exception_tag;
      (hx->owner->cancellation)++;
      set_cancelled (thr, hx->owner, hx->eldest);
    }
  else if (reason == 2)
    {
      hx->stat = TASK_HOME_ABORTED;
    }
  else;
  if (hx == thr->sub)
    {
      pthread_cond_broadcast (&thr->cond_r);
      pthread_cond_broadcast (&thr->cond);
    }
  else;
  pthread_mutex_unlock (&thr->mut);
  send_command (&rcmd, 0, 0);
}

struct task *
have_task (struct thread_data *thr, enum addr *task_spec, enum node task_from)
{
  struct task *tx;
  tx = thr->task_top;
  while (tx)
    {
      if ((tx->stat == TASK_INITIALIZED || tx->stat == TASK_SUSPENDED
           || tx->stat == TASK_STARTED) && tx->rslt_to == task_from
          && address_equal (tx->rslt_head, task_spec))
        return tx;
      else;
      tx = tx->next;
    }
  return 0;
}

int
try_treq (struct cmd *pcmd, enum addr id)
{
  enum addr *from_addr = (pcmd->v)[0];
  enum addr *dest_addr = (pcmd->v)[1];
  struct task_home *hx;
  struct thread_data *thr;
  int fail_reason = 0;
  int avail = 0;
  int rank_p = sv_socket < 0 ? 1 : 0;
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  pthread_mutex_lock (&thr->rack_mut);
  if (thr->w_rack > 0)
    {
      fail_reason = 1;
    }
  else if (!thr->task_top)
    {
      fail_reason = 2;
    }
  else if (dest_addr[rank_p + 0] == ANY)
    if (!
        (thr->task_top->stat == TASK_STARTED
         || thr->task_top->stat == TASK_INITIALIZED))
      fail_reason = 3;
    else;
  else if (!have_task (thr, from_addr, pcmd->node))
    fail_reason = 4;
  else;
  if (!fail_reason && dest_addr[rank_p + 0] == ANY)
    if (thr->probability < my_random_probability (thr))
      fail_reason = 5;
    else;
  else;
  avail = !fail_reason;
  pthread_mutex_unlock (&thr->rack_mut);
  if (option.verbose >= 2)
    if (!avail)
      {
        char from_str[1280];
        char buf1[1280];
        char rsn_str[1280];

        serialize_arg (from_str, from_addr);
        switch (fail_reason)
          {
          case 1:
            sprintf (rsn_str, "w-rack=%d", thr->w_rack);
            break;
          case 2:
            strcpy (rsn_str, "of having no task");
            break;
          case 3:
            sprintf (rsn_str, "the task is %s",
                     task_stat_strings[thr->task_top->stat]);
            break;
          case 4:
            serialize_arg (buf1, from_addr);
            sprintf (rsn_str, "%s is already finished", buf1);
            break;
          case 5:
            sprintf (rsn_str, "of probability (%lf)", thr->probability);
            break;
          default:
            strcpy (rsn_str, "Unexpected reason");
            break;
          }
        fprintf (stderr, "(%d): Thread %d refused treq from %s because %s.\n",
                 get_universal_real_time (), id, from_str, rsn_str);
      }
    else;
  else;
  if (avail)
    {
      hx = thr->treq_free;
      if (!hx)
        mem_error ("Not enough task-home memory");
      else;
      thr->treq_free = hx->next;
      hx->next = thr->treq_top;
      hx->stat = TASK_HOME_ALLOCATED;
      if (dest_addr[rank_p + 0] == ANY)
        (hx->waiting_head)[0] = TERM;
      else
        copy_address (hx->waiting_head, from_addr);
      copy_address (hx->task_head, (pcmd->v)[0]);
      if (pcmd->node != OUTSIDE)
        hx->req_from = INSIDE;
      else
        hx->req_from = OUTSIDE;
      thr->treq_top = hx;
      thr->req = hx;
    }
  else;
  pthread_mutex_unlock (&thr->mut);
  return avail;
}

int
choose_treq (enum addr from_addr)
{
  if (0 <= from_addr)
    {
      struct thread_data *thr = threads + from_addr;

      thr->last_choose = (1 + thr->last_choose) % 2;
      if (CHS_RANDOM == thr->last_choose)
        return my_random (num_thrs, &thr->random_seed1, &thr->random_seed2);
      else if (CHS_ORDER == thr->last_choose)
        {
          thr->last_treq = (1 + thr->last_treq) % num_thrs;
          return thr->last_treq;
        }
      else
        return 0;
    }
  else if (PARENT == from_addr)
    return my_random (num_thrs, &random_seed1, &random_seed2);
  else
    return 0;
}

void
recv_treq (struct cmd *pcmd)
{
  struct cmd rcmd;
  struct thread_data *thr;
  enum addr dst0;
  char sender[1280];
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 2)
    proto_error ("Wrong treq", pcmd);
  else;
  serialize_arg (sender, (pcmd->v)[0]);
  dst0 = (pcmd->v)[1][rank_p + 0];
  if (dst0 == ANY)
    {
      int myid;
      int start_id;
      int d;
      int id;

      myid = (pcmd->v)[0][rank_p + 0];
      start_id = choose_treq (myid);
      for (d = 0; d < num_thrs; d++)
        {
          id = (d + start_id) % num_thrs;
          if (pcmd->node != OUTSIDE && id == myid)
            continue;
          else;
          if (try_treq (pcmd, id))
            {
              if (option.verbose >= 2)
                fprintf (stderr, "(%d): treq(any) %s->%d... accepted.\n",
                         get_universal_real_time (), sender, id);
              else;
              break;
            }
          else;
          if (option.verbose >= 4)
            fprintf (stderr, "(%d): treq(any) %s->%d... refused.\n",
                     get_universal_real_time (), sender, id);
          else;
        }
      if (d < num_thrs)
        return;
      else;
    }
  else
    {
      if (!(0 <= dst0 && dst0 < num_thrs))
        proto_error ("Wrong task-head", pcmd);
      else;
      if (try_treq (pcmd, dst0))
        {
          if (option.verbose >= 2)
            fprintf (stderr,
                     "(%d): treq %s->%d (stealing back)... accepted.\n",
                     get_universal_real_time (), sender, dst0);
          else;
          return;
        }
      else;
      if (option.verbose >= 2)
        fprintf (stderr, "(%d): treq %s->%d (stealing back)... refused.\n",
                 get_universal_real_time (), sender, dst0);
      else;
    }
  if (dst0 == ANY && pcmd->node == INSIDE && (pcmd->v)[0][rank_p + 0] == 0)
    if (receive_buf && sv_socket < 0 && my_rank == 0)
      {
        get_worker_id (thr);
        send_block_start (my_rank, num_thrs, wid);
        send_string (receive_buf, sv_socket);
        send_block_end (my_rank);
        free (receive_buf);
        receive_buf = 0;
        return;
      }
    else
      {
        pcmd->node = OUTSIDE;
        if (sv_socket < 0)
          (pcmd->v)[1][0] = choose_rank ();
        else;
        send_command (pcmd, 0, 0);
        return;
      }
  else;
  rcmd.c = 1;
  rcmd.node = pcmd->node;
  rcmd.w = NONE;
  copy_address ((rcmd.v)[0], (pcmd->v)[0]);
  send_command (&rcmd, 0, 0);
}

void
recv_rack (struct cmd *pcmd)
{
  struct task *tx;
  struct thread_data *thr;
  enum addr id;
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 1)
    proto_error ("Wrong rack", pcmd);
  else;
  id = (pcmd->v)[0][rank_p + 0];
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", pcmd);
  else;
  thr = threads + id;
  pthread_mutex_lock (&thr->rack_mut);
  (thr->w_rack)--;
  pthread_mutex_unlock (&thr->rack_mut);
}

void
recv_bcst (struct cmd *pcmd)
{
  struct cmd rcmd;
  int task_no;
  void *body;
  if (pcmd->c < 2)
    proto_error ("wrong-task", pcmd);
  else;
  task_no = (pcmd->v)[1][0];
  body = (task_allocators[task_no]) ();
  (task_receivers[task_no]) (body);
  read_to_eol ();
  free (body);
  rcmd.c = 1;
  rcmd.node = pcmd->node;
  rcmd.w = BCAK;
  copy_address ((rcmd.v)[0], (pcmd->v)[0]);
  send_command (&rcmd, 0, task_no);
}

void
recv_cncl (struct cmd *pcmd)
{
  enum addr *from_addr;
  enum addr dst0;
  struct thread_data *thr;
  struct task *tx;
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 2)
    proto_error ("Wrong cncl", pcmd);
  else;
  from_addr = (pcmd->v)[0];
  dst0 = (pcmd->v)[1][rank_p + 0];
  if (!(0 <= dst0 && dst0 < num_thrs))
    proto_error ("Wrong cncl-head", pcmd);
  else;
  thr = threads + dst0;
  pthread_mutex_lock (&thr->mut);
  if (tx = have_task (thr, from_addr, pcmd->node))
    {
      (tx->cancellation)++;
      set_cancelled (thr, tx, 0);
      if (option.verbose >= 1)
        fprintf (stderr, "Task %p of worker %d is cancelled by cncl\n", tx,
                 dst0);
      else;
    }
  else;
  pthread_mutex_unlock (&thr->mut);
}

void
recv_bcak (struct cmd *pcmd)
{
  struct thread_data *thr;
  enum addr id;
  int rank_p = sv_socket < 0 ? 1 : 0;
  if (pcmd->c < 1)
    proto_error ("wrong-task", pcmd);
  else;
  id = (pcmd->v)[0][rank_p + 0];
  thr = threads + id;
  pthread_mutex_lock (&thr->mut);
  thr->w_bcak = 0;
  pthread_cond_broadcast (&thr->cond);
  pthread_mutex_unlock (&thr->mut);
}

void
node_to_string (char *buf, enum node node)
{
  switch (node)
    {
    case INSIDE:
      strcpy (buf, "INSIDE");
      break;
    case OUTSIDE:
      strcpy (buf, "OUTSIDE");
      break;
    default:
      strcpy (buf, "wrong-value");
      break;
    }
}

void
addr_to_string (char *buf, enum addr addr)
{
  switch (addr)
    {
    case ANY:
      strcpy (buf, "ANY");
      break;
    case PARENT:
      strcpy (buf, "PARENT");
      break;
    case TERM:
      strcpy (buf, "TERM");
      break;
    default:
      sprintf (buf, "%d", addr);
      break;
    }
}

void
print_task_list (struct task *task_top, char *name)
{
  struct task *cur;
  char buf1[1280];
  char buf2[1280];
  fprintf (stderr, "%s= {", name);
  for (cur = task_top; cur; cur = cur->next)
    {
      fprintf (stderr,
               "{stat=%s, task-no=%d, body=%p, ndiv=%d, cancellation=%d, rslt-to=%s, rslt-head=%s}, ",
               task_stat_strings[cur->stat], cur->task_no, cur->body,
               cur->ndiv, cur->cancellation,
               (node_to_string (buf1, cur->rslt_to), buf1),
               (serialize_arg (buf2, cur->rslt_head), buf2));
    }
  fprintf (stderr, "}, ");
  return;
}

void
print_task_home_list (struct task_home *treq_top, char *name)
{
  struct task_home *cur;
  char buf0[1280];
  char buf1[1280];
  char buf2[1280];
  fprintf (stderr, "%s= {", name);
  for (cur = treq_top; cur; cur = cur->next)
    {
      fprintf (stderr,
               "{stat=%s, id=%d, exception-tag=%d, msg-cncl=%d, waiting=%s, owner=%p, eldest=%p(%d), task-no=%d, body=%p, req-from=%s, task-head=%s}, ",
               task_home_stat_strings[cur->stat], cur->id, cur->exception_tag,
               cur->msg_cncl, (serialize_arg (buf0, cur->waiting_head), buf0),
               cur->owner, cur->eldest, cur->eldest ? cur->eldest->id : 0,
               cur->task_no, cur->body, (node_to_string (buf1, cur->req_from),
                                         buf1), (serialize_arg (buf2,
                                                                cur->
                                                                task_head),
                                                 buf2));
    }
  fprintf (stderr, "}, ");
  return;
}

void
print_thread_status (struct thread_data *thr)
{
  fprintf (stderr, "*** Worker %d ***\n", thr->id);
  fprintf (stderr, "req=%p, ", thr->req);
  fprintf (stderr, "w-rack=%d, ", thr->w_rack);
  fprintf (stderr, "w-none=%d, ", thr->w_none);
  fprintf (stderr, "ndiv=%d, ", thr->ndiv);
  fprintf (stderr, "exiting=%s, ", exiting_rsn_strings[thr->exiting]);
  fprintf (stderr, "exception-tag=%d, ", thr->exception_tag);
  fprintf (stderr, "probability=%lf, ", thr->probability);
  fprintf (stderr, "last-treq=%d, ", thr->last_treq);
  fprintf (stderr, "last-choose=%s, ", choose_strings[thr->last_choose]);
  fprintf (stderr, "random-seed(1,2)=(%f,%f), ", thr->random_seed1,
           thr->random_seed2);
  fprintf (stderr, "\n");
  print_task_list (thr->task_top, "tasks");
  fprintf (stderr, "\n");
  print_task_home_list (thr->treq_top, "treq-top");
  fprintf (stderr, "\n");
  print_task_home_list (thr->sub, "sub");
  fprintf (stderr, "\n");
  return;
}

void
print_status (struct cmd *pcmd)
{
  int i;
  fprintf (stderr, "worker-name: %s\n",
           option.node_name ? option.node_name : "Unnamed");
  fprintf (stderr, "num-thrs: %d\n", num_thrs);
  fprintf (stderr, "verbose-level: %d\n", option.verbose);
  for (i = 0; i < num_thrs; i++)
    {
      print_thread_status (&threads[i]);
    }
  return;
}

void
set_verbose_level (struct cmd *pcmd)
{
  if (pcmd->c < 1)
    proto_error ("Wrong verb", pcmd);
  else;
  option.verbose = (pcmd->v)[0][0];
  return;
}

void
recv_exit (struct cmd *pcmd)
{
  fprintf (stderr, "Received \"exit\"... terminate.\n");
  exit (0);
}

void
handle_req (int (*_bk) (void), struct thread_data *_thr)
{
  if (_thr->req && !_thr->task_top->cancellation)
    {
      _thr->exiting = EXITING_SPAWN;
      tcounter_change_state (_thr, TCOUNTER_SPWN, OBJ_NULL, 0);
      _bk ();
      tcounter_change_state (_thr, TCOUNTER_EXEC, OBJ_NULL, 0);
      _thr->exiting = EXITING_NORMAL;
      _thr->req = _thr->treq_top;
    }
  else;
}

int
send_cncl_for_flagged_subtasks (struct thread_data *thr)
{
  struct cmd rcmd;
  struct task_home *cur;
  int count = 0;
  rcmd.w = CNCL;
  rcmd.c = 2;
  (rcmd.v)[0][0] = thr->id;
  (rcmd.v)[0][2] = TERM;
  for (cur = thr->sub; cur; cur = cur->next)
    {
      if (cur->msg_cncl == 1 && cur->stat == TASK_HOME_INITIALIZED)
        {
          rcmd.node = cur->req_from;
          (rcmd.v)[0][1] = cur->id;
          copy_address ((rcmd.v)[1], cur->task_head);
          send_command (&rcmd, 0, 0);
          cur->msg_cncl = 2;
          evcounter_count (thr,
                           rcmd.node ==
                           INSIDE ? EV_SEND_CNCL_INSIDE :
                           EV_SEND_CNCL_OUTSIDE, OBJ_PADDR, cur->task_head);
          count++;
        }
      else;
    }
  return count;
}

void
handle_req_cncl (int (*_bk) (void), struct thread_data *_thr)
{
  if (_thr->req_cncl)
    {
      if (option.verbose >= 1)
        fprintf (stderr, "(%d): (Thread %d) detected cncl message request\n",
                 get_universal_real_time (), _thr->id);
      else;
      send_cncl_for_flagged_subtasks (_thr);
      _thr->req_cncl = 0;
    }
  else;
}

void
handle_exception (int (*_bk) (void), struct thread_data *_thr, int excep)
{
  _thr->exiting = EXITING_EXCEPTION;
  _thr->exception_tag = excep;
  tcounter_change_state (_thr, TCOUNTER_EXCP, OBJ_INT,
                         (void *) ((long) excep));
  _bk ();
  perror ("Unexpectedly returned from backtracking for an exception.");
}

void
handle_cancellation (int (*_bk) (void), struct thread_data *_thr)
{
  if (_thr->task_top->cancellation)
    {
      if (option.verbose >= 1)
        fprintf (stderr,
                 "(%d): (Thread %d) detected cancellation flag (%d)\n",
                 get_universal_real_time (), _thr->id,
                 _thr->task_top->cancellation);
      else;
      _thr->exiting = EXITING_CANCEL;
      tcounter_change_state (_thr, TCOUNTER_ABRT, OBJ_INT,
                             (void *) ((long) _thr->task_top->cancellation));
      pthread_mutex_unlock (&_thr->mut);
      _bk ();
      perror ("Unexpectedly returned from backtracking for abortion.");
    }
  else;
}

void
handle_reqs (int (*_bk) (void), struct thread_data *_thr)
{
  pthread_mutex_lock (&_thr->mut);
  handle_req (_bk, _thr);
  handle_req_cncl (_bk, _thr);
  handle_cancellation (_bk, _thr);
  pthread_mutex_unlock (&_thr->mut);
}

void
make_and_send_task (struct thread_data *thr, int task_no, void *body,
                    int eldest_p)
{
  struct cmd tcmd;
  struct task_home *hx = thr->treq_top;
  int rank_p = sv_socket < 0 ? 1 : 0;
  thr->treq_top = hx->next;
  hx->next = thr->sub;
  thr->sub = hx;
  hx->task_no = task_no;
  hx->body = body;
  hx->id = hx->next ? hx->next->id + 1 : 0;
  hx->owner = thr->task_top;
  hx->eldest = eldest_p ? hx : hx->next->eldest;
  hx->msg_cncl = 0;
  hx->stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = hx->req_from;
  tcmd.w = TASK;
  (tcmd.v)[0][0] = ++thr->ndiv;
  (tcmd.v)[0][1] = TERM;
  if (rank_p)
    (tcmd.v)[1][0] = my_rank;
  else;
  (tcmd.v)[1][rank_p + 0] = thr->id;
  (tcmd.v)[1][rank_p + 1] = hx->id;
  (tcmd.v)[1][rank_p + 2] = TERM;
  copy_address ((tcmd.v)[2], hx->task_head);
  (tcmd.v)[3][0] = task_no;
  (tcmd.v)[3][1] = TERM;
  evcounter_count (thr,
                   tcmd.node ==
                   INSIDE ? EV_SEND_TASK_INSIDE : EV_SEND_TASK_OUTSIDE,
                   OBJ_PADDR, hx->task_head);
  send_command (&tcmd, body, task_no);
}

void *
wait_rslt (struct thread_data *thr, int stback)
{
  void *body;
  struct task_home *sub;
  enum tcounter tcnt_stat;
  enum tcounter tcnt_stat_w;
  pthread_mutex_lock (&thr->mut);
  tcnt_stat = thr->tcnt_stat;
  sub = thr->sub;
  while (sub->stat != TASK_HOME_DONE && sub->stat != TASK_HOME_EXCEPTION
         && sub->stat != TASK_HOME_ABORTED)
    {
      tcnt_stat_w =
        tcnt_stat == TCOUNTER_EXEC ? TCOUNTER_WAIT : (tcnt_stat ==
                                                      TCOUNTER_ABRT ?
                                                      TCOUNTER_ABRT_WAIT :
                                                      TCOUNTER_EXCP_WAIT);
      tcounter_change_state (thr, tcnt_stat_w, OBJ_NULL, 0);
      thr->task_top->stat = TASK_SUSPENDED;
      if (thr->exiting == EXITING_EXCEPTION)
        {
          set_cancelled (thr, thr->task_top, sub->eldest);
          send_cncl_for_flagged_subtasks (thr);
        }
      else;
      if (OUTSIDE == sub->req_from)
        {
          struct timeval now;
          struct timespec t_until;

          gettimeofday (&now, 0);
          timeval_plus_nsec_to_timespec (&t_until, &now, 1000);
          pthread_cond_timedwait (&thr->cond_r, &thr->mut, &t_until);
        }
      else;
      if (sub->stat == TASK_HOME_DONE || sub->stat == TASK_HOME_EXCEPTION
          || sub->stat == TASK_HOME_ABORTED)
        break;
      else;
      if (stback)
        {
          tcounter_change_state (thr, TCOUNTER_TREQ_BK, OBJ_ADDR,
                                 sub->task_head);
          recv_exec_send (thr, sub->task_head, sub->req_from);
          tcounter_change_state (thr, tcnt_stat_w, OBJ_NULL, 0);
        }
      else
        pthread_cond_wait (&thr->cond_r, &thr->mut);
    }
  if (sub->stat == TASK_HOME_EXCEPTION)
    {
      thr->exiting = EXITING_EXCEPTION;
      thr->exception_tag = sub->exception_tag;
      (sub->owner->cancellation)--;
      tcnt_stat = TCOUNTER_EXCP;
    }
  else;
  tcounter_change_state (thr, tcnt_stat, OBJ_NULL, 0);
  if (sub->stat == TASK_HOME_EXCEPTION || sub->stat == TASK_HOME_ABORTED)
    {
      free (body);
      body = 0;
    }
  else
    body = sub->body;
  thr->sub = sub->next;
  sub->next = thr->treq_free;
  thr->treq_free = sub;
  thr->task_top->stat = TASK_STARTED;
  pthread_mutex_unlock (&thr->mut);
  return body;
}

int tcell_bcst_wait_bcak = 1;

void
broadcast_task (struct thread_data *thr, int task_no, void *body)
{
  struct cmd bcmd;
  bcmd.c = 2;
  bcmd.node = OUTSIDE;
  bcmd.w = BCST;
  (bcmd.v)[0][0] = thr->id;
  (bcmd.v)[0][1] = TERM;
  (bcmd.v)[1][0] = task_no;
  (bcmd.v)[1][1] = TERM;
  send_command (&bcmd, body, task_no);
  if (tcell_bcst_wait_bcak)
    {
      pthread_mutex_lock (&thr->mut);
      thr->w_bcak = 1;
      while (thr->w_bcak)
        {
          pthread_cond_wait (&thr->cond, &thr->mut);
        }
      pthread_mutex_unlock (&thr->mut);
    }
  else;
}

void
usage (int argc, char **argv)
{
  fprintf (stderr,
           "Usage: %s [-s hostname] [-p port-num(-1 for MPI mode)] [-n n-threads] [-i initial-task-parms] [-a] [-v verbosity] [-T timechart-prefix]\n",
           argv[0]);
  exit (1);
}

void
set_option (int argc, char **argv)
{
  int i;
  int ch;
  FILE *fp;
  char buf[256];
  char *command;
  option.sv_hostname = 0;
  option.port = -1;
  option.num_thrs = 1;
  option.node_name = 0;
  option.initial_task = 0;
  option.auto_exit = 0;
  option.affinity = 0;
  option.always_flush_accepted_treq = 0;
  option.thread_affinity = COMPACT;
  option.cpu_num = 0;
  option.thread_per_cpu = 0;
  option.verbose = 0;
  option.timechart_file = 0;
  while (-1 != (ch = getopt (argc, argv, "n:s:p:N:i:xat:c:r:fP:v:T:h")))
    {
      switch (ch)
        {
        case 'n':
          option.num_thrs = atoi (optarg);
          break;
        case 's':
          option.sv_hostname = strcmp ("stdout", optarg) ? optarg : 0;
          break;
        case 'p':
          option.port = atoi (optarg);
          break;
        case 'N':
          if (option.node_name)
            free (option.node_name);
          else;
          option.node_name =
            (char *) malloc ((1 + strlen (optarg)) * sizeof (char));
          strcpy (option.node_name, optarg);
          break;
        case 'i':
          if (option.initial_task)
            free (option.initial_task);
          else;
          option.initial_task =
            (char *) malloc ((1 + strlen (optarg)) * sizeof (char));
          strcpy (option.initial_task, optarg);
          option.auto_exit = 1;
          break;
        case 'x':
          option.auto_exit = 1;
          break;
        case 'a':
          if (0 == option.affinity)
            fprintf (stderr, "setaffinity enabled.\n");
          else;
          option.affinity = 1;
          break;
        case 't':
          if (strcmp ("compact", optarg) == 0)
            {
              option.thread_affinity = COMPACT;
            }
          else if (strcmp ("scatter", optarg) == 0)
            {
              option.thread_affinity = SCATTER;
            }
          else if (strcmp ("sharedmemory", optarg) == 0)
            {
              option.thread_affinity = SHAREDMEMORY;
            }
          else;
          break;
        case 'c':
          option.cpu_num = atoi (optarg);
          break;
        case 'r':
          option.thread_per_cpu = atoi (optarg);
          break;
        case 'f':
          option.always_flush_accepted_treq = 1;
          break;
        case 'v':
          option.verbose = atoi (optarg);
          break;
        case 'T':
          command = "hostname -s";
          if ((fp = popen (command, "r")) == NULL)
            {
              fprintf (stderr, "popen error!\n");
              exit (EXIT_FAILURE);
            }
          else;
          fgets (buf, 256, fp);
          strtok (buf, "\n");
          (void) pclose (fp);
          {
            option.timechart_file = optarg;
            strcat (option.timechart_file, buf);
          }
          break;
        case 'h':
          usage (argc, argv);
          break;
        default:
          fprintf (stderr, "Unknown option: %c\n", ch);
          usage (argc, argv);
          break;
        }
    }
  return;
}

void
initialize_task_list (struct task *tlist, int len, struct task **p_top,
                      struct task **p_free)
{
  int i;
  *p_top = 0;
  *p_free = tlist;
  for (i = 0; i < len - 1; i++)
    {
      (tlist[i]).prev = &tlist[i + 1];
      (tlist[i + 1]).next = &tlist[i];
    }
  (tlist[0]).next = 0;
  (tlist[len - 1]).prev = 0;
  for (i = 0; i < len; i++)
    {
      (tlist[i]).rslt_to = TERM;
      ((tlist[i]).rslt_head)[0] = TERM;
    }
  return;
}

void
initialize_task_home_list (struct task_home *hlist, int len,
                           struct task_home **p_top,
                           struct task_home **p_free)
{
  int i;
  *p_top = 0;
  *p_free = hlist;
  for (i = 0; i < len - 1; i++)
    {
      (hlist[i]).next = &hlist[i + 1];
      (hlist[len - 1]).next = 0;
    }
  return;
}

void
set_aux_data (struct aux_data *paux, enum obj_type aux_type, void *aux_body)
{
  paux->type = aux_type;
  switch (aux_type)
    {
    case OBJ_INT:
      paux->body.aux_int = (long) aux_body;
      break;
    case OBJ_ADDR:
      copy_address (paux->body.aux_addr, (enum addr *) aux_body);
      break;
    case OBJ_PADDR:
      paux->body.aux_paddr = (enum addr *) aux_body;
      break;
    }
}

void
print_aux_data (FILE * fp, struct aux_data *paux)
{
  char buf[1280];
  switch (paux->type)
    {
    case OBJ_INT:
      fprintf (fp, " %d", paux->body.aux_int);
      break;
    case OBJ_ADDR:
      serialize_arg (buf, paux->body.aux_addr);
      fputc (' ', fp);
      fputs (buf, fp);
      break;
    case OBJ_PADDR:
      serialize_arg (buf, paux->body.aux_paddr);
      fputc (' ', fp);
      fputs (buf, fp);
      break;
    }
}

void
initialize_tcounter (struct thread_data *thr)
{
  int i;
  struct timeval tp;
  thr->tcnt_stat = TCOUNTER_INIT;
  gettimeofday (&tp, 0);
  for (i = 0; i < 10; i++)
    {
      (thr->tcnt)[i] = 0;
      (thr->tcnt_tp)[i] = tp;
    }
  thr->tc_aux.type = OBJ_NULL;
}

void
tcounter_start (struct thread_data *thr, enum tcounter tcnt_stat)
{
  struct timeval tp;
  gettimeofday (&tp, 0);
  (thr->tcnt_tp)[tcnt_stat] = tp;
}

void
tcounter_end (struct thread_data *thr, enum tcounter tcnt_stat)
{
  struct timeval tp;
  gettimeofday (&tp, 0);
  (thr->tcnt)[tcnt_stat] += diff_timevals (&tp, &(thr->tcnt_tp)[tcnt_stat]);
  (thr->tcnt_tp)[tcnt_stat] = tp;
}

enum tcounter
tcounter_change_state (struct thread_data *thr, enum tcounter tcnt_stat,
                       enum obj_type aux_type, void *aux_body)
{
  struct timeval tp;
  enum tcounter tcnt_stat0;
  double tcnt0;
  double tcnt;
  tcnt_stat0 = thr->tcnt_stat;
  if (tcnt_stat0 != tcnt_stat)
    {
      tcnt0 = (thr->tcnt)[tcnt_stat0];
      gettimeofday (&tp, 0);
      tcnt = tcnt0 + diff_timevals (&tp, &(thr->tcnt_tp)[tcnt_stat0]);
      (thr->tcnt)[tcnt_stat0] = tcnt;
      (thr->tcnt_tp)[tcnt_stat] = tp;
      thr->tcnt_stat = tcnt_stat;
      if (thr->fp_tc)
        {
          struct timeval *tp0;

          tp0 = &(thr->tcnt_tp)[tcnt_stat0];
          if (tcnt_stat0 == TCOUNTER_INIT && thr->id == 0)
            tp_strt = tp;
          else;
          fprintf (thr->fp_tc, "%s %lf %lf", tcounter_strings[tcnt_stat0],
                   diff_timevals (tp0, &tp_strt), diff_timevals (&tp,
                                                                 &tp_strt));
          fputc (' ', thr->fp_tc);
          print_aux_data (thr->fp_tc, &thr->tc_aux);
          fputc ('\n', thr->fp_tc);
          set_aux_data (&thr->tc_aux, aux_type, aux_body);
        }
      else;
    }
  else;
  return tcnt_stat0;
}

void
initialize_evcounter (struct thread_data *thr)
{
  int i;
  for (i = 0; i < 12; i++)
    {
      (thr->ev_cnt)[i] = 0;
    }
}

int
evcounter_count (struct thread_data *thr, enum event ev,
                 enum obj_type aux_type, void *aux_body)
{
  struct timeval tp;
  struct aux_data aux;
  ((thr->ev_cnt)[ev])++;
  if (thr->fp_tc)
    {
      gettimeofday (&tp, 0);
      fprintf (thr->fp_tc, "%s %lf", ev_strings[ev],
               diff_timevals (&tp, &tp_strt));
      fputc (' ', thr->fp_tc);
      set_aux_data (&aux, aux_type, aux_body);
      print_aux_data (thr->fp_tc, &aux);
      fputc ('\n', thr->fp_tc);
    }
  else;
  return (thr->ev_cnt)[ev];
}

void
show_counters ()
{
  int i;
  int j;
  struct thread_data *thr;
  for (i = 0; i < num_thrs; i++)
    {
      fprintf (stderr, "*** Worker %d ***\n", i);
      thr = threads + i;
      for (j = 0; j < 10; j++)
        {
          fprintf (stderr, "%s: %lf\n", tcounter_strings[j], (thr->tcnt)[j]);
        }
      for (j = 0; j < 12; j++)
        {
          fprintf (stderr, "%s: %ld\n", ev_strings[j], (thr->ev_cnt)[j]);
        }
    }
  return;
}

void
finalize_tcounter ()
{
  {
    int i;
    struct thread_data *thr;
    FILE *fp;

    for (i = 0; i < num_thrs; i++)
      {
        thr = threads + i;
        tcounter_change_state (thr, TCOUNTER_INIT, OBJ_NULL, 0);
        if (thr->fp_tc)
          {
            fp = thr->fp_tc;
            thr->fp_tc = 0;
            fclose (fp);
          }
        else;
      }
  }
}

int
main (int argc, char **argv)
{
  int i;
  int mpi_provided;
  fprintf (stderr,
           "compile-time options: VERBOSE=1 PROFILE=1 NF-TYPE=GCC USE-AFFINITY=SCHED USEMPI=yes\n");
  MPI_Init_thread (&argc, &argv, MPI_THREAD_MULTIPLE, &mpi_provided);
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size (MPI_COMM_WORLD, &num_procs);
  if (my_rank == 0)
    {
      char *provided_msg;
      switch (mpi_provided)
        {
        case MPI_THREAD_SINGLE:
          provided_msg = "SINGLE";
          break;
        case MPI_THREAD_FUNNELED:
          provided_msg = "FUNNELED";
          break;
        case MPI_THREAD_SERIALIZED:
          provided_msg = "SERIALIZED";
          break;
        case MPI_THREAD_MULTIPLE:
          provided_msg = "MULTIPLE";
          break;
        }
      fprintf (stderr, "Available level of thread support %s\n",
               provided_msg);
    }
  else;
  set_option (argc, argv);
  if (option.port < 0)
    sv_socket = -1;
  else
    sv_socket =
      option.sv_hostname ? connect_to (option.sv_hostname, option.port) : 0;
  pthread_mutexattr_t m_attr;
  pthread_mutexattr_init (&m_attr);
  pthread_mutex_init (&send_mut, &m_attr);
  num_thrs = option.num_thrs;
  for (i = 0; i < num_thrs; i++)
    {
      {
        struct thread_data *thr = threads + i;
        struct task *tx;
        struct task_home *hx;

        thr->req = 0;
        thr->req_cncl = 0;
        thr->id = i;
        thr->w_rack = 0;
        thr->w_none = 0;
        thr->w_bcak = 0;
        thr->ndiv = 0;
        thr->probability = 1.0;
        thr->last_treq = i;
        thr->last_choose = CHS_RANDOM;
        thr->exiting = EXITING_NORMAL;
        thr->exception_tag = 0;
        {
          double r;
          double q;

          r = sqrt (0.5 + i);
          q = sqrt (r + i);
          r -= (int) r;
          thr->random_seed1 = r;
          thr->random_seed2 = q;
        }
        (thr->random_seed_probability)[0] = 3 + 3 * i;
        (thr->random_seed_probability)[1] = 4 + 3 * i;
        (thr->random_seed_probability)[2] = 5 + 3 * i;
        pthread_mutex_init (&thr->mut, &m_attr);
        pthread_mutex_init (&thr->rack_mut, &m_attr);
        pthread_cond_init (&thr->cond, 0);
        pthread_cond_init (&thr->cond_r, 0);
        tx = (struct task *) malloc (sizeof (struct task) * (4 * 65536));
        initialize_task_list (tx, 4 * 65536, &thr->task_top, &thr->task_free);
        hx =
          (struct task_home *) malloc (sizeof (struct task_home) *
                                       (4 * 65536));
        initialize_task_home_list (hx, 4 * 65536, &thr->treq_top,
                                   &thr->treq_free);
        thr->sub = 0;
        if (option.timechart_file)
          {
            char *fname;
            int len;

            len = strlen (option.timechart_file) + 10;
            fname = malloc (sizeof (char) * len);
            snprintf (fname, len, "%s-%04d.dat", option.timechart_file,
                      thr->id);
            thr->fp_tc = fopen (fname, "w");
            if (!thr->fp_tc)
              perror ("Failed to open timechart-file for writing");
            else;
            free (fname);
          }
        else
          thr->fp_tc = 0;
      }
    }
  if (option.initial_task && (sv_socket >= 0 || my_rank == 0))
    {
      char *p_src;
      char *p_dst;
      char *header;
      header = sv_socket < 0 ? "task 0 -1:0 0:0 " : "task 0 0 0 ";
      receive_buf =
        (char *) malloc ((3 + strlen (option.initial_task) + strlen (header))
                         * sizeof (char));
      receive_buf_p = receive_buf;
      strcpy (receive_buf, header);
      for ((p_src = option.initial_task, p_dst =
            receive_buf + strlen (header)); *p_src; p_src++, p_dst++)
        {
          *p_dst = ' ' == *p_src ? '\n' : *p_src;
        }
      *p_dst++ = '\n';
      *p_dst++ = '\n';
      *p_dst = 0;
      if (option.verbose >= 1)
        fprintf (stderr, "%s", receive_buf);
      else;
    }
  else;
  for (i = 0; i < num_thrs; i++)
    {
      {
        struct thread_data *thr = threads + i;

        initialize_evcounter (thr);
        initialize_tcounter (thr);
        systhr_create (&thr->pthr_id, worker, thr);
      }
    }
  sleep (1);
  if (sv_socket < 0)
    {
      mpi_loop ();
      MPI_Finalize ();
      exit (0);
    }
  else
    {
      struct cmd cmd;
      char buf[1280];
      while (1)
        {
          if (read_command (buf, &cmd))
            proc_cmd (&cmd, 0);
          else
            break;
        }
      sleep (2147483647);
      exit (0);
    }
  return 0;
}
