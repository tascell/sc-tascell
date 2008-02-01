#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

int a[20];
int lb[40];
int rb[40];


void init(int n){
  int i;
  for(i = 0 ; i < n ; i++) a[i] = i;
  for(i = 0 ; i < 2 * n + 1; i++) lb[i] = rb[i] = 0;
}

int 
nqueens(int n, int k)
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
	  int ak = a[k];
	  /* allocate a temporary array and copy <a> into it */
	  a[i] = ak; a[k] = ai;
	  lb[n - 1 - ai + k] = 1; rb[ai + k] = 1;
          if (n > k+1)
            s += nqueens(n, k + 1);
          else
            s++;
	  ai = a[k]; ak = a[i];
	  a[i] = ai; a[k] = ak;
	  lb[n - 1 - ai + k] = 0; rb[ai + k] = 0;
	}
      }
      return s;
    }
}


main(int argc, char *argv[]){
  struct timeval tv[2];
  int i, n, s;
  if (argc < 2) {
    printf("%s: number of queens required\n", argv[0]);
    return 1;
  }
  if ( argc > 2 )
    n = atoi(argv[2]);  // XCC版等との呼出し方法互換のため
  else
    n = atoi(argv[1]);
  init(n);
  gettimeofday(tv+0, 0);
  s = nqueens(n, 0);
  gettimeofday(tv+1, 0);
  printf("answers: %d\n", s);
  printf("%lf\n", tv[1].tv_sec-tv[0].tv_sec
	 +(tv[1].tv_usec-tv[0].tv_usec)*0.000001);
}
