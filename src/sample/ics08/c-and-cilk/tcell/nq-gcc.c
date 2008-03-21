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

void		handle_req(int (*) (void), struct thread_data *);

#include<sys/time.h>

int		printf     (char const *,...);

int		fprintf    (FILE *, char const *,...);

void           *malloc(size_t);

void		free      (void *);

struct nqueens {
  int		  r;
  int		  n;
  int		  k;
  int		  i1;
  int		  i2;
  int		  a        [20];
  int		  lb       [40];
  int		  rb       [40];
};

int		nqueens    (int (*_bk) (void), struct thread_data *_thr, int n, int k, int ix, int iy, struct nqueens *tsk){
  int		  s = 0;

  {
    int		    i = ix;
    int		    i_end = iy;
    struct nqueens *pthis;
    int		    spawned = 0;

    int		    do_many_bk(void){
      if (!spawned)
	_bk();
      while (i + 1 < i_end && (*_thr).treq_top) {
	int		i2 = i_end;
	int		i1 = (1 + i + i_end) / 2;
			i_end =	i1;
			pthis =       (struct nqueens *)malloc(sizeof(struct nqueens));
	               *pthis = *tsk;
	                (*pthis).k = k;
	                (*pthis).i1 = i1;
	                (*pthis).i2 = i2;
			spawned++;
			make_and_send_task(_thr, 0, pthis);
      }
      		      return  0;
    }
    if (divisibility_flag == 1)
      send_divisible();
    if ((*_thr).req)
      handle_req(do_many_bk, _thr);
    for (; i < i_end; i++) {
      int	      ai = ((*tsk).a)[i];

      if (!(((*tsk).lb)[n + -1 + -ai + k] || ((*tsk).rb)[ai + k]))
	if (k == n - 1) {
	  s++;
	} else {
	  {
	    ((*tsk).lb)[n + -1 + -ai + k] = 1;
	    ((*tsk).rb)[ai + k] = 1;
	    ((*tsk).a)[i] = ((*tsk).a)[k];
	    ((*tsk).a)[k] = ai;
	  }
	  {

	    int		    dwind_bk(void){
	      {
		((*tsk).lb)[n + -1 + -ai + k] = 0;
		((*tsk).rb)[ai + k] = 0;
		ai = ((*tsk).a)[k];
		((*tsk).a)[k] = ((*tsk).a)[i];
		((*tsk).a)[i] = ai;
	      }
	      do_many_bk();
	      {
		((*tsk).lb)[n + -1 + -ai + k] = 1;
		((*tsk).rb)[ai + k] = 1;
		((*tsk).a)[i] = ((*tsk).a)[k];
		((*tsk).a)[k] = ai;
	      }
	    }
	    {
	      s += nqueens(dwind_bk, _thr, n, k + 1, k + 1, n, tsk);
	    }
	    {
	      ((*tsk).lb)[n + -1 + -ai + k] = 0;
	      ((*tsk).rb)[ai + k] = 0;
	      ai = ((*tsk).a)[k];
	      ((*tsk).a)[k] = ((*tsk).a)[i];
	      ((*tsk).a)[i] = ai;
	    }
	  }
	}
    }
    while (spawned-- > 0) {
      pthis = (struct nqueens *)wait_rslt(_thr);
      s += (*pthis).r;
      free(pthis);
    }
  }
  return s;
}

void		do_nqueens_task(struct thread_data *_thr, struct nqueens *pthis)
{

  int		  _bk      (void){
    return 0;
  }
  int		  i;
  int		  n = (*pthis).n;
  int		  k = (*pthis).k;

  fprintf(stderr, "start %d %d %d %d\n", n, k, (*pthis).i1, (*pthis).i2);
  if (k < 0) {
    {
      i = 0;
      for (; i < n; i++) {
	((*pthis).a)[i] = i;
      }
    }
    {
      i = 0;
      for (; i < 2 * n - 1; i++) {
	((*pthis).lb)[i] = 0;
	((*pthis).rb)[i] = 0;
      }
    }
    (*pthis).r = nqueens(_bk, _thr, n, 0, 0, n, &(*pthis));
  } else {
    (*pthis).r = nqueens(_bk, _thr, n, k, (*pthis).i1, (*pthis).i2, &(*pthis));
  }
  fprintf(stderr, "end %d %d %d %d\n", k, n, (*pthis).i1, (*pthis).i2);
}

void		send_nqueens_task(struct thread_data *_thr, struct nqueens *pthis)
{
  send_int((*pthis).n);
  send_int((*pthis).k);
  int		  i = 0;

  if ((*pthis).k >= 0) {
    send_int((*pthis).i1);
    send_int((*pthis).i2);
    {
      i = 0;
      for (; i < (*pthis).n; i++) {
	send_int(((*pthis).a)[i]);
      }
    }
    {
      i = 0;
      for (; i < 2 * (*pthis).n - 1; i++) {
	send_int(((*pthis).lb)[i]);
      }
    }
    {
      i = 0;
      for (; i < 2 * (*pthis).n - 1; i++) {
	send_int(((*pthis).rb)[i]);
      }
    }
  }
}

struct nqueens *recv_nqueens_task(struct thread_data *_thr)
{
  struct nqueens *pthis = malloc(sizeof(struct nqueens));

  (*pthis).n = recv_int();
  (*pthis).k = recv_int();
  int		  i = 0;

  if ((*pthis).k >= 0) {
    (*pthis).i1 = recv_int();
    (*pthis).i2 = recv_int();
    {
      i = 0;
      for (; i < (*pthis).n; i++) {
	((*pthis).a)[i] = recv_int();
      }
    }
    {
      i = 0;
      for (; i < 2 * (*pthis).n - 1; i++) {
	((*pthis).lb)[i] = recv_int();
      }
    }
    {
      i = 0;
      for (; i < 2 * (*pthis).n - 1; i++) {
	((*pthis).rb)[i] = recv_int();
      }
    }
  }
  return pthis;
}

void		send_nqueens_rslt(struct thread_data *_thr, struct nqueens *pthis)
{
  send_int((*pthis).r);
  free(pthis);
}

void		recv_nqueens_rslt(struct thread_data *_thr, struct nqueens *pthis)
{
  (*pthis).r = recv_int();
}
void            (*task_doers[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))do_nqueens_task
};
void            (*task_senders[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))send_nqueens_task
};
void           *(*task_receivers[256]) (struct thread_data *)= {
  (void *(*) (struct thread_data *))recv_nqueens_task
};
void            (*rslt_senders[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))send_nqueens_rslt
};
void            (*rslt_receivers[256]) (struct thread_data *, void *)= {
  (void (*) (struct thread_data *, void *))recv_nqueens_rslt
};
