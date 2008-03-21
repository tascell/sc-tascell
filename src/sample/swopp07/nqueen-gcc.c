#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#ifndef NOXCC
//#include "xccmem.h"
#include "xccmem-dummy.h"
#endif
#include "systhr.h"

#define MAXNPROC 64
#define DPRINT0

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

typedef struct _proc_locals *proc_env;

typedef struct _task_buf {
  void (*f)(proc_env, struct _task_buf *);
  int sender; /* optional */
  int why;    /* optional */
  union {
    struct {
      int *result_buf_p;
      int *result_port_p;
      int n, k;
      int a[20];
      int lb[40];
      int rb[40];
    } t1;
  } a;
} task_buf_t;

typedef struct _req_buf {
  task_buf_t *task_buf_p;
  int *task_port_p;
  void **req_port_p;		/* (struct _req_buf **) */
} req_buf_t;

struct _proc_locals {
  int n_proc;
  int myid;
  task_buf_t task_buf;
  int task_port;
  req_buf_t req_buf;
  void *dumy1[64];
  void *req_port;		/* (req_buf_t *) */
  void *dumy2[64];  
  int a[20];
  int lb[40];
  int rb[40];
  void *dumy3[64];  
} pe[MAXNPROC];

void init_proc_locals(proc_env pr, int id, int num_proc){
  int i;
  for(i = 0 ; i < 20 ; i++) pr->a[i] = i;
  for(i = 0 ; i < 40; i++) pr->lb[i] = pr->rb[i] = 0;
  pr->n_proc = num_proc;
  pr->myid = id;
  pr->req_port = 0;
}

void tf_do_nothing(proc_env pr, task_buf_t *task_buf_p){
}

void refuse_req(proc_env pr){
  /* transfer a null task to refuse a request. */
  void *req_buf_p = atomic_read_ptr(pr->req_port);
  if(req_buf_p){
    req_buf_t *rbp = (req_buf_t *)req_buf_p;
    start_read_after_read();
    atomic_write_ptr(pr->req_port,0);
    rbp->task_buf_p->f = tf_do_nothing;
    finish_write_before_write();
    atomic_write_int(*(rbp->task_port_p), 1);
  }
}

void steal_run_task(proc_env pr, void **req_port_p, int why){
#ifndef NOXCC
  pr->task_port = 0;
  pr->task_buf.why = why;
  /* request a task */
  pr->req_buf.task_buf_p  = &(pr->task_buf);
  pr->req_buf.task_port_p = &(pr->task_port);
  pr->req_buf.req_port_p  = &(pr->req_port);
  finish_write_before_write();
  while(cas_ptr(*req_port_p, 0, (void *)(&(pr->req_buf))))
    refuse_req(pr);
  /* wait for a task */
  while(atomic_read_int(pr->task_port) == 0) refuse_req(pr);
  start_read_after_read();
  /* run the stolen task */
  (pr->task_buf.f)(pr, &(pr->task_buf));
#endif
}

int bv;

void *helper(void *p){
#ifndef NOXCC
  proc_env pr=(proc_env)p;
  double r, q;
  int n;
  systhr_bind_to_proc(pr->myid);
  systhr_bind_to_proc(pr->myid);
  do{n = bv;}while(cas_int(bv, n, n+1)); /* atomic-inc */
  for(r = sqrt(0.5 + pr->myid), q = sqrt(r + pr->myid), r -= (int)r;;
      r = r * 3.0 + q, r -= (int)r){
    int tg = (int)(pr->n_proc * r);
    if(tg != pr->myid){
      steal_run_task(pr, &(pe[tg].req_port), 0);
    }
  }
#endif
}

void init_parallel(int num_proc){
  int i;
  for(i=0; i<num_proc; i++) init_proc_locals(pe+i, i, num_proc);
  systhr_init(num_proc);
  systhr_bind_to_proc(0); bv = 1;
  for(i=1; i<num_proc; i++) systhr_create(helper, pe+i);
  while(atomic_read_int(bv) < num_proc) start_read_after_read();
}

#define POLL(req_port, bk) \
    do{ \
      void *req_buf_p = atomic_read_ptr((req_port)); \
      if(req_buf_p){ \
        req_buf_t *rbp = (req_buf_t *)req_buf_p; \
        start_read_after_read(); \
        atomic_write_ptr((req_port),0); \
        if((bk)(rbp) == 0){ \
	  rbp->task_buf_p->f = tf_do_nothing; \
	  finish_write_before_write(); \
	  atomic_write_int(*(rbp->task_port_p), 1); \
	} \
      } \
    }while(0)


struct timeval tv0;

void tf_backup_and_nqueens(proc_env pr, task_buf_t *task_buf_p){
  int bk(req_buf_t *req_buf_p){ return 0; }
  int a_bak[20];
  int lb_bak[40];
  int rb_bak[40];
  int jj;
  int *saved_result_port_p;
  int *saved_result_buf_p;
  task_buf_t *tbp  = task_buf_p;
  /* backup */
  for(jj=0;jj<20;jj++) a_bak[jj] = pr->a[jj];
  for(jj=0;jj<40;jj++) lb_bak[jj] = pr->lb[jj];
  for(jj=0;jj<40;jj++) rb_bak[jj] = pr->rb[jj];
  /* save info for "result back" */
  saved_result_port_p = tbp->a.t1.result_port_p;
  saved_result_buf_p  = tbp->a.t1.result_buf_p;
  /* get a task */
#ifdef DPRINT
  {
    struct timeval tv1;
    gettimeofday(&tv1, 0);
    printf("%9d %2d gets %2d %2d of %2d for %d\n",
	   (tv1.tv_sec-tv0.tv_sec)*1000000+tv1.tv_usec-tv0.tv_usec,
	   pr->myid, tbp->a.t1.k, tbp->a.t1.i, tbp->sender, tbp->why); 
  }
#endif
  for(jj=0;jj<20;jj++) pr->a[jj]  = tbp->a.t1.a[jj];
  for(jj=0;jj<40;jj++) pr->lb[jj] = tbp->a.t1.lb[jj];
  for(jj=0;jj<40;jj++) pr->rb[jj] = tbp->a.t1.rb[jj];
  {
    int n  = tbp->a.t1.n,  k  = tbp->a.t1.k, s = 0;
    s = nqueens(pr, bk, n, k);
    *saved_result_buf_p = s;
  }
  finish_write_before_write();
  atomic_write_int(*saved_result_port_p, 1);
  /* restore */
  for(jj=0;jj<20;jj++) pr->a[jj]  = a_bak[jj];
  for(jj=0;jj<40;jj++) pr->lb[jj] = lb_bak[jj];
  for(jj=0;jj<40;jj++) pr->rb[jj] = rb_bak[jj];
}

int 
nqueens(proc_env pr, int (*bkf)(req_buf_t *), int n, int k)
{
  if (n == k)
    return 1;
  else
    {
      int s = 0;
      int si = k, ei = n;

      int ret = 1;
      void **saved_req_port_p[12];	/* (req_buf_t **) */
      int result_buf[12];
      int result_port[12];

      int bk(req_buf_t *req_buf_p){
	if(ret) ret = bkf(req_buf_p);
	if(ret) return 1;
	while(si < ei)
	  {
	    int i = --ei;
	    int ai = pr->a[i];
	    if (! (pr->lb[n - 1 - ai + k] || pr->rb[ai + k])){
	      int ak = pr->a[k];
	      pr->a[i] = ak; pr->a[k] = ai;
	      pr->lb[n - 1 - ai + k] = 1; pr->rb[ai + k] = 1;
	      {
		int jj;
		task_buf_t *tbp = req_buf_p->task_buf_p;
		int *tpp        = req_buf_p->task_port_p;
		/* save info for "request back" */
		saved_req_port_p[i] = req_buf_p->req_port_p;
		/* result not available yet */
		result_port[i] = 0;
		/* put a task */
		tbp->f = tf_backup_and_nqueens;
		tbp->sender = pr->myid;
		tbp->a.t1.result_buf_p = result_buf+i;
		tbp->a.t1.result_port_p = result_port+i;
		tbp->a.t1.n = n;
		tbp->a.t1.k = k + 1;
		for(jj=0;jj<20;jj++) tbp->a.t1.a[jj]  = pr->a[jj];
		for(jj=0;jj<40;jj++) tbp->a.t1.lb[jj] = pr->lb[jj];
		for(jj=0;jj<40;jj++) tbp->a.t1.rb[jj] = pr->rb[jj];
		/* transfer the task */
		finish_write_before_write();
		atomic_write_int(*tpp, 1);
	      }       /* おくる */
	      ai = pr->a[k]; ak = pr->a[i];
	      pr->a[i] = ai; pr->a[k] = ak;
	      pr->lb[n - 1 - ai + k] = 0; pr->rb[ai + k] = 0;
	      return 1; /* おくるのに成功したら */
	    }
	    else
	      {
		/* avoid a trivial task */
		result_port[i] = 1;
		result_buf[i]  = 0;
	      }
	  }
	return 0;
      }
      while(si < ei)
	{
	  int i = si++;
	  POLL(pr->req_port, bk);
	  {
	    
	    int ai = pr->a[i];
	    if (! (pr->lb[n - 1 - ai + k] || pr->rb[ai + k])){
	      int ak = pr->a[k];
	      pr->a[i] = ak; pr->a[k] = ai;
	      pr->lb[n - 1 - ai + k] = 1; pr->rb[ai + k] = 1;
	      {
		int bk2(req_buf_t *req_buf_p){
		  int r;
		  ai = pr->a[k]; ak = pr->a[i];
		  pr->a[i] = ai; pr->a[k] = ak;
		  pr->lb[n - 1 - ai + k] = 0; pr->rb[ai + k] = 0;
		  r = bk (req_buf_p);
		  ai = pr->a[i]; ak = pr->a[k];
		  pr->a[i] = ak; pr->a[k] = ai;
		  pr->lb[n - 1 - ai + k] = 1; pr->rb[ai + k] = 1;
		  return r;
		}
		s += nqueens(pr, bk2, n, k + 1);
	      }
	      ai = pr->a[k]; ak = pr->a[i];
	      pr->a[i] = ai; pr->a[k] = ak;
	      pr->lb[n - 1 - ai + k] = 0; pr->rb[ai + k] = 0;
	    }
	  }
	}
      while(si < n){
	int i = si++;
	while(atomic_read_int(result_port[i]) == 0)
	  /* request back a task and run*/
	  steal_run_task(pr, saved_req_port_p[i], 1);
	start_read_after_read();
	s += result_buf[i];
      }
      return s;
    }
}

main(int argc, char *argv[]){
  struct timeval tv[2];
  int i, n, s;
  int bk(req_buf_t *req_buf_p){ return 0; }
  int num_proc=2;
  if(argc>1) num_proc=atoi(argv[1]);
  if(argc>2) n=atoi(argv[2]);

  init_parallel(num_proc);
  gettimeofday(tv+0, 0); tv0 = tv[0];

  s = nqueens(pe+0, bk, n, 0);

  gettimeofday(tv+1, 0);
  printf("answers: %d\n", s);
  printf("time: %lf\n", tv[1].tv_sec-tv[0].tv_sec
	 +(tv[1].tv_usec-tv[0].tv_usec)*0.000001);
}
