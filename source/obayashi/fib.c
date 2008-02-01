#include "worker.h"
void send_int(int n){ fprintf(stdout, " %d", n); }
void recv_int(int *n){ fscanf(stdin, " %ld", n); }

/* app */

/* 
  (def (task task-fib)
    (def n int)
    (def r int)
    (def (send-task) n) ?
    (def (send-rslt) r) ?
    (def (recv-rslt) r) ?

  (def (task task-fib)
    (def n int :in)
    (def r int :out))
 */
struct task_fib {
  int n;
  int r;
};

void send_task_body(struct thread_data *thr, void *x0){
  struct task_fib *x = x0;
  send_int(x->n);
}

void *recv_task_body(struct thread_data *thr){
  struct task_fib *x = (struct task_fib *) malloc(sizeof(struct task_fib));
#ifdef TEST_MSG_ALLOC
  fprintf(stderr,"id=%d allocate task body\n", thr->id);
#endif
  recv_int(&x->n);
  return x;
}

void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_fib *x = x0;
  send_int(x->r);
  free(x);
#ifdef TEST_MSG_ALLOC
  fprintf(stderr,"id=%d free task body\n", thr->id);
#endif
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_fib *x = x0;
  recv_int(&x->r);
}

/*
  (def-WF (fib n) (fn int int) ....)
  (def (fib n) (w-fn int int) ....)

  (def (fib _bk _x n) (fn int (ptr (fn int)) (prt (struct task)) int) ....)

  (do-two 
    (def st (task task-fib))
    (= s1 (fib (- n 1)))
    (= s2 (fib (- n 2)))
    (= (fref st n) (- n 2))
    (= s2 (fref st r)))

  do_two(task task_fib st){
    s1 = fib(n-1);
  } with {
    s2 = fib(n-2);
  } if_req {
    st.n = n - 2;
  } with {
    s2 = st.r;
  }

  Lazy RPC だと
   s2 = fib(n-2) !! { s1 = fib(n-1); }
   (= s2 (conc (fib (- n 2)) (= s1 (fib (- n 1)))))
  この書き方は，特定の 関数型言語的な場合のみに有効．(lu とかは...)

 */


/* DO_MANYに合わせて st.n ではなく， st->n とした */


int fib(FF, int n){
  if(n <= 2){
    return 1;
  }else{
    int s1, s2;
    DO_TWO(struct task_fib, st,
           s1 = fib(FA, n-1),
	   s2 = fib(FA, n-2),
	   st->n = n-2,
	   s2 = st->r);
    return s1+s2;
  }
}


/* 
   bk2 を余分に呼び出していた 
*/
/*
int fib(FF, int n){
  if(n <= 2){
    return 1;
  }else{
    int s1, s2;
    {
      struct task_fib st[1];
      int spawned = 0;
      {
	int (*_bk2)() = _bk;
	int _bk(){
	  if(spawned)
	    return 0;
	  _bk2();
	  if(_thr->treq_top){
	    (st->n = n-2);
	    spawned = 1;
	    make_and_send_task(_thr, st);
	    return 1;
	  }
	  return 0;
	}
        if(_thr->req)
	  handle_req(FA);
	(s1 = fib(FA, n-1));
      }
      if(spawned){
	wait_rslt(_thr);
	(s2 = st->r);
      } else {
	(s2 = fib(FA, n-2));
      }
    }
    return s1+s2;
  }
}
*/ 


/*
  int foo(int) { ... bar() ... }
  double bar(double) { ... foo() ... }
 */

void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_fib *x = x0;
  int n = x->n;
  fprintf(stderr, "start %d\n", n);
  x->r = fib(FA, n);
}
