typedef char *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t fun;
  void *fr;
} closure_t;
typedef unsigned int size_t;
typedef double Align_t;

char *lw_call(char *esp);

struct func_arg {
  void *(*func) (char *, void *);
  void *arg;
};

void *thread_origin(void *farg);

#include <pthread.h>
#include <stdio.h>
enum node { OUTSIDE, INSIDE, ANY };

struct cmd {
  int c;
  enum node node;
  char *v[5];
};

struct cmd_list {
  struct cmd cmd;
  void *body;
  struct cmd_list *next;
};

void read_to_eol(void);

void recv_rslt(struct cmd, void *);

void recv_task(struct cmd, void *);

void recv_treq(struct cmd);

void recv_rack(struct cmd);

void recv_none(struct cmd);
struct task;
struct thread_data;
void (*task_doers[256]) (char *, struct thread_data *, void *);
void (*task_senders[256]) (struct thread_data *, void *);
void *(*task_receivers[256]) (struct thread_data *);
void (*rslt_senders[256]) (struct thread_data *, void *);
void (*rslt_receivers[256]) (struct thread_data *, void *);
enum task_stat { TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE,
      TASK_NONE, TASK_SUSPENDED };
enum task_home_stat { TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED,
      TASK_HOME_DONE };

struct task {
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int id;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  char task_head[256];
};

struct thread_data {
  struct task_home *volatile req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char ndiv_buf[32];
  char tno_buf[8];
  char id_str[32];
  char buf[1024];
};
extern int divisibility_flag;

void send_divisible(void);

void make_and_send_task(struct thread_data *thr, int task_no, void *body);

void *wait_rslt(char *esp, struct thread_data *thr);

void send_int(int n);

int recv_int(void);

void handle_req(char *esp, closure_t *, struct thread_data *);

#include<sys/time.h>

int printf(char const *, ...);

int fprintf(FILE *, char const *, ...);

void *malloc(size_t);

void free(void *);

struct pentomino {
  int r;
  int k;
  int i0;
  int i1;
  int i2;
  int a[12];
  int b[70];
};

int search(char *esp, closure_t * _bk, struct thread_data *_thr, int k, int j0,
    int j1, int j2, struct pentomino *tsk);

struct do_pentomino_task_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  int i2;
  int i1;
  int i0;
  int k;
  int i;
  int tmp;
  struct pentomino *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

struct _bk_in_do_pentomino_task_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_pentomino_task_frame *xfp;
};

char *_bk_in_do_pentomino_task(char *esp, void *xfp0)
{
  struct _bk_in_do_pentomino_task_frame *efp;
  struct do_pentomino_task_frame *xfp = xfp0;
  char *parmp = (char *)esp;

LGOTO:;
  efp = (struct _bk_in_do_pentomino_task_frame *)esp;
  esp =
      (char *)((Align_t *) esp +
      (sizeof(struct _bk_in_do_pentomino_task_frame) + sizeof(Align_t) +
	  -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void do_pentomino_task(char *esp, struct thread_data *_thr,
    struct pentomino *pthis)
{
  struct do_pentomino_task_frame *efp;

LGOTO:;
  efp = (struct do_pentomino_task_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct do_pentomino_task_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_pentomino_task;
  efp->_bk.fr = (void *)efp;
  efp->k = (*efp->pthis).k;
  efp->i0 = (*efp->pthis).i0;
  efp->i1 = (*efp->pthis).i1;
  efp->i2 = (*efp->pthis).i2;
  fprintf(stderr, "start %d %d %d %d\n", efp->k, efp->i0, efp->i1, efp->i2);
  {
    while ((efp->tmp =
	    search(esp, &efp->_bk, efp->_thr, efp->k, efp->i0, efp->i1, efp->i2,
		&(*efp->pthis))) == (int)0 - 1
	&& (efp->tmp_esp = *((char **)esp)) != 0) {
      if ((char *)efp->tmp_esp < (char *)efp)
	return;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
    (*efp->pthis).r = efp->tmp;
  }
  fprintf(stderr, "end %d\n", (*efp->pthis).r);
}

struct start_pentomino {
  int r;
};

struct do_start_pentomino_task_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  struct pentomino pen;
  int tmp2;
  struct start_pentomino *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

struct _bk_in_do_start_pentomino_task_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_start_pentomino_task_frame *xfp;
};

char *_bk_in_do_start_pentomino_task(char *esp, void *xfp0)
{
  struct _bk_in_do_start_pentomino_task_frame *efp;
  struct do_start_pentomino_task_frame *xfp = xfp0;
  char *parmp = (char *)esp;

LGOTO:;
  efp = (struct _bk_in_do_start_pentomino_task_frame *)esp;
  esp =
      (char *)((Align_t *) esp +
      (sizeof(struct _bk_in_do_start_pentomino_task_frame) + sizeof(Align_t) +
	  -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void do_start_pentomino_task(char *esp, struct thread_data *_thr,
    struct start_pentomino *pthis)
{
  struct do_start_pentomino_task_frame *efp;

LGOTO:;
  efp = (struct do_start_pentomino_task_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct do_start_pentomino_task_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_start_pentomino_task;
  efp->_bk.fr = (void *)efp;
  {
    efp->i = 0;
    for (; efp->i < 12; (efp->i)++) {
      ((efp->pen).a)[efp->i] = efp->i;
    }
  }
  {
    efp->i = 0;
    for (; efp->i < 70; (efp->i)++) {
      ((efp->pen).b)[efp->i] = 0;
    }
  }
  {
    efp->i = 6;
    for (; efp->i < 70; efp->i += 7) {
      ((efp->pen).b)[efp->i] = '\n';
    }
  }
  {
    while ((efp->tmp2 =
	    search(esp, &efp->_bk, efp->_thr, 0, 0, 0, 12,
		&efp->pen)) == (int)0 - 1
	&& (efp->tmp_esp = *((char **)esp)) != 0) {
      if ((char *)efp->tmp_esp < (char *)efp)
	return;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
    (*efp->pthis).r = efp->tmp2;
  }
}
int ps[57][5] =
    { {1, 1, 1, 1}, {7, 7, 7, 7}, {4, 1, 1, 1}, {7, 7, 6, 1}, {6, 1, 1, 1}, {5,
      1, 1, 1}, {1, 1, 1, 5}, {1, 1, 1, 6}, {7, 1, 6, 7}, {7, 7, 1, 6}, {7, 6,
      1, 7}, {6, 1, 7, 7}, {7, 7, 1, 1}, {1, 1, 5, 7}, {1, 1, 7, 7}, {7, 5, 1,
      1}, {2, 5, 1, 1}, {1, 1, 5, 2}, {1, 6, 7, 1}, {1, 7, 6, 1}, {7, 1, 1, 5},
    {5, 1, 1, 7}, {7, 6, 1, 1}, {1, 1, 6, 7}, {7, 1, 1, 6}, {5, 1, 1, 6}, {6, 1,
      1, 5}, {6, 1, 1, 7}, {1, 7, 1, 6}, {7, 1, 5, 1}, {1, 5, 1, 7}, {6, 1, 7,
      1}, {7, 1, 1, 7}, {5, 1, 1, 5}, {1, 7, 7, 1}, {1, 6, 6, 1}, {6, 1, 1, 6},
    {1, 6, 1, 1}, {1, 1, 5, 1}, {1, 5, 1, 1}, {1, 1, 6, 1}, {7, 1, 6, 1}, {1, 6,
      1, 6}, {6, 1, 6, 1}, {1, 6, 1, 7}, {1, 4, 1, 1}, {1, 1, 7, 1}, {1, 7, 1,
      1}, {1, 1, 4, 1}, {7, 1, 7, 7}, {7, 6, 1, 6}, {6, 1, 6, 7}, {7, 7, 1, 7},
    {7, 1, 7, 1}, {1, 5, 1, 6}, {1, 7, 1, 7}, {6, 1, 5, 1} };
int pos[13] = { 0, 2, 4, 12, 16, 20, 24, 32, 36, 37, 45, 53, 57 };

struct search_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp4;
  int tmp3;
  int ifexp_result7;
  int ifexp_result10;
  int ifexp_result11;
  int ifexp_result12;
  int ifexp_result13;
  int ifexp_result16;
  int ifexp_result14;
  int ifexp_result15;
  int ifexp_result8;
  int ifexp_result9;
  int ifexp_result5;
  int ifexp_result6;
  int l;
  int kk;
  int *pss;
  int i;
  int ap;
  int spawned;
  struct pentomino *pthis;
  int p_end;
  int p;
  int s;
  struct pentomino *tsk;
  int j2;
  int j1;
  int j0;
  int k;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t dwind_bk;
  closure_t do_many_bk;
};

struct do_many_bk_in_search_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  int i1;
  int i2;
  int ifexp_result3;
  int ifexp_result4;
  int ifexp_result;
  int ifexp_result2;
  struct search_frame *xfp;
};

char *do_many_bk_in_search(char *esp, void *xfp0)
{
  struct do_many_bk_in_search_frame *efp;
  struct search_frame *xfp = xfp0;
  char *parmp = (char *)esp;
  char *argp;

LGOTO:;
  efp = (struct do_many_bk_in_search_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct do_many_bk_in_search_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  if (!xfp->spawned) {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->_bk;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
  }
  {
    if (xfp->p + 1 < xfp->p_end) {
      if ((*xfp->_thr).treq_top) {
	efp->ifexp_result2 = 1;
      } else {
	efp->ifexp_result2 = 0;
      }
      efp->ifexp_result = efp->ifexp_result2;
    } else {
      efp->ifexp_result = 0;
    }
    if (efp->ifexp_result) {
      while (1) {
	if (xfp->p + 1 < xfp->p_end) {
	  if ((*xfp->_thr).treq_top) {
	    efp->ifexp_result4 = 1;
	  } else {
	    efp->ifexp_result4 = 0;
	  }
	  efp->ifexp_result3 = efp->ifexp_result4;
	} else {
	  efp->ifexp_result3 = 0;
	}
	if (!efp->ifexp_result3)
	  goto loop_end;
	efp->i2 = xfp->p_end;
	efp->i1 = (1 + xfp->p + xfp->p_end) / 2;
	xfp->p_end = efp->i1;
	xfp->pthis = (struct pentomino *)malloc(sizeof(struct pentomino));
	*xfp->pthis = *xfp->tsk;
	(*xfp->pthis).k = xfp->k;
	(*xfp->pthis).i0 = xfp->j0;
	(*xfp->pthis).i1 = efp->i1;
	(*xfp->pthis).i2 = efp->i2;
	(xfp->spawned)++;
	make_and_send_task(xfp->_thr, 0, xfp->pthis);
      }
    loop_end:;
    }
  }
  *((int *)efp) = 0;
  return 0;
  return 0;
}

struct dwind_bk_in_search_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct search_frame *xfp;
};

char *dwind_bk_in_search(char *esp, void *xfp0)
{
  struct dwind_bk_in_search_frame *efp;
  struct search_frame *xfp = xfp0;
  char *parmp = (char *)esp;
  char *argp;

LGOTO:;
  efp = (struct dwind_bk_in_search_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct dwind_bk_in_search_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->xfp = xfp;
  {
    xfp->ap = ((*xfp->tsk).a)[xfp->j0];
    ((*xfp->tsk).a)[xfp->j0] = ((*xfp->tsk).a)[xfp->p];
    ((*xfp->tsk).a)[xfp->p] = xfp->ap;
    ((*xfp->tsk).b)[xfp->kk = xfp->k] = 0;
    {
      xfp->l = 0;
      for (; xfp->l < 4; (xfp->l)++) {
	((*xfp->tsk).b)[xfp->kk += (xfp->pss)[xfp->l]] = 0;
      }
    }
  }
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = &xfp->do_many_bk;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
  }
  {
    ((*xfp->tsk).b)[xfp->kk = xfp->k] = xfp->p + 'A';
    {
      xfp->l = 0;
      for (; xfp->l < 4; (xfp->l)++) {
	((*xfp->tsk).b)[xfp->kk += (xfp->pss)[xfp->l]] = xfp->p + 'A';
      }
    }
    ((*xfp->tsk).a)[xfp->p] = ((*xfp->tsk).a)[xfp->j0];
    ((*xfp->tsk).a)[xfp->j0] = xfp->ap;
  }
  return 0;
}

int search(char *esp, closure_t * _bk, struct thread_data *_thr, int k, int j0,
    int j1, int j2, struct pentomino *tsk)
{
  struct search_frame *efp;

LGOTO:;
  efp = (struct search_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct search_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->k = k;
  efp->j0 = j0;
  efp->j1 = j1;
  efp->j2 = j2;
  efp->tsk = tsk;
  efp->dwind_bk.fun = dwind_bk_in_search;
  efp->dwind_bk.fr = (void *)efp;
  efp->do_many_bk.fun = do_many_bk_in_search;
  efp->do_many_bk.fr = (void *)efp;
  efp->s = 0;
  {
    efp->p = efp->j1;
    efp->p_end = efp->j2;
    efp->spawned = 0;
    if (divisibility_flag == 1) {
      send_divisible();
    }
    if ((*efp->_thr).req)
      while (handle_req(esp, &efp->do_many_bk, efp->_thr), (efp->tmp_esp =
	      *((char **)esp)) != 0) {
	if ((char *)efp->tmp_esp < (char *)efp)
	  return (int)0 - 1;
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    for (; efp->p < efp->p_end; (efp->p)++) {
      efp->ap = ((*efp->tsk).a)[efp->p];
      {
	efp->i = pos[efp->ap];
	for (; efp->i < pos[efp->ap + 1]; (efp->i)++) {
	  efp->pss = ps[efp->i];
	  efp->kk = efp->k;
	  {
	    efp->l = 0;
	    for (; efp->l < 4; (efp->l)++) {
	      if ((efp->kk += (efp->pss)[efp->l]) >= 70) {
		efp->ifexp_result5 = 1;
	      } else {
		if (((*efp->tsk).b)[efp->kk] != 0) {
		  efp->ifexp_result6 = 1;
		} else {
		  efp->ifexp_result6 = 0;
		}
		efp->ifexp_result5 = efp->ifexp_result6;
	      }
	      if (efp->ifexp_result5)
		goto Ln;
	    }
	  }
	  {
	    {
	      ((*efp->tsk).b)[efp->kk = efp->k] = efp->p + 'A';
	      {
		efp->l = 0;
		for (; efp->l < 4; (efp->l)++) {
		  ((*efp->tsk).b)[efp->kk += (efp->pss)[efp->l]] = efp->p + 'A';
		}
	      }
	      ((*efp->tsk).a)[efp->p] = ((*efp->tsk).a)[efp->j0];
	      ((*efp->tsk).a)[efp->j0] = efp->ap;
	    }
	    {
	      {
		{
		  efp->kk = efp->k;
		  for (; efp->kk < 70; (efp->kk)++) {
		    if (((*efp->tsk).b)[efp->kk] == 0)
		      break;
		  }
		}
		if (efp->kk == 70) {
		  efp->s += 1;
		} else {
		  {
		    if (efp->kk + 7 >= 70) {
		      efp->ifexp_result8 = 1;
		    } else {
		      if (((*efp->tsk).b)[efp->kk + 7] != 0) {
			efp->ifexp_result9 = 1;
		      } else {
			efp->ifexp_result9 = 0;
		      }
		      efp->ifexp_result8 = efp->ifexp_result9;
		    }
		    if (efp->ifexp_result8) {
		      {
			if (((*efp->tsk).b)[efp->kk + 1] != 0) {
			  efp->ifexp_result11 = 1;
			} else {
			  {
			    {
			      if (efp->kk + 8 >= 70) {
				efp->ifexp_result14 = 1;
			      } else {
				if (((*efp->tsk).b)[efp->kk + 8] != 0) {
				  efp->ifexp_result15 = 1;
				} else {
				  efp->ifexp_result15 = 0;
				}
				efp->ifexp_result14 = efp->ifexp_result15;
			      }
			      if (efp->ifexp_result14) {
				if (((*efp->tsk).b)[efp->kk + 2] != 0) {
				  efp->ifexp_result16 = 1;
				} else {
				  efp->ifexp_result16 = 0;
				}
				efp->ifexp_result13 = efp->ifexp_result16;
			      } else {
				efp->ifexp_result13 = 0;
			      }
			    }
			    if (efp->ifexp_result13) {
			      efp->ifexp_result12 = 1;
			    } else {
			      efp->ifexp_result12 = 0;
			    }
			  }
			  efp->ifexp_result11 = efp->ifexp_result12;
			}
			if (efp->ifexp_result11) {
			  efp->ifexp_result10 = 1;
			} else {
			  efp->ifexp_result10 = 0;
			}
		      }
		      efp->ifexp_result7 = efp->ifexp_result10;
		    } else {
		      efp->ifexp_result7 = 0;
		    }
		  }
		  if (efp->ifexp_result7) {
		  } else {
		    while ((efp->tmp3 =
			    search(esp, &efp->dwind_bk, efp->_thr, efp->kk,
				efp->j0 + 1, efp->j0 + 1, 12,
				efp->tsk)) == (int)0 - 1
			&& (efp->tmp_esp = *((char **)esp)) != 0) {
		      if ((char *)efp->tmp_esp < (char *)efp)
			return (int)0 - 1;
		      efp->tmp_esp = 0;
		      goto LGOTO;
		    }
		    efp->s += efp->tmp3;
		  }
		}
	      }
	      {
		efp->ap = ((*efp->tsk).a)[efp->j0];
		((*efp->tsk).a)[efp->j0] = ((*efp->tsk).a)[efp->p];
		((*efp->tsk).a)[efp->p] = efp->ap;
		((*efp->tsk).b)[efp->kk = efp->k] = 0;
		{
		  efp->l = 0;
		  for (; efp->l < 4; (efp->l)++) {
		    ((*efp->tsk).b)[efp->kk += (efp->pss)[efp->l]] = 0;
		  }
		}
	      }
	    }
	  }
	Ln:;
	  continue;
	}
      }
    }
    while ((efp->spawned)-- > 0) {
      {
	while ((efp->tmp4 = wait_rslt(esp, efp->_thr)) == (void *)0 - 1
	    && (efp->tmp_esp = *((char **)esp)) != 0) {
	  if ((char *)efp->tmp_esp < (char *)efp)
	    return (int)0 - 1;
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
	efp->pthis = (struct pentomino *)efp->tmp4;
      }
      efp->s += (*efp->pthis).r;
      free(efp->pthis);
    }
  }
  return efp->s;
}

void send_start_pentomino_task(struct thread_data *_thr,
    struct start_pentomino *pthis)
{
}

struct start_pentomino *recv_start_pentomino_task(struct thread_data *_thr)
{
  struct start_pentomino *pthis = malloc(sizeof(struct start_pentomino));

  return pthis;
}

void send_start_pentomino_rslt(struct thread_data *_thr,
    struct start_pentomino *pthis)
{
  send_int((*pthis).r);
  free(pthis);
}

void recv_start_pentomino_rslt(struct thread_data *_thr,
    struct start_pentomino *pthis)
{
  (*pthis).r = recv_int();
}

void send_pentomino_task(struct thread_data *_thr, struct pentomino *pthis)
{
  send_int((*pthis).k);
  send_int((*pthis).i0);
  send_int((*pthis).i1);
  send_int((*pthis).i2);
  {
    int i3;
    int sz = 12;

    {
      i3 = 0;
      for (; i3 < sz; i3++) {
	send_int(((*pthis).a)[i3]);
      }
    }
  }
  {
    int i4;
    int sz2 = 70;

    {
      i4 = 0;
      for (; i4 < sz2; i4++) {
	send_int(((*pthis).b)[i4]);
      }
    }
  }
}

struct pentomino *recv_pentomino_task(struct thread_data *_thr)
{
  struct pentomino *pthis = malloc(sizeof(struct pentomino));

  (*pthis).k = recv_int();
  (*pthis).i0 = recv_int();
  (*pthis).i1 = recv_int();
  (*pthis).i2 = recv_int();
  {
    int i5;
    int sz3 = 12;

    {
      i5 = 0;
      for (; i5 < sz3; i5++) {
	((*pthis).a)[i5] = recv_int();
      }
    }
  }
  {
    int i6;
    int sz4 = 70;

    {
      i6 = 0;
      for (; i6 < sz4; i6++) {
	((*pthis).b)[i6] = recv_int();
      }
    }
  }
  return pthis;
}

void send_pentomino_rslt(struct thread_data *_thr, struct pentomino *pthis)
{
  send_int((*pthis).r);
  free(pthis);
}

void recv_pentomino_rslt(struct thread_data *_thr, struct pentomino *pthis)
{
  (*pthis).r = recv_int();
}

void (*task_doers[256]) (char *, struct thread_data *, void *) = {
(void (*)(char *, struct thread_data *, void *))do_pentomino_task,
      (void (*)(char *, struct thread_data *, void *))do_start_pentomino_task};
void (*task_senders[256]) (struct thread_data *, void *) = {
(void (*)(struct thread_data *, void *))send_pentomino_task,
      (void (*)(struct thread_data *, void *))send_start_pentomino_task};
void *(*task_receivers[256]) (struct thread_data *) = {
(void *(*)(struct thread_data *))recv_pentomino_task,
      (void *(*)(struct thread_data *))recv_start_pentomino_task};
void (*rslt_senders[256]) (struct thread_data *, void *) = {
(void (*)(struct thread_data *, void *))send_pentomino_rslt,
      (void (*)(struct thread_data *, void *))send_start_pentomino_rslt};
void (*rslt_receivers[256]) (struct thread_data *, void *) = {
(void (*)(struct thread_data *, void *))recv_pentomino_rslt,
      (void (*)(struct thread_data *, void *))recv_start_pentomino_rslt};
