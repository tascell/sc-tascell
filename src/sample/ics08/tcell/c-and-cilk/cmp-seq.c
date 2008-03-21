#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

int cmp_2(int n1, int n2, int *d1, int *d2){
  int i, j;
  int s = 0;
  for(i=0;i<n1;i++)
    for(j=0;j<n2;j++)
      if((d1[i] ^ d2[j]) == -1) s++;
  return s;
}

int cmp_1(int n1, int n2, int *d1, int *d2){
  int s1, s2;
  if(n1 < 5)
    return cmp_2(n1, n2, d1, d2);
  if(n1>n2){
    int n1_1 = n1/2, n1_2 = n1 - n1_1;
    s1 = cmp_1(n1_1, n2, d1, d2);
    s2 = cmp_1(n1_2, n2, d1+n1_1, d2);
  }else{
    int n2_1 = n2/2, n2_2 = n2 - n2_1;
    s1 = cmp_1(n1, n2_1, d1, d2);
    s2 = cmp_1(n1, n2_2, d1, d2+n2_1);
  }
  return s1 + s2;
}

double
elapsed_time(struct timeval tp[2])
{
  return tp[1].tv_sec-tp[0].tv_sec+1e-6*(tp[1].tv_usec-tp[0].tv_usec);
}

int main (int argc, char *argv[]){
  struct timeval tp[2];
  int i, n1=1000, n2, *d1, *d2, s;

  if(argc > 1) n1 = atol(argv[1]);
  
  d1 = (int *)malloc(sizeof(int)*n1);
  for(i=0;i<n1;i++)
    d1[i] = i;
  n2 = n1;
  d2 = (int *)malloc(sizeof(int)*n2);
  for(i=0;i<n2;i++)
    d2[i] = -i;

  gettimeofday(tp, 0);
  s = cmp_1(n1, n2, d1, d2);
  gettimeofday(tp+1, 0);

  printf("%lf %d\n", elapsed_time(tp), s);
  return 0;
}
