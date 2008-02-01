#include "worker.h"
void send_int(int n){ fprintf(stdout, " %d", n); }
void recv_int(int *n){ fscanf(stdin, " %ld", n); }

struct task_cmp {
  int r;
  int n1;
  int n2;
  int *d1;
  int *d2;
};

void send_task_body(struct thread_data *thr, void *x0){
  struct task_cmp *x = x0;
  int i;
  send_int(x->n1);
  send_int(x->n2);
  if(x->n2 < 0) return;
  for(i=0; i < x->n1; i++)
    send_int(x->d1[i]);
  for(i=0; i < x->n2; i++)
    send_int(x->d2[i]);
}

void *recv_task_body(struct thread_data *thr){
  int i;
  struct task_cmp *x = (struct task_cmp *) malloc(sizeof(struct task_cmp));
#ifdef TEST_MSG_ALLOC
  fprintf(stderr, "allocate id=%d\n", thr->id);
#endif

  recv_int(&x->n1);
  recv_int(&x->n2);
  if(x->n2 < 0) return x;
  x->d1 = (int *)malloc(sizeof(int)*x->n1);
#ifdef TEST_MSG_ALLOC
  fprintf(stderr, "allocate id=%d\n", thr->id);
#endif
  x->d2 = (int *)malloc(sizeof(int)*x->n2);
#ifdef TEST_MSG_ALLOC
  fprintf(stderr, "allocate id=%d\n", thr->id);
#endif
  for(i=0; i < x->n1; i++)
    recv_int(&x->d1[i]);
  for(i=0; i < x->n2; i++)
    recv_int(&x->d2[i]);
  return x;
}

void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_cmp *x = x0;
  send_int(x->r);
  free(x->d1);
#ifdef TEST_MSG_ALLOC
    fprintf(stderr, "free id=%d\n", thr->id);
#endif
  free(x->d2);
#ifdef TEST_MSG_ALLOC
    fprintf(stderr, "free id=%d\n", thr->id);
#endif
  free(x);
#ifdef TEST_MSG_ALLOC
    fprintf(stderr, "free id=%d\n", thr->id);
#endif
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_cmp *x = x0;
  recv_int(&x->r);
}

int cmp_2(int n1, int n2, int *d1, int *d2){
  int i, j;
  int s = 0;
  for(i=0;i<n1;i++)
    for(j=0;j<n2;j++)
      if((d1[i] ^ d2[j]) == -1) s++;
  return s;
}

int cmp_1(FF, int n1, int n2, int *d1, int *d2){
  int s1, s2;
  if(n1 < 5)
    return cmp_2(n1, n2, d1, d2);
  if(n1>n2){
    int n1_1 = n1/2, n1_2 = n1 - n1_1;
    DO_TWO(struct task_cmp, st,
	   s1 = cmp_1(FA, n1_1, n2, d1, d2),
	   s2 = cmp_1(FA, n1_2, n2, d1+n1_1, d2),
	   (st->n1 = n1_2, st->n2 = n2,
	    st->d1 = d1+n1_1, st->d2 = d2),
	   s2 = st->r);
  }else{
    int n2_1 = n2/2, n2_2 = n2 - n2_1;
    DO_TWO(struct task_cmp, st,
	   s1 = cmp_1(FA, n1, n2_1, d1, d2),
	   s2 = cmp_1(FA, n1, n2_2, d1, d2+n2_1),
	   (st->n1 = n1, st->n2 = n2_2,
	    st->d1 = d1, st->d2 = d2+n2_1),
	   s2 = st->r);
  }
  return s1 + s2;
}

void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_cmp *x = x0;
  int i, j;
  fprintf(stderr, "start %d %d\n", x->n1, x->n2);
  if(x->n2 < 0){
    x->d1 = (int *)malloc(sizeof(int)*x->n1);
#ifdef TEST_MSG_ALLOC
    fprintf(stderr, "allocate id=%d\n", _thr->id);
#endif
    for(i=0;i<x->n1;i++)
      x->d1[i] = i;
    x->n2 = x->n1;
    x->d2 = (int *)malloc(sizeof(int)*x->n2);
#ifdef TEST_MSG_ALLOC
    fprintf(stderr, "allocate id=%d\n", _thr->id);
#endif
    for(i=0;i<x->n2;i++)
      x->d2[i] = -i;
  }
  x->r = cmp_1(FA, x->n1, x->n2, x->d1, x->d2);
}
