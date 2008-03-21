#include "worker.h"
void send_int(int n){ fprintf(stdout, " %d", n); }
void recv_int(int *n){ fscanf(stdin, " %ld", n); }

/* app */

// thread local strogae
struct task_nq {
  int r;         // （部分）解の数
  int n;         // 問題サイズ
  int k;         // 初期化->探索開始 or 部分探索開始かのフラグ
  int i1, i2;
  int a[20];     // 使用済み行の管理
  int lb[40];    // 盤面情報1
  int rb[40];    // 盤面情報2
};

void send_task_body(struct thread_data *thr, void *x0){
  struct task_nq *x = x0;
  int i;
  send_int(x->n);
  send_int(x->k);
  if(x->k < 0) return;
  send_int(x->i1);
  send_int(x->i2);
  for(i=0;i < x->n; i++)
    send_int(x->a[i]);
  for(i=0;i < 2 * x->n - 1; i++)
    send_int(x->lb[i]);
  for(i=0;i < 2 * x->n - 1; i++)
    send_int(x->rb[i]);
}

void *recv_task_body(struct thread_data *thr){
  int i;
  struct task_nq *x = (struct task_nq *) malloc(sizeof(struct task_nq));
  recv_int(&x->n);
  recv_int(&x->k);
  if(x->k < 0) return x;
  recv_int(&x->i1);
  recv_int(&x->i2);
  for(i=0;i < x->n; i++)
    recv_int(&x->a[i]);
  for(i=0;i < 2 * x->n - 1; i++)
    recv_int(&x->lb[i]);
  for(i=0;i < 2 * x->n - 1; i++)
    recv_int(&x->rb[i]);
  return x;
}


void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_nq *x = x0;
  send_int(x->r);
  free(x);
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_nq *x = x0;
  int r;
  /* merge はどこで? */
  recv_int(&x->r);
}

// 逐次版
#if 0
int 
nqueens(int n, int k, int *a, int *lb, int *rb)
{
  if (n == k)
    return 1;
  else
    {
      int s = 0;
      int i;
      /* try each possible position for queen <k> */
      for (i = k; i < n; i++) {
	int ai = a[i];
	if (! (lb[n - 1 - ai + k] || rb[ai + k])){
	  lb[n - 1 - ai + k] = 1; rb[ai + k] = 1;
	  a[i] = a[k]; a[k] = ai;
	  s += nqueens(n, k + 1, a, lb, rb);
	  ai = a[k]; a[k] = a[i]; a[i] = ai;
	  lb[n - 1 - ai + k] = 0; rb[ai + k] = 0;
	}
      }
      return s;
    }
}
#endif

// k: a[j] (0<k)までの行にはもう置いた
// ix, iy:  探索範囲
int 
nqueens(FF, int n, int k, int ix, int iy, struct task_nq *tsk)
{
  int s = 0;
  // for (i=ix; i<iy; i++) （次の駒をどこにおくかで並列化）
  DO_MANY_BEGIN(struct task_nq, st, i, ix, iy, i1, i2){
    *st = *tsk;
    st->k = k;
    st->i1 = i1;
    st->i2 = i2;
  }
  // body
  DO_MANY_BODY(st, i){
    int ai = tsk->a[i];
    // 駒が置けるかチェック
    if (! (tsk->lb[n - 1 - ai + k] || tsk->rb[ai + k])){
      // 最後までおけたら
      if(k == n - 1)
	s += 1;
      else
	DO_INI_FIN({
            // 駒を置く
          tsk->lb[n - 1 - ai + k] = 1; tsk->rb[ai + k] = 1;
	  tsk->a[i] = tsk->a[k]; tsk->a[k] = ai;
	},{ 
            // 次のステップ
	  s += nqueens(FA, n, k + 1, k + 1, n, tsk) ; 
	},{
            // 置いた駒をとりのぞく: backtrack
	  ai = tsk->a[k]; tsk->a[k] = tsk->a[i]; tsk->a[i] = ai; 
	  tsk->lb[n - 1 - ai + k] = 0; tsk->rb[ai + k] = 0;
	});
    }
  }
  // 結果の統合（spawnしていた場合）
  DO_MANY_FINISH(st){
    s += st->r;
  }
  DO_MANY_END(st);
  return s;
}

// 初期化->探索開始
void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_nq *x = x0;
  int i;
  int n = x->n;
  int k = x->k;
  int i1 = x->i1;
  int i2 = x->i2;
  fprintf(stderr, "start %d %d %d %d\n", n, k, i1, i2);
  if(k < 0){
    for(i=0;i<n;i++)
      x->a[i] = i;
    for(i=0;i<2*n-1;i++)
      x->lb[i] = x->rb[i] = 0;
    x->r = nqueens(FA, n, 0, 0, n, x);  
  }else
    x->r = nqueens(FA, n, k, i1, i2, x);
}
