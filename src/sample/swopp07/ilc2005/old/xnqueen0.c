#include <stdio.h>
#include <memory.h>
#include <sys/time.h>

#ifndef CT
#define CT
#endif


int
extend_ok(int j, int k, char *a)
{
  int i;
  for (i = 0; i < k; i++)
    if (j == a[i] || j == a[i] - (k - i) || j == a[i] + (k - i))
      return 0;
  return 1;
}

int 
nqueens( 
#ifndef RAWC
        CT int (*m)(),
#endif /* RAWC */
        int n, int k, char *a)
{
#ifndef RAWC
  CT int mon(){
    return m()+n+k;
  }
#endif /* RAWC */
  if (n == k)
    return 1;
  else
    {
      int s = 0;
      int i;
      /* try each possible position for queen <k> */
      for (i = 0; i < n; i++) {
	if (extend_ok(i, k, a)){
	  /* allocate a temporary array and copy <a> into it */
	  char *b = malloc((k + 1) * sizeof(char));
	  memcpy(b, a, k * sizeof(char));
	  b[k] = i;	  
	  s += nqueens(
#ifndef RAWC
                       mon,
#endif /* RAWC */
                       n, k + 1, b);
          free(b);
	}
      }
      return s;
    }
}

int
main(int argc, char *argv[])
{
#ifndef RAWC
  CT int mon(){
    return 0;
  }
#endif /* RAWC */
  int n, s;
  char *a;
  struct timeval tp1, tp2;
  if (argc < 2) {
    printf("%s: number of queens required\n", argv[0]);
    return 1;
  }
  n = atoi(argv[1]);
  a = alloca(n * sizeof(char));

  printf("running queens %d\n", n);
  gettimeofday (&tp1, 0);
  s = nqueens(
#ifndef RAWC
              mon,
#endif /* RAWC */
              n, 0, a);
  gettimeofday (&tp2, 0);
  printf("%d answers\n", s);
  printf("time=%f\n", 
         (tp2.tv_sec - tp1.tv_sec) + (tp2.tv_usec - tp1.tv_usec) * 0.000001);
  return 0;
}
