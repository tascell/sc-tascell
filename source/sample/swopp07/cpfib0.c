#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef CT
#define CT
#endif
#ifndef CTOLD
#define CTOLD
#endif

void save_int(int i){  printf("int: %d\n", i); }
void save_pc(int pc){  printf("pc: %d\n", pc); }

int n_in;

int fib(int n)
{
  if (n <= 2)
    return 1;
  else
    {
      int s = 0;
      s += fib(n-1);
      s += fib(n-2);
      return s;
    }
}

int done = 0;


#ifndef RAWC
int cpfib(CTOLD void (*scan0) CT (), int n)
{
  int pc = 0;
  int s = 0;
  CTOLD void save1 CT (){
    scan0();
    save_pc(pc);
    save_int(s);
    save_int(n);
  }
  if (n <= 2) 
    {
      /* if(!done){ done = 1;  save1(); } */
      return 1;
    }
  else
    {
      pc = 1;
      s += cpfib(save1, n-1);
      pc = 2;
      s += cpfib(save1, n-2);
      return s;
    }
}
#endif

/* #define SUN */
#ifdef SUN
#include <sys/processor.h>
#include <sys/procset.h>
#include <sys/lwp.h>
#endif

int main(int argc, char *argv[]) {
  struct timeval tp1, tp2;
  int i;

#ifndef RAWC
  CTOLD void save1 CT (void){

  }
#endif

#ifdef SUN
  {
    lwpid_t my_lwp_id = _lwp_self(); /* ¼«¥¹¥ì¥Ã¥É¤Î£Ì£×£Ð¤Î£É£Ä¤òÆÀ¤ë */
    processorid_t p;
    for(p=0;p<100;p++) if(p_online(p, P_STATUS) == P_ONLINE) break;
    printf("lwp: %d, p: %d\n", my_lwp_id, p);
    if(processor_bind(P_LWPID, my_lwp_id, (processorid_t)p, NULL) != 0) 
      fprintf(stderr, "BIND ERROR\n");
  }
#endif

  n_in = argc > 1 ? atoi(argv[1]) : 30;

  printf("cpfib(%d)\n", n_in);

  /* time */
  gettimeofday(&tp1, 0); 

#ifdef RAWC
  i = fib(n_in);
#else
  i = cpfib(save1, n_in);
#endif

  /* time */
  gettimeofday(&tp2, 0);

  printf("fib(%d)=%d\n",  n_in, i);

  {
    double ttime = 
      (tp2.tv_sec - tp1.tv_sec) + (tp2.tv_usec - tp1.tv_usec) * 0.000001;
    printf("time: %f\n", ttime);
  }
  return 0;
}
