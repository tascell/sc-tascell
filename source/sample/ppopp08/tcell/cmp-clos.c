typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef unsigned int size_t;
typedef double	Align_t;

char           *lw_call(char *esp);

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

void           *thread_origin(void *farg);

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
void            (*task_doers[256]) (char *, struct thread_data *, void *);
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

void           *wait_rslt(char *esp, struct thread_data *thr);

void		send_int  (int n);

int		recv_int   (void);

void		handle_req(char *esp, closure_t *, struct thread_data *);

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

int		cmp_1      (char *esp, closure_t * _bk, struct thread_data *_thr, int n1, int n2, int *d1, int *d2);

struct do_cmp_task_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp;
  int		  j;
  int		  i;
  struct cmp     *pthis;
  struct thread_data *_thr;
  closure_t	  _bk;
};

struct _bk_in_do_cmp_task_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct do_cmp_task_frame *xfp;
};

char           *_bk_in_do_cmp_task(char *esp, void *xfp0)
{
  struct _bk_in_do_cmp_task_frame *efp;
  struct do_cmp_task_frame *xfp = xfp0;
  char           *parmp = (char *)esp;

LGOTO:
  ;
  efp = (struct _bk_in_do_cmp_task_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct _bk_in_do_cmp_task_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void		do_cmp_task(char *esp, struct thread_data *_thr, struct cmp *pthis)
{
  struct do_cmp_task_frame *efp;

LGOTO:
  ;
  efp = (struct do_cmp_task_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct do_cmp_task_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_cmp_task;
  efp->_bk.fr = (void *)efp;
  fprintf(stderr, "start %d %d\n", (*efp->pthis).n1, (*efp->pthis).n2);
  if (0 > (*efp->pthis).n2) {
    (*efp->pthis).d1 = (int *)malloc(sizeof(int) * (*efp->pthis).n1);
    (*efp->pthis).n2 = (*efp->pthis).n1;
    (*efp->pthis).d2 = (int *)malloc(sizeof(int) * (*efp->pthis).n2);
    {
      efp->i = 0;
      while (efp->i < (*efp->pthis).n1) {
	((*efp->pthis).d1)[efp->i] = efp->i;
	(efp->i)++;
      }
    }
    {
      efp->i = 0;
      while (efp->i < (*efp->pthis).n2) {
	((*efp->pthis).d2)[efp->i] = -efp->i;
	(efp->i)++;
      }
    }
  }
  while ((efp->tmp = cmp_1(esp, &efp->_bk, efp->_thr, (*efp->pthis).n1, (*efp->pthis).n2, (*efp->pthis).d1, (*efp->pthis).d2)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
    if ((char *)efp->tmp_esp < (char *)efp)
      return;
    efp->tmp_esp = 0;
    goto LGOTO;
  }
  (*efp->pthis).r = efp->tmp;
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

struct cmp_1_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  closure_t      *_bk02;
  int		  spawned2;
  struct cmp     *pthis2;
  struct cmp	  st2;
  int		  n2_2;
  int		  n2_1;
  closure_t      *_bk0;
  int		  spawned;
  struct cmp     *pthis;
  struct cmp	  st;
  int		  n1_2;
  int		  n1_1;
  int		  s2;
  int		  s1;
  int            *d2;
  int            *d1;
  int		  n2;
  int		  n1;
  struct thread_data *_thr;
  closure_t      *_bk;
  closure_t	  _bk3;
  closure_t	  _bk2;
};

struct _bk2_in_cmp_1_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct cmp_1_frame *xfp;
};

char           *_bk2_in_cmp_1(char *esp, void *xfp0)
{
  struct _bk2_in_cmp_1_frame *efp;
  struct cmp_1_frame *xfp = xfp0;
  char           *parmp = (char *)esp;
  char           *argp;

LGOTO:
  ;
  efp = (struct _bk2_in_cmp_1_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct _bk2_in_cmp_1_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  if (xfp->spawned) {
    *((int *)efp) = 0;
    return 0;
  } {
    char           *goto_fr;

    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->_bk0;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
  }
  if ((*xfp->_thr).treq_top) {
    (*xfp->pthis).n1 = xfp->n1_2;
    (*xfp->pthis).n2 = xfp->n2;
    (*xfp->pthis).d1 = xfp->d1 + xfp->n1_1;
    (*xfp->pthis).d2 = xfp->d2;
    xfp->spawned = 1;
    make_and_send_task(xfp->_thr, 0, xfp->pthis);
    *((int *)efp) = 1;
    return 0;
  }
  *((int *)efp) = 0;
  return 0;
  return 0;
}

struct _bk3_in_cmp_1_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct cmp_1_frame *xfp;
};

char           *_bk3_in_cmp_1(char *esp, void *xfp0)
{
  struct _bk3_in_cmp_1_frame *efp;
  struct cmp_1_frame *xfp = xfp0;
  char           *parmp = (char *)esp;
  char           *argp;

LGOTO:
  ;
  efp = (struct _bk3_in_cmp_1_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct _bk3_in_cmp_1_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  if (xfp->spawned2) {
    *((int *)efp) = 0;
    return 0;
  } {
    char           *goto_fr;

    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->_bk02;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
  }
  if ((*xfp->_thr).treq_top) {
    (*xfp->pthis2).n1 = xfp->n1;
    (*xfp->pthis2).n2 = xfp->n2_2;
    (*xfp->pthis2).d1 = xfp->d1;
    (*xfp->pthis2).d2 = xfp->d2 + xfp->n2_1;
    xfp->spawned2 = 1;
    make_and_send_task(xfp->_thr, 0, xfp->pthis2);
    *((int *)efp) = 1;
    return 0;
  }
  *((int *)efp) = 0;
  return 0;
  return 0;
}

int		cmp_1      (char *esp, closure_t * _bk, struct thread_data *_thr, int n1, int n2, int *d1, int *d2)
{
  struct cmp_1_frame *efp;

LGOTO:
  ;
  efp = (struct cmp_1_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct cmp_1_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->n1 = n1;
  efp->n2 = n2;
  efp->d1 = d1;
  efp->d2 = d2;
  efp->_bk3.fun = _bk3_in_cmp_1;
  efp->_bk3.fr = (void *)efp;
  efp->_bk2.fun = _bk2_in_cmp_1;
  efp->_bk2.fr = (void *)efp;
  if (efp->n1 < 5)
    return cmp_2(efp->n1, efp->n2, efp->d1, efp->d2);
  if (efp->n1 > efp->n2) {
    efp->n1_1 = efp->n1 / 2;
    efp->n1_2 = efp->n1 - efp->n1_1;
    {
      efp->pthis = &efp->st;
      efp->spawned = 0;
      if (divisibility_flag == 1) {
	send_divisible();
      } {
	efp->_bk0 = efp->_bk;
	if ((*efp->_thr).req)
	  while (handle_req(esp, &efp->_bk2, efp->_thr), (efp->tmp_esp = *((char **)esp)) != 0) {
	    if ((char *)efp->tmp_esp < (char *)efp)
	      return (int)0 - 1;
	    efp->tmp_esp = 0;
	    goto LGOTO;
	  }
	while ((efp->s1 = cmp_1(esp, &efp->_bk2, efp->_thr, efp->n1_1, efp->n2, efp->d1, efp->d2)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	  if ((char *)efp->tmp_esp < (char *)efp)
	    return (int)0 - 1;
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
	if (efp->spawned) {
	  while (wait_rslt(esp, efp->_thr), (efp->tmp_esp = *((char **)esp)) != 0) {
	    if ((char *)efp->tmp_esp < (char *)efp)
	      return (int)0 - 1;
	    efp->tmp_esp = 0;
	    goto LGOTO;
	  }
	  efp->s2 = (*efp->pthis).r;
	} else
	  while ((efp->s2 = cmp_1(esp, &efp->_bk2, efp->_thr, efp->n1_2, efp->n2, efp->d1 + efp->n1_1, efp->d2)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	    if ((char *)efp->tmp_esp < (char *)efp)
	      return (int)0 - 1;
	    efp->tmp_esp = 0;
	    goto LGOTO;
	  }
      }
    }
  } else {
    efp->n2_1 = efp->n2 / 2;
    efp->n2_2 = efp->n2 - efp->n2_1;
    {
      efp->pthis2 = &efp->st2;
      efp->spawned2 = 0;
      if (divisibility_flag == 1) {
	send_divisible();
      } {
	efp->_bk02 = efp->_bk;
	if ((*efp->_thr).req)
	  while (handle_req(esp, &efp->_bk3, efp->_thr), (efp->tmp_esp = *((char **)esp)) != 0) {
	    if ((char *)efp->tmp_esp < (char *)efp)
	      return (int)0 - 1;
	    efp->tmp_esp = 0;
	    goto LGOTO;
	  }
	while ((efp->s1 = cmp_1(esp, &efp->_bk3, efp->_thr, efp->n1, efp->n2_1, efp->d1, efp->d2)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	  if ((char *)efp->tmp_esp < (char *)efp)
	    return (int)0 - 1;
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
	if (efp->spawned2) {
	  while (wait_rslt(esp, efp->_thr), (efp->tmp_esp = *((char **)esp)) != 0) {
	    if ((char *)efp->tmp_esp < (char *)efp)
	      return (int)0 - 1;
	    efp->tmp_esp = 0;
	    goto LGOTO;
	  }
	  efp->s2 = (*efp->pthis2).r;
	} else
	  while ((efp->s2 = cmp_1(esp, &efp->_bk3, efp->_thr, efp->n1, efp->n2_2, efp->d1, efp->d2 + efp->n2_1)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	    if ((char *)efp->tmp_esp < (char *)efp)
	      return (int)0 - 1;
	    efp->tmp_esp = 0;
	    goto LGOTO;
	  }
      }
    }
  }
  return efp->s1 + efp->s2;
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
void            (*task_doers[256]) (char *, struct thread_data *, void *)= {
  (void (*) (char *, struct thread_data *, void *))do_cmp_task
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
