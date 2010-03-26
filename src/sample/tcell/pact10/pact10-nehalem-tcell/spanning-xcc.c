#include <pthread.h>
#include <stdio.h>

int connect_to (char *hostname, unsigned short port);

void close_socket (int socket);

int send_char (char, int);

int send_string (char *str, int socket);

int send_fmt_string (int socket, char *fmt_string, ...);

int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket);

int receive_char (int socket);

char *receive_line (char *buf, int maxlen, int socket);

int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket);
extern char *receive_buf;
extern char *receive_buf_p;
enum node
{ OUTSIDE = -1, INSIDE = -2, ANY = -3, PARENT = -4, FORWARD = -5, TERM = -99 };
enum command
{ TASK, RSLT, TREQ, NONE, BACK, RACK, DREQ, DATA, STAT, VERB, EXIT, WRNG };
extern char *cmd_strings[];
enum choose
{ CHS_RANDOM, CHS_ORDER };

struct cmd
{
  enum command w;
  int c;
  enum node node;
  enum node v[4][16];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  int task_no;
  struct cmd_list *next;
};
struct task;
struct thread_data;
void (*task_doers[256]) (struct thread_data *, void *);
void (*task_senders[256]) (void *);
void *(*task_receivers[256]) ();
void (*rslt_senders[256]) (void *);
void (*rslt_receivers[256]) (void *);

void data_allocate (int);

void data_send (int, int);

void data_receive (int, int);
struct worker_data;

void worker_init (struct thread_data *);

void _setup_data (int);

void _request_data (struct thread_data *, int, int);

void _wait_data (int, int);

void _set_exist_flag (int, int);
enum task_stat
{ TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE,
    TASK_SUSPENDED };
enum task_home_stat
{ TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE };

struct task
{
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  enum node rslt_to;
  enum node rslt_head[16];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  struct task *owner;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  enum node task_head[16];
};

struct thread_data
{
  struct task_home *req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  int last_treq;
  enum choose last_choose;
  double random_seed1;
  double random_seed2;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  void *wdptr;
  char dummy[1000];
};
enum DATA_FLAG
{ DATA_NONE, DATA_REQUESTING, DATA_EXIST };

struct dhandler_arg
{
  enum node data_to;
  enum node head[16];
  struct cmd dreq_cmd;
  struct cmd dreq_cmd_fwd;
  int start;
  int end;
};

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (struct thread_data *thr);

void proto_error (char const *str, struct cmd *pcmd);

void read_to_eol (void);

void init_data_flag (int);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_dreq (struct cmd *);

void recv_data (struct cmd *);

void recv_none (struct cmd *);

void recv_back (struct cmd *);

void print_task_list (struct task *task_top, char *name);

void print_task_home_list (struct task_home *treq_top, char *name);

void print_thread_status (struct thread_data *thr);

void print_status (struct cmd *);

void set_verbose_level (struct cmd *);

void recv_exit (struct cmd *);

int serialize_cmdname (char *buf, enum command w);

int deserialize_cmdname (enum command *buf, char *str);

int serialize_arg (char *buf, enum node *arg);

enum node deserialize_node (char *str);

int deserialize_arg (enum node *buf, char *str);

int serialize_cmd (char *buf, struct cmd *pcmd);

int deserialize_cmd (struct cmd *pcmd, char *str);

int copy_address (enum node *dst, enum node *src);

int address_equal (enum node *adr1, enum node *adr2);

struct runtime_option
{
  int num_thrs;
  char sv_hostname[256];
  unsigned short port;
  char *initial_task;
  int auto_exit;
  int affinity;
  int prefetch;
  int verbose;
};
extern struct runtime_option option;

void handle_req (int (*)lightweight (void), struct thread_data *);

#include<sys/time.h>
#include<stdlib.h>
#include"xccmem.h"

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

double sqrt (double);

double fabs (double);

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

int V_Length;

struct vertex
{
  int degree;
  int first_e;
  int parent;
};
struct vertex VV[16 * 1024 * 1024 + 321];
int E_Length;

struct edge
{
  int next_v;
  int next_e;
};
struct edge EE[64 * 1024 * 1024 + 321];

struct v_list_body
{
  struct v_list_body *next;
  int i;
  int v[600];
};
int V_List_Length;
struct v_list_body *V_List;

struct v_list_head
{
  struct v_list_body *first;
  struct v_list_body *last;
  int length;
};

struct worker_data
{
  struct v_list_head V_List;
  char _dummy_[1000];
};

struct v_list_body *alloc_v_list (struct v_list_body *next);

void worker_init (struct thread_data *_thr)
{
  (*((struct worker_data *) (*_thr).wdptr)).V_List.first = 0;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.last = 0;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 0;
}

struct v_list_body *alloc_v_list (struct v_list_body *next)
{
  struct v_list_body *a;
  a = (struct v_list_body *) malloc (sizeof (struct v_list_body));
  (*a).next = next;
  (*a).i = 600;
  return a;
}

struct v_list_body *dealloc_v_list (struct v_list_body *a)
{
  struct v_list_body *next;

  next = (*a).next;
  free (a);
  return next;
}

void check_last (struct v_list_body *v_list, struct v_list_body *v_list_last,
                 char *info)
{
  struct v_list_body *cur;

  if (!v_list)
    return;
  else;
  {
    cur = v_list;
    for (; (*cur).next; cur = (*cur).next)
      {
      }
  }
  if (cur != v_list_last)
    {
      fprintf (stderr, "!! last is incorrect. %s \n", info);
    }
  else;
}

void check_length (struct v_list_body *v_list, int len_expect, char *info)
{
  struct v_list_body *cur;
  int len = 0;

  {
    cur = v_list;
    for (; cur; cur = (*cur).next)
      {
        len++;
      }
  }
  if (len != len_expect)
    {
      fprintf (stderr, "!! Unexpected length len:%d expect:%d %s\n", len,
               len_expect, info);
    }
  else;
}

void check_v_list (struct v_list_body *v_list, struct v_list_body *v_list_last,
                   int len_expect, char *info)
{
  check_length (v_list, len_expect, info);
  check_last (v_list, v_list_last, info);
}

struct v_list_body *nconc_v_list (struct v_list_body *a,
                                  struct v_list_body *a_last,
                                  struct v_list_body *b)
{
  struct v_list_body *cur;

  if (a_last)
    {
      (*a_last).next = b;
      return a;
    }
  else if (a)
    {
      {
        cur = a;
        for (; (*cur).next; cur = (*cur).next)
          {
          }
      }
      (*cur).next = b;
      return a;
    }
  else
    return b;
}

void v_list_to_array (struct v_list_body **buf,
                      struct v_list_body *v_list_first, int len)
{
  int i;
  struct v_list_body *v_list_cur;

  {
    i = 0, v_list_cur = v_list_first;
    for (; i < len; (i++, v_list_cur = (*v_list_cur).next))
      {
        buf[i] = v_list_cur;
      }
  }
}

void search_s2 (int fv)
{
  struct v_list_body *my_v_list;
  int my_v_list_length;
  int vi;

  (VV[fv]).parent = fv + 1;
  my_v_list = alloc_v_list (0);
  my_v_list_length = 1;
  vi = (*my_v_list).i;
  ((*my_v_list).v)[--vi] = fv;
  while (vi < 600)
    {
      int v = ((*my_v_list).v)[vi++];
      int i;
      int d = (VV[v]).degree;
      int e = (VV[v]).first_e;

      {
        i = 0;
        for (; i < d; (i++, e = (EE[e]).next_e))
          {
            int nv = (EE[e]).next_v;

            if ((VV[nv]).parent == 0)
              if (1)
                {
                  (VV[nv]).parent = v + 1;
                  if (vi <= 0)
                    {
                      (*my_v_list).i = vi;
                      my_v_list = alloc_v_list (my_v_list);
                      my_v_list_length++;
                      vi = (*my_v_list).i;
                    }
                  else;
                  ((*my_v_list).v)[--vi] = nv;
                }
              else;
            else;
          }
      }
      while (vi == 600 && my_v_list_length > 1)
        {
          my_v_list = dealloc_v_list (my_v_list);
          my_v_list_length--;
          vi = (*my_v_list).i;
        }
    }
  (*my_v_list).i = vi;
}

void search1 (int v, int b)
{
  int i;
  int d = (VV[v]).degree;
  int e = (VV[v]).first_e;

  {
    i = 0;
    for (; i < d; (i++, e = (EE[e]).next_e))
      {
        int nv = (EE[e]).next_v;

        if ((VV[nv]).parent == 0)
          if (1)
            {
              (VV[nv]).parent = v + 1;
              if (b > 0)
                search1 (nv, b - 1);
              else
                {
                  if ((*V_List).i <= 0)
                    {
                      V_List = alloc_v_list (V_List);
                      V_List_Length++;
                    }
                  else;
                  ((*V_List).v)[--(*V_List).i] = nv;
                }
            }
          else;
        else;
      }
  }
}

void search (int v)
{
  struct v_list_body *my_v_list;
  struct v_list_body *mlist;
  struct v_list_body *next;
  int i;
  int j;
  int my_v_list_length;
  int mlist_length;

  {
  }
  (VV[v]).parent = v + 1;
  V_List = alloc_v_list (0);
  V_List_Length = 1;
  search1 (v, 30);
  {
  }
  do
    {
      my_v_list = V_List;
      mlist = my_v_list;
      my_v_list_length = V_List_Length;
      mlist_length = my_v_list_length;
      V_List = alloc_v_list (0);
      V_List_Length = 1;
      {
        i = 0;
        for (; i < my_v_list_length; (i++, my_v_list = (*my_v_list).next))
          {
            j = (*my_v_list).i;
            for (; j < 600; j++)
              {
                search1 (((*my_v_list).v)[j], 30);
              }
          }
      }
      {
        i = 0;
        for (; i < mlist_length; i++)
          {
            mlist = dealloc_v_list (mlist);
          }
      }
      {
      }
    }
  while (V_List_Length > 1 || (*V_List).i < 600);
}

void search1_p (int (*_bk) lightweight (void), struct thread_data *_thr, int v,
                int b, int k0, int k1);

void search_p_b (int (*_bk) lightweight (void), struct thread_data *_thr,
                 struct v_list_body *my_v_list, int i0, int i1);

void search_p_b_array (int (*_bk) lightweight (void), struct thread_data *_thr,
                       struct v_list_body **my_v_array, int i0, int i1);

void search_p_c (int (*_bk) lightweight (void), struct thread_data *_thr,
                 struct v_list_body *my_v_list, int j0, int j1);

struct t_search1
{
  int v;
  int b;
  int k0;
  int k1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

void do_t_search1_task (struct thread_data *_thr, struct t_search1 *pthis)
{

  int _bk lightweight (void)
  {
    return 0;
  }
  int stack_created_p = 0;

  if (!(*((struct worker_data *) (*_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
      stack_created_p = 1;
    }
  else;
  search1_p (_bk, _thr, (*pthis).v, (*pthis).b, (*pthis).k0, (*pthis).k1);
  if (stack_created_p)
    {
      if ((*((struct worker_data *) (*_thr).wdptr)).V_List.length > 1
          || (*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).i < 600)
        {
          (*pthis).v_list.first =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
          (*pthis).v_list.last =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.last;
          (*pthis).v_list.length =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.length;
        }
      else
        {
          dealloc_v_list ((*((struct worker_data *) (*_thr).wdptr)).V_List.
                          first);
          (*pthis).v_list.first = 0;
          (*pthis).v_list.last = 0;
          (*pthis).v_list.length = 0;
        }
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*pthis).v_list.first = 0;
      (*pthis).v_list.last = 0;
      (*pthis).v_list.length = 0;
    }
}

void search1_p (int (*_bk) lightweight (void), struct thread_data *_thr, int v,
                int b, int k0, int k1)
{
  int i;
  int e = (VV[v]).first_e;
  int nv;

  {
    i = 0;
    for (; i < k0; i++)
      {
        e = (EE[e]).next_e;
      }
  }
  {
    int i = k0;
    int I_end = k1;
    struct t_search1 *pthis;
    int spawned = 0;

    int do_many_bk lightweight (void)
    {
      if (!spawned)
        _bk ();
      else;
      while ((*_thr).treq_top && i + 1 < I_end)
        {
          int k1_2 = I_end;
          int k0_2 = (1 + i + I_end) / 2;

          I_end = k0_2;
          pthis = (struct t_search1 *) malloc (sizeof (struct t_search1));
          (*pthis).v = v;
          (*pthis).b = b;
          (*pthis).k0 = k0_2;
          (*pthis).k1 = k1_2;
          spawned++;
          make_and_send_task (_thr, 0, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk, _thr);
    else;
    for (; i < I_end; i++)
      {
        nv = (EE[e]).next_v;
        if (atomic_read_int ((VV[nv]).parent) == 0)
          {
            atomic_write_int_to_start_read ((VV[nv]).parent, v + 1);
            if (b > 0)
              search1_p (do_many_bk, _thr, nv, b - 1, 0, (VV[nv]).degree);
            else
              {
                if ((*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).
                    i <= 0)
                  {
                    (*((struct worker_data *) (*_thr).wdptr)).V_List.first =
                      alloc_v_list ((*((struct worker_data *) (*_thr).wdptr)).
                                    V_List.first);
                    ((*((struct worker_data *) (*_thr).wdptr)).V_List.length)++;
                  }
                else;
                ((*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).
                 v)[--(*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).
                    i] = nv;
              }
          }
        else;
        e = (EE[e]).next_e;
      }
    while (spawned-- > 0)
      {
        pthis = (struct t_search1 *) wait_rslt (_thr);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first =
          nconc_v_list ((*pthis).v_list.first, (*pthis).v_list.last,
                        (*((struct worker_data *) (*_thr).wdptr)).V_List.first);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.length +=
          (*pthis).v_list.length;
        free (pthis);
      }
  }
}

void search_p (int (*_bk) lightweight (void), struct thread_data *_thr, int v)
{
  struct v_list_head my_v_list;
  struct v_list_head mlist;
  int i;

  {
  }
  (VV[v]).parent = v + 1;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
  (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
    (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
  search1_p (_bk, _thr, v, 30, 0, (VV[v]).degree);
  {
  }
  while ((*((struct worker_data *) (*_thr).wdptr)).V_List.length > 1
         || (*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).i < 600)
    {
      my_v_list = (*((struct worker_data *) (*_thr).wdptr)).V_List;
      mlist = my_v_list;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
      {
      }
      search_p_b (_bk, _thr, my_v_list.first, 0, my_v_list.length);
      {
        i = 0;
        for (; i < mlist.length; i++)
          {
            mlist.first = dealloc_v_list (mlist.first);
          }
      }
    }
  {
  }
}

struct t_search_b
{
  struct v_list_body *my_v_list_first;
  int i0;
  int i1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

void do_t_search_b_task (struct thread_data *_thr, struct t_search_b *pthis)
{

  int _bk lightweight (void)
  {
    return 0;
  }
  int stack_created_p = 0;

  if (!(*((struct worker_data *) (*_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
      stack_created_p = 1;
    }
  else;
  search_p_b (_bk, _thr, (*pthis).my_v_list_first, (*pthis).i0, (*pthis).i1);
  if (stack_created_p)
    {
      if ((*((struct worker_data *) (*_thr).wdptr)).V_List.length > 1
          || (*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).i < 600)
        {
          (*pthis).v_list.first =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
          (*pthis).v_list.last =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.last;
          (*pthis).v_list.length =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.length;
        }
      else
        {
          dealloc_v_list ((*((struct worker_data *) (*_thr).wdptr)).V_List.
                          first);
          (*pthis).v_list.first = 0;
          (*pthis).v_list.last = 0;
          (*pthis).v_list.length = 0;
        }
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*pthis).v_list.first = 0;
      (*pthis).v_list.last = 0;
      (*pthis).v_list.length = 0;
    }
}

void search_p_b (int (*_bk) lightweight (void), struct thread_data *_thr,
                 struct v_list_body *my_v_list_first, int i0, int i1)
{
  int i;
  struct v_list_body *my_v_list_first0;

  {
  }
  {
    i = 0;
    for (; i < i0; i++)
      {
        my_v_list_first = (*my_v_list_first).next;
      }
  }
  i1 -= i0;
  i0 = 0;
  my_v_list_first0 = my_v_list_first;
  {
    int i = i0;
    int I_end2 = i1;
    struct t_search_b *pthis;
    int spawned2 = 0;

    int do_many_bk2 lightweight (void)
    {
      if (!spawned2)
        _bk ();
      else;
      while ((*_thr).treq_top && i + 1 < I_end2)
        {
          int i1_2 = I_end2;
          int i0_2 = (1 + i + I_end2) / 2;

          I_end2 = i0_2;
          pthis = (struct t_search_b *) malloc (sizeof (struct t_search_b));
          (*pthis).my_v_list_first = my_v_list_first0;
          (*pthis).i0 = i0_2;
          (*pthis).i1 = i1_2;
          spawned2++;
          make_and_send_task (_thr, 1, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk2, _thr);
    else;
    for (; i < I_end2; i++)
      {
        search_p_c (do_many_bk2, _thr, my_v_list_first, (*my_v_list_first).i,
                    600);
        my_v_list_first = (*my_v_list_first).next;
      }
    while (spawned2-- > 0)
      {
        pthis = (struct t_search_b *) wait_rslt (_thr);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first =
          nconc_v_list ((*pthis).v_list.first, (*pthis).v_list.last,
                        (*((struct worker_data *) (*_thr).wdptr)).V_List.first);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.length +=
          (*pthis).v_list.length;
        free (pthis);
      }
  }
}

void search_p_array (int (*_bk) lightweight (void), struct thread_data *_thr,
                     int v)
{
  struct v_list_body **my_v_array;
  int my_v_array_length;
  int buf_size;
  int i;

  {
  }
  (VV[v]).parent = v + 1;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
  (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
    (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
  search1_p (_bk, _thr, v, 30, 0, (VV[v]).degree);
  {
  }
  my_v_array = 0;
  buf_size = 0;
  while ((*((struct worker_data *) (*_thr).wdptr)).V_List.length > 1
         || (*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).i < 600)
    {
      my_v_array_length =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.length;
      if (my_v_array_length > buf_size)
        {
          buf_size = 2 * my_v_array_length;
          my_v_array =
            (struct v_list_body **) realloc (my_v_array,
                                             buf_size *
                                             sizeof (struct v_list_body *));
        }
      else;
      v_list_to_array (my_v_array,
                       (*((struct worker_data *) (*_thr).wdptr)).V_List.first,
                       (*((struct worker_data *) (*_thr).wdptr)).V_List.length);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
      search_p_b_array (_bk, _thr, my_v_array, 0, my_v_array_length);
      {
        i = 0;
        for (; i < my_v_array_length; i++)
          {
            dealloc_v_list (my_v_array[i]);
          }
      }
    }
  free (my_v_array);
  {
  }
}

struct t_search_b_array
{
  struct v_list_body **my_v_array;
  int i0;
  int i1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

void do_t_search_b_array_task (struct thread_data *_thr,
                               struct t_search_b_array *pthis)
{

  int _bk lightweight (void)
  {
    return 0;
  }
  int stack_created_p = 0;

  if (!(*((struct worker_data *) (*_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
      stack_created_p = 1;
    }
  else;
  search_p_b_array (_bk, _thr, (*pthis).my_v_array, (*pthis).i0, (*pthis).i1);
  if (stack_created_p)
    {
      if ((*((struct worker_data *) (*_thr).wdptr)).V_List.length > 1
          || (*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).i < 600)
        {
          (*pthis).v_list.first =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
          (*pthis).v_list.last =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.last;
          (*pthis).v_list.length =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.length;
        }
      else
        {
          dealloc_v_list ((*((struct worker_data *) (*_thr).wdptr)).V_List.
                          first);
          (*pthis).v_list.first = 0;
          (*pthis).v_list.last = 0;
          (*pthis).v_list.length = 0;
        }
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*pthis).v_list.first = 0;
      (*pthis).v_list.last = 0;
      (*pthis).v_list.length = 0;
    }
}

void search_p_b_array (int (*_bk) lightweight (void), struct thread_data *_thr,
                       struct v_list_body **my_v_array, int i0, int i1)
{
  int i;

  {
  }
  {
    int i = i0;
    int I_end3 = i1;
    struct t_search_b_array *pthis;
    int spawned3 = 0;

    int do_many_bk3 lightweight (void)
    {
      if (!spawned3)
        _bk ();
      else;
      while ((*_thr).treq_top && i + 1 < I_end3)
        {
          int i1_2 = I_end3;
          int i0_2 = (1 + i + I_end3) / 2;

          I_end3 = i0_2;
          pthis =
            (struct t_search_b_array *)
            malloc (sizeof (struct t_search_b_array));
          (*pthis).my_v_array = my_v_array;
          (*pthis).i0 = i0_2;
          (*pthis).i1 = i1_2;
          spawned3++;
          make_and_send_task (_thr, 2, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk3, _thr);
    else;
    for (; i < I_end3; i++)
      {
        search_p_c (do_many_bk3, _thr, my_v_array[i], (*my_v_array[i]).i, 600);
      }
    while (spawned3-- > 0)
      {
        pthis = (struct t_search_b_array *) wait_rslt (_thr);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first =
          nconc_v_list ((*pthis).v_list.first, (*pthis).v_list.last,
                        (*((struct worker_data *) (*_thr).wdptr)).V_List.first);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.length +=
          (*pthis).v_list.length;
        free (pthis);
      }
  }
}

struct t_search_c
{
  struct v_list_body *my_v_list_cur;
  int j0;
  int j1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

void do_t_search_c_task (struct thread_data *_thr, struct t_search_c *pthis)
{

  int _bk lightweight (void)
  {
    return 0;
  }
  int stack_created_p = 0;

  if (!(*((struct worker_data *) (*_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = alloc_v_list (0);
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
      stack_created_p = 1;
    }
  else;
  search_p_c (_bk, _thr, (*pthis).my_v_list_cur, (*pthis).j0, (*pthis).j1);
  if (stack_created_p)
    {
      if ((*((struct worker_data *) (*_thr).wdptr)).V_List.length > 1
          || (*(*((struct worker_data *) (*_thr).wdptr)).V_List.first).i < 600)
        {
          (*pthis).v_list.first =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.first;
          (*pthis).v_list.last =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.last;
          (*pthis).v_list.length =
            (*((struct worker_data *) (*_thr).wdptr)).V_List.length;
        }
      else
        {
          dealloc_v_list ((*((struct worker_data *) (*_thr).wdptr)).V_List.
                          first);
          (*pthis).v_list.first = 0;
          (*pthis).v_list.last = 0;
          (*pthis).v_list.length = 0;
        }
      (*((struct worker_data *) (*_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*pthis).v_list.first = 0;
      (*pthis).v_list.last = 0;
      (*pthis).v_list.length = 0;
    }
}

void search_p_c (int (*_bk) lightweight (void), struct thread_data *_thr,
                 struct v_list_body *my_v_list_cur, int j0, int j1)
{
  int j;

  {
  }
  {
    int j = j0;
    int J_end = j1;
    struct t_search_c *pthis;
    int spawned4 = 0;

    int do_many_bk4 lightweight (void)
    {
      if (!spawned4)
        _bk ();
      else;
      while ((*_thr).treq_top && j + 1 < J_end)
        {
          int j1_2 = J_end;
          int j0_2 = (1 + j + J_end) / 2;

          J_end = j0_2;
          pthis = (struct t_search_c *) malloc (sizeof (struct t_search_c));
          (*pthis).my_v_list_cur = my_v_list_cur;
          (*pthis).j0 = j0_2;
          (*pthis).j1 = j1_2;
          spawned4++;
          make_and_send_task (_thr, 3, pthis);
        }
      return 0;
    }
    if ((*_thr).req)
      handle_req (do_many_bk4, _thr);
    else;
    for (; j < J_end; j++)
      {
        search1_p (do_many_bk4, _thr, ((*my_v_list_cur).v)[j], 30, 0,
                   (VV[((*my_v_list_cur).v)[j]]).degree);
      }
    while (spawned4-- > 0)
      {
        pthis = (struct t_search_c *) wait_rslt (_thr);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.first =
          nconc_v_list ((*pthis).v_list.first, (*pthis).v_list.last,
                        (*((struct worker_data *) (*_thr).wdptr)).V_List.first);
        (*((struct worker_data *) (*_thr).wdptr)).V_List.length +=
          (*pthis).v_list.length;
        free (pthis);
      }
  }
}

int make_edge1 (int v1, int v2, int e)
{
  (EE[e]).next_v = v2;
  (EE[e]).next_e = (VV[v1]).first_e;
  (VV[v1]).first_e = e++;
  ((VV[v1]).degree)++;
  return e;
}

int check_dup1 (int v1, int v2)
{
  int i;
  int d = (VV[v1]).degree;
  int e = (VV[v1]).first_e;

  {
    i = 0;
    for (; i < d; (i++, e = (EE[e]).next_e))
      {
        int nv = (EE[e]).next_v;

        if (nv == v2)
          return 0;
        else;
      }
  }
  return 1;
}

int check_make_edge1 (int v1, int v2, int e)
{
  if (check_dup1 (v1, v2))
    {
      e = make_edge1 (v1, v2, e);
    }
  else;
  return e;
}

int make_edge (int v1, int v2, int e)
{
  e = make_edge1 (v1, v2, e);
  e = make_edge1 (v2, v1, e);
  return e;
}

int check_make_edge (int v1, int v2, int e)
{
  e = check_make_edge1 (v1, v2, e);
  e = check_make_edge1 (v2, v1, e);
  return e;
}

void make_bintree (int depth)
{
  int n = (1 << depth) - 1;
  unsigned long next = depth;
  int i;
  int e;

  fprintf (stderr, "Depth %d binary tree (%d vertices)\n", depth, n);
  {
    i = 0;
    for (; i < n; i++)
      {
        (VV[i]).degree = 0;
        (VV[i]).first_e = 0;
        (VV[i]).parent = 0;
      }
  }
  e = 0;
  {
    i = 0;
    for (; i < n; i++)
      {
        if (1)
          {
            int v[3];
            int p;
            int q;
            int n_e;

            if (i == 0)
              {
                n_e = 2;
                v[0] = i * 2 + 1;
                v[1] = i * 2 + 2;
              }
            else if (i < (n - 1) / 2)
              {
                n_e = 3;
                v[0] = (i - 1) / 2;
                v[1] = i * 2 + 1;
                v[2] = i * 2 + 2;
              }
            else
              {
                n_e = 1;
                v[0] = (i - 1) / 2;
              }
            next = next * 1103515245 + 12345;
            {
              q = n_e;
              for (; q > 0; q--)
                {
                  next = next * 1103515245 + 12345;
                  p = q * (next / 65536 % 32768) / 32768;
                  e = check_make_edge1 (i, v[p], e);
                  v[p] = v[q - 1];
                }
            }
          }
        else if (i > 0)
          e = make_edge (i, (i - 1) / 2, e);
        else;
      }
  }
  V_Length = n;
  E_Length = e;
}

void make_2dt (int n)
{
  int ix;
  int iy;
  int e;
  unsigned long next = n;

  fprintf (stderr, "%dx%d nodes 2D torus\n", n, n);
  {
    ix = 0;
    for (; ix < n; ix++)
      {
        iy = 0;
        for (; iy < n; iy++)
          {
            int k = ix * n + iy;

            (VV[k]).degree = 0;
            (VV[k]).first_e = 0;
            (VV[k]).parent = 0;
          }
      }
  }
  e = 0;
  {
    ix = 0;
    for (; ix < n; ix++)
      {
        iy = 0;
        for (; iy < n; iy++)
          {
            int k = ix * n + iy;
            int px = ix - 1;
            int py = iy - 1;
            int sx = ix + 1;
            int sy = iy + 1;

            if (px < 0)
              {
                px += n;
              }
            else;
            if (py < 0)
              {
                py += n;
              }
            else;
            if (sx >= n)
              {
                sx -= n;
              }
            else;
            if (sy >= n)
              {
                sy -= n;
              }
            else;
            {
              int v[4];
              int p;
              int q;

              v[0] = ix * n + py;
              v[1] = px * n + iy;
              v[2] = ix * n + sy;
              v[3] = sx * n + iy;
              next = next * 1103515245 + 12345;
              {
                q = 4;
                for (; q > 0; --q)
                  {
                    next = next * 1103515245 + 12345;
                    p = q * (next / 65536 % 32768) / 32768;
                    e = check_make_edge1 (k, v[p], e);
                    v[p] = v[q - 1];
                  }
              }
            }
          }
      }
  }
  V_Length = n * n;
  E_Length = e;
}

void make_ncube (int n)
{
  int i;
  int j;
  int s = 1 << n;
  int e = 0;
  unsigned long next = n;

  fprintf (stderr, "%d-D hypercube\n", n, n);
  {
    i = 0;
    for (; i < s; i++)
      {
        (VV[i]).degree = 0;
        (VV[i]).first_e = 0;
        (VV[i]).parent = 0;
      }
  }
  {
    i = 0;
    for (; i < s; i++)
      {
        int p;
        int q;
        int *v = (int *) malloc (n * sizeof (int));

        {
          j = 0;
          for (; j < n; j++)
            {
              v[j] = i ^ 1 << j;
            }
        }
        next = next * 1103515245 + 12345;
        {
          q = n;
          for (; q > 0; --q)
            {
              next = next * 1103515245 + 12345;
              p = q * (next / 65536 % 32768) / 32768;
              e = make_edge1 (i, v[p], e);
              v[p] = v[q - 1];
            }
        }
        free (v);
      }
  }
  V_Length = s;
  E_Length = e;
}

void make_random (int n, int m)
{
  int i;
  int j;
  int e = 0;
  unsigned long next = n + m;

  fprintf (stderr, "%d nodes random (%d edges per node)\n", n, m);
  {
    i = 0;
    for (; i < n; i++)
      {
        (VV[i]).degree = 0;
        (VV[i]).first_e = 0;
        (VV[i]).parent = 0;
      }
  }
  {
    i = 0;
    for (; i < n; i++)
      {
        next = next * 1103515245 + 12345;
        {
          j = 0;
          for (; j < m; j++)
            {
              int v;

              next = next * 1103515245 + 12345;
              v = n * (next / 65536 % 32768) / 32768;
              e = check_make_edge (i, v, e);
            }
        }
      }
  }
  V_Length = n;
  E_Length = e;
}

void show_graph ()
{
  int v;
  int e;
  int i;

  {
    v = 0;
    for (; v < V_Length; v++)
      {
        fprintf (stderr, "%9d[%d] (", v, (VV[v]).parent - 1);
        {
          int d = (VV[v]).degree;
          int e = (VV[v]).first_e;

          if (d > 0)
            {
              fprintf (stderr, "%d", (EE[e]).next_v);
              e = (EE[e]).next_e;
              {
                i = 1;
                for (; i < d; (i++, e = (EE[e]).next_e))
                  {
                    fprintf (stderr, ",%d", (EE[e]).next_v);
                  }
              }
            }
          else;
        }
        fprintf (stderr, ")\n");
      }
  }
}

void show_graph2 ()
{
  int v;
  int e;
  int i;

  {
    v = 0;
    for (; v < V_Length; v++)
      {
        fprintf (stderr, "%9d[%d] e: %d\n", v, (VV[v]).degree, (VV[v]).first_e);
      }
  }
  {
    e = 0;
    for (; e < E_Length; e++)
      {
        fprintf (stderr, "%9d => %d -> e: %d\n", e, (EE[e]).next_v,
                 (EE[e]).next_e);
      }
  }
}

void dump_graph (char *name, char *file)
{
  int v;
  int e;
  FILE *fp;

  fp = fopen (file, "w");
  fprintf (fp, "%s\n", name);
  fprintf (fp, "vertex\n");
  fprintf (fp, "%d\n", V_Length);
  {
    v = 0;
    for (; v < V_Length; v++)
      {
        fprintf (fp, "%d %d\n", (VV[v]).degree, (VV[v]).first_e);
      }
  }
  fprintf (fp, "edge\n");
  fprintf (fp, "%d\n", E_Length);
  {
    e = 0;
    for (; e < E_Length; e++)
      {
        fprintf (fp, "%d %d\n", (EE[e]).next_v, (EE[e]).next_e);
      }
  }
  fclose (fp);
  fprintf (stderr, "Graph \"%s\" successfully dumped.\n", name);
  return;
}

void expect_int (char *str, int expct, int n)
{
  if (expct != n)
    {
      fprintf (stderr, "%s: expectes %d but %d", str, expct, n);
      exit (-1);
    }
  else;
  return;
}

void read_graph (char *file)
{
  int v;
  int e;
  char name[1024];
  char str[1024];
  FILE *fp;

  fp = fopen (file, "r");
  expect_int ("fscanf", 1, fscanf (fp, "%s", name));
  expect_int ("fscanf", 1, fscanf (fp, "%s", str));
  if (!(str[0] == 'v'))
    {
      fprintf (stderr, "File format error\n");
      exit (-1);
    }
  else;
  expect_int ("fscanf", 1, fscanf (fp, "%d", &V_Length));
  {
    v = 0;
    for (; v < V_Length; v++)
      {
        (VV[v]).parent = 0;
        expect_int ("fscanf", 2,
                    fscanf (fp, "%d %d", &(VV[v]).degree, &(VV[v]).first_e));
      }
  }
  expect_int ("fscanf", 1, fscanf (fp, "%s", str));
  if (!(str[0] == 'e'))
    {
      fprintf (stderr, "File format error\n");
      exit (-1);
    }
  else;
  expect_int ("fscanf", 1, fscanf (fp, "%d", &E_Length));
  {
    e = 0;
    for (; e < E_Length; e++)
      {
        expect_int ("fscanf", 2,
                    fscanf (fp, "%d %d", &(EE[e]).next_v, &(EE[e]).next_e));
      }
  }
  fclose (fp);
  fprintf (stderr, "Graph \"%s\" successfully loaded.\n", name);
  return;
}

void check_graph ()
{
  int v;

  {
    v = 0;
    for (; v < V_Length; v++)
      {
        if ((VV[v]).parent - 1 < 0 || (VV[v]).parent - 1 >= V_Length)
          {
            fprintf (stderr, "!! %9d[%d]\n", v, (VV[v]).parent - 1);
          }
        else;
      }
  }
}

struct spanning_start
{
  int graph;
  int algorithm;
  int show_graph_p;
  int show_tree_p;
  char _dummy_[1000];
};

void do_spanning_start_task (struct thread_data *_thr,
                             struct spanning_start *pthis)
{

  int _bk lightweight (void)
  {
    return 0;
  }
  struct timeval tp[2];
  char *algstr;
  char *gname;

  fprintf (stderr, "CALL-BOUND: %d\n", 30);
  fprintf (stderr, "BARRIER: %s\n",
           "type 3 (atomic_read->atomic_write_to_start_read)");
  gettimeofday (tp, 0);
  switch ((*pthis).graph)
    {
    case 1:
      make_random (2000 * 2000, 2);
      gname = "Random4000000.graph";
      break;
    case 2:
      make_2dt (2000);
      gname = "2DTorus.graph";
      break;
    case 3:
      make_ncube (20);
      gname = "HCube20.graph";
      break;
    case 4:
      make_bintree (24);
      gname = "Bintree24.graph";
      break;
    case 5:
      make_ncube (21);
      gname = "HCube21.graph";
      break;
    case 11:
      gname = "Random4000000.graph";
      read_graph (gname);
      break;
    case 12:
      gname = "2DTorus.graph";
      read_graph (gname);
      break;
    case 13:
      gname = "HCube20.graph";
      read_graph (gname);
      break;
    case 14:
      gname = "Bintree24.graph";
      read_graph (gname);
      break;
    case 15:
      gname = "HCube21.graph";
      read_graph (gname);
      break;
    default:
      make_random (2000 * 2000, 2);
      gname = "Random4000000.graph";
      break;
    }
  gettimeofday (tp + 1, 0);
  fprintf (stderr, "Graph constuction time: %lf\n", elapsed_time (tp));
  fprintf (stderr, "v: %d e: %d\n", V_Length, E_Length);
  switch ((*pthis).show_graph_p)
    {
    case 1:
      show_graph ();
      break;
    case 2:
      show_graph2 ();
      break;
    }
  gettimeofday (tp, 0);
  switch ((*pthis).algorithm)
    {
    case 1:
      search_s2 (0);
      algstr = "search-s2";
      break;
    case 2:
      search (0);
      algstr = "search";
      break;
    case 3:
      search_p (_bk, _thr, 0);
      algstr = "search-p";
      break;
    case 4:
      search_p_array (_bk, _thr, 0);
      algstr = "search-p-array";
      break;
    case 99:
      dump_graph (gname, gname);
      exit (0);
    default:
      search_s2 (0);
      algstr = "search-s2";
      break;
    }
  gettimeofday (tp + 1, 0);
  fprintf (stderr, "%s finished.\ntime: %lf\n", algstr, elapsed_time (tp));
  switch ((*pthis).show_tree_p)
    {
    case 1:
      show_graph ();
      break;
    case 2:
      show_graph2 ();
      break;
    }
  check_graph ();
}

void data_allocate (int n1)
{
}

void data_send (int n1, int n2)
{
}

void data_receive (int n1, int n2)
{
}

void send_spanning_start_task (struct spanning_start *pthis)
{
  send_int ((*pthis).graph);
  send_int ((*pthis).algorithm);
  send_int ((*pthis).show_graph_p);
  send_int ((*pthis).show_tree_p);
}

struct spanning_start *recv_spanning_start_task ()
{
  struct spanning_start *pthis = malloc (sizeof (struct spanning_start));

  (*pthis).graph = recv_int ();
  (*pthis).algorithm = recv_int ();
  (*pthis).show_graph_p = recv_int ();
  (*pthis).show_tree_p = recv_int ();
  return pthis;
}

void send_spanning_start_rslt (struct spanning_start *pthis)
{
  free (pthis);
}

void recv_spanning_start_rslt (struct spanning_start *pthis)
{
}

void send_t_search_c_task (struct t_search_c *pthis)
{
  ;
  send_int ((*pthis).j0);
  send_int ((*pthis).j1);
}

struct t_search_c *recv_t_search_c_task ()
{
  struct t_search_c *pthis = malloc (sizeof (struct t_search_c));

  ;
  (*pthis).j0 = recv_int ();
  (*pthis).j1 = recv_int ();
  return pthis;
}

void send_t_search_c_rslt (struct t_search_c *pthis)
{
  ;
  free (pthis);
}

void recv_t_search_c_rslt (struct t_search_c *pthis)
{
  ;
}

void send_t_search_b_array_task (struct t_search_b_array *pthis)
{
  ;
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
}

struct t_search_b_array *recv_t_search_b_array_task ()
{
  struct t_search_b_array *pthis = malloc (sizeof (struct t_search_b_array));

  ;
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  return pthis;
}

void send_t_search_b_array_rslt (struct t_search_b_array *pthis)
{
  ;
  free (pthis);
}

void recv_t_search_b_array_rslt (struct t_search_b_array *pthis)
{
  ;
}

void send_t_search_b_task (struct t_search_b *pthis)
{
  ;
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
}

struct t_search_b *recv_t_search_b_task ()
{
  struct t_search_b *pthis = malloc (sizeof (struct t_search_b));

  ;
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  return pthis;
}

void send_t_search_b_rslt (struct t_search_b *pthis)
{
  ;
  free (pthis);
}

void recv_t_search_b_rslt (struct t_search_b *pthis)
{
  ;
}

void send_t_search1_task (struct t_search1 *pthis)
{
  send_int ((*pthis).v);
  send_int ((*pthis).b);
  send_int ((*pthis).k0);
  send_int ((*pthis).k1);
}

struct t_search1 *recv_t_search1_task ()
{
  struct t_search1 *pthis = malloc (sizeof (struct t_search1));

  (*pthis).v = recv_int ();
  (*pthis).b = recv_int ();
  (*pthis).k0 = recv_int ();
  (*pthis).k1 = recv_int ();
  return pthis;
}

void send_t_search1_rslt (struct t_search1 *pthis)
{
  ;
  free (pthis);
}

void recv_t_search1_rslt (struct t_search1 *pthis)
{
  ;
}

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_t_search1_task,
    (void (*)(struct thread_data *, void *)) do_t_search_b_task,
    (void (*)(struct thread_data *, void *)) do_t_search_b_array_task,
    (void (*)(struct thread_data *, void *)) do_t_search_c_task,
    (void (*)(struct thread_data *, void *)) do_spanning_start_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_t_search1_task,
    (void (*)(void *)) send_t_search_b_task,
    (void (*)(void *)) send_t_search_b_array_task,
    (void (*)(void *)) send_t_search_c_task,
    (void (*)(void *)) send_spanning_start_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_t_search1_task, (void *(*)()) recv_t_search_b_task,
    (void *(*)()) recv_t_search_b_array_task,
    (void *(*)()) recv_t_search_c_task, (void *(*)()) recv_spanning_start_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_t_search1_rslt,
    (void (*)(void *)) send_t_search_b_rslt,
    (void (*)(void *)) send_t_search_b_array_rslt,
    (void (*)(void *)) send_t_search_c_rslt,
    (void (*)(void *)) send_spanning_start_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_t_search1_rslt,
    (void (*)(void *)) recv_t_search_b_rslt,
    (void (*)(void *)) recv_t_search_b_array_rslt,
    (void (*)(void *)) recv_t_search_c_rslt,
    (void (*)(void *)) recv_spanning_start_rslt};
