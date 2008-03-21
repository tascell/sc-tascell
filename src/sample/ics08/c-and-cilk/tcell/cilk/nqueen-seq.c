#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

double		elapsed_time(struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec - (tp[0]).tv_usec);
}

int nqueens(int n, int k, int *a, int *lb, int *rb)
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

int main(int argc, char *argv[])
{
  int n, rslt, i;
  int a[20], lb[40], rb[40];
  struct timeval  tp[2];

  if (argc < 2) {
    printf("%s: number of queens required\n", argv[0]);
    return 1;
  }
  if (argc > 2)
    printf("%s: extra arguments being ignored\n", argv[0]);

  n = atoi(argv[1]);

  for(i=0;i<n;i++)
    a[i] = i;
  for(i=0;i<2*n-1;i++)
    lb[i] = rb[i] = 0;

  printf("running queens %d\n", n);

  gettimeofday(tp, 0);
  rslt = nqueens(n, 0, a, lb, rb);  
  gettimeofday(tp + 1, 0);
  fprintf(stderr, "time: %lf\n", elapsed_time(tp));

  if (rslt > 0)
    printf("%d possible result!\n", rslt);
  else
    printf("no possible result!\n");

  return 0;
}
