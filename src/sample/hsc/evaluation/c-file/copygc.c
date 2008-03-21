#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "copygc.h"

#ifndef TOSIZE
#define TOSIZE   (5*1024*1024)
#endif

#ifndef ROOTSIZEMAX
#define ROOTSIZEMAX (8*1024)
#endif

#ifndef GC_STACK_SIZE
#define GC_STACK_SIZE (params.tosize/(sizeof(double)))
#endif

#ifndef GC_LIMITED_STACK_SIZE
#define GC_LIMITED_STACK_SIZE 256
#endif

#ifndef GC_LIMITED_STACK_MAX
#define GC_LIMITED_STACK_MAX 32
#endif

#define IN_FROM(p) ((unsigned)((char *)(p) - old_memory) < params.tosize)
//pから古いメモリを引いたものが、params.tosizeより小さい　　？？
#define IN_TOSP(p) ((unsigned)((char *)(p) - new_memory) < params.tosize)
//pから新しいメモリを引いたものが、params.tosizeより小さい　？？
#define FWPTR(p) (*((void **)(p)))
//pを、ポインタのポインタにしたもののポインタ　　？？

#if 0
#define MEMCPY(d,s,sz) memcpy((d),(s),(sz))
#else
#define MEMCPY(d,s,sz) \
  { long *_des=(long *)(d), *_src=(long *)(s),  \
         *_til=(long *)(((char *)_src)+(sz)); \
    do{ *(_des++) = *(_src++); } while( _src < _til); }
//MEMCPYは、srcからdesへとコピーする関数
//おそらくコピーGCのコピー部分のこと。
//szは、コピーするメモリの量
#endif

/* util */

static void error(char *s)//エラー処理
{
  fprintf(stderr, "ERROR: %s\n", s);
  exit(EXIT_FAILURE);
}

static void *mymalloc(size_t size)
{
  void *p = malloc(size);
  //mallocの返り値:確保したメモリブロックを指すポインタ
  //失敗(メモリ不足)ならNULL。
  if(p == NULL)
    error("Not enough memory.");//メモリ不足
  return p;
}

static void *myrealloc(void *p, size_t size) {
  void *q = realloc(p, size);
  //pの位置のメモリを、sizeの大きさにして新たにコピーする。
  //コピー先はqになる。

  if(q == NULL)
    error("Not enough memory.");
  return q;
}

/* root + heap area */

gc_params params;

static int allocated_size;
static char *old_memory, *new_memory;//文字型へのポインタ
static char *old_memory_end, *new_memory_end;

/* gc */

static char *b;

/* *link = move(*link); */

void *move(void *vp)
     //move関数。リンクを辿るもの。
     //*vpを入力、ポインタを返す
{
  char *p = vp;
  if(!IN_FROM(p))//((unsigned)((char *)(p) - old_memory) > params.tosize)
    return vp;
  if(IN_TOSP(FWPTR(p)))//((unsigned)((char *)(p) - new_memory) < params.tosize)
    return FWPTR(p);//pを(*((void **)(p)))にして返す


  //どちらでもなければ
  {
    desc_t d = *(desc_t *)p;//desc_tは構造体
    
    char *np = b, *nb = np + d->asize;
    //bは文字列。gc_breadth_firstの中で出てくる。new_memoryを代入。
    //dはp由来
    
    if(nb >= new_memory_end)
      error("buffer overrun.");
    //恐らく、前のポインタに今確保した分を加えたのが
    //メモリの量をオーバーしたときに出るエラー。
    
    MEMCPY(np, p, d->size);//pからnpへと、d->size分だけコピーする。
    b = nb;//nbをbへ。
    FWPTR(p) = np;//(*((void **)(p)))にnpを代入
    return np;
  }
}

/* BREADTH_FIRST_GC */

void gc_breadth_first(sht scan) {
  //引数は関数。gc()関数の中で使う。
  int i;
  char *tmp;
  desc_t d;
  char *p;
  void **link;
  char *s;

  if(params.gcv)
    printf("BREADTH_FIRST_GC start\n");
  //params.gcvはソースを書き変えない限りは1。
  
  s = b = new_memory;//new_memoryをs,bに代入
  //new_memoryの初期値は？ -> コマンドライン引数を処理する関数
  //init(bt0.cの中)およびgetmem_initで決める。コマンドライン引数によって決定する。
  //bはグローバル変数
  scan();
  //スキャン(この関数の引数のやつ) scan1()なら、move関数を実行する。
  //moveの返り値は、移動前のメモリへのポインタの位置
  //void scan1(){root=move(root);}
  
  while(s < b) {
    //sは開始位置、bは確保しまくった最後の指している点
    d = *(desc_t *)s;
    p = (char *)s;
    for(i = 0; i < d->fli_len; i++) {
      //dの中のパラメータの回数だけ続ける
      //pにd->fli[i]を加えたものをlinkとする。
      link = (void **)(p + d->fli[i]);
      *link = move(*link);//moveする。
      //よく分からないが、linkもmoveする。
    }
    s += d->asize;//sを1単位分だけ進める
  }
  allocated_size = b - new_memory;//確保されたメモリのサイズ
  tmp = new_memory;
  new_memory = old_memory;
  old_memory = tmp;//new_memoryとold_memoryの入れ替え
  tmp = new_memory_end;
  new_memory_end = old_memory_end;
  old_memory_end = tmp;//new_memory_endとold_memory_endの入れ替え
  if(params.gcv)  //params.gcvはソースを書き変えない限りは1。
    printf("GC complete (%d)\n\n", allocated_size);//GC完了、確保した量を書く
}

/* */

double gc_ttime;

void gc(sht scan){
  //gc関数
  //引数は関数scan
  //恐らく、GCにかかった時間を調べる。
  struct timeval tp1, tp2;
  gettimeofday(&tp1, 0);
  
  switch(params.gctype){
    //params.gctypeはコマンドラインの第三引数。0以上の数字を指定。
    //何も無ければ0となる。
  case 0: gc_breadth_first(scan); break;//params.gctypeが0ならばgc_breadth_firstを行う。
  }
  gettimeofday(&tp2, 0); 
  gc_ttime += ((tp2.tv_sec - tp1.tv_sec) + (tp2.tv_usec - tp1.tv_usec) * 0.000001);
  //そして時間計測
  printf("gc(scan)\n");
  printf("tp1.tv_sec=%f, tp2.tv_sec=%f\n", tp1.tv_sec, tp2.tv_sec);
  printf("tp1.tv_usec=%f, tp2.tv_usec=%f\n", tp1.tv_usec, tp2.tv_usec);
}

/* memory allocation interface */

void getmem_init(gc_params p) {
  //コマンドラインから指定した、GCのパラメータが引数となる関数
  char *mp;
  static int called = 0;

  if(called)//"called"つまり「呼び出された」ということ。おそらくこの関数は一度きりしか使わない。
    return;
  called=1;//"called"にする。
  gc_ttime = 0.0;
  params = p;//gc_params型のグローバル変数params。
  if(params.tosize == 0)//params.tosizeはコマンドライン引数4で指定可能。
    params.tosize = TOSIZE;//5*1024*1024
  
  params.tosize += 3;
  params.tosize -= (params.tosize & 3);
  //3加えたのち、下位2ビットの値を0にする。
  
  if(params.stack_size == 0)//コマンドライン引数5
    params.stack_size = GC_STACK_SIZE;
  if(params.limited_stack_max == 0)//コマンドライン引数6
    params.limited_stack_max = GC_LIMITED_STACK_MAX;
  //いずれも指定出来る。指定が無かったときはデフォルトの値を与える。
  
  printf("tosize=%d, stack=%d, limit=%d\n",params.tosize,params.stack_size,params.limited_stack_max);
  //これらの値を出力する。
  
  old_memory = mymalloc(params.tosize);
  //params.tosizeだけメモリを確保し、確保したブロックの先頭の位置を入れる
  old_memory_end = old_memory + params.tosize;
  //確保したメモリの最後の位置を入れる
  new_memory = mymalloc(params.tosize);
  new_memory_end = new_memory + params.tosize;
  //old_memoryと同じことをやる。
  allocated_size = 0;
/*  
  for(mp = new_memory; mp < new_memory_end; mp+= 4096)
    *mp = 1;
  for(mp = old_memory; mp < old_memory_end; mp+= 4096)
    *mp = 1;*/
  //4096毎に区切って1を入れる。まだ意味が分からない。
}

void *try_getmem(desc_t d) {
  //getmem関数の中で使う。
  //メモリが得られるかどうか、tryする関数
  //引数は構造体
  size_t size;
  char *p;

  size = d->asize;
  if(allocated_size + size > params.tosize)//GCが必要な場合
      return 0;

  p = old_memory + allocated_size;//確保が終わったらポインタを移す。
  allocated_size += size;//今まで確保した合計分を計算。
  memset(p, 0, size);//pの指すオブジェクトの先頭size文字に、0を代入する。
  *(desc_t *)p = d;
  return p;
}

void *getmem(sht scan, desc_t d) {
  //関数scanと構造体を引数にとる
  //メモリを得る関数？
  void *p;
  p = try_getmem(d);
  if(p == 0) {//try_getmem(d)の返り値が0、すなわちメモリ不足ならばGCを実行する。
    gc(scan);
    p = try_getmem(d);
    if(p == 0)//GCを実行してもメモリが足りなければ、メモリ不足という表示を出す。
      error("getmem: Not enough memory.");
  }
  return p;//メモリ確保終了
}
