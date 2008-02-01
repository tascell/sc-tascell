#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#define BUFSIZE 1024
#define MAXCMDC 4

struct task;
struct thread_data;

void do_task_body(struct thread_data *, void *);
void send_task_body(struct thread_data *, void *);
void *recv_task_body(struct thread_data *);
void send_rslt_body(struct thread_data *, void *);
void recv_rslt_body(struct thread_data *, void *);
enum task_stat {
  TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE,
  TASK_NONE, TASK_SUSPENDED
};
enum node {OUTSIDE, INSIDE, ANY};

enum task_home_stat {
  TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED,
  /* 結果待ちは? => task のほうでわかる? */
  TASK_HOME_DONE
};

struct task {
  enum task_stat stat;
  struct task* next;
  struct task* prev;
  void *body;
  int ndiv;
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int id;
  enum node req_from;
  struct task_home* next;
  void *body;
  char task_head[256];
};

struct thread_data {
  struct task_home * volatile req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  struct task *task_free;
  struct task *task_top;
  struct task_home* treq_free;
  struct task_home* treq_top;
  struct task_home* sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char ndiv_buf[32];
  char id_str[32];
  char buf[BUFSIZE];
};
extern int divisibility_flag;
#define FF int (*_bk)(), struct thread_data *_thr
#define FA _bk, _thr
void make_and_send_task(struct thread_data *thr, void *body);
void handle_req(FF);
#define DO_TWO(type1, struct1, work1, work2, put_task1, get_rslt1)	\
do{									\
  type1 struct1[1];							\
  int spawned = 0;							\
  if (divisibility_flag == 1) send_divisible();                         \
  {									\
    int (*_bk2)() = _bk;						\
    int _bk(){								\
      if(spawned)							\
	return 0;							\
      _bk2();								\
      if(_thr->treq_top){						\
	(put_task1);							\
	spawned = 1;							\
	make_and_send_task(_thr, struct1);				\
	return 1;							\
      }									\
      return 0;								\
    }									\
    if(_thr->req)							\
      handle_req(FA);							\
    (work1);								\
  }									\
  if(spawned){								\
    wait_rslt(_thr);							\
    (get_rslt1);							\
  } else {								\
    (work2);								\
  }									\
}while(0)

#define DO_MANY_BEGIN(type1, struct1,			\
inum1, o_inum1, o_inum2, n_inum1, n_inum2)		\
do{							\
  int inum1 = (o_inum1);				\
  int einum1 = (o_inum2);				\
  type1 *struct1;					\
  int spawned = 0;					\
  int (*_bk2)() = _bk;					\
  int _bk(){						\
    if(!spawned)					\
      _bk2();						\
    while(inum1 + 1 < einum1 && _thr->treq_top){	\
      int n_inum2 = einum1;				\
      int n_inum1 = ((inum1+1)+n_inum2)/2;		\
      einum1 = n_inum1;					\
      struct1 = (type1 *)malloc(sizeof (type1));

#define DO_MANY_BODY(struct1, inum1)		\
      spawned++;				\
      make_and_send_task(_thr, struct1);	\
    }						\
    return 0;					\
  }						\
  if (divisibility_flag == 1) send_divisible(); \
  if(_thr->req)					\
    handle_req(FA);				\
  for (; inum1 < einum1; inum1++)

#define DO_MANY_FINISH(struct1)			\
  while(spawned-- > 0){				\
    struct1 = wait_rslt(_thr);

#define DO_MANY_END(struct1)			\
    free(struct1);				\
  }						\
}while(0)

#define DO_INI_FIN(init1, body1, fin1)		\
do{						\
  { init1 ; }					\
  {						\
    int (*_bk2)() = _bk;			\
    int _bk(){					\
      { fin1 ; }				\
      _bk2();					\
      { init1 ; }				\
      return 0;					\
    }						\
    { body1 ; }					\
  }						\
  { fin1 ; }					\
}while(0)
