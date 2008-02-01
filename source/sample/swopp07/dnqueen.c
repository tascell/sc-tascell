typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);

extern int systhr_create (void *(*start_func) (), void *arg);

extern int systhr_bind_to_proc (int pe);

extern int systhr_init (int np);
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

void *atomic_read_ptr ();

int atomic_read_int ();

void atomic_write_ptr ();

void atomic_write_int ();

void start_read_after_read ();

void finish_write_before_write ();

int cas_ptr ();

int cas_int ();

#include "xccmem.h"
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
  struct steal_run_task_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int tmp;
  int tmp3;
  int tmp2;
  int tmp4;
  void (*tmp_fp) ();
  struct _task_buf *tmp_arg;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct steal_run_task_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct steal_run_task_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL;
	}
      goto L_CALL;
    }
  efp = (struct steal_run_task_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct steal_run_task_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  (*pr).task_port = 0;
  ((*pr).task_buf).why = why;
  ((*pr).req_buf).task_buf_p = &(*pr).task_buf;
  ((*pr).req_buf).task_port_p = &(*pr).task_port;
  ((*pr).req_buf).req_port_p = &(*pr).req_port;
  finish_write_before_write ();
  tmp = cas_ptr (*req_port_p, 0, (void *) &(*pr).req_buf);
  if (tmp)
    do
      {
	refuse_req (pr);
	tmp2 = cas_ptr (*req_port_p, 0, (void *) &(*pr).req_buf);
      }
    while (tmp2);
  tmp3 = atomic_read_int ((*pr).task_port);
  if (tmp3 == 0)
    do
      {
	refuse_req (pr);
	tmp4 = atomic_read_int ((*pr).task_port);
      }
    while (tmp4 == 0);
  start_read_after_read ();
  new_esp = esp;
  tmp_fp = ((*pr).task_buf).f;
  tmp_arg = &(*pr).task_buf;
  while (tmp_fp (new_esp, pr, tmp_arg), (efp->tmp_esp = *((char **) esp)) != 0)
    {
      efp->tmp4 = tmp4;
      efp->tmp2 = tmp2;
      efp->tmp3 = tmp3;
      efp->tmp = tmp;
      efp->why = why;
      efp->req_port_p = req_port_p;
      efp->pr = pr;
      efp->call_id = 0;
      return;
    L_CALL:
      ;
      tmp4 = efp->tmp4;
      tmp2 = efp->tmp2;
      tmp3 = efp->tmp3;
      tmp = efp->tmp;
      why = efp->why;
      req_port_p = efp->req_port_p;
      pr = efp->pr;
      tmp_fp = efp->tmp_fp;
      tmp_arg = efp->tmp_arg;
      new_esp = esp + 1;
    }
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
  struct helper_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  proc_env pr;
  double r;
  double q;
  int n;
  int tmp5;
  double tmp6;
  double tmp7;
  int tg;
  void **tmp_arg2;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct helper_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct helper_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL2;
	}
      goto L_CALL2;
    }
  efp = (struct helper_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct helper_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  pr = (struct _proc_locals *) p;
  systhr_bind_to_proc ((*pr).myid);
  systhr_bind_to_proc ((*pr).myid);
  do
    {
      n = bv;
      tmp5 = cas_int (bv, n, n + 1);
    }
  while (tmp5);
  {
    tmp6 = sqrt (0.5 + (*pr).myid);
    r = tmp6;
    tmp7 = sqrt (r + (*pr).myid);
    q = tmp7;
    r -= (int) r;
    if (0)
      do
	{
	  tg = (int) ((*pr).n_proc * r);
	  if (tg != (*pr).myid)
	    {
	      new_esp = esp;
	      tmp_arg2 = &(pe[tg]).req_port;
	      while (steal_run_task (new_esp, pr, tmp_arg2, 0),
		     (efp->tmp_esp = *((char **) esp)) != 0)
		{
		  efp->tg = tg;
		  efp->tmp7 = tmp7;
		  efp->tmp6 = tmp6;
		  efp->tmp5 = tmp5;
		  efp->n = n;
		  efp->q = q;
		  efp->r = r;
		  efp->pr = pr;
		  efp->p = p;
		  efp->call_id = 0;
		  return (void *) 0 - 1;
		L_CALL2:
		  ;
		  tg = efp->tg;
		  tmp7 = efp->tmp7;
		  tmp6 = efp->tmp6;
		  tmp5 = efp->tmp5;
		  n = efp->n;
		  q = efp->q;
		  r = efp->r;
		  pr = efp->pr;
		  p = efp->p;
		  tmp_arg2 = efp->tmp_arg2;
		  new_esp = esp + 1;
		}
	    }
	  r = r * 3.0 + q;
	  r -= (int) r;
	}
      while (0);
  }
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
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int i;
  int tmp8;
  int tmp9;

lgoto:
  ;
  efp = (struct init_parallel_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct init_parallel_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    i = 0;
    if (i < num_proc)
      do
	{
	  init_proc_locals (pe + i, i, num_proc);
	  i++;
	}
      while (i < num_proc);
  }
  systhr_init (num_proc);
  systhr_bind_to_proc (0);
  bv = 1;
  {
    i = 1;
    if (i < num_proc)
      do
	{
	  systhr_create (helper, pe + i);
	  i++;
	}
      while (i < num_proc);
  }
  tmp8 = atomic_read_int (bv);
  if (tmp8 < num_proc)
    do
      {
	start_read_after_read ();
	tmp9 = atomic_read_int (bv);
      }
    while (tmp9 < num_proc);
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
  char *new_esp;
  struct bk_in_tf_backup_and_nqueens_frame *efp;
  struct tf_backup_and_nqueens_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  req_buf_t *req_buf_p = (parmp =
			  (char *) ((Align_t *) parmp -
				    (sizeof (req_buf_t *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t)),
			  *((req_buf_t **) parmp));
lgoto:
  ;
  efp = (struct bk_in_tf_backup_and_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk_in_tf_backup_and_nqueens_frame) +
		 sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  *((int *) efp) = 0;
  return 0;
  return 0;
}

void
tf_backup_and_nqueens (char *esp, proc_env pr, task_buf_t * task_buf_p)
{
  struct tf_backup_and_nqueens_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int jj;
  int *saved_result_port_p;
  int *saved_result_buf_p;
  task_buf_t *tbp;
  int n;
  int k;
  int s;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct tf_backup_and_nqueens_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct tf_backup_and_nqueens_frame) +
		     sizeof (Align_t) + -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL3;
	}
      goto L_CALL3;
    }
  efp = (struct tf_backup_and_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct tf_backup_and_nqueens_frame) +
		 sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  tbp = task_buf_p;
  {
    jj = 0;
    if (jj < 20)
      do
	{
	  (efp->a_bak)[jj] = ((*pr).a)[jj];
	  jj++;
	}
      while (jj < 20);
  }
  {
    jj = 0;
    if (jj < 40)
      do
	{
	  (efp->lb_bak)[jj] = ((*pr).lb)[jj];
	  jj++;
	}
      while (jj < 40);
  }
  {
    jj = 0;
    if (jj < 40)
      do
	{
	  (efp->rb_bak)[jj] = ((*pr).rb)[jj];
	  jj++;
	}
      while (jj < 40);
  }
  saved_result_port_p = (((*tbp).a).t1).result_port_p;
  saved_result_buf_p = (((*tbp).a).t1).result_buf_p;
  {
    jj = 0;
    if (jj < 20)
      do
	{
	  ((*pr).a)[jj] = ((((*tbp).a).t1).a)[jj];
	  jj++;
	}
      while (jj < 20);
  }
  {
    jj = 0;
    if (jj < 40)
      do
	{
	  ((*pr).lb)[jj] = ((((*tbp).a).t1).lb)[jj];
	  jj++;
	}
      while (jj < 40);
  }
  {
    jj = 0;
    if (jj < 40)
      do
	{
	  ((*pr).rb)[jj] = ((((*tbp).a).t1).rb)[jj];
	  jj++;
	}
      while (jj < 40);
  }
  {
    n = (((*tbp).a).t1).n;
    k = (((*tbp).a).t1).k;
    s = 0;
    new_esp = esp;
    while ((s = nqueens (new_esp, pr, &efp->bk, n, k)) == (int) 0 - 1
	   && (efp->tmp_esp = *((char **) esp)) != 0)
      {
	efp->s = s;
	efp->k = k;
	efp->n = n;
	efp->tbp = tbp;
	efp->saved_result_buf_p = saved_result_buf_p;
	efp->saved_result_port_p = saved_result_port_p;
	efp->jj = jj;
	efp->task_buf_p = task_buf_p;
	efp->pr = pr;
	efp->bk.fun = bk_in_tf_backup_and_nqueens;
	efp->bk.fr = (void *) efp;
	efp->call_id = 0;
	return;
      L_CALL3:
	;
	s = efp->s;
	k = efp->k;
	n = efp->n;
	tbp = efp->tbp;
	saved_result_buf_p = efp->saved_result_buf_p;
	saved_result_port_p = efp->saved_result_port_p;
	jj = efp->jj;
	task_buf_p = efp->task_buf_p;
	pr = efp->pr;
	new_esp = esp + 1;
      }
    *saved_result_buf_p = s;
  }
  finish_write_before_write ();
  atomic_write_int (*saved_result_port_p, 1);
  {
    jj = 0;
    if (jj < 20)
      do
	{
	  ((*pr).a)[jj] = (efp->a_bak)[jj];
	  jj++;
	}
      while (jj < 20);
  }
  {
    jj = 0;
    if (jj < 40)
      do
	{
	  ((*pr).lb)[jj] = (efp->lb_bak)[jj];
	  jj++;
	}
      while (jj < 40);
  }
  {
    jj = 0;
    if (jj < 40)
      do
	{
	  ((*pr).rb)[jj] = (efp->rb_bak)[jj];
	  jj++;
	}
      while (jj < 40);
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
  char *new_esp;
  struct bk_in_nqueens_frame *efp;
  struct nqueens_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  req_buf_t *req_buf_p = (parmp =
			  (char *) ((Align_t *) parmp -
				    (sizeof (req_buf_t *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t)),
			  *((req_buf_t **) parmp));
  char *argp;
  closure_t *tmp_fp2;
  int i;
  int jj;
  task_buf_t *tbp;
  int *tpp;

lgoto:
  ;
  efp = (struct bk_in_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk_in_nqueens_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    if (xfp->ret)
      {
	char *goto_fr;

	tmp_fp2 = xfp->bkf;
	argp =
	    (char *) ((Align_t *) esp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((struct _req_buf **) argp) = req_buf_p, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (struct _req_buf *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((closure_t **) argp) = tmp_fp2;
	efp->req_buf_p = req_buf_p;
	efp->xfp = xfp;
	goto_fr = lw_call (argp);
	if (goto_fr)
	  if ((char *) goto_fr < (char *) efp)
	    return goto_fr;
	  else
	    {
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	req_buf_p = efp->req_buf_p;
	xfp = efp->xfp;
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
	  i = --xfp->ei;
	  efp->ai = ((*xfp->pr).a)[i];
	  if (!
	      (((*xfp->pr).lb)[xfp->n - 1 - efp->ai +
			       xfp->k] ? 1 : (((*xfp->pr).rb)[efp->ai +
							      xfp->k] ? 1 : 0)))
	    {
	      efp->ak = ((*xfp->pr).a)[xfp->k];
	      ((*xfp->pr).a)[i] = efp->ak;
	      ((*xfp->pr).a)[xfp->k] = efp->ai;
	      ((*xfp->pr).lb)[xfp->n - 1 - efp->ai + xfp->k] = 1;
	      ((*xfp->pr).rb)[efp->ai + xfp->k] = 1;
	      {
		tbp = (*req_buf_p).task_buf_p;
		tpp = (*req_buf_p).task_port_p;
		(xfp->saved_req_port_p)[i] = (*req_buf_p).req_port_p;
		(xfp->result_port)[i] = 0;
		(*tbp).f = tf_backup_and_nqueens;
		(*tbp).sender = (*xfp->pr).myid;
		(((*tbp).a).t1).result_buf_p = xfp->result_buf + i;
		(((*tbp).a).t1).result_port_p = xfp->result_port + i;
		(((*tbp).a).t1).n = xfp->n;
		(((*tbp).a).t1).k = xfp->k + 1;
		{
		  jj = 0;
		  if (jj < 20)
		    do
		      {
			((((*tbp).a).t1).a)[jj] = ((*xfp->pr).a)[jj];
			jj++;
		      }
		    while (jj < 20);
		}
		{
		  jj = 0;
		  if (jj < 40)
		    do
		      {
			((((*tbp).a).t1).lb)[jj] = ((*xfp->pr).lb)[jj];
			jj++;
		      }
		    while (jj < 40);
		}
		{
		  jj = 0;
		  if (jj < 40)
		    do
		      {
			((((*tbp).a).t1).rb)[jj] = ((*xfp->pr).rb)[jj];
			jj++;
		      }
		    while (jj < 40);
		}
		finish_write_before_write ();
		atomic_write_int (*tpp, 1);
	      }
	      *(&efp->ai) = ((*xfp->pr).a)[xfp->k];
	      *(&efp->ak) = ((*xfp->pr).a)[i];
	      ((*xfp->pr).a)[i] = efp->ai;
	      ((*xfp->pr).a)[xfp->k] = efp->ak;
	      ((*xfp->pr).lb)[xfp->n - 1 - efp->ai + xfp->k] = 0;
	      ((*xfp->pr).rb)[efp->ai + xfp->k] = 0;
	      *((int *) efp) = 1;
	      return 0;
	    }
	  else
	    {
	      (xfp->result_port)[i] = 1;
	      (xfp->result_buf)[i] = 0;
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
  char *new_esp;
  struct bk2_in_nqueens_frame *efp;
  struct nqueens_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  req_buf_t *req_buf_p = (parmp =
			  (char *) ((Align_t *) parmp -
				    (sizeof (req_buf_t *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t)),
			  *((req_buf_t **) parmp));
  char *argp;
  int r;
  closure_t *tmp_fp4;

lgoto:
  ;
  efp = (struct bk2_in_nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk2_in_nqueens_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  *(&xfp->ai) = ((*xfp->pr).a)[xfp->k];
  *(&xfp->ak) = ((*xfp->pr).a)[xfp->i2];
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
    *((struct _req_buf **) argp) = req_buf_p, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (struct _req_buf *) + sizeof (Align_t) +
		   -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp4;
    efp->r = r;
    efp->req_buf_p = req_buf_p;
    efp->xfp = xfp;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    r = efp->r;
    req_buf_p = efp->req_buf_p;
    xfp = efp->xfp;
    r = *((int *) argp);
  }
  xfp->ai = ((*xfp->pr).a)[xfp->i2];
  xfp->ak = ((*xfp->pr).a)[xfp->k];
  ((*xfp->pr).a)[xfp->i2] = xfp->ak;
  ((*xfp->pr).a)[xfp->k] = xfp->ai;
  ((*xfp->pr).lb)[xfp->n - 1 - xfp->ai + xfp->k] = 1;
  ((*xfp->pr).rb)[xfp->ai + xfp->k] = 1;
  *((int *) efp) = r;
  return 0;
  return 0;
}

int
nqueens (char *esp, proc_env pr, closure_t * bkf, int n, int k)
{
  char *argp;
  struct nqueens_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int s;
  int si;
  int ei;
  int ret;
  int i2;
  void *req_buf_p;
  req_buf_t *rbp;
  int tmp10;
  closure_t *tmp_fp3;
  int tmp11;
  int tmp_arg3;
  int i3;
  int tmp12;
  int tmp13;
  void **tmp_arg4;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct nqueens_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct nqueens_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL4;
	case 1:
	  goto L_CALL5;
	case 2:
	  goto L_CALL6;
	}
      goto L_CALL4;
    }
  efp = (struct nqueens_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct nqueens_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (n == k)
    {
      return 1;
    }
  else
    {
      s = 0;
      si = k;
      ei = n;
      ret = 1;
      if (si < ei)
	do
	  {
	    i2 = si++;
	    do
	      {
		req_buf_p = atomic_read_ptr ((*pr).req_port);
		if (req_buf_p)
		  {
		    rbp = (req_buf_t *) req_buf_p;
		    start_read_after_read ();
		    atomic_write_ptr ((*pr).req_port, 0);
		    {
		      tmp_fp3 = &efp->bk;
		      argp =
			  (char *) ((Align_t *) esp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((struct _req_buf **) argp) = rbp, argp =
			  (char *) ((Align_t *) argp +
				    (sizeof (struct _req_buf *) +
				     sizeof (Align_t) + -1) / sizeof (Align_t));
		      *((closure_t **) argp) = tmp_fp3;
		      efp->bk.fun = bk_in_nqueens;
		      efp->bk.fr = (void *) efp;
		      efp->tmp10 = tmp10;
		      efp->rbp = rbp;
		      efp->req_buf_p = req_buf_p;
		      efp->i2 = i2;
		      efp->ret = ret;
		      efp->ei = ei;
		      efp->si = si;
		      efp->s = s;
		      efp->k = k;
		      efp->n = n;
		      efp->bkf = bkf;
		      efp->pr = pr;
		      efp->argp = argp;
		      efp->tmp_esp = argp;
		      efp->call_id = 0;
		      return (int) 0 - 1;
		    L_CALL4:
		      ;
		      tmp10 = efp->tmp10;
		      rbp = efp->rbp;
		      req_buf_p = efp->req_buf_p;
		      i2 = efp->i2;
		      ret = efp->ret;
		      ei = efp->ei;
		      si = efp->si;
		      s = efp->s;
		      k = efp->k;
		      n = efp->n;
		      bkf = efp->bkf;
		      pr = efp->pr;
		      tmp10 = *((int *) efp->argp);
		    }
		    if (tmp10 == 0)
		      {
			(*(*rbp).task_buf_p).f = tf_do_nothing;
			finish_write_before_write ();
			atomic_write_int (*(*rbp).task_port_p, 1);
		      }
		  }
	      }
	    while (0);
	    {
	      efp->ai = ((*pr).a)[i2];
	      if (!
		  (((*pr).lb)[n - 1 - efp->ai +
			      k] ? 1 : (((*pr).rb)[efp->ai + k] ? 1 : 0)))
		{
		  efp->ak = ((*pr).a)[k];
		  ((*pr).a)[i2] = efp->ak;
		  ((*pr).a)[k] = efp->ai;
		  ((*pr).lb)[n - 1 - efp->ai + k] = 1;
		  ((*pr).rb)[efp->ai + k] = 1;
		  if (n > k + 1)
		    {
		      new_esp = esp;
		      tmp_arg3 = k + 1;
		      while ((tmp11 =
			      nqueens (new_esp, pr, &efp->bk2, n,
				       tmp_arg3)) == (int) 0 - 1
			     && (efp->tmp_esp = *((char **) esp)) != 0)
			{
			  efp->tmp11 = tmp11;
			  efp->tmp10 = tmp10;
			  efp->rbp = rbp;
			  efp->req_buf_p = req_buf_p;
			  efp->i2 = i2;
			  efp->ret = ret;
			  efp->ei = ei;
			  efp->si = si;
			  efp->s = s;
			  efp->k = k;
			  efp->n = n;
			  efp->bkf = bkf;
			  efp->pr = pr;
			  efp->bk2.fun = bk2_in_nqueens;
			  efp->bk2.fr = (void *) efp;
			  efp->bk.fun = bk_in_nqueens;
			  efp->bk.fr = (void *) efp;
			  efp->call_id = 1;
			  return (int) 0 - 1;
			L_CALL5:
			  ;
			  tmp11 = efp->tmp11;
			  tmp10 = efp->tmp10;
			  rbp = efp->rbp;
			  req_buf_p = efp->req_buf_p;
			  i2 = efp->i2;
			  ret = efp->ret;
			  ei = efp->ei;
			  si = efp->si;
			  s = efp->s;
			  k = efp->k;
			  n = efp->n;
			  bkf = efp->bkf;
			  pr = efp->pr;
			  tmp_arg3 = efp->tmp_arg3;
			  new_esp = esp + 1;
			}
		      s += tmp11;
		    }
		  else
		    {
		      s++;
		    }
		  efp->ai = ((*pr).a)[k];
		  efp->ak = ((*pr).a)[i2];
		  ((*pr).a)[i2] = efp->ai;
		  ((*pr).a)[k] = efp->ak;
		  ((*pr).lb)[n - 1 - efp->ai + k] = 0;
		  ((*pr).rb)[efp->ai + k] = 0;
		}
	    }
	  }
	while (si < ei);
      if (si < n)
	do
	  {
	    i3 = si++;
	    tmp12 = atomic_read_int ((efp->result_port)[i3]);
	    if (tmp12 == 0)
	      do
		{
		  new_esp = esp;
		  tmp_arg4 = (efp->saved_req_port_p)[i3];
		  while (steal_run_task (new_esp, pr, tmp_arg4, 1),
			 (efp->tmp_esp = *((char **) esp)) != 0)
		    {
		      efp->tmp13 = tmp13;
		      efp->tmp12 = tmp12;
		      efp->i3 = i3;
		      efp->tmp11 = tmp11;
		      efp->tmp10 = tmp10;
		      efp->rbp = rbp;
		      efp->req_buf_p = req_buf_p;
		      efp->i2 = i2;
		      efp->ret = ret;
		      efp->ei = ei;
		      efp->si = si;
		      efp->s = s;
		      efp->k = k;
		      efp->n = n;
		      efp->bkf = bkf;
		      efp->pr = pr;
		      efp->bk2.fun = bk2_in_nqueens;
		      efp->bk2.fr = (void *) efp;
		      efp->bk.fun = bk_in_nqueens;
		      efp->bk.fr = (void *) efp;
		      efp->call_id = 2;
		      return (int) 0 - 1;
		    L_CALL6:
		      ;
		      tmp13 = efp->tmp13;
		      tmp12 = efp->tmp12;
		      i3 = efp->i3;
		      tmp11 = efp->tmp11;
		      tmp10 = efp->tmp10;
		      rbp = efp->rbp;
		      req_buf_p = efp->req_buf_p;
		      i2 = efp->i2;
		      ret = efp->ret;
		      ei = efp->ei;
		      si = efp->si;
		      s = efp->s;
		      k = efp->k;
		      n = efp->n;
		      bkf = efp->bkf;
		      pr = efp->pr;
		      tmp_arg4 = efp->tmp_arg4;
		      new_esp = esp + 1;
		    }
		  tmp13 = atomic_read_int ((efp->result_port)[i3]);
		}
	      while (tmp13 == 0);
	    start_read_after_read ();
	    s += (efp->result_buf)[i3];
	  }
	while (si < n);
      return s;
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
  char *new_esp;
  struct bk_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  req_buf_t *req_buf_p = (parmp =
			  (char *) ((Align_t *) parmp -
				    (sizeof (req_buf_t *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t)),
			  *((req_buf_t **) parmp));
lgoto:
  ;
  efp = (struct bk_in_main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct bk_in_main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  *((int *) efp) = 0;
  return 0;
  return 0;
}

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  int i;
  int n;
  int s;
  int num_proc;
  struct _proc_locals *tmp_arg5;

lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  num_proc = 2;
  if (argc > 1)
    {
      num_proc = atoi (argv[1]);
    }
  if (argc > 2)
    {
      n = atoi (argv[2]);
    }
  new_esp = esp;
  while (init_parallel (new_esp, num_proc),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->num_proc = num_proc;
      efp->s = s;
      efp->n = n;
      efp->i = i;
      efp->argv = argv;
      efp->argc = argc;
      efp->bk.fun = bk_in_main;
      efp->bk.fr = (void *) efp;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      num_proc = efp->num_proc;
      s = efp->s;
      n = efp->n;
      i = efp->i;
      argv = efp->argv;
      argc = efp->argc;
      new_esp = esp + 1;
    }
  gettimeofday (efp->tv + 0, 0);
  tv0 = (efp->tv)[0];
  new_esp = esp;
  tmp_arg5 = pe + 0;
  while ((s = nqueens (new_esp, tmp_arg5, &efp->bk, n, 0)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->num_proc = num_proc;
      efp->s = s;
      efp->n = n;
      efp->i = i;
      efp->argv = argv;
      efp->argc = argc;
      efp->bk.fun = bk_in_main;
      efp->bk.fr = (void *) efp;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      num_proc = efp->num_proc;
      s = efp->s;
      n = efp->n;
      i = efp->i;
      argv = efp->argv;
      argc = efp->argc;
      tmp_arg5 = efp->tmp_arg5;
      new_esp = esp + 1;
    }
  gettimeofday (efp->tv + 1, 0);
  printf ("answers: %d\n", s);
  printf ("%lf\n",
	  ((efp->tv)[1]).tv_sec - ((efp->tv)[0]).tv_sec +
	  (((efp->tv)[1]).tv_usec - ((efp->tv)[0]).tv_usec) * 1.0e-6);
}
