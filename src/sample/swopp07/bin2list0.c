#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "copygc.h"


/* 構造体定義 */

typedef struct _Bintree Bintree;

struct _Bintree {
  desc_t d;
  int key;
  int val;
  Bintree *left, *right;
};

typedef struct _KVpair KVpair;

struct _KVpair {
  desc_t d;
  int key;
  int val;
};

typedef struct _Alist Alist;

struct _Alist {
  desc_t d;
  KVpair *kv;
  Alist *cdr;
};

/* 構造体定義: ポインタの位置 */

size_t Bintree_node[] =
{offsetof(Bintree, left), offsetof(Bintree, right)};
descriptor Bintree_d = DESC(Bintree, Bintree_node);

size_t Alist_node[] =
{offsetof(Alist, kv), offsetof(Alist, cdr)};

descriptor Alist_d = DESC(Alist, Alist_node);

size_t KVpair_node[] = {};

descriptor KVpair_d = DESC(KVpair, KVpair_node);


/* 計測パラメータ */
static int maxins, maxsearch;


Bintree *newBintree(
#ifndef RAWC
		    sht scan0,
#endif
		    int k, int v){
  Bintree *p = (Bintree *)getmem(
#ifndef RAWC
				 scan0,
#endif
				 &Bintree_d);
  p->key = k;
  p->val = v;
  return p;
}

/* x がすでに存在する場合 */
void insert(
#ifndef RAWC
	    sht scan0,
#endif
	    Bintree *x, int k, int v){
  Bintree *y = 0;
#ifndef RAWC
  CTOLD void scan1 CT (void){
    x = move(x);
    y = move(y);
    scan0();
  }
#endif
  while(1){
    if(x->key == k){
      x->val = v;
      return;
    } else if(k < x->key){
      y = x->left;
      if(!y) {
	x->left = newBintree(
#ifndef RAWC
			     scan1,
#endif
			     k, v);
	return;
      }
    }else{
      y = x->right;
      if(!y) {
	x->right = newBintree(
#ifndef RAWC
			      scan1,
#endif
			      k, v);
	return;
      }
    }
    x = y;
  }
}

int search(
#ifndef RAWC
	   sht scan0,
#endif
	   Bintree *x, int k, int v0){
  while(x){
    if(x->key == k){
      return x->val;
    } else if(k < x->key){
      x = x->left;
    }else{
      x = x->right;
    }
  }
  return v0;
}

Alist *bin2list(
#ifndef RAWC
		sht scan0,
#endif
		Bintree *x, Alist *rest){
  Alist *a = 0;
  KVpair *kv = 0;
#ifndef RAWC
  CTOLD void scan1 CT (void){
    x = move(x);
    rest = move(rest);
    a = move(a);
    kv = move(kv);
    scan0();
  }
#endif
  if(x->right) {
    rest = bin2list(
#ifndef RAWC
		    scan1,
#endif
		    x->right, rest);
    x->right = 0;
  }
  kv = getmem(
#ifndef RAWC
	      scan1,
#endif
	      &KVpair_d);
  kv->key = x->key;
  kv->val = x->val;
  a = getmem(
#ifndef RAWC
	     scan1,
#endif
	     &Alist_d);
  a->kv = kv;
  a->cdr = rest;
  rest = a;
  if(x->left) {
    rest = bin2list(
#ifndef RAWC
		    scan1,
#endif
		    x->left, rest);
    x->left = 0;
  }
  return rest;
}

void randinsert(
#ifndef RAWC
		sht scan0,
#endif
		Bintree *this, int n){
#ifndef RAWC
  CTOLD void scan1 CT (void){
    this = move(this);
    scan0();
  }
#endif
  int i, k;
  unsigned short seed[3];
  seed[0] = 3; seed[1] = 4; seed[2] = 5;
  for(i=0; i<n; i++){
    k = nrand48(seed);
    insert(
#ifndef RAWC
	   scan1,
#endif
	   this, k, k);
  }
}

void randsearch(
#ifndef RAWC
		sht scan0,
#endif
		Bintree *this, int n){
#ifndef RAWC
  CTOLD void scan1 CT (void){
    this = move(this);
    scan0();
  }
#endif
  int i, k;
  unsigned short seed[3];
  seed[0] = 8; seed[1] = 9; seed[2] = 10;
  for(i=0; i<n; i++){
    k = nrand48(seed);
    search(
#ifndef RAWC
	   scan1,
#endif
	   this, k, 0);
  }
}

/* gcパラメータの設定 */
void init(int tp, int tosize, int stack_size, int limited_max) {
  gc_params p;
  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init(p);
}

#ifdef SUN
#include <sys/processor.h>
#include <sys/procset.h>
#include <sys/lwp.h>
#endif

int main(int argc, char *argv[]) {
  struct timeval tp1, tp2;
  int i, gctp, searchskip = 0;
  Bintree *root;

#ifndef RAWC
  CTOLD void scan1 CT (void){
    root = move(root);
  }

#endif
#ifdef SUN
  {
    lwpid_t my_lwp_id = _lwp_self(); /* 自スレッドのＬＷＰのＩＤを得る */
    processorid_t p;
    for(p=0;p<100;p++) if(p_online(p, P_STATUS) == P_ONLINE) break;
    printf("lwp: %d, p: %d\n", my_lwp_id, p);
    if(processor_bind(P_LWPID, my_lwp_id, (processorid_t)p, NULL) != 0) 
      fprintf(stderr, "BIND ERROR\n");
  }
#endif

  if(argc > 1 && strcmp(argv[1],"-s") == 0){
    argc--; argv++;
    searchskip++;
  }

  maxins = argc > 1 ? atoi(argv[1]) : 100000;
  maxsearch = argc > 2 ? atoi(argv[2]) : 300000;

  printf("Bintree=%d, maxins=%d, maxsearch=%d\n",
	 sizeof(Bintree),  maxins, maxsearch);

  gctp = argc > 3 ? atoi(argv[3]) : 0;

  init(
       gctp < 0 ? 0 : gctp,
       argc > 4 ? atoi(argv[4]) : 0,
       argc > 5 ? atoi(argv[5]) : 0,
       argc > 6 ? atoi(argv[6]) : 0
       );

  root = getmem(
#ifndef RAWC
                 scan1,
#endif
                 &Bintree_d);
  root->key = 0;
  root->val = 0;
  


  randinsert(
#ifndef RAWC
	     scan1,
#endif
	     root, maxins);


#ifndef RAWC
  if(gctp>=0)
    gc(scan1);
#endif
  /* time */
  gc_ttime=0;
  gettimeofday(&tp1, 0); 

  if(!searchskip){

    bin2list(
#ifndef RAWC
	     scan1,
#endif
	     root, 0);

  }

  /* time */
  gettimeofday(&tp2, 0);


  {
    double ttime = 
      (tp2.tv_sec - tp1.tv_sec) + (tp2.tv_usec - tp1.tv_usec) * 0.000001;
    /*     printf("total=%f gc=%f other=%f\n", ttime, gc_ttime, ttime-gc_ttime); */
    printf("time: %f\n",ttime);
  }
  return 0;
}
