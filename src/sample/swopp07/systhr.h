#ifndef SYSTHR_H
#define SYSTHR_H

/*
  select from:
  USE_SOLARIS_THREAD
  USE_SOLARIS_PTHREAD  
  USE_IRIX_SPROC
  USE_IRIX_PTHREAD

  set NOT_USE_PTHREAD_SCOPE_SYSTEM for those system.
  */

extern int systhr_create(void *(*start_func)(void *), void *arg);
extern int systhr_bind_to_proc(int pe);
extern int systhr_init(int np);

#endif /* not SYSTHR_H */
