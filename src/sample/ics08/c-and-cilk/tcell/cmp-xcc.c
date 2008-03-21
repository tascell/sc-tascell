#include <pthread.h>
#include <stdio.h>
enum node {
  OUTSIDE, INSIDE, ANY
};

struct cmd {
  int		  c;
  enum node	  node;
  char           *v[5];
};

struct cmd_list {
  struct cmd	  cmd;
  void           *body;
  struct cmd_list *next;
};

void		read_to_eol(void);

void		recv_rslt (struct cmd, void *);

void		recv_task (struct cmd, void *);

void		recv_treq (struct cmd);

void		recv_rack (struct cmd);

void		recv_none (struct cmd);
struct task;
struct thread_data;
void            (*task_doers[256]) (struct thread_data *, void *);
void            (*task_senders[256]) (struct thread_data *, void *);
void           *(*task_receivers[256]) (struct thread_data *);
void            (*rslt_senders[256]) (struct thread_data *, void *);
void            (*rslt_receivers[256]) (struct thread_data *, void *);
enum task_stat {
  TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE, TASK_SUSPENDED
};
enum task_home_stat {
  TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE
};

struct task {
  enum task_stat  stat;
  struct task    *next;
  struct task    *prev;
  int		  task_no;
  void           *body;
  int		  ndiv;
  enum node	  rslt_to;
  char		  rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int		  id;
  int		  task_no;
  enum node	  req_from;
  struct task_home *next;
  void           *body;
  char		  task_head[256];
};

struct thread_data {
  struct task_home *volatile req;
  int		  id;
  int		  w_rack;
  int		  w_none;
  int		  ndiv;
  struct task    *task_free;
  struct task    *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t  cond;
  pthread_cond_t  cond_r;
  char		  ndiv_buf[32];
  char		  tno_buf [8];
  char		  id_str  [32];
  char		  buf     [1024];
};
extern int	divisibility_flag;

void		send_divisible(void);

void		make_and_send_task(struct thread_data *thr, int task_no, void *body);

void           *wait_rslt(struct thread_data *thr);

void		send_int  (int n);

int		recv_int   (void);

void		handle_req(int (*) lightweight(void), struct thread_data *);

#include<sys/time.h>

int		printf     (char const *,...);

int		fprintf    (FILE *, char const *,...);

void           *malloc(size_t);

void		free      (void *);

struct cmp {
  int		  r;
  int		  n1;
  int		  n2;
  int            *d1;
  int            *d2;
};

int		cmp_1      (int (*_bk) lightweight(void), struct thread_data *_thr, int n1, int n2, int *d1, int *d2);

void		do_cmp_task(struct thread_data *_thr, struct cmp *pthis)
{

  int _bk	  lightweight(void){
    return 0;
  }
  int		  i;
  int		  j;

  fprintf(stderr, "start %d %d\n", (*pthis).n1, (*pthis).n2);
  if (0 > (*pthis).n2) {
    (*pthis).d1 = (int *)malloc(sizeof(int) * (*pthis).n1);
    (*pthis).n2 = (*pthis).n1;
    (*pthis).d2 = (int *)malloc(sizeof(int) * (*pthis).n2);
    {
      i = 0;
      for (; i < (*pthis).n1; i++) {
	((*pthis).d1)[i] = i;
      }
    }
    {
      i = 0;
      for (; i < (*pthis).n2; i++) {
	((*pthis).d2)[i] = -i;
      }
    }
  }
  (*pthis).r = cmp_1(_bk, _thr, (*pthis).n1, (*pthis).n2, (*pthis).d1, (*pthis).d2);
}

int		cmp_2      (int n1, int n2, int *d1, int *d2)
{
  int		  i;
  int		  j;
  int		  s = 0;

  {
    i = 0;
    for (; i < n1; i++) {
      j = 0;
      for (; j < n2; j++) {
	if ((d1[i] ^ d2[j]) == -1)
	  s++;
      }
    }
  }
  return s;
}

int		cmp_1      (int (*_bk) lightweight(void), struct thread_data *_thr, int n1, int n2, int *d1, int *d2){
  int		  s1;
  int		  s2;

  if (n1 < 5)
    return cmp_2(n1, n2, d1, d2);
  if (n1 > n2) {
    int		    n1_1 = n1 / 2;
    int		    n1_2 = n1 - n1_1;

    {
      struct cmp      st;
      struct cmp     *pthis = &st;
      int	      spawned = 0;

      if (divisibility_flag == 1)
	send_divisible();
      {
	int             (*_bk0) lightweight(void)= _bk;

	int _bk		lightweight(void){
	  if (spawned)
	    return 0;
	  _bk0();
	  if ((*_thr).treq_top) {
	    (*pthis).n1 = n1_2;
	    (*pthis).n2 = n2;
	    (*pthis).d1 = d1 + n1_1;
	    (*pthis).d2 = d2;
	    spawned = 1;
	    make_and_send_task(_thr, 0, pthis);
	    return 1;
	  }
	  		  return	  0;
	}
	if ((*_thr).req)
	  handle_req(_bk, _thr);
	{
	  s1 = cmp_1(_bk, _thr, n1_1, n2, d1, d2);
	}
	if (spawned) {
	  wait_rslt(_thr);
	  s2 = (*pthis).r;
	} else {
	  s2 = cmp_1(_bk, _thr, n1_2, n2, d1 + n1_1, d2);
	}
      }
    }
  } else {
    int		    n2_1 = n2 / 2;
    int		    n2_2 = n2 - n2_1;

    {
      struct cmp      st2;
      struct cmp     *pthis = &st2;
      int	      spawned2 = 0;

      if (divisibility_flag == 1)
	send_divisible();
      {
	int             (*_bk0) lightweight(void)= _bk;

	int _bk		lightweight(void){
	  if (spawned2)
	    return 0;
	  _bk0();
	  if ((*_thr).treq_top) {
	    (*pthis).n1 = n1;
	    (*pthis).n2 = n2_2;
	    (*pthis).d1 = d1;
	    (*pthis).d2 = d2 + n2_1;
	    spawned2 = 1;
	    make_and_send_task(_thr, 0, pthis);
	    return 1;
	  }
	  		  return	  0;
	}
	if ((*_thr).req)
	  handle_req(_bk, _thr);
	{
	  s1 = cmp_1(_bk, _thr, n1, n2_1, d1, d2);
	}
	if (spawned2) {
	  wait_rslt(_thr);
	  s2 = (*pthis).r;
	} else {
	  s2 = cmp_1(_bk, _thr, n1, n2_2, d1, d2 + n2_1);
	}
      }
    }
  }
  return s1 + s2;
}

void		send_cmp_task(struct thread_data *_thr, struct cmp *pthis)
{
  send_int((*pthis).n1);
  send_int((*pthis).n2);
  int		  i;

  if (0 > (*pthis).n2)
    return;
  {
    i = 0;
    for (; i < (*pthis).n1; i++) {
      send_int(((*pthis).d1)[i]);
    }
  }
  {
    i = 0;
    for (; i < (*pthis).n2; i++) {
      send_int(((*pthis).d2)[i]);
    }
  }
}

struct cmp     *recv_cmp_task(struct thread_data *_thr)
{
  struct cmp     *pthis = malloc(sizeof(struct cmp));

  (*pthis).n1 = recv_int();
  (*pthis).n2 = recv_int();
  int		  i;

  if (!(0 > (*pthis).n2)) {
    (*pthis).d1 = (int *)malloc(sizeof(int) * (*pthis).n1);
    (*pthis).d2 = (int *)malloc(sizeof(int) * (*pthis).n2);
    {
      i = 0;
      for (; i < (*pthis).n1; i++) {
	((*pthis).d1)[i] = recv_int();
      }
    }
    {
      i = 0;
      for (; i < (*pthis).n2; i++) {
	((*pthis).d2)[i] = recv_int();
      }
    }
  }
  return pthis;
}

void		send_cmp_rslt(struct thread_data *_thr, struct cmp *pthis)
{
  send_int((*pthis).r);
  free((*pthis).d1);
  free((*pthis).d2);
  free(pthis);
}

void		recv_cmp_rslt(struct thread_data *_thr, struct cmp *pthis)
{
  (*pthis).r = recv_int();
}
void            (*task_doers[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))do_cmp_task
};
void            (*task_senders[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))send_cmp_task
};
void           *(*task_receivers[256]) (struct thread_data *)= {
  (void *(*) (struct thread_data *))recv_cmp_task
};
void            (*rslt_senders[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))send_cmp_rslt
};
void            (*rslt_receivers[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))recv_cmp_rslt
};
