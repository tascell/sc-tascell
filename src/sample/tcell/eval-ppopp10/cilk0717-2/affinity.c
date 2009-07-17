#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(int argc, char* argv[], char* envp[])
{
  int i;
  int n;
  cpu_set_t mask;
  
  if (argc < 3) {
    printf("Usage: %s <ncore> <command> [<arg> ...]\n", argv[0]);
    return 1;
  }
  
  n = atoi(argv[1]);
  
  CPU_ZERO (&mask);
  for (i=0; i<n ; i++) {
    CPU_SET (i, &mask);
  }
  
  if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) < 0) {
    perror("sched_setaffinity");
    return 1;
  }
  execvp(argv[2], argv + 2);
  perror(argv[2]);
  
  return 1;
}

