typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef unsigned int size_t;
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
  struct cmd_list *next;
};

void read_to_eol (void);

void recv_rslt (struct cmd, void *);

void recv_task (struct cmd, void *);

void recv_treq (struct cmd);

void recv_rack (struct cmd);

void recv_none (struct cmd);
struct task;
struct thread_data;
void (*task_doers[256]) (char *, struct thread_data *, void *);
void (*task_senders[256]) (struct thread_data *, void *);
void *(*task_receivers[256]) (struct thread_data *);
void (*rslt_senders[256]) (struct thread_data *, void *);
void (*rslt_receivers[256]) (struct thread_data *, void *);
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
  struct task_home *volatile req;
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
  char buf[1024];
};
extern int divisibility_flag;

void send_divisible (void);

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (char *esp, struct thread_data *thr);

void send_int (int n);

int recv_int (void);

void handle_req (char *esp, closure_t *, struct thread_data *);
#include<sys/time.h>

int fib (char *esp, closure_t * _bk, struct thread_data *_thr, int n);

struct fib
{
  int n;
  int r;
};

struct do_fib_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp;
  struct fib *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

struct _bk_in_do_fib_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_fib_task_frame *xfp;
};

char *
_bk_in_do_fib_task (char *esp, void *xfp0)
{
  struct _bk_in_do_fib_task_frame *efp;
  struct do_fib_task_frame *xfp = xfp0;
  char *parmp = (char *) esp;
LGOTO:
  ;
  efp = (struct _bk_in_do_fib_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
	      (sizeof (struct _bk_in_do_fib_task_frame) + sizeof (Align_t) +
	       -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  *((int *) efp) = 0;
  return 0;
  return 0;
}

void
do_fib_task (char *esp, struct thread_data *_thr, struct fib *pthis)
{
  struct do_fib_task_frame *efp;
LGOTO:
  ;
  efp = (struct do_fib_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
	      (sizeof (struct do_fib_task_frame) + sizeof (Align_t) +
	       -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_fib_task;
  efp->_bk.fr = (void *) efp;
  fprintf (stderr, "start fib(%d)\n", (*efp->pthis).n);
  while ((efp->tmp =
	  fib (esp, &efp->_bk, efp->_thr, (*efp->pthis).n)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  (*efp->pthis).r = efp->tmp;
}

struct fib_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  closure_t *_bk0;
  int spawned;
  struct fib *pthis;
  struct fib st;
  int s2;
  int s1;
  int n;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t _bk2;
};

struct _bk2_in_fib_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct fib_frame *xfp;
};

char *
_bk2_in_fib (char *esp, void *xfp0)
{
  struct _bk2_in_fib_frame *efp;
  struct fib_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
LGOTO:
  ;
  efp = (struct _bk2_in_fib_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
	      (sizeof (struct _bk2_in_fib_frame) + sizeof (Align_t) +
	       -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  if (xfp->spawned)
    {
      *((int *) efp) = 0;
      return 0;
    }
  {
    char *goto_fr;
    argp =
      (char *) ((Align_t *) esp +
		(sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = xfp->_bk0;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
  }
  if ((*xfp->_thr).treq_top)
    {
      (*xfp->pthis).n = xfp->n - 2;
      xfp->spawned = 1;
      make_and_send_task (xfp->_thr, 0, xfp->pthis);
      *((int *) efp) = 1;
      return 0;
    }
  *((int *) efp) = 0;
  return 0;
  return 0;
}

int
fib (char *esp, closure_t * _bk, struct thread_data *_thr, int n)
{
  struct fib_frame *efp;
LGOTO:
  ;
  efp = (struct fib_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
	      (sizeof (struct fib_frame) + sizeof (Align_t) +
	       -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->n = n;
  efp->_bk2.fun = _bk2_in_fib;
  efp->_bk2.fr = (void *) efp;
  if (efp->n <= 2)
    return 1;
  else
    {
      {
	efp->pthis = &efp->st;
	efp->spawned = 0;
	if (divisibility_flag == 1)
	  {
	    send_divisible ();
	  }
	{
	  efp->_bk0 = efp->_bk;
	  if ((*efp->_thr).req)
	    while (handle_req (esp, &efp->_bk2, efp->_thr),
		   (efp->tmp_esp = *((char **) esp)) != 0)
	      {
		if ((char *) efp->tmp_esp < (char *) efp)
		  return (int) 0 - 1;
		efp->tmp_esp = 0;
		goto LGOTO;
	      }
	  while ((efp->s1 =
		  fib (esp, &efp->_bk2, efp->_thr, efp->n - 1)) == (int) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return (int) 0 - 1;
	      efp->tmp_esp = 0;
	      goto LGOTO;
	    }
	  if (efp->spawned)
	    {
	      while (wait_rslt (esp, efp->_thr),
		     (efp->tmp_esp = *((char **) esp)) != 0)
		{
		  if ((char *) efp->tmp_esp < (char *) efp)
		    return (int) 0 - 1;
		  efp->tmp_esp = 0;
		  goto LGOTO;
		}
	      efp->s2 = (*efp->pthis).r;
	    }
	  else
	    while ((efp->s2 =
		    fib (esp, &efp->_bk2, efp->_thr,
			 efp->n - 2)) == (int) 0 - 1
		   && (efp->tmp_esp = *((char **) esp)) != 0)
	      {
		if ((char *) efp->tmp_esp < (char *) efp)
		  return (int) 0 - 1;
		efp->tmp_esp = 0;
		goto LGOTO;
	      }
	}
      }
      return efp->s1 + efp->s2;
    }
}

void
send_fib_task (struct thread_data *_thr, struct fib *pthis)
{
  send_int ((*pthis).n);
  fprintf (stderr, "send %d\n", (*pthis).n);
}

struct fib *
recv_fib_task (struct thread_data *_thr)
{
  struct fib *pthis = malloc (sizeof (struct fib));
  (*pthis).n = recv_int ();
  fprintf (stderr, "receive %d\n", (*pthis).n);
  return pthis;
}

void
send_fib_rslt (struct thread_data *_thr, struct fib *pthis)
{
  send_int ((*pthis).r);
  fprintf (stderr, "send fib(%d)=%d\n", (*pthis).n, (*pthis).r);
  free (pthis);
}

void
recv_fib_rslt (struct thread_data *_thr, struct fib *pthis)
{
  (*pthis).r = recv_int ();
  fprintf (stderr, "receive fib(%d)=%d\n", (*pthis).n, (*pthis).r);
}

void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_fib_task};
void (*task_senders[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) send_fib_task};
void *(*task_receivers[256]) (struct thread_data *) =
{
(void *(*)(struct thread_data *)) recv_fib_task};
void (*rslt_senders[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) send_fib_rslt};
void (*rslt_receivers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) recv_fib_rslt};
