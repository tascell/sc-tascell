#ifdef NOXCC
#define systhr_create(work,arg) 0
#define systhr_bind_to_proc(pid) 0
#define systhr_init(n) 0
#define atomic_read_int(x) x
#define atomic_read_ptr(x) x
#define atomic_write_int(x,y) ((x)=(y))
#define atomic_write_ptr(x,y) ((x)=(y))
#define finish_write_before_write() 0
#define start_read_after_read() 0
#endif 

typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);

#ifndef NOXCC
extern int systhr_create (void *(*start_func) (), void *arg);
extern int systhr_bind_to_proc (int pe);
extern int systhr_init (int np);
#endif

typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef int __int64_t;
typedef unsigned int __uint64_t;
typedef unsigned long __clock_t;
typedef unsigned int __cpumask_t;
typedef __int32_t __critical_t;
typedef double __double_t;
typedef double __float_t;
typedef __int32_t __intfptr_t;
typedef __int64_t __intmax_t;
typedef __int32_t __intptr_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;
typedef __int32_t __ptrdiff_t;
typedef __int32_t __register_t;
typedef __int32_t __segsz_t;
typedef __uint32_t __size_t;
typedef __int32_t __ssize_t;
typedef __int32_t __time_t;
typedef __uint32_t __uintfptr_t;
typedef __uint64_t __uintmax_t;
typedef __uint32_t __uintptr_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;
typedef __uint32_t __u_register_t;
typedef __uint32_t __vm_offset_t;
typedef __int64_t __vm_ooffset_t;
typedef __uint32_t __vm_paddr_t;
typedef __uint64_t __vm_pindex_t;
typedef __uint32_t __vm_size_t;
typedef __builtin_va_list __va_list;
typedef __va_list __gnuc_va_list;
typedef __int32_t __clockid_t;
typedef __uint32_t __fflags_t;
typedef __uint64_t __fsblkcnt_t;
typedef __uint64_t __fsfilcnt_t;
typedef __uint32_t __gid_t;
typedef __int64_t __id_t;
typedef __uint32_t __ino_t;
typedef long __key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int __nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;
typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long __suseconds_t;
typedef __int32_t __timer_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;
typedef int __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;
typedef __uint32_t __dev_t;
typedef __uint32_t __fixpt_t;

union unnamed0
{
  char __mbstate8[128];
  __int64_t _MBSTATEl;
};
typedef union unnamed0 __mbstate_t;
typedef __ptrdiff_t ptrdiff_t;
typedef __rune_t rune_t;
typedef __size_t size_t;
typedef __wchar_t wchar_t;
typedef double align_t;

struct unnamed1
{
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
};
typedef struct unnamed1 descriptor;
typedef struct unnamed1 *desc_t;

struct unnamed2
{
  int gcv;
  int gctype;
  size_t tosize;
  size_t stack_size;
  size_t limited_stack_max;
};
typedef struct unnamed2 gc_params;
typedef closure_t *sht;

extern void getmem_init (gc_params);

extern void *getmem (char *esp, sht, desc_t);

extern void *move (void *vp);

extern void gc (char *esp, sht);
extern double gc_ttime;

long nrand48 (unsigned short xseed[3]);

int strcmp (char const *s1, char const *s2);

void *memset (void *b, int c, size_t len);

void *memcpy (void *dest, void const *src, size_t len);

int atoi (char const *nptr);

int printf (char const *format, ...);

void *malloc (size_t size);

void *alloca (size_t size);

void *realloc (void *pt, size_t size);

void free (void *pt);

struct timeval
{
  long tv_sec;
  long tv_usec;
};

struct timezone
{
  int tz_minuteswest;
  int tz_dsttime;
};

int gettimeofday (struct timeval *tp, struct timezone *tzp);

#ifndef NOXCC
void *atomic_read_ptr ();

int atomic_read_int ();

void atomic_write_ptr ();

void atomic_write_int ();

void start_read_after_read ();

void finish_write_before_write ();

int cas_ptr ();

int cas_int ();

#include "xccmem-dummy.h"
//#include "xccmem.h"

#endif
typedef struct _proc_locals *proc_env;

double sqrt (double x);

struct nq_unnamed1
{
  int *result_buf_p;
  int *result_port_p;
  int n;
  int k;
  int a[20];
  int lb[40];
  int rb[40];
};

union nq_unnamed0
{
  struct nq_unnamed1 t1;
};

struct _task_buf
{
  void (*f) ();
  int sender;
  int why;
  union nq_unnamed0 a;
};
typedef struct _task_buf task_buf_t;

struct _req_buf
{
  task_buf_t *task_buf_p;
  int *task_port_p;
  void **req_port_p;
};
typedef struct _req_buf req_buf_t;

struct _proc_locals
{
  int n_proc;
  int myid;
  task_buf_t task_buf;
  int task_port;
  req_buf_t req_buf;
  void *dumy1[64];
  void *req_port;
  void *dumy2[64];
  int a[20];
  int lb[40];
  int rb[40];
  void *dumy3[64];
};
struct _proc_locals pe[64];

void
init_proc_locals (proc_env pr, int id, int num_proc)
{
  int i;

  ;
  {
    i = 0;
    if (i < 20)
      do
	{
	  ((*pr).a)[i] = i;
	  i++;
	}
      while (i < 20);
    else;
  }
  {
    i = 0;
    if (i < 40)
      do
	{
	  ((*pr).lb)[i] = ((*pr).rb)[i] = 0;
	  i++;
	}
      while (i < 40);
    else;
  }
  (*pr).n_proc = num_proc;
  (*pr).myid = id;
  (*pr).req_port = 0;
}

void
tf_do_nothing (char *esp, proc_env pr, task_buf_t * task_buf_p)
{
}

void
refuse_req (proc_env pr)
{
  void *req_buf_p = atomic_read_ptr ((*pr).req_port);

  ;
  if (req_buf_p)
    {
      req_buf_t *rbp = (req_buf_t *) req_buf_p;

      ;
      start_read_after_read ();
      atomic_write_ptr ((*pr).req_port, 0);
      (*(*rbp).task_buf_p).f = tf_do_nothing;
      finish_write_before_write ();
      atomic_write_int (*(*rbp).task_port_p, 1);
    }
  else;
}

struct steal_run_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp4;
  int tmp2;
  int tmp3;
  int tmp;
  int why;
  void **req_port_p;
  proc_env pr;
  struct _task_buf *tmp_arg;
  void (*tmp_fp) ();
};

void
steal_run_task (char *esp, proc_env pr, void **req_port_p, int why)
{
#ifndef NOXCC
  struct steal_run_task_frame *efp;
  void (*tmp_fp) ();
  struct _task_buf *tmp_arg;

lgoto:
  ;
  efp = (struct steal_run_task_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct steal_run_task_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->pr = pr;
  efp->req_port_p = req_port_p;
  efp->why = why;
  (*efp->pr).task_port = 0;
  ((*efp->pr).task_buf).why = efp->why;
  ((*efp->pr).req_buf).task_buf_p = &(*efp->pr).task_buf;
  ((*efp->pr).req_buf).task_port_p = &(*efp->pr).task_port;
  ((*efp->pr).req_buf).req_port_p = &(*efp->pr).req_port;
  finish_write_before_write ();
  efp->tmp = cas_ptr (*efp->req_port_p, 0, (void *) &(*efp->pr).req_buf);
  if (efp->tmp)
    do
      {
	refuse_req (efp->pr);
	efp->tmp2 = cas_ptr (*efp->req_port_p, 0, (void *) &(*efp->pr).req_buf);
      }
    while (efp->tmp2);
  efp->tmp3 = atomic_read_int ((*efp->pr).task_port);
  if (efp->tmp3 == 0)
    do
      {
	refuse_req (efp->pr);
	efp->tmp4 = atomic_read_int ((*efp->pr).task_port);
      }
    while (efp->tmp4 == 0);
  start_read_after_read ();
  tmp_fp = ((*efp->pr).task_buf).f;
  tmp_arg = &(*efp->pr).task_buf;
  while (tmp_fp (esp, efp->pr, tmp_arg), (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return;
      efp->tmp_esp = 0;
      goto lgoto;
    }
#endif
}
int bv;

struct helper_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tg;
  double tmp7;
  double tmp6;
  int tmp5;
  int n;
  double q;
  double r;
  proc_env pr;
  void *p;
  void **tmp_arg2;
};

void *
helper (char *esp, void *p)
{
#ifndef NOXCC
  struct helper_frame *efp;
  void **tmp_arg2;

lgoto:
  ;
  efp = (struct helper_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct helper_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->p = p;
  efp->pr = (struct _proc_locals *) efp->p;
  systhr_bind_to_proc ((*efp->pr).myid);
  systhr_bind_to_proc ((*efp->pr).myid);
  do
    {
      efp->n = bv;
      efp->tmp5 = cas_int (bv, efp->n, efp->n + 1);
    }
  while (efp->tmp5);
  {
    efp->tmp6 = sqrt (0.5 + (*efp->pr).myid);
    efp->r = efp->tmp6;
    efp->tmp7 = sqrt (efp->r + (*efp->pr).myid);
    efp->q = efp->tmp7;
    efp->r -= (int) efp->r;
    if (0)
      do
	{
	  efp->tg = (int) ((*efp->pr).n_proc * efp->r);
	  if (efp->tg != (*efp->pr).myid)
	    {
	      tmp_arg2 = &(pe[efp->tg]).req_port;
	      while (steal_run_task (esp, efp->pr, tmp_arg2, 0),
		     (efp->tmp_esp = *((char **) esp)) != 0)
		{
		  if ((char *) efp->tmp_esp < (char *) efp)
		    return (void *) 0 - 1;
		  efp->tmp_esp = 0;
		  goto lgoto;
		}
	    }
	  efp->r = efp->r * 3.0 + efp->q;
	  efp->r -= (int) efp->r;
	}
      while (0);
  }
#endif
}

struct init_parallel_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp9;
  int tmp8;
  int i;
  int num_proc;
};

void
init_parallel (char *esp, int num_proc)
{
  struct init_parallel_frame *efp;

lgoto:
  ;
  efp = (struct init_parallel_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct init_parallel_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->num_proc = num_proc;
  {
    efp->i = 0;
    if (efp->i < efp->num_proc)
      do
	{
	  init_proc_locals (pe + efp->i, efp->i, efp->num_proc);
	  (efp->i)++;
	}
      while (efp->i < efp->num_proc);
  }
  systhr_init (efp->num_proc);
  systhr_bind_to_proc (0);
  bv = 1;
  {
    efp->i = 1;
    if (efp->i < efp->num_proc)
      do
	{
	  systhr_create (helper, pe + efp->i);
	  (efp->i)++;
	}
      while (efp->i < efp->num_proc);
  }
  efp->tmp8 = atomic_read_int (bv);
  if (efp->tmp8 < efp->num_proc)
    do
      {
	start_read_after_read ();
	efp->tmp9 = atomic_read_int (bv);
      }
    while (efp->tmp9 < efp->num_proc);
}
struct timeval tv0;

int nqueens (char *esp, proc_env pr, closure_t * bkf, int n, int k);

struct tf_backup_and_nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int s;
  int k;
  int n;
  task_buf_t *tbp;
  int *saved_result_buf_p;
  int *saved_result_port_p;
  int jj;
  int rb_bak[40];
  int lb_bak[40];
  int a_bak[20];
  task_buf_t *task_buf_p;
  proc_env pr;
  closure_t bk;
};

struct bk_in_tf_backup_and_nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  req_buf_t *req_buf_p;
  struct tf_backup_and_nqueens_frame *xfp;
};

char *
bk_in_tf_backup_and_nqueens (char *esp, void *xfp0)
{
  struct bk_in_tf_backup_and_nqueens_frame *efp;
  struct tf_backup_and_nqueens_frame *xfp = xfp0;
  char *parmp = (char *) esp;

lgoto:
  ;
  efp = (struct bk_in_tf_backup_and_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk_in_tf_backup_and_nqueens_frame) +
		 sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  efp->req_buf_p = (parmp =
		    (char *) ((Align_t *) parmp -
			      (sizeof (req_buf_t *) + sizeof (Align_t) +
			       -1) / sizeof (Align_t)),
		    *((req_buf_t **) parmp));
  *((int *) efp) = 0;
  return 0;
  return 0;
}

void
tf_backup_and_nqueens (char *esp, proc_env pr, task_buf_t * task_buf_p)
{
  struct tf_backup_and_nqueens_frame *efp;

lgoto:
  ;
  efp = (struct tf_backup_and_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct tf_backup_and_nqueens_frame) +
		 sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->pr = pr;
  efp->task_buf_p = task_buf_p;
  efp->bk.fun = bk_in_tf_backup_and_nqueens;
  efp->bk.fr = (void *) efp;
  efp->tbp = efp->task_buf_p;
  {
    efp->jj = 0;
    if (efp->jj < 20)
      do
	{
	  (efp->a_bak)[efp->jj] = ((*efp->pr).a)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 20);
  }
  {
    efp->jj = 0;
    if (efp->jj < 40)
      do
	{
	  (efp->lb_bak)[efp->jj] = ((*efp->pr).lb)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 40);
  }
  {
    efp->jj = 0;
    if (efp->jj < 40)
      do
	{
	  (efp->rb_bak)[efp->jj] = ((*efp->pr).rb)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 40);
  }
  efp->saved_result_port_p = (((*efp->tbp).a).t1).result_port_p;
  efp->saved_result_buf_p = (((*efp->tbp).a).t1).result_buf_p;
  {
    efp->jj = 0;
    if (efp->jj < 20)
      do
	{
	  ((*efp->pr).a)[efp->jj] = ((((*efp->tbp).a).t1).a)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 20);
  }
  {
    efp->jj = 0;
    if (efp->jj < 40)
      do
	{
	  ((*efp->pr).lb)[efp->jj] = ((((*efp->tbp).a).t1).lb)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 40);
  }
  {
    efp->jj = 0;
    if (efp->jj < 40)
      do
	{
	  ((*efp->pr).rb)[efp->jj] = ((((*efp->tbp).a).t1).rb)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 40);
  }
  {
    efp->n = (((*efp->tbp).a).t1).n;
    efp->k = (((*efp->tbp).a).t1).k;
    efp->s = 0;
    while ((efp->s =
	    nqueens (esp, efp->pr, &efp->bk, efp->n, efp->k)) == (int) 0 - 1
	   && (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return;
	efp->tmp_esp = 0;
	goto lgoto;
      }
    *efp->saved_result_buf_p = efp->s;
  }
  finish_write_before_write ();
  atomic_write_int (*efp->saved_result_port_p, 1);
  {
    efp->jj = 0;
    if (efp->jj < 20)
      do
	{
	  ((*efp->pr).a)[efp->jj] = (efp->a_bak)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 20);
  }
  {
    efp->jj = 0;
    if (efp->jj < 40)
      do
	{
	  ((*efp->pr).lb)[efp->jj] = (efp->lb_bak)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 40);
  }
  {
    efp->jj = 0;
    if (efp->jj < 40)
      do
	{
	  ((*efp->pr).rb)[efp->jj] = (efp->rb_bak)[efp->jj];
	  (efp->jj)++;
	}
      while (efp->jj < 40);
  }
}

struct nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp13;
  int tmp12;
  int i3;
  int tmp11;
  int ak;
  int ai;
  int tmp10;
  req_buf_t *rbp;
  void *req_buf_p;
  int i2;
  int result_port[30];
  int result_buf[30];
  void **saved_req_port_p[30];
  int ret;
  int ei;
  int si;
  int s;
  int k;
  int n;
  closure_t *bkf;
  proc_env pr;
  void **tmp_arg4;
  int tmp_arg3;
  closure_t bk2;
  closure_t bk;
};

struct bk_in_nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int *tpp;
  task_buf_t *tbp;
  int jj;
  int ak;
  int ai;
  int i;
  req_buf_t *req_buf_p;
  struct nqueens_frame *xfp;
};

char *
bk_in_nqueens (char *esp, void *xfp0)
{
  struct bk_in_nqueens_frame *efp;
  struct nqueens_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
  closure_t *tmp_fp2;

lgoto:
  ;
  efp = (struct bk_in_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk_in_nqueens_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  efp->req_buf_p = (parmp =
		    (char *) ((Align_t *) parmp -
			      (sizeof (req_buf_t *) + sizeof (Align_t) +
			       -1) / sizeof (Align_t)),
		    *((req_buf_t **) parmp));
  {
    if (xfp->ret)
      {
	char *goto_fr;

	tmp_fp2 = xfp->bkf;
	argp =
	    (char *) ((Align_t *) esp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((struct _req_buf **) argp) = efp->req_buf_p, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (struct _req_buf *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((closure_t **) argp) = tmp_fp2;
	goto_fr = lw_call (argp);
	if (goto_fr)
	  if ((char *) goto_fr < (char *) efp)
	    return goto_fr;
	  else
	    {
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	xfp->ret = *((int *) argp);
      }
    if (xfp->ret)
      {
	*((int *) efp) = 1;
	return 0;
      }
    if (xfp->si < xfp->ei)
      do
	{
	  efp->i = --xfp->ei;
	  efp->ai = ((*xfp->pr).a)[efp->i];
	  if (!
	      (((*xfp->pr).lb)[xfp->n - 1 - efp->ai +
			       xfp->k] ? 1 : (((*xfp->pr).rb)[efp->ai +
							      xfp->k] ? 1 : 0)))
	    {
	      efp->ak = ((*xfp->pr).a)[xfp->k];
	      ((*xfp->pr).a)[efp->i] = efp->ak;
	      ((*xfp->pr).a)[xfp->k] = efp->ai;
	      ((*xfp->pr).lb)[xfp->n - 1 - efp->ai + xfp->k] = 1;
	      ((*xfp->pr).rb)[efp->ai + xfp->k] = 1;
	      {
		efp->tbp = (*efp->req_buf_p).task_buf_p;
		efp->tpp = (*efp->req_buf_p).task_port_p;
		(xfp->saved_req_port_p)[efp->i] = (*efp->req_buf_p).req_port_p;
		(xfp->result_port)[efp->i] = 0;
		(*efp->tbp).f = tf_backup_and_nqueens;
		(*efp->tbp).sender = (*xfp->pr).myid;
		(((*efp->tbp).a).t1).result_buf_p = xfp->result_buf + efp->i;
		(((*efp->tbp).a).t1).result_port_p = xfp->result_port + efp->i;
		(((*efp->tbp).a).t1).n = xfp->n;
		(((*efp->tbp).a).t1).k = xfp->k + 1;
		{
		  efp->jj = 0;
		  if (efp->jj < 20)
		    do
		      {
			((((*efp->tbp).a).t1).a)[efp->jj] =
			    ((*xfp->pr).a)[efp->jj];
			(efp->jj)++;
		      }
		    while (efp->jj < 20);
		}
		{
		  efp->jj = 0;
		  if (efp->jj < 40)
		    do
		      {
			((((*efp->tbp).a).t1).lb)[efp->jj] =
			    ((*xfp->pr).lb)[efp->jj];
			(efp->jj)++;
		      }
		    while (efp->jj < 40);
		}
		{
		  efp->jj = 0;
		  if (efp->jj < 40)
		    do
		      {
			((((*efp->tbp).a).t1).rb)[efp->jj] =
			    ((*xfp->pr).rb)[efp->jj];
			(efp->jj)++;
		      }
		    while (efp->jj < 40);
		}
		finish_write_before_write ();
		atomic_write_int (*efp->tpp, 1);
	      }
	      efp->ai = ((*xfp->pr).a)[xfp->k];
	      efp->ak = ((*xfp->pr).a)[efp->i];
	      ((*xfp->pr).a)[efp->i] = efp->ai;
	      ((*xfp->pr).a)[xfp->k] = efp->ak;
	      ((*xfp->pr).lb)[xfp->n - 1 - efp->ai + xfp->k] = 0;
	      ((*xfp->pr).rb)[efp->ai + xfp->k] = 0;
	      *((int *) efp) = 1;
	      return 0;
	    }
	  else
	    {
	      (xfp->result_port)[efp->i] = 1;
	      (xfp->result_buf)[efp->i] = 0;
	    }
	}
      while (xfp->si < xfp->ei);
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

struct bk2_in_nqueens_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int r;
  req_buf_t *req_buf_p;
  struct nqueens_frame *xfp;
};

char *
bk2_in_nqueens (char *esp, void *xfp0)
{
  struct bk2_in_nqueens_frame *efp;
  struct nqueens_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
  closure_t *tmp_fp4;

lgoto:
  ;
  efp = (struct bk2_in_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk2_in_nqueens_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  efp->req_buf_p = (parmp =
		    (char *) ((Align_t *) parmp -
			      (sizeof (req_buf_t *) + sizeof (Align_t) +
			       -1) / sizeof (Align_t)),
		    *((req_buf_t **) parmp));
  xfp->ai = ((*xfp->pr).a)[xfp->k];
  xfp->ak = ((*xfp->pr).a)[xfp->i2];
  ((*xfp->pr).a)[xfp->i2] = xfp->ai;
  ((*xfp->pr).a)[xfp->k] = xfp->ak;
  ((*xfp->pr).lb)[xfp->n - 1 - xfp->ai + xfp->k] = 0;
  ((*xfp->pr).rb)[xfp->ai + xfp->k] = 0;
  {
    char *goto_fr;

    tmp_fp4 = &xfp->bk;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((struct _req_buf **) argp) = efp->req_buf_p, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (struct _req_buf *) + sizeof (Align_t) +
		   -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp4;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    efp->r = *((int *) argp);
  }
  xfp->ai = ((*xfp->pr).a)[xfp->i2];
  xfp->ak = ((*xfp->pr).a)[xfp->k];
  ((*xfp->pr).a)[xfp->i2] = xfp->ak;
  ((*xfp->pr).a)[xfp->k] = xfp->ai;
  ((*xfp->pr).lb)[xfp->n - 1 - xfp->ai + xfp->k] = 1;
  ((*xfp->pr).rb)[xfp->ai + xfp->k] = 1;
  *((int *) efp) = efp->r;
  return 0;
  return 0;
}

int
nqueens (char *esp, proc_env pr, closure_t * bkf, int n, int k)
{
  char *argp;
  struct nqueens_frame *efp;
  closure_t *tmp_fp3;
  int tmp_arg3;
  void **tmp_arg4;

lgoto:
  ;
  efp = (struct nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct nqueens_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->pr = pr;
  efp->bkf = bkf;
  efp->n = n;
  efp->k = k;
  efp->bk2.fun = bk2_in_nqueens;
  efp->bk2.fr = (void *) efp;
  efp->bk.fun = bk_in_nqueens;
  efp->bk.fr = (void *) efp;
  if (efp->n == efp->k)
    {
      return 1;
    }
  else
    {
      efp->s = 0;
      efp->si = efp->k;
      efp->ei = efp->n;
      efp->ret = 1;
      if (efp->si < efp->ei)
	do
	  {
	    efp->i2 = (efp->si)++;
	    do
	      {
		efp->req_buf_p = atomic_read_ptr ((*efp->pr).req_port);
		if (efp->req_buf_p)
		  {
		    efp->rbp = (req_buf_t *) efp->req_buf_p;
		    start_read_after_read ();
		    atomic_write_ptr ((*efp->pr).req_port, 0);
		    {
		      char *goto_fr;

		      tmp_fp3 = &efp->bk;
		      argp =
			  (char *) ((Align_t *) esp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((struct _req_buf **) argp) = efp->rbp, argp =
			  (char *) ((Align_t *) argp +
				    (sizeof (struct _req_buf *) +
				     sizeof (Align_t) + -1) / sizeof (Align_t));
		      *((closure_t **) argp) = tmp_fp3;
		      goto_fr = lw_call (argp);
		      if (goto_fr)
			if ((char *) goto_fr < (char *) efp)
			  {
			    efp->tmp_esp = goto_fr;
			    return (int) 0 - 1;
			  }
			else
			  {
			    efp->tmp_esp = 0;
			    goto lgoto;
			  }
		      efp->tmp10 = *((int *) argp);
		    }
		    if (efp->tmp10 == 0)
		      {
			(*(*efp->rbp).task_buf_p).f = tf_do_nothing;
			finish_write_before_write ();
			atomic_write_int (*(*efp->rbp).task_port_p, 1);
		      }
		  }
	      }
	    while (0);
	    {
	      efp->ai = ((*efp->pr).a)[efp->i2];
	      if (!
		  (((*efp->pr).lb)[efp->n - 1 - efp->ai +
				   efp->k] ? 1 : (((*efp->pr).rb)[efp->ai +
								  efp->
								  k] ? 1 : 0)))
		{
		  efp->ak = ((*efp->pr).a)[efp->k];
		  ((*efp->pr).a)[efp->i2] = efp->ak;
		  ((*efp->pr).a)[efp->k] = efp->ai;
		  ((*efp->pr).lb)[efp->n - 1 - efp->ai + efp->k] = 1;
		  ((*efp->pr).rb)[efp->ai + efp->k] = 1;
		  if (efp->n > efp->k + 1)
		    {
		      tmp_arg3 = efp->k + 1;
		      while ((efp->tmp11 =
			      nqueens (esp, efp->pr, &efp->bk2, efp->n,
				       tmp_arg3)) == (int) 0 - 1
			     && (efp->tmp_esp = *((char **) esp)) != 0)
			{
			  if ((char *) efp->tmp_esp < (char *) efp)
			    return (int) 0 - 1;
			  efp->tmp_esp = 0;
			  goto lgoto;
			}
		      efp->s += efp->tmp11;
		    }
		  else
		    {
		      (efp->s)++;
		    }
		  efp->ai = ((*efp->pr).a)[efp->k];
		  efp->ak = ((*efp->pr).a)[efp->i2];
		  ((*efp->pr).a)[efp->i2] = efp->ai;
		  ((*efp->pr).a)[efp->k] = efp->ak;
		  ((*efp->pr).lb)[efp->n - 1 - efp->ai + efp->k] = 0;
		  ((*efp->pr).rb)[efp->ai + efp->k] = 0;
		}
	    }
	  }
	while (efp->si < efp->ei);
      if (efp->si < efp->n)
	do
	  {
	    efp->i3 = (efp->si)++;
	    efp->tmp12 = atomic_read_int ((efp->result_port)[efp->i3]);
	    if (efp->tmp12 == 0)
	      do
		{
		  tmp_arg4 = (efp->saved_req_port_p)[efp->i3];
		  while (steal_run_task (esp, efp->pr, tmp_arg4, 1),
			 (efp->tmp_esp = *((char **) esp)) != 0)
		    {
		      if ((char *) efp->tmp_esp < (char *) efp)
			return (int) 0 - 1;
		      efp->tmp_esp = 0;
		      goto lgoto;
		    }
		  efp->tmp13 = atomic_read_int ((efp->result_port)[efp->i3]);
		}
	      while (efp->tmp13 == 0);
	    start_read_after_read ();
	    efp->s += (efp->result_buf)[efp->i3];
	  }
	while (efp->si < efp->n);
      return efp->s;
    }
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int num_proc;
  int s;
  int n;
  int i;
  struct timeval tv[2];
  char **argv;
  int argc;
  struct _proc_locals *tmp_arg5;
  closure_t bk;
};

struct bk_in_main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  req_buf_t *req_buf_p;
  struct main_frame *xfp;
};

char *
bk_in_main (char *esp, void *xfp0)
{
  struct bk_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  char *parmp = (char *) esp;

lgoto:
  ;
  efp = (struct bk_in_main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk_in_main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  efp->req_buf_p = (parmp =
		    (char *) ((Align_t *) parmp -
			      (sizeof (req_buf_t *) + sizeof (Align_t) +
			       -1) / sizeof (Align_t)),
		    *((req_buf_t **) parmp));
  *((int *) efp) = 0;
  return 0;
  return 0;
}

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char estack[65536];
  char *esp = estack;
  struct _proc_locals *tmp_arg5;

lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->argc = argc;
  efp->argv = argv;
  efp->bk.fun = bk_in_main;
  efp->bk.fr = (void *) efp;
  efp->num_proc = 2;
  if (efp->argc > 1)
    {
      efp->num_proc = atoi ((efp->argv)[1]);
    }
  if (efp->argc > 2)
    {
      efp->n = atoi ((efp->argv)[2]);
    }
  while (init_parallel (esp, efp->num_proc),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return (int) 0 - 1;
      efp->tmp_esp = 0;
      goto lgoto;
    }
  gettimeofday (efp->tv + 0, 0);
  tv0 = (efp->tv)[0];
  tmp_arg5 = pe + 0;
  while ((efp->s = nqueens (esp, tmp_arg5, &efp->bk, efp->n, 0)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return (int) 0 - 1;
      efp->tmp_esp = 0;
      goto lgoto;
    }
  gettimeofday (efp->tv + 1, 0);
  printf ("answers: %d\n", efp->s);
  printf ("%lf\n",
	  ((efp->tv)[1]).tv_sec - ((efp->tv)[0]).tv_sec +
	  (((efp->tv)[1]).tv_usec - ((efp->tv)[0]).tv_usec) * 1.0e-6);
}
