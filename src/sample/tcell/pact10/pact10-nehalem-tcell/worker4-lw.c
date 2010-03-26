typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t        fun;
  void           *fr;
}               closure_t;
typedef double  Align_t;

char           *lw_call(char *esp);

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

void           *thread_origin(void *farg);

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include<getopt.h>

int             connect_to(char *hostname, unsigned short port);

void            close_socket(int socket);

int             send_char(char, int);

int             send_string(char *str, int socket);

int             send_fmt_string(int socket, char *fmt_string,...);

int             send_binary(void *src, unsigned long elm_size, unsigned long n_elm, int socket);

int             receive_char(int socket);

char           *receive_line(char *buf, int maxlen, int socket);

int             receive_binary(void *dst, unsigned long elm_size, unsigned long n_elm, int socket);
enum node {
OUTSIDE = -1, INSIDE = -2, ANY = -3, PARENT = -4, TERM = -5};
enum command {
  TASK, RSLT, TREQ, NONE, RACK, EXIT, WRNG
};
extern char    *cmd_strings[];
enum choose {
  CHS_RANDOM, CHS_ORDER
};

struct cmd {
  enum command    w;
  int             c;
  enum node       node;
  enum node       v[4][16];
};

struct cmd_list {
  struct cmd      cmd;
  void           *body;
  int             task_no;
  struct cmd_list *next;
};

void            proto_error(char const *str, struct cmd *pcmd);

void            read_to_eol(void);

void            recv_rslt(struct cmd *, void *);

void            recv_task(struct cmd *, void *);

void            recv_treq(struct cmd *);

void            recv_rack(struct cmd *);

void            recv_none(struct cmd *);
struct task;
struct thread_data;
void            (*task_doers[256]) (char *, struct thread_data *, void *);
void            (*task_senders[256]) (void *);
void           *(*task_receivers[256]) ();
void            (*rslt_senders[256]) (void *);
void            (*rslt_receivers[256]) (void *);
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
  int             task_no;
  void           *body;
  int             ndiv;
  enum node       rslt_to;
  enum node       rslt_head[16];
};

struct task_home {
  enum task_home_stat stat;
  int             id;
  int             task_no;
  enum node       req_from;
  struct task_home *next;
  void           *body;
  enum node       task_head[16];
};

struct thread_data {
  struct task_home *req;
  int             id;
  int             w_rack;
  int             w_none;
  int             ndiv;
  int             last_treq;
  enum choose     last_choose;
  double          random_seed1;
  double          random_seed2;
  struct task    *task_free;
  struct task    *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t  cond;
  pthread_cond_t  cond_r;
};

void            make_and_send_task(struct thread_data *thr, int task_no, void *body);

void           *wait_rslt(char *esp, struct thread_data *thr);

int             serialize_cmdname(char *buf, enum command w);

int             deserialize_cmdname(enum command *buf, char *str);

int             serialize_arg(char *buf, enum node *arg);

enum node       deserialize_node(char *str);

int             deserialize_arg(enum node *buf, char *str);

int             serialize_cmd(char *buf, struct cmd *pcmd);

int             deserialize_cmd(struct cmd *pcmd, char *str);

int             copy_address(enum node *dst, enum node *src);

struct systhr_create_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  pthread_attr_t  attr;
  pthread_t       tid;
  int             status;
  void           *arg;
  void           *(*start_func) (char *, void *);
};

int             systhr_create(char *esp, void *(*start_func) (char *, void *), void *arg){
  struct systhr_create_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  int             status;
  struct func_arg *farg;
  struct func_arg *farg2;

LGOTO:;
  efp = (struct systhr_create_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct systhr_create_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  status = 0;
  pthread_attr_init(&efp->attr);
  status = pthread_attr_setscope(&efp->attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0) {
    status = (farg = (struct func_arg *)malloc(sizeof(struct func_arg)), farg->func = start_func, farg->arg = arg, pthread_create(&efp->tid, &efp->attr, thread_origin, farg));
  } else {
    status = (farg2 = (struct func_arg *)malloc(sizeof(struct func_arg)), farg2->func = start_func, farg2->arg = arg, pthread_create(&efp->tid, 0, thread_origin, farg2));
  }
  return status;
}

void            mem_error(char const *str)
{
  fputs(str, stderr);
  fputc('\n', stderr);
}

void            proto_error(char const *str, struct cmd *pcmd)
{
  int             i;
  char            buf[1280];

  serialize_cmd(buf, pcmd);
  fputs(str, stderr);
  fputc('>', stderr);
  fputc(' ', stderr);
  fputs(buf, stderr);
  fputc('\n', stderr);
}
pthread_mutex_t send_mut;
int             sv_socket;

void            read_to_eol(void)
{
  int             c;

  while (EOF != (c = receive_char(sv_socket))) {
    if (c == '\n')
      break;
  }
}

void            write_eol(void)
{
  send_char('\n', sv_socket);
}

void            flush_send(void)
{
  if (sv_socket < 0)
    fflush(stdout);
}
char            buf[1280];
struct cmd      cmd_buf;

struct cmd     *read_command()
{
  char            p;
  char            c;
  char           *b = buf;
  int             cmdc;

  receive_line(b, 1280, sv_socket);
  cmd_buf.node = OUTSIDE;
  deserialize_cmd(&cmd_buf, b);
  return &cmd_buf;
}
char            send_buf[1280];

void            send_out_command(struct cmd *pcmd, void *body, int task_no)
{
  int             ret;
  enum command    w;

  w = (*pcmd).w;
  pthread_mutex_lock(&send_mut);
  serialize_cmd(send_buf, pcmd);
  send_string(send_buf, sv_socket);
  write_eol();
  if (body)
    if (w == TASK) {
      (task_senders[task_no]) (body);
      write_eol();
    } else if (w == RSLT) {
      (rslt_senders[task_no]) (body);
      write_eol();
    } else;
  flush_send();
  pthread_mutex_unlock(&send_mut);
}

void            proc_cmd(struct cmd *pcmd, void *body)
{
  enum command    w;

  w = (*pcmd).w;
  if (w == TASK) {
    recv_task(pcmd, body);
  } else if (w == RSLT) {
    recv_rslt(pcmd, body);
  } else if (w == TREQ) {
    recv_treq(pcmd);
  } else if (w == NONE) {
    recv_none(pcmd);
  } else if (w == RACK) {
    recv_rack(pcmd);
  } else if (w == EXIT) {
    exit(0);
  } else {
    proto_error("wrong cmd", pcmd);
  }
}

void            send_command(struct cmd *pcmd, void *body, int task_no)
{
  if ((*pcmd).node == OUTSIDE)
    send_out_command(pcmd, body, task_no);
  else
    proc_cmd(pcmd, body);
}
struct thread_data threads[64];
unsigned int    num_thrs;

void            flush_treq_with_none(struct thread_data *thr)
{
  struct cmd      rcmd;
  struct task_home *hx;

  rcmd.c = 1;
  rcmd.w = NONE;
  while (hx = (*thr).treq_top) {
    rcmd.node = (*hx).req_from;
    copy_address((rcmd.v)[0], (*hx).task_head);
    send_command(&rcmd, 0, 0);
    (*thr).treq_top = (*hx).next;
    (*hx).next = (*thr).treq_free;
    (*thr).treq_free = hx;
  }
}

struct recv_exec_send_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  long            nsec;
  struct timeval  now;
  struct timespec t1;
  int             ifexp_result8;
  long            ifexp_result7;
  int             ifexp_result9;
  int             ifexp_result10;
  int             ifexp_result3;
  int             ifexp_result4;
  int             ifexp_result5;
  int             ifexp_result6;
  int             ifexp_result;
  int             ifexp_result2;
  int             old_ndiv;
  long            delay;
  struct task    *tx;
  struct cmd      rcmd;
  enum node       req_to;
  enum node      *treq_head;
  struct thread_data *thr;
};

void            recv_exec_send(char *esp, struct thread_data *thr, enum node *treq_head, enum node req_to)
{
  struct recv_exec_send_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  struct task    *tx;
  long            delay;
  int             old_ndiv;
  int             ifexp_result2;
  int             ifexp_result;
  int             ifexp_result6;
  int             ifexp_result5;
  int             ifexp_result4;
  int             ifexp_result3;
  int             ifexp_result10;
  int             ifexp_result9;
  long            ifexp_result7;
  int             ifexp_result8;
  long            nsec;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct recv_exec_send_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct recv_exec_send_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL;
    }
    goto L_CALL;
  }
  efp = (struct recv_exec_send_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct recv_exec_send_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  while ((*thr).w_none > 0) {
    pthread_cond_wait(&(*thr).cond, &(*thr).mut);
    {
      if ((*thr).sub) {
        if ((*(*thr).sub).stat == TASK_HOME_DONE) {
          ifexp_result2 = 1;
        } else {
          ifexp_result2 = 0;
        }
        ifexp_result = ifexp_result2;
      } else {
        ifexp_result = 0;
      }
      if (ifexp_result)
        return;
    }
  }
  tx = (*thr).task_free;
  (*tx).stat = TASK_ALLOCATED;
  if (!tx) {
    mem_error("Not enough task memory");
  }
  (*thr).task_top = tx;
  (*thr).task_free = (*tx).prev;
  delay = 1000;
  (efp->rcmd).c = 2;
  if (num_thrs > 1) {
    (efp->rcmd).node = req_to;
  } else {
    (efp->rcmd).node = OUTSIDE;
  }
  (efp->rcmd).w = TREQ;
  ((efp->rcmd).v)[0][0] = (*thr).id;
  ((efp->rcmd).v)[0][1] = TERM;
  copy_address(((efp->rcmd).v)[1], treq_head);
  do {
    flush_treq_with_none(thr);
    (*tx).stat = TASK_ALLOCATED;
    {
      pthread_mutex_unlock(&(*thr).mut);
      send_command(&efp->rcmd, 0, 0);
      pthread_mutex_lock(&(*thr).mut);
    }
    while (1) {
      {
        if ((*tx).stat != TASK_INITIALIZED) {
          {
            if ((*thr).sub) {
              if ((*(*thr).sub).stat == TASK_HOME_DONE) {
                ifexp_result6 = 1;
              } else {
                ifexp_result6 = 0;
              }
              ifexp_result5 = ifexp_result6;
            } else {
              ifexp_result5 = 0;
            }
            if (ifexp_result5) {
              ifexp_result4 = 1;
            } else {
              ifexp_result4 = 0;
            }
          }
          ifexp_result3 = ifexp_result4;
        } else {
          ifexp_result3 = 0;
        }
        if (ifexp_result3) {
          if ((*tx).stat != TASK_NONE) {
            ((*thr).w_none)++;
          }
          goto Lnone;
        }
      }
      if ((*tx).stat != TASK_ALLOCATED)
        break;
      pthread_cond_wait(&(*thr).cond, &(*thr).mut);
    }
    if ((*tx).stat == TASK_NONE) {
      if (1) {
        gettimeofday(&efp->now, 0);
        nsec = (efp->now).tv_usec * 1000;
        nsec += delay;
        delay += delay;
        if (delay > 40 * 1000 * 1000) {
          delay = 40 * 1000 * 1000;
        } {
          if (nsec > 999999999) {
            ifexp_result7 = nsec - 999999999;
          } else {
            ifexp_result7 = nsec;
          }
          (efp->t1).tv_nsec = ifexp_result7;
        }
        {
          if (nsec > 999999999) {
            ifexp_result8 = 1;
          } else {
            ifexp_result8 = 0;
          }
          (efp->t1).tv_sec = (efp->now).tv_sec + ifexp_result8;
        }
        pthread_cond_timedwait(&(*thr).cond_r, &(*thr).mut, &efp->t1);
      } {
        if ((*thr).sub) {
          if ((*(*thr).sub).stat == TASK_HOME_DONE) {
            ifexp_result10 = 1;
          } else {
            ifexp_result10 = 0;
          }
          ifexp_result9 = ifexp_result10;
        } else {
          ifexp_result9 = 0;
        }
        if (ifexp_result9)
          goto Lnone;
      }
    }
  } while ((*tx).stat != TASK_INITIALIZED);
  (*tx).stat = TASK_STARTED;
  old_ndiv = (*thr).ndiv;
  (*thr).ndiv = (*tx).ndiv;
  pthread_mutex_unlock(&(*thr).mut);
  new_esp = esp;
  while ((task_doers[(*tx).task_no]) (new_esp, thr, (*tx).body), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
    efp->nsec = nsec;
    efp->ifexp_result8 = ifexp_result8;
    efp->ifexp_result7 = ifexp_result7;
    efp->ifexp_result9 = ifexp_result9;
    efp->ifexp_result10 = ifexp_result10;
    efp->ifexp_result3 = ifexp_result3;
    efp->ifexp_result4 = ifexp_result4;
    efp->ifexp_result5 = ifexp_result5;
    efp->ifexp_result6 = ifexp_result6;
    efp->ifexp_result = ifexp_result;
    efp->ifexp_result2 = ifexp_result2;
    efp->old_ndiv = old_ndiv;
    efp->delay = delay;
    efp->tx = tx;
    efp->req_to = req_to;
    efp->treq_head = treq_head;
    efp->thr = thr;
    efp->call_id = 0;
    return;
L_CALL:;
    nsec = efp->nsec;
    ifexp_result8 = efp->ifexp_result8;
    ifexp_result7 = efp->ifexp_result7;
    ifexp_result9 = efp->ifexp_result9;
    ifexp_result10 = efp->ifexp_result10;
    ifexp_result3 = efp->ifexp_result3;
    ifexp_result4 = efp->ifexp_result4;
    ifexp_result5 = efp->ifexp_result5;
    ifexp_result6 = efp->ifexp_result6;
    ifexp_result = efp->ifexp_result;
    ifexp_result2 = efp->ifexp_result2;
    old_ndiv = efp->old_ndiv;
    delay = efp->delay;
    tx = efp->tx;
    req_to = efp->req_to;
    treq_head = efp->treq_head;
    thr = efp->thr;
    new_esp = esp + 1;
  }
  (efp->rcmd).w = RSLT;
  (efp->rcmd).c = 1;
  (efp->rcmd).node = (*tx).rslt_to;
  copy_address(((efp->rcmd).v)[0], (*tx).rslt_head);
  send_command(&efp->rcmd, (*tx).body, (*tx).task_no);
  flush_treq_with_none(thr);
  pthread_mutex_lock(&(*thr).rack_mut);
  ((*thr).w_rack)++;
  pthread_mutex_unlock(&(*thr).rack_mut);
  pthread_mutex_lock(&(*thr).mut);
  (*thr).ndiv = old_ndiv;
Lnone:;
  {
    (*thr).task_free = tx;
    (*thr).task_top = (*tx).next;
  }
}

struct worker_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct thread_data *thr;
  void           *arg;
};

void           *worker(char *esp, void *arg)
{
  struct worker_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  struct thread_data *thr;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct worker_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct worker_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL2;
    }
    goto L_CALL2;
  }
  efp = (struct worker_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct worker_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  thr = arg;
  pthread_mutex_lock(&(*thr).mut);
  while (1) {
    new_esp = esp;
      while (recv_exec_send(new_esp, thr, (enum node[2]){
      ANY, TERM
      }, ANY), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
      efp->thr = thr;
      efp->arg = arg;
      efp->call_id = 0;
      return (void *)0 - 1;
  L_CALL2:;
      thr = efp->thr;
      arg = efp->arg;
      new_esp = esp + 1;
      }
  }
  pthread_mutex_unlock(&(*thr).mut);
}

void            recv_task(struct cmd *pcmd, void *body)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int    id;
  int             task_no;
  size_t          len;

  if ((*pcmd).c < 4)
    proto_error("wrong-task", pcmd);
  task_no = ((*pcmd).v)[3][0];
  if ((*pcmd).node == OUTSIDE) {
    body = (task_receivers[task_no]) ();
    read_to_eol();
  }
  id = ((*pcmd).v)[2][0];
  if (!(id < num_thrs))
    proto_error("wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  tx = (*thr).task_top;
  (*tx).rslt_to = (*pcmd).node;
  copy_address((*tx).rslt_head, ((*pcmd).v)[1]);
  (*tx).ndiv = ((*pcmd).v)[0][0];
  (*tx).task_no = task_no;
  (*tx).body = body;
  (*tx).stat = TASK_INITIALIZED;
  pthread_mutex_unlock(&(*thr).mut);
  pthread_cond_signal(&(*thr).cond);
}

void            recv_none(struct cmd *pcmd)
{
  struct thread_data *thr;
  unsigned int    id;
  size_t          len;

  if ((*pcmd).c < 1)
    proto_error("Wrong none", pcmd);
  id = ((*pcmd).v)[0][0];
  if (!(id < num_thrs))
    proto_error("Wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  if ((*thr).w_none > 0)
    ((*thr).w_none)--;
  else
    (*(*thr).task_top).stat = TASK_NONE;
  pthread_mutex_unlock(&(*thr).mut);
  pthread_cond_signal(&(*thr).cond);
}

void            recv_rslt(struct cmd *pcmd, void *body)
{
  struct cmd      rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int    tid;
  unsigned int    sid;

  if ((*pcmd).c < 1)
    proto_error("Wrong rslt", pcmd);
  tid = ((*pcmd).v)[0][0];
  if (!(tid < num_thrs))
    proto_error("wrong rslt-head", pcmd);
  sid = ((*pcmd).v)[0][1];
  if (TERM == sid)
    proto_error("Wrong rslt-head (no task-id)", pcmd);
  thr = threads + tid;
  pthread_mutex_lock(&(*thr).mut);
  hx = (*thr).sub;
  while (hx && (*hx).id != sid) {
    hx = (*hx).next;
  }
  if (!hx)
    proto_error("Wrong rslt-head (specified task not exists)", pcmd);
  if ((*pcmd).node == OUTSIDE) {
    (rslt_receivers[(*hx).task_no]) ((*hx).body);
    read_to_eol();
  } else if ((*pcmd).node == INSIDE) {
    (*hx).body = body;
  } else {
    proto_error("Wrong cmd.node", pcmd);
  }
  rcmd.c = 1;
  rcmd.node = (*pcmd).node;
  rcmd.w = RACK;
  copy_address((rcmd.v)[0], (*hx).task_head);
  (*hx).stat = TASK_HOME_DONE;
  if (hx == (*thr).sub) {
    pthread_cond_signal(&(*thr).cond_r);
    pthread_cond_signal(&(*thr).cond);
  }
  pthread_mutex_unlock(&(*thr).mut);
  send_command(&rcmd, 0, 0);
}

int             try_treq(struct cmd *pcmd, unsigned int id, enum node *from_addr)
{
  struct task_home *hx;
  struct thread_data *thr;
  int             avail = 0;
  enum node       from_head = from_addr[0];

  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  pthread_mutex_lock(&(*thr).rack_mut);
  if ((*thr).w_rack == 0 && ((*thr).task_top && ((*(*thr).task_top).stat == TASK_STARTED || (*(*thr).task_top).stat == TASK_INITIALIZED)))
    avail = 1;
  pthread_mutex_unlock(&(*thr).rack_mut);
  if (avail) {
    hx = (*thr).treq_free;
    if (!hx)
      mem_error("Not enough task-home memory");
    (*thr).treq_free = (*hx).next;
    (*hx).next = (*thr).treq_top;
    (*hx).stat = TASK_HOME_ALLOCATED;
    copy_address((*hx).task_head, ((*pcmd).v)[0]);
    if ((*pcmd).node != OUTSIDE)
      (*hx).req_from = INSIDE;
    else
      (*hx).req_from = OUTSIDE;
    (*thr).treq_top = hx;
    (*thr).req = hx;
  }
  pthread_mutex_unlock(&(*thr).mut);
  return avail;
}
double          random_seed1 = 0.2403703;
double          random_seed2 = 3.638732;

int             my_random(int max, double *pseed1, double *pseed2)
{
  *pseed1 = *pseed1 * 3.0 + *pseed2;
  *pseed1 -= (int)*pseed1;
  return max ** pseed1;
}

int             choose_treq(enum node from_node)
{
  if (0 <= from_node) {
    struct thread_data *thr = threads + from_node;

    (*thr).last_choose = (1 + (*thr).last_choose) % 2;
    if (CHS_RANDOM == (*thr).last_choose)
      return my_random(num_thrs, &(*thr).random_seed1, &(*thr).random_seed2);
    else if (CHS_ORDER == (*thr).last_choose) {
      (*thr).last_treq = (1 + (*thr).last_treq) % num_thrs;
      return (*thr).last_treq;
    } else
      return 0;
  } else if (PARENT == from_node)
    return my_random(num_thrs, &random_seed1, &random_seed2);
  else
    return 0;
}

void            recv_treq(struct cmd *pcmd)
{
  struct cmd      rcmd;
  unsigned int    id;

  if ((*pcmd).c < 2)
    proto_error("Wrong treq", pcmd);
  if (((*pcmd).v)[1][0] == ANY) {
    int             myid;
    int             start_id;
    int             d;

    myid = ((*pcmd).v)[0][0];
    start_id = choose_treq(myid);
    {
      d = 0;
      for (; d < num_thrs; d++) {
        id = (d + start_id) % num_thrs;
        if ((*pcmd).node != OUTSIDE && id == myid)
          continue;
        if (try_treq(pcmd, id, ((*pcmd).v)[0]))
          break;
      }
    }
    if (d != num_thrs)
      return;
  } else {
    id = ((*pcmd).v)[1][0];
    if (!(id < num_thrs))
      proto_error("Wrong task-head", pcmd);
    if (try_treq(pcmd, id, ((*pcmd).v)[0]))
      return;
  }
  if ((*pcmd).node == ANY)
    if (((*pcmd).v)[0][0] == 0) {
      (*pcmd).node = OUTSIDE;
      send_command(pcmd, 0, 0);
      return;
    } else
      (*pcmd).node = INSIDE;
  rcmd.c = 1;
  rcmd.node = (*pcmd).node;
  rcmd.w = NONE;
  copy_address((rcmd.v)[0], ((*pcmd).v)[0]);
  send_command(&rcmd, 0, 0);
}

void            recv_rack(struct cmd *pcmd)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int    id;
  size_t          len;

  if ((*pcmd).c < 1)
    proto_error("Wrong rack", pcmd);
  id = ((*pcmd).v)[0][0];
  if (!(id < num_thrs))
    proto_error("Wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).rack_mut);
  ((*thr).w_rack)--;
  pthread_mutex_unlock(&(*thr).rack_mut);
}

struct handle_req_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct thread_data *_thr;
  closure_t      *_bk;
};

void            handle_req(char *esp, closure_t * _bk, struct thread_data *_thr)
{
  char           *argp;
  struct handle_req_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct handle_req_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct handle_req_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL3;
    }
    goto L_CALL3;
  }
  efp = (struct handle_req_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct handle_req_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  pthread_mutex_lock(&(*_thr).mut);
  if ((*_thr).req) {
    {
      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = _bk;
      efp->_thr = _thr;
      efp->_bk = _bk;
      efp->argp = argp;
      efp->tmp_esp = argp;
      efp->call_id = 0;
      return;
  L_CALL3:;
      _thr = efp->_thr;
      _bk = efp->_bk;
    }
    (*_thr).req = (*_thr).treq_top;
  }
  pthread_mutex_unlock(&(*_thr).mut);
}

void            make_and_send_task(struct thread_data *thr, int task_no, void *body)
{
  struct cmd      tcmd;
  struct task_home *hx = (*thr).treq_top;

  (*thr).treq_top = (*hx).next;
  (*hx).next = (*thr).sub;
  (*thr).sub = hx;
  (*hx).task_no = task_no;
  (*hx).body = body;
  (*hx).id = (*hx).next ? (*(*hx).next).id + 1 : 0;
  (*hx).stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = (*hx).req_from;
  tcmd.w = TASK;
  (tcmd.v)[0][0] = ++(*thr).ndiv;
  (tcmd.v)[0][1] = TERM;
  (tcmd.v)[1][0] = (*thr).id;
  (tcmd.v)[1][1] = (*hx).id;
  (tcmd.v)[1][2] = TERM;
  copy_address((tcmd.v)[2], (*hx).task_head);
  (tcmd.v)[3][0] = task_no;
  (tcmd.v)[3][1] = TERM;
  send_command(&tcmd, body, task_no);
}

struct wait_rslt_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  int             ifexp_result12;
  long            ifexp_result11;
  struct task_home *sub;
  long            nsec;
  struct timeval  now;
  struct timespec t1;
  void           *body;
  struct thread_data *thr;
};

void           *wait_rslt(char *esp, struct thread_data *thr)
{
  struct wait_rslt_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  void           *body;
  long            nsec;
  struct task_home *sub;
  long            ifexp_result11;
  int             ifexp_result12;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct wait_rslt_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct wait_rslt_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL4;
    }
    goto L_CALL4;
  }
  efp = (struct wait_rslt_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct wait_rslt_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  pthread_mutex_lock(&(*thr).mut);
  sub = (*thr).sub;
  while ((*sub).stat != TASK_HOME_DONE) {
    if (PARENT == ((*sub).task_head)[0]) {
      gettimeofday(&efp->now, 0);
      nsec = 1000 * (efp->now).tv_usec;
      nsec += 1000;
      {
        if (nsec > 999999999) {
          ifexp_result11 = nsec - 999999999;
        } else {
          ifexp_result11 = nsec;
        }
        (efp->t1).tv_nsec = ifexp_result11;
      }
      {
        if (nsec > 999999999) {
          ifexp_result12 = 1;
        } else {
          ifexp_result12 = 0;
        }
        (efp->t1).tv_sec = (efp->now).tv_sec + ifexp_result12;
      }
      pthread_cond_timedwait(&(*thr).cond_r, &(*thr).mut, &efp->t1);
    }
    (*(*thr).task_top).stat = TASK_STARTED;
    if ((*sub).stat == TASK_HOME_DONE)
      break;
    new_esp = esp;
    while (recv_exec_send(new_esp, thr, (*sub).task_head, (*sub).req_from), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
      efp->ifexp_result12 = ifexp_result12;
      efp->ifexp_result11 = ifexp_result11;
      efp->sub = sub;
      efp->nsec = nsec;
      efp->body = body;
      efp->thr = thr;
      efp->call_id = 0;
      return (void *)0 - 1;
  L_CALL4:;
      ifexp_result12 = efp->ifexp_result12;
      ifexp_result11 = efp->ifexp_result11;
      sub = efp->sub;
      nsec = efp->nsec;
      body = efp->body;
      thr = efp->thr;
      new_esp = esp + 1;
    }
  }
  body = (*sub).body;
  (*thr).sub = (*sub).next;
  (*sub).next = (*thr).treq_free;
  (*thr).treq_free = sub;
  pthread_mutex_unlock(&(*thr).mut);
  return body;
}

struct runtime_option {
  int             num_thrs;
  char            sv_hostname[256];
  unsigned short  port;
};
static struct runtime_option option;

void            usage(int argc, char **argv)
{
  fprintf(stderr, "Usage: %s [-s hostname] [-p port-num] [-n n-threads]\n", argv[0]);
  exit(1);
}

void            set_option(int argc, char **argv)
{
  int             i;
  int             ch;

  option.num_thrs = 1;
  (option.sv_hostname)[0] = '\x0';
  option.port = 8888;
  while (-1 != (ch = getopt(argc, argv, "n:s:p:"))) {
    i = 0;
    for (; i < argc; i++) {
      switch (ch) {
      case 'n':
        option.num_thrs = atoi(optarg);
        break;
      case 's':
        if (strcmp("stdout", optarg)) {
          strncpy(option.sv_hostname, optarg, 256);
          (option.sv_hostname)[256 - 1] = 0;
        } else
          (option.sv_hostname)[0] = '\x0';
        break;
      case 'p':
        option.port = atoi(optarg);
        break;
      case 'h':
        usage(argc, argv);
        break;
      default:
        fprintf(stderr, "Unknown option: %c\n", ch);
        usage(argc, argv);
        break;
      }
    }
  }
  return;
}

struct main_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct thread_data *thr2;
  double          q;
  double          r;
  struct task_home *hx;
  struct task    *tx;
  struct thread_data *thr;
  struct cmd     *pcmd;
  int             j;
  int             i;
  int             ifexp_result13;
  char          **argv;
  int             argc;
};

int             main(int argc, char **argv)
{
  struct main_frame *efp;
  char           *new_esp;
  char            estack[16384];
  char           *esp = estack;
  int             ifexp_result13;
  int             i;
  int             j;
  struct cmd     *pcmd;
  struct thread_data *thr;
  struct task    *tx;
  struct task_home *hx;
  double          r;
  double          q;
  struct thread_data *thr2;

LGOTO:;
  efp = (struct main_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  set_option(argc, argv);
  {
    if ('\x0' == (option.sv_hostname)[0]) {
      ifexp_result13 = -1;
    } else {
      ifexp_result13 = connect_to(option.sv_hostname, option.port);
    }
    sv_socket = ifexp_result13;
  }
  pthread_mutex_init(&send_mut, 0);
  num_thrs = option.num_thrs;
  {
    i = 0;
    for (; i < num_thrs; i++) {
      thr = threads + i;
      (*thr).req = 0;
      (*thr).id = i;
      (*thr).w_rack = 0;
      (*thr).w_none = 0;
      (*thr).ndiv = 0;
      (*thr).last_treq = i;
      (*thr).last_choose = CHS_RANDOM;
      {
        r = sqrt(0.5 + i);
        q = sqrt(r + i);
        r -= (int)r;
        (*thr).random_seed1 = r;
        (*thr).random_seed2 = q;
      }
      pthread_mutex_init(&(*thr).mut, 0);
      pthread_mutex_init(&(*thr).rack_mut, 0);
      pthread_cond_init(&(*thr).cond, 0);
      pthread_cond_init(&(*thr).cond_r, 0);
      tx = (struct task *)malloc(sizeof(struct task) * 1024);
      (*thr).task_top = 0;
      (*thr).task_free = tx;
      {
        j = 0;
        for (; j < 1024 - 1; j++) {
          (tx[j]).prev = &tx[j + 1];
          (tx[j + 1]).next = &tx[j];
        }
      }
      (tx[0]).next = 0;
      (tx[1024 - 1]).prev = 0;
      hx = (struct task_home *)malloc(sizeof(struct task_home) * 1024);
      (*thr).treq_top = 0;
      (*thr).treq_free = hx;
      (*thr).sub = 0;
      {
        j = 0;
        for (; j < 1024 - 1; j++) {
          (hx[j]).next = &hx[j + 1];
        }
      }
      (hx[1024 - 1]).next = 0;
    }
  }
  {
    i = 0;
    for (; i < num_thrs; i++) {
      thr2 = threads + i;
      new_esp = esp;
      while (systhr_create(new_esp, worker, thr2), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
        char           *goto_fr;

        *((char **)esp) = 0;
        efp->thr2 = thr2;
        efp->q = q;
        efp->r = r;
        efp->hx = hx;
        efp->tx = tx;
        efp->thr = thr;
        efp->pcmd = pcmd;
        efp->j = j;
        efp->i = i;
        efp->ifexp_result13 = ifexp_result13;
        efp->argv = argv;
        efp->argc = argc;
        goto_fr = lw_call(efp->tmp_esp);
        if ((char *)goto_fr == (char *)efp)
          goto LGOTO;
        thr2 = efp->thr2;
        q = efp->q;
        r = efp->r;
        hx = efp->hx;
        tx = efp->tx;
        thr = efp->thr;
        pcmd = efp->pcmd;
        j = efp->j;
        i = efp->i;
        ifexp_result13 = efp->ifexp_result13;
        argv = efp->argv;
        argc = efp->argc;
        new_esp = esp + 1;
      }
    }
  }
  while (1) {
    pcmd = read_command();
    proc_cmd(pcmd, 0);
  }
  exit(0);
}
