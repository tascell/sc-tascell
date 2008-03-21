#include<stdlib.h>
#include<pthread.h>
typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef double	Align_t;

char           *lw_call(char *esp)
{
  closure_t      *clos = *((closure_t **) esp);

  return ((*clos).fun) (esp, (*clos).fr);
}

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

struct tho_frame {
  char           *tmp_esp;
};

void           *thread_origin(void *farg)
{
  struct tho_frame *efp;
  struct func_arg *x = farg;
  void           *(*func) (char *, void *)= (*x).func;
  void           *arg = (*x).arg;
  void           *ret;
  char           *new_esp;
  char		  estack  [8192];
  char           *esp = estack;

  pthread_cleanup_push(free, farg);
  efp = (struct tho_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct tho_frame) + sizeof(Align_t) - 1) / sizeof(Align_t));
  *((char **)esp) = 0;
  new_esp = esp;
  while ((ret = func(new_esp, arg)) == (void *)0 - 1 && ((*efp).tmp_esp = *((char **)esp)) != 0) {
    lw_call((*efp).tmp_esp);
    new_esp = esp + 1;
  }
  pthread_cleanup_pop(1);
  return ret;
}
