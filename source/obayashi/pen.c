#include "worker.h"
void send_int(int n){ fprintf(stdout, " %d", n); }
void recv_int(int *n){ fscanf(stdin, " %ld", n); }

// thread local storage
struct task_pen {
  int r;             // （部分）解の総数
  int k;             // 初期化->探索開始 or 部分解かのフラグ
  int i0, i1, i2;    // searchの引数を参照
  int a[12];         // manages which piece has been used
  int b[70];         // board（6x10 だが各列右端は壁）
};

void send_task_body(struct thread_data *thr, void *x0){
  struct task_pen *x = x0;
  int i;
  send_int(x->k);
  if(x->k < 0) return;
  send_int(x->i0);
  send_int(x->i1);
  send_int(x->i2);
  for(i=0;i < 12; i++)
    send_int(x->a[i]);
  for(i=0;i < 70; i++)
    send_int(x->b[i]);
}

void *recv_task_body(struct thread_data *thr){
  int i;
  struct task_pen *x = (struct task_pen *) malloc(sizeof(struct task_pen));
  recv_int(&x->k);
  if(x->k < 0) return x;
  recv_int(&x->i0);
  recv_int(&x->i1);
  recv_int(&x->i2);
  for(i=0;i < 12; i++)
    recv_int(&x->a[i]);
  for(i=0;i < 70; i++)
    recv_int(&x->b[i]);
  return x;
}

void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_pen *x = x0;
  send_int(x->r);
  free(x);
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_pen *x = x0;
  int r;
  recv_int(&x->r);
}

int ps[57][5] = { // ps[i] represents the shape of the \TT{i}-th (piece, direcion)
  {1,1,1,1},{7,7,7,7},
  {4,1,1,1},{7,7,6,1},
  {6,1,1,1},{5,1,1,1},{1,1,1,5},{1,1,1,6},
  {7,1,6,7},{7,7,1,6},{7,6,1,7},{6,1,7,7},
  {7,7,1,1},{1,1,5,7},{1,1,7,7},{7,5,1,1},
  {2,5,1,1},{1,1,5,2},{1,6,7,1},{1,7,6,1},
  {7,1,1,5},{5,1,1,7},{7,6,1,1},{1,1,6,7},
  {7,1,1,6},{5,1,1,6},{6,1,1,5},{6,1,1,7},
  {1,7,1,6},{7,1,5,1},{1,5,1,7},{6,1,7,1},
  {7,1,1,7},{5,1,1,5},{1,7,7,1},{1,6,6,1},
  {6,1,1,6},
  {1,6,1,1},{1,1,5,1},{1,5,1,1},{1,1,6,1},
  {7,1,6,1},{1,6,1,6},{6,1,6,1},{1,6,1,7},
  {1,4,1,1},{1,1,7,1},{1,7,1,1},{1,1,4,1},
  {7,1,7,7},{7,6,1,6},{6,1,6,7},{7,7,1,7},
  {7,1,7,1},{1,5,1,6},{1,7,1,7},{6,1,5,1}
};

int pos[13] = { 0, 2, 4, 12, 16, 20, 24, 32, 36, 37, 45, 53, 57};
  // ps[i] for pos[p]<i<pos[p+1] corresponds the shape for the \TT{p}-th piece


// 探索メイン
// Try from the \TT{j1}-th piece to the \TT{j2}-th piece in \TT{a[]}.
// The \IT{i}-th piece for \IT{i}}<\RM{\TT{j0} is already used.
// \TT{b[k]} is the first empty cell in the board.
int 
search(FF, int k, int j0, int j1, int j2, struct task_pen *tsk)
{
  int s = 0;
  // for (p=j1; p<j2; p++)  // iterate for each piece: このループを並列化したい
  DO_MANY_BEGIN(struct task_pen, st, p, j1, j2, i1, i2){
    *st = *tsk;
    st->k = k;
    st->i0 = j0;
    st->i1 = i1; // i0, i1, i2はループ範囲分割化の際のループ範囲なのでCilkでは気にしなくてよい？
    st->i2 = i2;
  }
  // body of for(do_many) loop
  DO_MANY_BODY(st, p){
    int ap = tsk->a[p], i;
    for(i = pos[ap]; i < pos[ap+1]; i++)
      /* examine the "i"-th (piece, direction)
	 at the first empty location "k" */
      {
	int *pss = ps[i];
	int kk=k,l;
	/* room available? */
	for(l=0;l<4;l++) if((kk += pss[l]) >= 70 || tsk->b[kk] !=0) goto Ln;

        // set the piece \TT{p} into the board
	DO_INI_FIN({
	  tsk->b[kk=k] = p+'A'; for(l=0;l<4;l++) tsk->b[kk += pss[l]] = p+'A';
	  tsk->a[p] = tsk->a[j0]; tsk->a[j0] = ap;
	},{ 
          // 次の空きセルをみつける
	  for(kk=k; kk<70; kk++) if( tsk->b[kk] == 0 ) break;
          // 全セルが埋まっていたら
	  if(kk == 70)
	    s += 1;
          // そうでなければ（周辺を見て置けそうなら？）次のステップ
	  else if((kk+7 >= 70 || tsk->b[kk+7] != 0) && 
		  (tsk->b[kk+1] != 0 || 
		   (kk+8 >= 70 || tsk->b[kk+8] != 0) && tsk->b[kk+2] != 0))
	    ;
	  else
	    s += search(FA, kk, j0 + 1, j0 + 1, 12, tsk);
	},{
            // remove the piece p: backtrack
	  ap = tsk->a[j0]; tsk->a[j0] = tsk->a[p]; tsk->a[p] = ap; 
	  tsk->b[kk=k] = 0; for(l=0;l<4;l++) tsk->b[kk += pss[l]] = 0;
	});
      Ln:
	continue;
      }
  }
  DO_MANY_FINISH(st){
    s += st->r;
  }
  DO_MANY_END(st);
  return s;
}

// 初期化->探索開始
void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_pen *x = x0;
  int i;
  int k = x->k;
  int i0 = x->i0;
  int i1 = x->i1;
  int i2 = x->i2;
  fprintf(stderr, "start %d %d %d %d\n", k, i0, i1, i2);
  if(k < 0){
    for(i=0;i<12;i++) x->a[i] = i;
    for(i=0;i<70;i++) x->b[i] = 0;
    for(i=6;i<70;i+=7) x->b[i] = '\n'; /* right side wall */
    x->r = search(FA, 0, 0, 0, 12, x);  
  }else{
    x->r = search(FA, k, i0, i1, i2, x);
    fprintf(stderr, "end %d\n", x->r);
  }
}
