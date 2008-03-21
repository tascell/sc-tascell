typedef char *(*nestfn_t)( char *, void * );
typedef struct {
  nestfn_t fun;
  void *fr;
} closure_t;
//typedef unsigned int size_t;
typedef double Align_t;

char *lw_call (char *esp);

struct func_arg{
  void *(*func)( char *, void * );
  void *arg;
};

void *thread_origin (void *farg);
#include <pthread.h>
#include <stdio.h>
enum node { OUTSIDE, INSIDE, ANY};

struct cmd{
  int c;
  enum node node;
  char *v[5];
};

struct cmd_list{
  struct cmd cmd;
  void *body;
  struct cmd_list *next;
};

void read_to_eol (void );

void recv_rslt (struct cmd , void *);

void recv_task (struct cmd , void *);

void recv_treq (struct cmd );

void recv_rack (struct cmd );

void recv_none (struct cmd );
struct task;
struct thread_data;
void (*task_doers[256])( char *, struct thread_data *, void * );
void (*task_senders[256])( struct thread_data *, void * );
void *(*task_receivers[256])( struct thread_data * );
void (*rslt_senders[256])( struct thread_data *, void * );
void (*rslt_receivers[256])( struct thread_data *, void * );
enum task_stat { TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE, TASK_SUSPENDED};
enum task_home_stat { TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE};

struct task{
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home{
  enum task_home_stat stat;
  int id;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  char task_head[256];
};

struct thread_data{
  struct task_home * volatile req;
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

void send_divisible (void );

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (char *esp, struct thread_data *thr);

void send_int (int n);

int recv_int (void );

void handle_req (char *esp, closure_t *, struct thread_data *);
#include<sys/time.h>

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec-(tp[0]).tv_sec+1.0e-6*((tp[1]).tv_usec-(tp[0]).tv_usec);
}

void decomp_lu (char *esp, closure_t *_bk, struct thread_data *_thr, int n, double *a, int nx, int anx);

void decomp_r (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, double *b, double *a, int nx, int an);

void decomp_d (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, double *b, double *a, int nx, int an);

void decomp_rd (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, int n3, double *b1, double *b2, double *a, int nx, int anx);

void zero_mat (double *a, int n1, int n2, int nx);
enum task_tp2 { TASK_CMD, TASK_LU, TASK_R, TASK_D, TASK_RD};

struct task_lu{
  enum task_tp2 tt;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  int anx;
  int tmpnx;
};

void put_task (struct task_lu *st, enum task_tp2 tt, int n1, int n2, int n3, double *b1, double *b2, double *a, int nx, int anx)
{
  (*st).tt = tt;
  (*st).n1 = n1;
  (*st).n2 = n2;
  (*st).n3 = n3;
  (*st).b1 = b1;
  (*st).b2 = b2;
  (*st).a = a;
  (*st).nx = nx;
  (*st).anx = anx;
  (*st).tmpnx = 0;
  if(a == 0 )
    {
      (*st).tmpnx = n3;
      (*st).a = (double *)malloc(sizeof(double )*n2*n3);
      zero_mat((*st).a, n2, n3, n3);
    }
}

void merge_task (struct task_lu *x, double *a, int anx)
{
  int n2 = (*x).n2;
  int n3 = (*x).n3;
  double *tmp = (*x).a;
  int i;
  int j;
  if(a == tmp )
    return ;
  {
    i = 0;
    for(  ; i < n2 ; i++ )
      {
        j = 0;
        for(  ; j < n3 ; j++ )
          {
            a[i*anx+j] += tmp[i*n3+j];
          }
      }
  }
  free(tmp);
}

void decomp_lu_0 (int n, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    double w = 1.0/a[0];
    {
      j = 1;
      for(  ; j < n ; j++ )
        {
          a[j] *= w;
        }
    }
  }
  {
    i = 1;
    for(  ; i < n ; i++ )
      {
        {
          k = 0;
          for(  ; k < i-1 ; k++ )
            {
              double aik = a[i*anx+k];
              {
                j = k+1;
                for(  ; j < n ; j++ )
                  {
                    a[i*anx+j] -= aik*a[k*anx+j];
                  }
              }
            }
        }
        {
          double aik2 = a[i*anx+i-1];
          double w2;
          a[i*anx+i] -= aik2*a[(i-1)*anx+i];
          w2 = 1.0/a[i*anx+i];
          {
            j = i+1;
            for(  ; j < n ; j++ )
              {
                a[i*anx+j] = w2*(a[i*anx+j]-aik2*a[(i-1)*anx+j]);
              }
          }
        }
      }
  }
}

struct decomp_lu_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned;
  struct task_lu *pthis;
  struct task_lu st2;
  int n2;
  int n1;
  int ifexp_result;
  int anx;
  int nx;
  double *a;
  int n;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_two_bk;
};

struct do_two_bk_in_decomp_lu_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct decomp_lu_frame *xfp;
};

char *do_two_bk_in_decomp_lu (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk_in_decomp_lu_frame *efp;
  struct decomp_lu_frame *xfp = xfp0;
  size_t esp_flag = (size_t )esp&3;
  char *parmp = (char *)((size_t )esp^esp_flag);
  char *argp;
 LGOTO: ;
  efp = (struct do_two_bk_in_decomp_lu_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct do_two_bk_in_decomp_lu_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  if(xfp->spawned )
    {
      *((int *)efp) = 0;
      return 0;
    }
  {
    char *goto_fr;
    argp = (char *)((Align_t *)esp+(sizeof(char *)+sizeof(Align_t )+-1)/sizeof(Align_t ));
    *((closure_t **)argp) = xfp->_bk;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if(goto_fr )
      if((char *)goto_fr < (char *)efp )
        return goto_fr;
      else {
        efp->tmp_esp = 0;
        goto LGOTO;
      }
    xfp = efp->xfp;
  }
  if((*xfp->_thr).treq_top )
    {
      put_task(&(*xfp->pthis), TASK_D, xfp->n1, xfp->n2, 0, xfp->a, 0, xfp->a+xfp->n1*xfp->nx, xfp->nx, xfp->anx);
      xfp->spawned = 1;
      make_and_send_task(xfp->_thr, 0, xfp->pthis);
      *((int *)efp) = 1;
      return 0;
    }
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void decomp_lu (char *esp, closure_t *_bk, struct thread_data *_thr, int n, double *a, int nx, int anx)
{
  struct decomp_lu_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int ifexp_result;
  int n1;
  int n2;
  struct task_lu *pthis;
  int spawned;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct decomp_lu_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct decomp_lu_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL;
        case 1:
          goto L_CALL2;
        case 2:
          goto L_CALL3;
        case 3:
          goto L_CALL4;
        case 4:
          goto L_CALL5;
        case 5:
          goto L_CALL6;
        case 6:
          goto L_CALL7;
        }
      goto L_CALL;
    }
  efp = (struct decomp_lu_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct decomp_lu_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  if(n <= 4 )
    {
      decomp_lu_0(n, a, nx, anx);
      return ;
    }
  {
    if(n > 16 )
      {
        ifexp_result = n/4;
      }
    else {
      ifexp_result = n/2;
    }
    {
      n1 = ifexp_result;
      n2 = n-n1;
      new_esp = esp;
      while(__builtin_expect(( decomp_lu(new_esp, _bk, _thr, n1, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->n2 = n2;
          efp->n1 = n1;
          efp->ifexp_result = ifexp_result;
          efp->anx = anx;
          efp->nx = nx;
          efp->a = a;
          efp->n = n;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->call_id = 0;
          return ;
        L_CALL: ;
          n2 = efp->n2;
          n1 = efp->n1;
          ifexp_result = efp->ifexp_result;
          anx = efp->anx;
          nx = efp->nx;
          a = efp->a;
          n = efp->n;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
      {
        pthis = &efp->st2;
        spawned = 0;
        if(divisibility_flag == 1 )
          {
            send_divisible();
          }
        {
          if((*_thr).req )
            {
              new_esp = esp;
              while(__builtin_expect(( handle_req(new_esp, &efp->do_two_bk, _thr),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
                {
                  efp->spawned = spawned;
                  efp->pthis = pthis;
                  efp->n2 = n2;
                  efp->n1 = n1;
                  efp->ifexp_result = ifexp_result;
                  efp->anx = anx;
                  efp->nx = nx;
                  efp->a = a;
                  efp->n = n;
                  efp->_thr = _thr;
                  efp->_bk = _bk;
                  efp->do_two_bk.fun = do_two_bk_in_decomp_lu;
                  efp->do_two_bk.fr = (void *)efp;
                  efp->call_id = 1;
                  return ;
                L_CALL2: ;
                  spawned = efp->spawned;
                  pthis = efp->pthis;
                  n2 = efp->n2;
                  n1 = efp->n1;
                  ifexp_result = efp->ifexp_result;
                  anx = efp->anx;
                  nx = efp->nx;
                  a = efp->a;
                  n = efp->n;
                  _thr = efp->_thr;
                  _bk = efp->_bk;
                  new_esp = esp+1;
                }
            }
          {
            new_esp = esp;
            while(__builtin_expect(( decomp_r(new_esp, &efp->do_two_bk, _thr, n1, n2, a, a+n1, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
              {
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->ifexp_result = ifexp_result;
                efp->anx = anx;
                efp->nx = nx;
                efp->a = a;
                efp->n = n;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk.fun = do_two_bk_in_decomp_lu;
                efp->do_two_bk.fr = (void *)efp;
                efp->call_id = 2;
                return ;
              L_CALL3: ;
                spawned = efp->spawned;
                pthis = efp->pthis;
                n2 = efp->n2;
                n1 = efp->n1;
                ifexp_result = efp->ifexp_result;
                anx = efp->anx;
                nx = efp->nx;
                a = efp->a;
                n = efp->n;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp+1;
              }
          }
        }
        if(spawned )
          {
            new_esp = esp;
            while(__builtin_expect(( wait_rslt(new_esp, _thr),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
              {
                efp->spawned = spawned;
                efp->pthis = pthis;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->ifexp_result = ifexp_result;
                efp->anx = anx;
                efp->nx = nx;
                efp->a = a;
                efp->n = n;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk.fun = do_two_bk_in_decomp_lu;
                efp->do_two_bk.fr = (void *)efp;
                efp->call_id = 3;
                return ;
              L_CALL4: ;
                spawned = efp->spawned;
                pthis = efp->pthis;
                n2 = efp->n2;
                n1 = efp->n1;
                ifexp_result = efp->ifexp_result;
                anx = efp->anx;
                nx = efp->nx;
                a = efp->a;
                n = efp->n;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp+1;
              }
          }
        else {
          new_esp = esp;
          while(__builtin_expect(( decomp_d(new_esp, _bk, _thr, n1, n2, a, a+n1*nx, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
            {
              efp->spawned = spawned;
              efp->pthis = pthis;
              efp->n2 = n2;
              efp->n1 = n1;
              efp->ifexp_result = ifexp_result;
              efp->anx = anx;
              efp->nx = nx;
              efp->a = a;
              efp->n = n;
              efp->_thr = _thr;
              efp->_bk = _bk;
              efp->do_two_bk.fun = do_two_bk_in_decomp_lu;
              efp->do_two_bk.fr = (void *)efp;
              efp->call_id = 4;
              return ;
            L_CALL5: ;
              spawned = efp->spawned;
              pthis = efp->pthis;
              n2 = efp->n2;
              n1 = efp->n1;
              ifexp_result = efp->ifexp_result;
              anx = efp->anx;
              nx = efp->nx;
              a = efp->a;
              n = efp->n;
              _thr = efp->_thr;
              _bk = efp->_bk;
              new_esp = esp+1;
            }
        }
      }
      new_esp = esp;
      while(__builtin_expect(( decomp_rd(new_esp, _bk, _thr, n1, n2, n2, a+n1*nx, a+n1, a+(n1*nx+n1), nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->spawned = spawned;
          efp->pthis = pthis;
          efp->n2 = n2;
          efp->n1 = n1;
          efp->ifexp_result = ifexp_result;
          efp->anx = anx;
          efp->nx = nx;
          efp->a = a;
          efp->n = n;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->do_two_bk.fun = do_two_bk_in_decomp_lu;
          efp->do_two_bk.fr = (void *)efp;
          efp->call_id = 5;
          return ;
        L_CALL6: ;
          spawned = efp->spawned;
          pthis = efp->pthis;
          n2 = efp->n2;
          n1 = efp->n1;
          ifexp_result = efp->ifexp_result;
          anx = efp->anx;
          nx = efp->nx;
          a = efp->a;
          n = efp->n;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
      new_esp = esp;
      while(__builtin_expect(( decomp_lu(new_esp, _bk, _thr, n2, a+(n1*nx+n1), nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->spawned = spawned;
          efp->pthis = pthis;
          efp->n2 = n2;
          efp->n1 = n1;
          efp->ifexp_result = ifexp_result;
          efp->anx = anx;
          efp->nx = nx;
          efp->a = a;
          efp->n = n;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->do_two_bk.fun = do_two_bk_in_decomp_lu;
          efp->do_two_bk.fr = (void *)efp;
          efp->call_id = 6;
          return ;
        L_CALL7: ;
          spawned = efp->spawned;
          pthis = efp->pthis;
          n2 = efp->n2;
          n1 = efp->n1;
          ifexp_result = efp->ifexp_result;
          anx = efp->anx;
          nx = efp->nx;
          a = efp->a;
          n = efp->n;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
    }
  }
}

void decomp_r_0 (int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    double w = 1.0/b[0];
    {
      j = 0;
      for(  ; j < n2 ; j++ )
        {
          a[j] *= w;
        }
    }
  }
  {
    i = 1;
    for(  ; i < n1 ; i++ )
      {
        {
          k = 0;
          for(  ; k < i-1 ; k++ )
            {
              double aik = b[i*nx+k];
              {
                j = 0;
                for(  ; j < n2 ; j++ )
                  {
                    a[i*anx+j] -= aik*a[k*anx+j];
                  }
              }
            }
        }
        {
          double aik3 = b[i*nx+i-1];
          double w3 = 1.0/b[i*nx+i];
          {
            j = 0;
            for(  ; j < n2 ; j++ )
              {
                a[i*anx+j] = w3*(a[i*anx+j]-aik3*a[(i-1)*anx+j]);
              }
          }
        }
      }
  }
}

struct decomp_r_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  int n1_2;
  int n1_1;
  int anx;
  int nx;
  double *a;
  double *b;
  int n2;
  int n1;
  struct thread_data *_thr;
  closure_t *_bk;
};

void decomp_r (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, double *b, double *a, int nx, int anx)
{
  struct decomp_r_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int n1_1;
  int n1_2;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct decomp_r_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct decomp_r_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL8;
        case 1:
          goto L_CALL9;
        case 2:
          goto L_CALL10;
        }
      goto L_CALL8;
    }
  efp = (struct decomp_r_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct decomp_r_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  if(n1 <= 4 )
    {
      decomp_r_0(n1, n2, b, a, nx, anx);
      return ;
    }
  {
    n1_1 = n1/2;
    n1_2 = n1-n1_1;
    new_esp = esp;
    while(__builtin_expect(( decomp_r(new_esp, _bk, _thr, n1_1, n2, b, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
      {
        efp->n1_2 = n1_2;
        efp->n1_1 = n1_1;
        efp->anx = anx;
        efp->nx = nx;
        efp->a = a;
        efp->b = b;
        efp->n2 = n2;
        efp->n1 = n1;
        efp->_thr = _thr;
        efp->_bk = _bk;
        efp->call_id = 0;
        return ;
      L_CALL8: ;
        n1_2 = efp->n1_2;
        n1_1 = efp->n1_1;
        anx = efp->anx;
        nx = efp->nx;
        a = efp->a;
        b = efp->b;
        n2 = efp->n2;
        n1 = efp->n1;
        _thr = efp->_thr;
        _bk = efp->_bk;
        new_esp = esp+1;
      }
    new_esp = esp;
    while(__builtin_expect(( decomp_rd(new_esp, _bk, _thr, n1_1, n1_2, n2, b+n1_1*nx, a, a+n1_1*nx, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
      {
        efp->n1_2 = n1_2;
        efp->n1_1 = n1_1;
        efp->anx = anx;
        efp->nx = nx;
        efp->a = a;
        efp->b = b;
        efp->n2 = n2;
        efp->n1 = n1;
        efp->_thr = _thr;
        efp->_bk = _bk;
        efp->call_id = 1;
        return ;
      L_CALL9: ;
        n1_2 = efp->n1_2;
        n1_1 = efp->n1_1;
        anx = efp->anx;
        nx = efp->nx;
        a = efp->a;
        b = efp->b;
        n2 = efp->n2;
        n1 = efp->n1;
        _thr = efp->_thr;
        _bk = efp->_bk;
        new_esp = esp+1;
      }
    new_esp = esp;
    while(__builtin_expect(( decomp_r(new_esp, _bk, _thr, n1_2, n2, b+(n1_1*nx+n1_1), a+n1_1*nx, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
      {
        efp->n1_2 = n1_2;
        efp->n1_1 = n1_1;
        efp->anx = anx;
        efp->nx = nx;
        efp->a = a;
        efp->b = b;
        efp->n2 = n2;
        efp->n1 = n1;
        efp->_thr = _thr;
        efp->_bk = _bk;
        efp->call_id = 2;
        return ;
      L_CALL10: ;
        n1_2 = efp->n1_2;
        n1_1 = efp->n1_1;
        anx = efp->anx;
        nx = efp->nx;
        a = efp->a;
        b = efp->b;
        n2 = efp->n2;
        n1 = efp->n1;
        _thr = efp->_thr;
        _bk = efp->_bk;
        new_esp = esp+1;
      }
  }
}

void decomp_d_0 (int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    i = 0;
    for(  ; i < n2 ; i++ )
      {
        k = 0;
        for(  ; k < n1-1 ; k++ )
          {
            double aik = a[i*anx+k];
            {
              j = k+1;
              for(  ; j < n1 ; j++ )
                {
                  a[i*anx+j] -= aik*b[k*nx+j];
                }
            }
          }
      }
  }
}

struct decomp_d_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned2;
  struct task_lu *pthis;
  struct task_lu st3;
  int n2_2;
  int n2_1;
  int anx;
  int nx;
  double *a;
  double *b;
  int n2;
  int n1;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_two_bk2;
};

struct do_two_bk2_in_decomp_d_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct decomp_d_frame *xfp;
};

char *do_two_bk2_in_decomp_d (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk2_in_decomp_d_frame *efp;
  struct decomp_d_frame *xfp = xfp0;
  size_t esp_flag = (size_t )esp&3;
  char *parmp = (char *)((size_t )esp^esp_flag);
  char *argp;
 LGOTO: ;
  efp = (struct do_two_bk2_in_decomp_d_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct do_two_bk2_in_decomp_d_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  if(xfp->spawned2 )
    {
      *((int *)efp) = 0;
      return 0;
    }
  {
    char *goto_fr;
    argp = (char *)((Align_t *)esp+(sizeof(char *)+sizeof(Align_t )+-1)/sizeof(Align_t ));
    *((closure_t **)argp) = xfp->_bk;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if(goto_fr )
      if((char *)goto_fr < (char *)efp )
        return goto_fr;
      else {
        efp->tmp_esp = 0;
        goto LGOTO;
      }
    xfp = efp->xfp;
  }
  if((*xfp->_thr).treq_top )
    {
      put_task(&(*xfp->pthis), TASK_D, xfp->n1, xfp->n2_2, 0, xfp->b, 0, xfp->a+xfp->n2_1*xfp->nx, xfp->nx, xfp->anx);
      xfp->spawned2 = 1;
      make_and_send_task(xfp->_thr, 0, xfp->pthis);
      *((int *)efp) = 1;
      return 0;
    }
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void decomp_d (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, double *b, double *a, int nx, int anx)
{
  struct decomp_d_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int n2_1;
  int n2_2;
  struct task_lu *pthis;
  int spawned2;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct decomp_d_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct decomp_d_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL11;
        case 1:
          goto L_CALL12;
        case 2:
          goto L_CALL13;
        case 3:
          goto L_CALL14;
        }
      goto L_CALL11;
    }
  efp = (struct decomp_d_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct decomp_d_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  if(n2 <= 4 )
    {
      decomp_d_0(n1, n2, b, a, nx, anx);
      return ;
    }
  {
    n2_1 = n2/2;
    n2_2 = n2-n2_1;
    {
      pthis = &efp->st3;
      spawned2 = 0;
      if(divisibility_flag == 1 )
        {
          send_divisible();
        }
      {
        if((*_thr).req )
          {
            new_esp = esp;
            while(__builtin_expect(( handle_req(new_esp, &efp->do_two_bk2, _thr),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
              {
                efp->spawned2 = spawned2;
                efp->pthis = pthis;
                efp->n2_2 = n2_2;
                efp->n2_1 = n2_1;
                efp->anx = anx;
                efp->nx = nx;
                efp->a = a;
                efp->b = b;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk2.fun = do_two_bk2_in_decomp_d;
                efp->do_two_bk2.fr = (void *)efp;
                efp->call_id = 0;
                return ;
              L_CALL11: ;
                spawned2 = efp->spawned2;
                pthis = efp->pthis;
                n2_2 = efp->n2_2;
                n2_1 = efp->n2_1;
                anx = efp->anx;
                nx = efp->nx;
                a = efp->a;
                b = efp->b;
                n2 = efp->n2;
                n1 = efp->n1;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp+1;
              }
          }
        {
          new_esp = esp;
          while(__builtin_expect(( decomp_d(new_esp, &efp->do_two_bk2, _thr, n1, n2_1, b, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
            {
              efp->spawned2 = spawned2;
              efp->pthis = pthis;
              efp->n2_2 = n2_2;
              efp->n2_1 = n2_1;
              efp->anx = anx;
              efp->nx = nx;
              efp->a = a;
              efp->b = b;
              efp->n2 = n2;
              efp->n1 = n1;
              efp->_thr = _thr;
              efp->_bk = _bk;
              efp->do_two_bk2.fun = do_two_bk2_in_decomp_d;
              efp->do_two_bk2.fr = (void *)efp;
              efp->call_id = 1;
              return ;
            L_CALL12: ;
              spawned2 = efp->spawned2;
              pthis = efp->pthis;
              n2_2 = efp->n2_2;
              n2_1 = efp->n2_1;
              anx = efp->anx;
              nx = efp->nx;
              a = efp->a;
              b = efp->b;
              n2 = efp->n2;
              n1 = efp->n1;
              _thr = efp->_thr;
              _bk = efp->_bk;
              new_esp = esp+1;
            }
        }
      }
      if(spawned2 )
        {
          new_esp = esp;
          while(__builtin_expect(( wait_rslt(new_esp, _thr),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
            {
              efp->spawned2 = spawned2;
              efp->pthis = pthis;
              efp->n2_2 = n2_2;
              efp->n2_1 = n2_1;
              efp->anx = anx;
              efp->nx = nx;
              efp->a = a;
              efp->b = b;
              efp->n2 = n2;
              efp->n1 = n1;
              efp->_thr = _thr;
              efp->_bk = _bk;
              efp->do_two_bk2.fun = do_two_bk2_in_decomp_d;
              efp->do_two_bk2.fr = (void *)efp;
              efp->call_id = 2;
              return ;
            L_CALL13: ;
              spawned2 = efp->spawned2;
              pthis = efp->pthis;
              n2_2 = efp->n2_2;
              n2_1 = efp->n2_1;
              anx = efp->anx;
              nx = efp->nx;
              a = efp->a;
              b = efp->b;
              n2 = efp->n2;
              n1 = efp->n1;
              _thr = efp->_thr;
              _bk = efp->_bk;
              new_esp = esp+1;
            }
        }
      else {
        new_esp = esp;
        while(__builtin_expect(( decomp_d(new_esp, _bk, _thr, n1, n2_2, b, a+n2_1*nx, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->spawned2 = spawned2;
            efp->pthis = pthis;
            efp->n2_2 = n2_2;
            efp->n2_1 = n2_1;
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b = b;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_two_bk2.fun = do_two_bk2_in_decomp_d;
            efp->do_two_bk2.fr = (void *)efp;
            efp->call_id = 3;
            return ;
          L_CALL14: ;
            spawned2 = efp->spawned2;
            pthis = efp->pthis;
            n2_2 = efp->n2_2;
            n2_1 = efp->n2_1;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b = efp->b;
            n2 = efp->n2;
            n1 = efp->n1;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp+1;
          }
      }
    }
  }
}

void decomp_rd_0 (int n1, int n2, int n3, double *b1, double *b2, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    i = 0;
    for(  ; i < n2 ; i++ )
      {
        k = 0;
        for(  ; k < n1 ; k++ )
          {
            double aik = b1[i*nx+k];
            {
              j = 0;
              for(  ; j < n3 ; j++ )
                {
                  a[i*anx+j] -= aik*b2[k*nx+j];
                }
            }
          }
      }
  }
}

struct decomp_rd_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned3;
  struct task_lu *pthis;
  struct task_lu st4;
  int n2_2;
  int n2_1;
  int n1_2;
  int n1_1;
  int ifexp_result2;
  int ifexp_result3;
  int anx;
  int nx;
  double *a;
  double *b2;
  double *b1;
  int n3;
  int n2;
  int n1;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_two_bk3;
};

struct do_two_bk3_in_decomp_rd_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct decomp_rd_frame *xfp;
};

char *do_two_bk3_in_decomp_rd (char *esp, void *xfp0)
{
  char *new_esp;
  struct do_two_bk3_in_decomp_rd_frame *efp;
  struct decomp_rd_frame *xfp = xfp0;
  size_t esp_flag = (size_t )esp&3;
  char *parmp = (char *)((size_t )esp^esp_flag);
  char *argp;
 LGOTO: ;
  efp = (struct do_two_bk3_in_decomp_rd_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct do_two_bk3_in_decomp_rd_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  if(xfp->spawned3 )
    {
      *((int *)efp) = 0;
      return 0;
    }
  {
    char *goto_fr;
    argp = (char *)((Align_t *)esp+(sizeof(char *)+sizeof(Align_t )+-1)/sizeof(Align_t ));
    *((closure_t **)argp) = xfp->_bk;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if(goto_fr )
      if((char *)goto_fr < (char *)efp )
        return goto_fr;
      else {
        efp->tmp_esp = 0;
        goto LGOTO;
      }
    xfp = efp->xfp;
  }
  if((*xfp->_thr).treq_top )
    {
      put_task(&(*xfp->pthis), TASK_RD, xfp->n1, xfp->n2_2, xfp->n3, xfp->b1+xfp->n2_1*xfp->nx, xfp->b2, xfp->a+xfp->n2_1*xfp->anx, xfp->nx, xfp->anx);
      xfp->spawned3 = 1;
      make_and_send_task(xfp->_thr, 0, xfp->pthis);
      *((int *)efp) = 1;
      return 0;
    }
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void decomp_rd (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, int n3, double *b1, double *b2, double *a, int nx, int anx)
{
  struct decomp_rd_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int ifexp_result3;
  int ifexp_result2;
  int n1_1;
  int n1_2;
  int n2_1;
  int n2_2;
  struct task_lu *pthis;
  int spawned3;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct decomp_rd_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct decomp_rd_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL15;
        case 1:
          goto L_CALL16;
        case 2:
          goto L_CALL17;
        case 3:
          goto L_CALL18;
        case 4:
          goto L_CALL19;
        case 5:
          goto L_CALL20;
        }
      goto L_CALL15;
    }
  efp = (struct decomp_rd_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct decomp_rd_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  {
    if(n1 <= 4 )
      {
        if(n2 <= 4 )
          {
            ifexp_result3 = 1;
          }
        else {
          ifexp_result3 = 0;
        }
        ifexp_result2 = ifexp_result3;
      }
    else {
      ifexp_result2 = 0;
    }
    if(ifexp_result2 )
      {
        decomp_rd_0(n1, n2, n3, b1, b2, a, nx, anx);
        return ;
      }
  }
  if(n1 > n2 )
    {
      n1_1 = n1/2;
      n1_2 = n1-n1_1;
      new_esp = esp;
      while(__builtin_expect(( decomp_rd(new_esp, _bk, _thr, n1_1, n2, n3, b1, b2, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->n1_2 = n1_2;
          efp->n1_1 = n1_1;
          efp->ifexp_result2 = ifexp_result2;
          efp->ifexp_result3 = ifexp_result3;
          efp->anx = anx;
          efp->nx = nx;
          efp->a = a;
          efp->b2 = b2;
          efp->b1 = b1;
          efp->n3 = n3;
          efp->n2 = n2;
          efp->n1 = n1;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->call_id = 0;
          return ;
        L_CALL15: ;
          n1_2 = efp->n1_2;
          n1_1 = efp->n1_1;
          ifexp_result2 = efp->ifexp_result2;
          ifexp_result3 = efp->ifexp_result3;
          anx = efp->anx;
          nx = efp->nx;
          a = efp->a;
          b2 = efp->b2;
          b1 = efp->b1;
          n3 = efp->n3;
          n2 = efp->n2;
          n1 = efp->n1;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
      new_esp = esp;
      while(__builtin_expect(( decomp_rd(new_esp, _bk, _thr, n1_2, n2, n3, b1+n1_1, b2+n1_1*nx, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->n1_2 = n1_2;
          efp->n1_1 = n1_1;
          efp->ifexp_result2 = ifexp_result2;
          efp->ifexp_result3 = ifexp_result3;
          efp->anx = anx;
          efp->nx = nx;
          efp->a = a;
          efp->b2 = b2;
          efp->b1 = b1;
          efp->n3 = n3;
          efp->n2 = n2;
          efp->n1 = n1;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->call_id = 1;
          return ;
        L_CALL16: ;
          n1_2 = efp->n1_2;
          n1_1 = efp->n1_1;
          ifexp_result2 = efp->ifexp_result2;
          ifexp_result3 = efp->ifexp_result3;
          anx = efp->anx;
          nx = efp->nx;
          a = efp->a;
          b2 = efp->b2;
          b1 = efp->b1;
          n3 = efp->n3;
          n2 = efp->n2;
          n1 = efp->n1;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
    }
  else {
    n2_1 = n2/2;
    n2_2 = n2-n2_1;
    {
      pthis = &efp->st4;
      spawned3 = 0;
      if(divisibility_flag == 1 )
        {
          send_divisible();
        }
      {
        if((*_thr).req )
          {
            new_esp = esp;
            while(__builtin_expect(( handle_req(new_esp, &efp->do_two_bk3, _thr),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
              {
                efp->spawned3 = spawned3;
                efp->pthis = pthis;
                efp->n2_2 = n2_2;
                efp->n2_1 = n2_1;
                efp->n1_2 = n1_2;
                efp->n1_1 = n1_1;
                efp->ifexp_result2 = ifexp_result2;
                efp->ifexp_result3 = ifexp_result3;
                efp->anx = anx;
                efp->nx = nx;
                efp->a = a;
                efp->b2 = b2;
                efp->b1 = b1;
                efp->n3 = n3;
                efp->n2 = n2;
                efp->n1 = n1;
                efp->_thr = _thr;
                efp->_bk = _bk;
                efp->do_two_bk3.fun = do_two_bk3_in_decomp_rd;
                efp->do_two_bk3.fr = (void *)efp;
                efp->call_id = 2;
                return ;
              L_CALL17: ;
                spawned3 = efp->spawned3;
                pthis = efp->pthis;
                n2_2 = efp->n2_2;
                n2_1 = efp->n2_1;
                n1_2 = efp->n1_2;
                n1_1 = efp->n1_1;
                ifexp_result2 = efp->ifexp_result2;
                ifexp_result3 = efp->ifexp_result3;
                anx = efp->anx;
                nx = efp->nx;
                a = efp->a;
                b2 = efp->b2;
                b1 = efp->b1;
                n3 = efp->n3;
                n2 = efp->n2;
                n1 = efp->n1;
                _thr = efp->_thr;
                _bk = efp->_bk;
                new_esp = esp+1;
              }
          }
        {
          new_esp = esp;
          while(__builtin_expect(( decomp_rd(new_esp, &efp->do_two_bk3, _thr, n1, n2_1, n3, b1, b2, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
            {
              efp->spawned3 = spawned3;
              efp->pthis = pthis;
              efp->n2_2 = n2_2;
              efp->n2_1 = n2_1;
              efp->n1_2 = n1_2;
              efp->n1_1 = n1_1;
              efp->ifexp_result2 = ifexp_result2;
              efp->ifexp_result3 = ifexp_result3;
              efp->anx = anx;
              efp->nx = nx;
              efp->a = a;
              efp->b2 = b2;
              efp->b1 = b1;
              efp->n3 = n3;
              efp->n2 = n2;
              efp->n1 = n1;
              efp->_thr = _thr;
              efp->_bk = _bk;
              efp->do_two_bk3.fun = do_two_bk3_in_decomp_rd;
              efp->do_two_bk3.fr = (void *)efp;
              efp->call_id = 3;
              return ;
            L_CALL18: ;
              spawned3 = efp->spawned3;
              pthis = efp->pthis;
              n2_2 = efp->n2_2;
              n2_1 = efp->n2_1;
              n1_2 = efp->n1_2;
              n1_1 = efp->n1_1;
              ifexp_result2 = efp->ifexp_result2;
              ifexp_result3 = efp->ifexp_result3;
              anx = efp->anx;
              nx = efp->nx;
              a = efp->a;
              b2 = efp->b2;
              b1 = efp->b1;
              n3 = efp->n3;
              n2 = efp->n2;
              n1 = efp->n1;
              _thr = efp->_thr;
              _bk = efp->_bk;
              new_esp = esp+1;
            }
        }
      }
      if(spawned3 )
        {
          new_esp = esp;
          while(__builtin_expect(( wait_rslt(new_esp, _thr),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
            {
              efp->spawned3 = spawned3;
              efp->pthis = pthis;
              efp->n2_2 = n2_2;
              efp->n2_1 = n2_1;
              efp->n1_2 = n1_2;
              efp->n1_1 = n1_1;
              efp->ifexp_result2 = ifexp_result2;
              efp->ifexp_result3 = ifexp_result3;
              efp->anx = anx;
              efp->nx = nx;
              efp->a = a;
              efp->b2 = b2;
              efp->b1 = b1;
              efp->n3 = n3;
              efp->n2 = n2;
              efp->n1 = n1;
              efp->_thr = _thr;
              efp->_bk = _bk;
              efp->do_two_bk3.fun = do_two_bk3_in_decomp_rd;
              efp->do_two_bk3.fr = (void *)efp;
              efp->call_id = 4;
              return ;
            L_CALL19: ;
              spawned3 = efp->spawned3;
              pthis = efp->pthis;
              n2_2 = efp->n2_2;
              n2_1 = efp->n2_1;
              n1_2 = efp->n1_2;
              n1_1 = efp->n1_1;
              ifexp_result2 = efp->ifexp_result2;
              ifexp_result3 = efp->ifexp_result3;
              anx = efp->anx;
              nx = efp->nx;
              a = efp->a;
              b2 = efp->b2;
              b1 = efp->b1;
              n3 = efp->n3;
              n2 = efp->n2;
              n1 = efp->n1;
              _thr = efp->_thr;
              _bk = efp->_bk;
              new_esp = esp+1;
            }
          merge_task(&(*pthis), a+n2_1*anx, anx);
        }
      else {
        new_esp = esp;
        while(__builtin_expect(( decomp_rd(new_esp, _bk, _thr, n1, n2_2, n3, b1+n2_1*nx, b2, a+n2_1*anx, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->spawned3 = spawned3;
            efp->pthis = pthis;
            efp->n2_2 = n2_2;
            efp->n2_1 = n2_1;
            efp->n1_2 = n1_2;
            efp->n1_1 = n1_1;
            efp->ifexp_result2 = ifexp_result2;
            efp->ifexp_result3 = ifexp_result3;
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b2 = b2;
            efp->b1 = b1;
            efp->n3 = n3;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->do_two_bk3.fun = do_two_bk3_in_decomp_rd;
            efp->do_two_bk3.fr = (void *)efp;
            efp->call_id = 5;
            return ;
          L_CALL20: ;
            spawned3 = efp->spawned3;
            pthis = efp->pthis;
            n2_2 = efp->n2_2;
            n2_1 = efp->n2_1;
            n1_2 = efp->n1_2;
            n1_1 = efp->n1_1;
            ifexp_result2 = efp->ifexp_result2;
            ifexp_result3 = efp->ifexp_result3;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b2 = efp->b2;
            b1 = efp->b1;
            n3 = efp->n3;
            n2 = efp->n2;
            n1 = efp->n1;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp+1;
          }
      }
    }
  }
}

struct decomp_lu_1_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  int n1;
  int i;
  int anx;
  int nx;
  double *a;
  int n;
  struct thread_data *_thr;
  closure_t *_bk;
};

void decomp_lu_1 (char *esp, closure_t *_bk, struct thread_data *_thr, int n, double *a, int nx, int anx)
{
  struct decomp_lu_1_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int i;
  int n1;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct decomp_lu_1_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct decomp_lu_1_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL21;
        }
      goto L_CALL21;
    }
  efp = (struct decomp_lu_1_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct decomp_lu_1_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  {
    i = 0;
    for(  ; i < n ; i += 16 )
      {
        n1 = 16;
        if(n1 > n-i )
          {
            n1 = n-i;
          }
        decomp_d_0(i, n1, a, a+i*nx, nx, anx);
        new_esp = esp;
        while(__builtin_expect(( decomp_rd(new_esp, _bk, _thr, i, n1, n-i, a+i*nx, a+i, a+(i*nx+i), nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->n1 = n1;
            efp->i = i;
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->n = n;
            efp->_thr = _thr;
            efp->_bk = _bk;
            efp->call_id = 0;
            return ;
          L_CALL21: ;
            n1 = efp->n1;
            i = efp->i;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            n = efp->n;
            _thr = efp->_thr;
            _bk = efp->_bk;
            new_esp = esp+1;
          }
        decomp_lu_0(n1, a+(i*nx+i), nx, anx);
        decomp_r_0(n1, n-i-n1, a+(i*nx+i), a+(i*nx+i+n1), nx, anx);
      }
  }
}

void genmat (int n, double *a, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        j = 0;
        for(  ; j < n ; j++ )
          {
            a[i*nx+j] = n-abs(i-j);
          }
      }
  }
}

void printmat (int n, double *a, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        j = 0;
        for(  ; j < n ; j++ )
          {
            fprintf(stderr, "%6.3lf%c", a[i*nx+j], j == n-1?'\n':' ');
          }
      }
  }
  putc('\n', stderr);
}

void copy_l (int n, double *a, double *l, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        j = 0;
        for(  ; j < n ; j++ )
          {
            l[i*nx+j] = i < j?0.0:a[i*nx+j];
          }
      }
  }
}

void copy_u (int n, double *a, double *u, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        j = 0;
        for(  ; j < n ; j++ )
          {
            u[i*nx+j] = i == j?1.0:(i > j?0.0:a[i*nx+j]);
          }
      }
  }
}

void transpose (int n, double *a, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        j = 0;
        for(  ; j < i ; j++ )
          {
            double t1 = a[i*nx+j];
            double t2 = a[j*nx+i];
            a[i*nx+j] = t2,a[j*nx+i] = t1;
          }
      }
  }
}

void matmul (int n, double *a, double *b, double *c, int nx)
{
  int bs = 12;
  int i0;
  int j0;
  int i;
  int j;
  int k;
  transpose(n, b, nx);
  {
    i0 = 0;
    for(  ; i0 < n ; i0 += bs )
      {
        j0 = 0;
        for(  ; j0 < n ; j0 += bs )
          {
            i = i0;
            for(  ; i < i0+bs&&i < n ; i++ )
              {
                j = j0;
                for(  ; j < j0+bs&&j < n ; j++ )
                  {
                    double s = 0.0;
                    {
                      k = 0;
                      for(  ; k < n ; k++ )
                        {
                          s += a[i*nx+k]*b[j*nx+k];
                        }
                    }
                    c[i*nx+j] = s;
                  }
              }
          }
      }
  }
}

void diffmat (int n, double *a, double *b, int nx)
{
  double s = 0.0;
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        j = 0;
        for(  ; j < n ; j++ )
          {
            s += (a[i*nx+j]-b[i*nx+j])*(a[i*nx+j]-b[i*nx+j]);
          }
      }
  }
  fprintf(stderr, "diff: %lf\n", s);
}

void swap_doubles (double *a, int n)
{
  int i;
  {
    i = 0;
    for(  ; i < n ; i++ )
      {
        unsigned long *xp = (unsigned long *)(a+i);
        unsigned long x0 = xp[0];
        unsigned long x1 = xp[1];
        unsigned long y0 = x0;
        unsigned long y1 = x1;
        y0 = y0<<8|(x0 >>= 8)&255;
        y0 = y0<<8|(x0 >>= 8)&255;
        y0 = y0<<8|x0>>8;
        y1 = y1<<8|(x1 >>= 8)&255;
        y1 = y1<<8|(x1 >>= 8)&255;
        y1 = y1<<8|x1>>8;
        xp[1] = y0;
        xp[0] = y1;
      }
  }
}

union int_4{
  int i;
  unsigned char c[4];
};

void input_mat (double *a, int n1, int n2, int nx)
{
  int i;
  int e;
  union int_4 x;
  x.i = 1;
  scanf(" (");
  read_to_eol();
  scanf(" %*d %*d %d", &e);
  read_to_eol();
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        size_t sz = 0;
        while( sz < n2 )
          {
            sz += fread(a+(i*nx+sz), sizeof(double ), n2-sz, stdin);
          }
      }
  }
  scanf(" )");
  if((int )(x.c)[0] != e )
    {
      i = 0;
      for(  ; i < n1 ; i++ )
        {
          swap_doubles(a+i*nx, n2);
        }
    }
}

void output_mat (double *a, int n1, int n2, int nx)
{
  int i;
  union int_4 x;
  x.i = 1;
  printf(" (\n %d %d %d\n", sizeof(double )*n1*n2, sizeof(double ), (int )(x.c)[0]);
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        size_t sz = 0;
        while( sz < n2 )
          {
            sz += fwrite(a+(i*nx+sz), sizeof(double ), n2-sz, stdout);
          }
      }
  }
  printf(" )");
}

void input_mat_l (double *a, int n1, int nx)
{
  int i;
  int e;
  union int_4 x;
  x.i = 1;
  scanf(" (");
  read_to_eol();
  scanf(" %*d %*d %d", &e);
  read_to_eol();
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        size_t sz = 0;
        while( sz <= i )
          {
            sz += fread(a+(i*nx+sz), sizeof(double ), i+1-sz, stdin);
          }
      }
  }
  scanf(" )");
  if((int )(x.c)[0] != e )
    {
      i = 0;
      for(  ; i < n1 ; i++ )
        {
          swap_doubles(a+i*nx, i+1);
        }
    }
}

void output_mat_l (double *a, int n1, int nx)
{
  int i;
  union int_4 x;
  x.i = 1;
  printf(" (\n %d %d %d\n", sizeof(double )*n1*(n1+1)/2, sizeof(double ), (int )(x.c)[0]);
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        size_t sz = 0;
        while( sz <= i )
          {
            sz += fwrite(a+(i*nx+sz), sizeof(double ), i+1-sz, stdout);
          }
      }
  }
  printf(" )");
}

void input_mat_u (double *a, int n1, int nx)
{
  int i;
  int e;
  union int_4 x;
  x.i = 1;
  scanf(" (");
  read_to_eol();
  scanf(" %*d %*d %d", &e);
  read_to_eol();
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        size_t sz = 0;
        while( sz < n1-i-1 )
          {
            sz += fread(a+(i*nx+i+1+sz), sizeof(double ), n1-i-1-sz, stdin);
          }
      }
  }
  scanf(" )");
  if((int )(x.c)[0] != e )
    {
      i = 0;
      for(  ; i < n1 ; i++ )
        {
          swap_doubles(a+(i*nx+i+1), n1-i-1);
        }
    }
}

void output_mat_u (double *a, int n1, int nx)
{
  int i;
  union int_4 x;
  x.i = 1;
  printf(" (\n %d %d %d\n", sizeof(double )*n1*(n1-1)/2, sizeof(double ), (int )(x.c)[0]);
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        size_t sz = 0;
        while( sz < n1-i-1 )
          {
            sz += fwrite(a+i*nx+i+1+sz, sizeof(double ), n1-i-1-sz, stdout);
          }
      }
  }
  printf(" )");
}

void zero_mat (double *a, int n1, int n2, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for(  ; i < n1 ; i++ )
      {
        j = 0;
        for(  ; j < n2 ; j++ )
          {
            a[i*nx+j] = 0.0;
          }
      }
  }
}

void proc_rslt (double *a)
{
  printf("%lf", *a);
}

struct proc_cmd_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  double *c;
  double *u;
  double *l;
  double *a;
  int nx;
  int d;
  int al;
  int n;
  struct timeval tp[2];
  double *ansp;
  int n3;
  int n2;
  int n1;
  struct thread_data *_thr;
  closure_t *_bk;
};

void proc_cmd (char *esp, closure_t *_bk, struct thread_data *_thr, int n1, int n2, int n3, double *ansp)
{
  struct proc_cmd_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int n;
  int al;
  int d;
  int nx;
  double *a;
  double *l;
  double *u;
  double *c;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct proc_cmd_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct proc_cmd_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL22;
        case 1:
          goto L_CALL23;
        }
      goto L_CALL22;
    }
  efp = (struct proc_cmd_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct proc_cmd_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  n = n1;
  al = n2;
  d = n3;
  nx = n;
  a = (double *)malloc(sizeof(double )*n*n);
  genmat(n, a, nx);
  if(d > 1 )
    {
      printmat(n, a, nx);
    }
  gettimeofday(efp->tp, 0);
  switch( al )
    {
    case 1:
      decomp_lu_0(n, a, nx, nx);
      break;
    case 2:
      new_esp = esp;
      while(__builtin_expect(( decomp_lu(new_esp, _bk, _thr, n, a, nx, nx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->c = c;
          efp->u = u;
          efp->l = l;
          efp->a = a;
          efp->nx = nx;
          efp->d = d;
          efp->al = al;
          efp->n = n;
          efp->ansp = ansp;
          efp->n3 = n3;
          efp->n2 = n2;
          efp->n1 = n1;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->call_id = 0;
          return ;
        L_CALL22: ;
          c = efp->c;
          u = efp->u;
          l = efp->l;
          a = efp->a;
          nx = efp->nx;
          d = efp->d;
          al = efp->al;
          n = efp->n;
          ansp = efp->ansp;
          n3 = efp->n3;
          n2 = efp->n2;
          n1 = efp->n1;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
      break;
    case 3:
      new_esp = esp;
      while(__builtin_expect(( decomp_lu_1(new_esp, _bk, _thr, n, a, nx, nx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
        {
          efp->c = c;
          efp->u = u;
          efp->l = l;
          efp->a = a;
          efp->nx = nx;
          efp->d = d;
          efp->al = al;
          efp->n = n;
          efp->ansp = ansp;
          efp->n3 = n3;
          efp->n2 = n2;
          efp->n1 = n1;
          efp->_thr = _thr;
          efp->_bk = _bk;
          efp->call_id = 1;
          return ;
        L_CALL23: ;
          c = efp->c;
          u = efp->u;
          l = efp->l;
          a = efp->a;
          nx = efp->nx;
          d = efp->d;
          al = efp->al;
          n = efp->n;
          ansp = efp->ansp;
          n3 = efp->n3;
          n2 = efp->n2;
          n1 = efp->n1;
          _thr = efp->_thr;
          _bk = efp->_bk;
          new_esp = esp+1;
        }
      break;
    }
  gettimeofday(efp->tp+1, 0);
  *ansp = elapsed_time(efp->tp);
  fprintf(stderr, "%lf\n", *ansp);
  if(d > 0 )
    {
      l = (double *)malloc(n*n*sizeof(double ));
      u = (double *)malloc(n*n*sizeof(double ));
      c = (double *)malloc(n*n*sizeof(double ));
      if(d > 1 )
        {
          printmat(n, a, nx);
        }
      copy_l(n, a, l, nx);
      copy_u(n, a, u, nx);
      if(d > 1 )
        {
          printmat(n, l, nx);
        }
      if(d > 1 )
        {
          printmat(n, u, nx);
        }
      matmul(n, l, u, c, nx);
      if(d > 1 )
        {
          printmat(n, c, nx);
        }
      genmat(n, a, nx);
      diffmat(n, a, c, nx);
      free(l);
      free(u);
      free(c);
    }
  free(a);
}

struct do_task_lu_task_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  int anx;
  int nx;
  double *a;
  double *b2;
  double *b1;
  int n3;
  int n2;
  int n1;
  int ifexp_result4;
  struct task_lu *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

struct _bk_in_do_task_lu_task_frame{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_task_lu_task_frame *xfp;
};

char *_bk_in_do_task_lu_task (char *esp, void *xfp0)
{
  char *new_esp;
  struct _bk_in_do_task_lu_task_frame *efp;
  struct do_task_lu_task_frame *xfp = xfp0;
  size_t esp_flag = (size_t )esp&3;
  char *parmp = (char *)((size_t )esp^esp_flag);
 LGOTO: ;
  efp = (struct _bk_in_do_task_lu_task_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct _bk_in_do_task_lu_task_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  *((int *)efp) = 0;
  return 0;
  return 0;
}

void do_task_lu_task (char *esp, struct thread_data *_thr, struct task_lu *pthis)
{
  struct do_task_lu_task_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t )esp&3;
  int ifexp_result4;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  int anx;
  if(esp_flag )
    {
      esp = (char *)((size_t )esp^esp_flag);
      efp = (struct do_task_lu_task_frame *)esp;
      esp = (char *)((Align_t *)esp+(sizeof(struct do_task_lu_task_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
      *((char **)esp) = 0;
    LGOTO: switch( (*efp).call_id )
        {
        case 0:
          goto L_CALL24;
        case 1:
          goto L_CALL25;
        case 2:
          goto L_CALL26;
        case 3:
          goto L_CALL27;
        case 4:
          goto L_CALL28;
        }
      goto L_CALL24;
    }
  efp = (struct do_task_lu_task_frame *)esp;
  esp = (char *)((Align_t *)esp+(sizeof(struct do_task_lu_task_frame )+sizeof(Align_t )+-1)/sizeof(Align_t ));
  *((char **)esp) = 0;
  n1 = (*pthis).n1;
  n2 = (*pthis).n2;
  n3 = (*pthis).n3;
  b1 = (*pthis).b1;
  b2 = (*pthis).b2;
  a = (*pthis).a;
  nx = (*pthis).nx;
  if((*pthis).tmpnx )
    {
      ifexp_result4 = (*pthis).tmpnx;
    }
  else {
    ifexp_result4 = (*pthis).anx;
  }
  {
    anx = ifexp_result4;
    //fprintf(stderr, "start %d\n", (*pthis).tt);
    switch( (*pthis).tt )
      {
      case TASK_CMD:
        new_esp = esp;
        while(__builtin_expect(( proc_cmd(new_esp, &efp->_bk, _thr, n1, n2, n3, a),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b2 = b2;
            efp->b1 = b1;
            efp->n3 = n3;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->ifexp_result4 = ifexp_result4;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_task_lu_task;
            efp->_bk.fr = (void *)efp;
            efp->call_id = 0;
            return ;
          L_CALL24: ;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b2 = efp->b2;
            b1 = efp->b1;
            n3 = efp->n3;
            n2 = efp->n2;
            n1 = efp->n1;
            ifexp_result4 = efp->ifexp_result4;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp+1;
          }
        break;
      case TASK_LU:
        new_esp = esp;
        while(__builtin_expect(( decomp_lu(new_esp, &efp->_bk, _thr, n1, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b2 = b2;
            efp->b1 = b1;
            efp->n3 = n3;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->ifexp_result4 = ifexp_result4;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_task_lu_task;
            efp->_bk.fr = (void *)efp;
            efp->call_id = 1;
            return ;
          L_CALL25: ;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b2 = efp->b2;
            b1 = efp->b1;
            n3 = efp->n3;
            n2 = efp->n2;
            n1 = efp->n1;
            ifexp_result4 = efp->ifexp_result4;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp+1;
          }
        break;
      case TASK_R:
        new_esp = esp;
        while(__builtin_expect(( decomp_r(new_esp, &efp->_bk, _thr, n1, n2, b1, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b2 = b2;
            efp->b1 = b1;
            efp->n3 = n3;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->ifexp_result4 = ifexp_result4;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_task_lu_task;
            efp->_bk.fr = (void *)efp;
            efp->call_id = 2;
            return ;
          L_CALL26: ;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b2 = efp->b2;
            b1 = efp->b1;
            n3 = efp->n3;
            n2 = efp->n2;
            n1 = efp->n1;
            ifexp_result4 = efp->ifexp_result4;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp+1;
          }
        break;
      case TASK_D:
        new_esp = esp;
        while(__builtin_expect(( decomp_d(new_esp, &efp->_bk, _thr, n1, n2, b1, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b2 = b2;
            efp->b1 = b1;
            efp->n3 = n3;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->ifexp_result4 = ifexp_result4;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_task_lu_task;
            efp->_bk.fr = (void *)efp;
            efp->call_id = 3;
            return ;
          L_CALL27: ;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b2 = efp->b2;
            b1 = efp->b1;
            n3 = efp->n3;
            n2 = efp->n2;
            n1 = efp->n1;
            ifexp_result4 = efp->ifexp_result4;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp+1;
          }
        break;
      case TASK_RD:
        new_esp = esp;
        while(__builtin_expect(( decomp_rd(new_esp, &efp->_bk, _thr, n1, n2, n3, b1, b2, a, nx, anx),(efp->tmp_esp = *((char **)esp)) != 0 ),0))
          {
            efp->anx = anx;
            efp->nx = nx;
            efp->a = a;
            efp->b2 = b2;
            efp->b1 = b1;
            efp->n3 = n3;
            efp->n2 = n2;
            efp->n1 = n1;
            efp->ifexp_result4 = ifexp_result4;
            efp->pthis = pthis;
            efp->_thr = _thr;
            efp->_bk.fun = _bk_in_do_task_lu_task;
            efp->_bk.fr = (void *)efp;
            efp->call_id = 4;
            return ;
          L_CALL28: ;
            anx = efp->anx;
            nx = efp->nx;
            a = efp->a;
            b2 = efp->b2;
            b1 = efp->b1;
            n3 = efp->n3;
            n2 = efp->n2;
            n1 = efp->n1;
            ifexp_result4 = efp->ifexp_result4;
            pthis = efp->pthis;
            _thr = efp->_thr;
            new_esp = esp+1;
          }
        break;
      }
  }
}

void send_task_lu_task (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int nx = (*pthis).nx;
  int anx = (*pthis).anx;
  send_int((*pthis).tt);
  switch( (*pthis).tt )
    {
    case TASK_LU:
      send_int(n1);
      output_mat(a, n1, n1, anx);
      break;
    case TASK_R:
      send_int(n1);
      send_int(n2);
      output_mat_l(b1, n1, nx);
      output_mat(a, n1, n2, anx);
      break;
    case TASK_D:
      send_int(n1);
      send_int(n2);
      output_mat_u(b1, n1, nx);
      output_mat(a, n2, n1, anx);
      break;
    case TASK_RD:
      send_int(n1);
      send_int(n2);
      send_int(n3);
      output_mat(b1, n2, n1, nx);
      output_mat(b2, n1, n3, nx);
      break;
    }
}

struct task_lu *recv_task_lu_task (struct thread_data *_thr)
{
  struct task_lu *pthis = malloc(sizeof(struct task_lu ));
  enum task_tp2 tt;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  static double ans;
  tt = recv_int();
  switch( tt )
    {
    case TASK_CMD:
      n1 = recv_int();
      n2 = recv_int();
      n3 = recv_int();
      a = &ans;
      break;
    case TASK_LU:
      n1 = recv_int();
      nx = n1;
      a = (double *)malloc(sizeof(double )*n1*n1);
      input_mat(a, n1, n1, nx);
      break;
    case TASK_R:
      n1 = recv_int();
      n2 = recv_int();
      nx = n1+n2;
      b1 = (double *)malloc(sizeof(double )*n1*nx);
      input_mat_l(b1, n1, nx);
      a = b1+n1;
      input_mat(a, n1, n2, nx);
      break;
    case TASK_D:
      n1 = recv_int();
      n2 = recv_int();
      nx = n1;
      b1 = (double *)malloc(sizeof(double )*(n1+n2)*nx);
      input_mat_u(b1, n1, nx);
      a = b1+n1*nx;
      input_mat(a, n2, n1, nx);
      break;
    case TASK_RD:
      n1 = recv_int();
      n2 = recv_int();
      n3 = recv_int();
      nx = n1+n3;
      a = (double *)malloc(sizeof(double )*(n1+n2)*nx);
      b1 = a+n1*nx;
      b2 = a+n1;
      a += n1*nx+n1;
      input_mat(b1, n2, n1, nx);
      input_mat(b2, n1, n3, nx);
      zero_mat(a, n2, n3, nx);
      break;
    }
  (*pthis).tt = tt;
  (*pthis).n1 = n1;
  (*pthis).n2 = n2;
  (*pthis).n3 = n3;
  (*pthis).b1 = b1;
  (*pthis).b2 = b2;
  (*pthis).a = a;
  (*pthis).nx = nx;
  (*pthis).anx = nx;
  (*pthis).tmpnx = 0;
  return pthis;
}

void send_task_lu_rslt (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int anx = (*pthis).anx;
  switch( (*pthis).tt )
    {
    case TASK_CMD:
      proc_rslt(a);
      break;
    case TASK_LU:
      output_mat(a, n1, n1, anx);
      free(a);
      break;
    case TASK_R:
      output_mat(a, n1, n2, anx);
      free(b1);
      break;
    case TASK_D:
      output_mat(a, n2, n1, anx);
      free(b1);
      break;
    case TASK_RD:
      output_mat(a, n2, n3, anx);
      a -= n1*anx+n1;
      free(a);
      break;
    }
  free(pthis);
}

void recv_task_lu_rslt (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int tmpnx = (*pthis).tmpnx;
  int anx = (*pthis).anx;
  switch( (*pthis).tt )
    {
    case TASK_LU:
      input_mat(a, n1, n1, anx);
      break;
    case TASK_R:
      input_mat(a, n1, n2, anx);
      break;
    case TASK_D:
      input_mat(a, n2, n1, anx);
      break;
    case TASK_RD:
      if(tmpnx == 0 )
        {
          tmpnx = n3;
          (*pthis).a = (double *)malloc(sizeof(double )*n2*n3);
        }
      input_mat((*pthis).a, n2, n3, tmpnx);
      break;
    }
}
void (*task_doers[256])( char *, struct thread_data *, void * ) = { (void (*)( char *, struct thread_data *, void * ))do_task_lu_task };
void (*task_senders[256])( struct thread_data *, void * ) = { (void (*)( struct thread_data *, void * ))send_task_lu_task };
void *(*task_receivers[256])( struct thread_data * ) = { (void *(*)( struct thread_data * ))recv_task_lu_task };
void (*rslt_senders[256])( struct thread_data *, void * ) = { (void (*)( struct thread_data *, void * ))send_task_lu_rslt };
void (*rslt_receivers[256])( struct thread_data *, void * ) = { (void (*)( struct thread_data *, void * ))recv_task_lu_rslt };
