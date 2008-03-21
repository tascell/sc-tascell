typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t        fun;
  void           *fr;
}               closure_t;
//typedef unsigned int size_t;
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
  OUTSIDE, INSIDE, ANY
};

struct cmd {
  int             c;
  enum node       node;
  char           *v[5];
};

struct cmd_list {
  struct cmd      cmd;
  void           *body;
  int             task_no;
  struct cmd_list *next;
};

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
  char            rslt_head[256];
};

struct task_home {
  enum task_home_stat stat;
  int             id;
  int             task_no;
  enum node       req_from;
  struct task_home *next;
  void           *body;
  char            task_head[256];
};

struct thread_data {
  struct task_home *req;
  int             id;
  int             w_rack;
  int             w_none;
  int             ndiv;
  struct task    *task_free;
  struct task    *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t  cond;
  pthread_cond_t  cond_r;
  char            ndiv_buf[32];
  char            tno_buf[8];
  char            id_str[32];
  char            buf[256];
};
extern int      divisibility_flag;

void            send_divisible(void);

void            make_and_send_task(struct thread_data *thr, int task_no, void *body);

void           *wait_rslt(char *esp, struct thread_data *thr);

void            send_int(int n);

int             recv_int(void);

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

  fputs(str, stderr);
  fputc('>', stderr);
  fputc(' ', stderr);
  {
    i = 0;
    for (; i < (*pcmd).c - 1; i++) {
      fputs(((*pcmd).v)[i], stderr);
      fputc(' ', stderr);
    }
  }
  if ((*pcmd).c > 0) {
    fputs(((*pcmd).v)[(*pcmd).c - 1], stderr);
    fputc('\n', stderr);
  }
}

struct cmd_queue {
  int             len;
  struct cmd_list *queue;
  char           *arg_buf;
  int             arg_size_max;
  struct cmd_list *in;
  struct cmd_list *out;
  pthread_mutex_t mut;
  pthread_cond_t  cond;
};

struct cmd_queue *make_cmd_queue(int length, int maxcmdc, int arg_size_max)
{
  struct cmd_queue *p_newq;
  int             i;
  int             j;

  p_newq = (struct cmd_queue *)malloc(sizeof(struct cmd_queue));
  (*p_newq).len = length;
  (*p_newq).queue = (struct cmd_list *)malloc(length * sizeof(struct cmd_list));
  (*p_newq).arg_buf = (char *)malloc(length * maxcmdc * arg_size_max * sizeof(char));
  (*p_newq).arg_size_max = arg_size_max;
  (*p_newq).in = (*p_newq).queue;
  (*p_newq).out = (*p_newq).queue;
  pthread_mutex_init(&(*p_newq).mut, 0);
  pthread_cond_init(&(*p_newq).cond, 0);
  {
    i = 0;
    for (; i < length; i++) {
      (((*p_newq).queue)[i]).next = &((*p_newq).queue)[(i + 1) % length];
      {
        j = 0;
        for (; j < maxcmdc; j++) {
          ((((*p_newq).queue)[i]).cmd.v)[j] = (*p_newq).arg_buf + maxcmdc * arg_size_max * i + arg_size_max * j;
        }
      }
    }
  }
  return p_newq;
}

void            enqueue_command(struct cmd *pcmd, void *body, int task_no, struct cmd_queue *pq)
{
  int             i;
  size_t          len;
  struct cmd_list *q;
  int             len_max = (*pq).arg_size_max;

  pthread_mutex_lock(&(*pq).mut);
  q = (*pq).in;
  if ((*q).next == (*pq).out) {
    perror("queue overflow\n");
    exit(1);
  }
  (*pq).in = (*q).next;
  (*q).cmd.c = (*pcmd).c;
  (*q).cmd.node = (*pcmd).node;
  {
    i = 0;
    for (; i < (*pcmd).c; i++) {
      len = strlen(((*pcmd).v)[i]);
      if (len >= len_max)
        proto_error("too long cmd", pcmd);
      strncpy(((*q).cmd.v)[i], ((*pcmd).v)[i], len + 1);
    }
  }
  (*q).body = body;
  (*q).task_no = task_no;
  pthread_mutex_unlock(&(*pq).mut);
  pthread_cond_signal(&(*pq).cond);
}
struct cmd_queue *inms_queue;
struct cmd_queue *send_queue;
char            buf[1280];
struct cmd      cmd_buf;
int             sv_socket;

struct cmd     *recv_command()
{
  char            p;
  char            c;
  char           *b = buf;
  int             cmdc;

  receive_line(b, 1280, sv_socket);
  cmd_buf.node = OUTSIDE;
  cmdc = 0;
  {
    p = '\x0', c = *b;
    for (; c; (p = c, c = *(++b))) {
      if (c == ' ' || c == '\n') {
        c = '\x0';
        *b = '\x0';
      } else if (p == '\x0' && cmdc < 5)
        (cmd_buf.v)[cmdc++] = b;
    }
  }
  cmd_buf.c = cmdc;
  return &cmd_buf;
}

void            send_out_command(struct cmd *pcmd)
{
  int             i;
  int             narg = (*pcmd).c;

  {
    i = 0;
    for (; i < narg - 1; i++) {
      send_string(((*pcmd).v)[i], sv_socket);
      send_char(' ', sv_socket);
    }
  }
  if (narg > 0) {
    send_string(((*pcmd).v)[narg - 1], sv_socket);
    send_char('\n', sv_socket);
  }
}

void            send_command(struct cmd *pcmd, void *body, int task_no)
{
  if ((*pcmd).node == OUTSIDE)
    enqueue_command(pcmd, body, task_no, send_queue);
  else
    enqueue_command(pcmd, body, task_no, inms_queue);
}

struct exec_queue_cmd_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  pthread_cond_t *p_cond;
  pthread_mutex_t *p_mut;
  void           *body;
  struct cmd     *pcmd;
  void           *arg;
};

void           *exec_queue_cmd(char *esp, void *arg)
{
  struct exec_queue_cmd_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  struct cmd     *pcmd;
  void           *body;
  pthread_mutex_t *p_mut;
  pthread_cond_t *p_cond;

LGOTO:;
  efp = (struct exec_queue_cmd_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct exec_queue_cmd_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  p_mut = &(*inms_queue).mut;
  p_cond = &(*inms_queue).cond;
  while (1) {
    pthread_mutex_lock(p_mut);
    while ((*inms_queue).in == (*inms_queue).out) {
      pthread_cond_wait(p_cond, p_mut);
    }
    pcmd = &(*(*inms_queue).out).cmd;
    body = (*(*inms_queue).out).body;
    pthread_mutex_unlock(p_mut);
    if (0 == strcmp(((*pcmd).v)[0], "task")) {
      //proto_error("INSIDE", pcmd);
      recv_task(pcmd, body);
    } else if (0 == strcmp(((*pcmd).v)[0], "rslt")) {
      //proto_error("INSIDE", pcmd);
      recv_rslt(pcmd, body);
    } else if (0 == strcmp(((*pcmd).v)[0], "treq")) {
      recv_treq(pcmd);
    } else if (0 == strcmp(((*pcmd).v)[0], "none")) {
      recv_none(pcmd);
    } else if (0 == strcmp(((*pcmd).v)[0], "rack")) {
      recv_rack(pcmd);
    } else {
      proto_error("wrong cmd", pcmd);
      exit(0);
    }
    pthread_mutex_lock(p_mut);
    (*inms_queue).out = (*(*inms_queue).out).next;
    pthread_mutex_unlock(p_mut);
  }
}

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
  fflush(stdout);
}

struct sender_loop_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  void           *body;
  pthread_cond_t *p_cond;
  pthread_mutex_t *p_mut;
  struct cmd     *pcmd;
  void           *arg;
};

void           *sender_loop(char *esp, void *arg)
{
  struct sender_loop_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  struct cmd     *pcmd;
  pthread_mutex_t *p_mut;
  pthread_cond_t *p_cond;
  void           *body;

LGOTO:;
  efp = (struct sender_loop_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct sender_loop_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  p_mut = &(*send_queue).mut;
  p_cond = &(*send_queue).cond;
  while (1) {
    pthread_mutex_lock(p_mut);
    while ((*send_queue).in == (*send_queue).out) {
      pthread_cond_wait(p_cond, p_mut);
    }
    pcmd = &(*(*send_queue).out).cmd;
    pthread_mutex_unlock(p_mut);
    send_out_command(pcmd);
    body = (*(*send_queue).out).body;
    if (body) {
      if (0 == strcmp(((*pcmd).v)[0], "task")) {
        (task_senders[(*(*send_queue).out).task_no]) (body);
        write_eol();
      } else if (0 == strcmp(((*pcmd).v)[0], "rslt")) {
        (rslt_senders[(*(*send_queue).out).task_no]) (body);
        write_eol();
      }
      (*(*send_queue).out).body = 0;
    }
    flush_send();
    pthread_mutex_lock(p_mut);
    (*send_queue).out = (*(*send_queue).out).next;
    pthread_mutex_unlock(p_mut);
    flush_send();
  }
}
struct thread_data threads[64];
unsigned int    num_thrs;

void            flush_treq_with_none(struct thread_data *thr)
{
  struct cmd      rcmd;
  struct task_home *hx;

  rcmd.c = 2;
  (rcmd.v)[0] = "none";
  while (hx = (*thr).treq_top) {
    rcmd.node = (*hx).req_from;
    (rcmd.v)[1] = (*hx).task_head;
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
  int             ifexp_result9;
  int             ifexp_result10;
  int             ifexp_result8;
  long            ifexp_result7;
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
  int             req_to;
  char           *treq_head;
  struct thread_data *thr;
};

void            recv_exec_send(char *esp, struct thread_data *thr, char *treq_head, int req_to)
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
  long            ifexp_result7;
  int             ifexp_result8;
  int             ifexp_result10;
  int             ifexp_result9;
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
  delay = 2 * 1000 * 1000;
  (efp->rcmd).c = 3;
  if (num_thrs > 1) {
    (efp->rcmd).node = req_to;
  } else {
    (efp->rcmd).node = OUTSIDE;
  }
  ((efp->rcmd).v)[0] = "treq";
  ((efp->rcmd).v)[1] = (*thr).id_str;
  ((efp->rcmd).v)[2] = treq_head;
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
      {
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
    efp->ifexp_result9 = ifexp_result9;
    efp->ifexp_result10 = ifexp_result10;
    efp->ifexp_result8 = ifexp_result8;
    efp->ifexp_result7 = ifexp_result7;
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
    ifexp_result9 = efp->ifexp_result9;
    ifexp_result10 = efp->ifexp_result10;
    ifexp_result8 = efp->ifexp_result8;
    ifexp_result7 = efp->ifexp_result7;
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
  pthread_mutex_lock(&(*thr).mut);
  (efp->rcmd).c = 2;
  (efp->rcmd).node = (*tx).rslt_to;
  ((efp->rcmd).v)[0] = "rslt";
  ((efp->rcmd).v)[1] = (*tx).rslt_head;
  send_command(&efp->rcmd, (*tx).body, (*tx).task_no);
  flush_treq_with_none(thr);
  pthread_mutex_lock(&(*thr).rack_mut);
  ((*thr).w_rack)++;
  pthread_mutex_unlock(&(*thr).rack_mut);
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
    while (recv_exec_send(new_esp, thr, "any", ANY), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
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

  if ((*pcmd).c < 5)
    proto_error("wrong-task", pcmd);
  task_no = atoi(((*pcmd).v)[4]);
  if ((*pcmd).node == OUTSIDE) {
    body = (task_receivers[task_no]) ();
    read_to_eol();
  }
  id = atoi(((*pcmd).v)[3]);
  if (!(id < num_thrs))
    proto_error("wrong task-head", pcmd);
  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  tx = (*thr).task_top;
  (*tx).rslt_to = (*pcmd).node;
  len = strlen(((*pcmd).v)[2]);
  if (len >= 256)
    proto_error("Too long rslt-head for task", pcmd);
  strncpy((*tx).rslt_head, ((*pcmd).v)[2], len + 1);
  (*tx).ndiv = atoi(((*pcmd).v)[1]);
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

  if ((*pcmd).c < 2)
    proto_error("Wrong none", pcmd);
  id = atoi(((*pcmd).v)[1]);
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
  char           *b;
  char            h_buf[256];

  if ((*pcmd).c < 2)
    proto_error("Wrong rslt", pcmd);
  b = ((*pcmd).v)[1];
  tid = atoi(b);
  if (!(tid < num_thrs))
    proto_error("wrong rslt-head", pcmd);
  b = strchr(b, ':');
  if (!b)
    proto_error("Wrong rslt-head", pcmd);
  sid = atoi(b + 1);
  thr = threads + tid;
  pthread_mutex_lock(&(*thr).mut);
  hx = (*thr).sub;
  while (hx && (*hx).id != sid) {
    hx = (*hx).next;
  }
  if (!hx)
    proto_error("Wrong rslt-head", pcmd);
  if ((*pcmd).node == OUTSIDE) {
    (rslt_receivers[(*hx).task_no]) ((*hx).body);
    read_to_eol();
  } else if ((*pcmd).node == INSIDE) {
    (*hx).body = body;
  } else {
    proto_error("Wrong cmd.node", pcmd);
  }
  rcmd.c = 2;
  rcmd.node = (*pcmd).node;
  (rcmd.v)[0] = "rack";
  strncpy(h_buf, (*hx).task_head, 256);
  (rcmd.v)[1] = h_buf;
  (*hx).stat = TASK_HOME_DONE;
  if (hx == (*thr).sub) {
    pthread_cond_signal(&(*thr).cond_r);
    pthread_cond_signal(&(*thr).cond);
  }
  pthread_mutex_unlock(&(*thr).mut);
  send_command(&rcmd, 0, 0);
}

int             try_treq(struct cmd *pcmd, unsigned int id)
{
  struct task_home *hx;
  struct thread_data *thr;
  size_t          len;
  int             avail = 0;

  thr = threads + id;
  pthread_mutex_lock(&(*thr).mut);
  pthread_mutex_lock(&(*thr).rack_mut);
  if ((*thr).task_top && ((*(*thr).task_top).stat == TASK_STARTED || (*(*thr).task_top).stat == TASK_INITIALIZED) && (*thr).w_rack == 0)
    avail = 1;
  pthread_mutex_unlock(&(*thr).rack_mut);
  if (avail) {
    hx = (*thr).treq_free;
    if (!hx)
      mem_error("Not enough task-home memory");
    (*thr).treq_free = (*hx).next;
    (*hx).next = (*thr).treq_top;
    (*hx).stat = TASK_HOME_ALLOCATED;
    len = strlen(((*pcmd).v)[1]);
    if (len >= 256)
      proto_error("Too long task-head for treq", pcmd);
    strncpy((*hx).task_head, ((*pcmd).v)[1], len + 1);
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

int rand(void);

void            recv_treq(struct cmd *pcmd)
{
  struct cmd      rcmd;
  unsigned int    id;

  if ((*pcmd).c < 3)
    proto_error("Wrong treq", pcmd);
  if (strcmp(((*pcmd).v)[2], "any") == 0) {
    int             myid;
    int  d;
    //int  r=rand()%num_thrs;
    myid = atoi(((*pcmd).v)[1]);
    {
      d = 0;
      for (; d < num_thrs; d++) {
        id = (1+d+myid)%num_thrs;
        if ((*pcmd).node != OUTSIDE && id == myid)
          continue;
        if (try_treq(pcmd, id))
          break;
      }
    }
    if (d != num_thrs)
      return;
  } else {
    id = atoi(((*pcmd).v)[2]);
    if (!(id < num_thrs))
      proto_error("Wrong task-head", pcmd);
    if (try_treq(pcmd, id))
      return;
  }
  if ((*pcmd).node == ANY)
    if (atoi(((*pcmd).v)[1]) == 0) {
      (*pcmd).node = OUTSIDE;
      send_command(pcmd, 0, 0);
      return;
    } else
      (*pcmd).node = INSIDE;
  rcmd.c = 2;
  rcmd.node = (*pcmd).node;
  (rcmd.v)[0] = "none";
  (rcmd.v)[1] = ((*pcmd).v)[1];
  send_command(&rcmd, 0, 0);
}

void            recv_rack(struct cmd *pcmd)
{
  struct task    *tx;
  struct thread_data *thr;
  unsigned int    id;
  size_t          len;

  if ((*pcmd).c < 2)
    proto_error("Wrong rack", pcmd);
  id = atoi(((*pcmd).v)[1]);
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
  tcmd.c = 5;
  tcmd.node = (*hx).req_from;
  (tcmd.v)[0] = "task";
  sprintf((*thr).ndiv_buf, "%d", ++(*thr).ndiv);
  sprintf((*thr).buf, "%s:%d", (*thr).id_str, (*hx).id);
  sprintf((*thr).tno_buf, "%d", task_no);
  (tcmd.v)[1] = (*thr).ndiv_buf;
  (tcmd.v)[2] = (*thr).buf;
  (tcmd.v)[3] = (*hx).task_head;
  (tcmd.v)[4] = (*thr).tno_buf;
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
    gettimeofday(&efp->now, 0);
    nsec = 1000 * (efp->now).tv_usec;
    nsec += 5 * 1000 * 1000;
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
  struct task_home *hx;
  struct task    *tx;
  struct thread_data *thr;
  struct cmd     *pcmd;
  void           *dummy;
  int             j;
  int             i;
  int             ifexp_result16;
  int             ifexp_result17;
  int             ifexp_result14;
  int             ifexp_result15;
  int             ifexp_result13;
  char          **argv;
  int             argc;
};

int             main(int argc, char **argv)
{
  struct main_frame *efp;
  char           *new_esp;
  char            estack[8192];
  char           *esp = estack;
  int             ifexp_result13;
  int             ifexp_result15;
  int             ifexp_result14;
  int             ifexp_result17;
  int             ifexp_result16;
  int             i;
  int             j;
  void           *dummy;
  struct cmd     *pcmd;
  struct thread_data *thr;
  struct task    *tx;
  struct task_home *hx;
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
  inms_queue = make_cmd_queue(512, 5, 256);
  new_esp = esp;
  while (systhr_create(new_esp, exec_queue_cmd, NULL), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
    char           *goto_fr;

    *((char **)esp) = 0;
    efp->pcmd = pcmd;
    efp->dummy = dummy;
    efp->j = j;
    efp->i = i;
    efp->ifexp_result16 = ifexp_result16;
    efp->ifexp_result17 = ifexp_result17;
    efp->ifexp_result14 = ifexp_result14;
    efp->ifexp_result15 = ifexp_result15;
    efp->ifexp_result13 = ifexp_result13;
    efp->argv = argv;
    efp->argc = argc;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto LGOTO;
    pcmd = efp->pcmd;
    dummy = efp->dummy;
    j = efp->j;
    i = efp->i;
    ifexp_result16 = efp->ifexp_result16;
    ifexp_result17 = efp->ifexp_result17;
    ifexp_result14 = efp->ifexp_result14;
    ifexp_result15 = efp->ifexp_result15;
    ifexp_result13 = efp->ifexp_result13;
    argv = efp->argv;
    argc = efp->argc;
    new_esp = esp + 1;
  }
  send_queue = make_cmd_queue(512, 5, 256);
  new_esp = esp;
  while (systhr_create(new_esp, sender_loop, NULL), __builtin_expect((efp->tmp_esp = *((char **)esp)) != 0, 0)) {
    char           *goto_fr;

    *((char **)esp) = 0;
    efp->pcmd = pcmd;
    efp->dummy = dummy;
    efp->j = j;
    efp->i = i;
    efp->ifexp_result16 = ifexp_result16;
    efp->ifexp_result17 = ifexp_result17;
    efp->ifexp_result14 = ifexp_result14;
    efp->ifexp_result15 = ifexp_result15;
    efp->ifexp_result13 = ifexp_result13;
    efp->argv = argv;
    efp->argc = argc;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto LGOTO;
    pcmd = efp->pcmd;
    dummy = efp->dummy;
    j = efp->j;
    i = efp->i;
    ifexp_result16 = efp->ifexp_result16;
    ifexp_result17 = efp->ifexp_result17;
    ifexp_result14 = efp->ifexp_result14;
    ifexp_result15 = efp->ifexp_result15;
    ifexp_result13 = efp->ifexp_result13;
    argv = efp->argv;
    argc = efp->argc;
    new_esp = esp + 1;
  }
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
      pthread_mutex_init(&(*thr).mut, 0);
      pthread_mutex_init(&(*thr).rack_mut, 0);
      pthread_cond_init(&(*thr).cond, 0);
      pthread_cond_init(&(*thr).cond_r, 0);
      sprintf((*thr).id_str, "%d", i);
      tx = (struct task *)malloc(sizeof(struct task) * 256);
      (*thr).task_top = 0;
      (*thr).task_free = tx;
      {
        j = 0;
        for (; j < 256 - 1; j++) {
          (tx[j]).prev = &tx[j + 1];
          (tx[j + 1]).next = &tx[j];
        }
      }
      (tx[0]).next = 0;
      (tx[256 - 1]).prev = 0;
      hx = (struct task_home *)malloc(sizeof(struct task_home) * 256);
      (*thr).treq_top = 0;
      (*thr).treq_free = hx;
      (*thr).sub = 0;
      {
        j = 0;
        for (; j < 256 - 1; j++) {
          (hx[j]).next = &hx[j + 1];
        }
      }
      (hx[256 - 1]).next = 0;
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
        efp->hx = hx;
        efp->tx = tx;
        efp->thr = thr;
        efp->pcmd = pcmd;
        efp->dummy = dummy;
        efp->j = j;
        efp->i = i;
        efp->ifexp_result16 = ifexp_result16;
        efp->ifexp_result17 = ifexp_result17;
        efp->ifexp_result14 = ifexp_result14;
        efp->ifexp_result15 = ifexp_result15;
        efp->ifexp_result13 = ifexp_result13;
        efp->argv = argv;
        efp->argc = argc;
        goto_fr = lw_call(efp->tmp_esp);
        if ((char *)goto_fr == (char *)efp)
          goto LGOTO;
        thr2 = efp->thr2;
        hx = efp->hx;
        tx = efp->tx;
        thr = efp->thr;
        pcmd = efp->pcmd;
        dummy = efp->dummy;
        j = efp->j;
        i = efp->i;
        ifexp_result16 = efp->ifexp_result16;
        ifexp_result17 = efp->ifexp_result17;
        ifexp_result14 = efp->ifexp_result14;
        ifexp_result15 = efp->ifexp_result15;
        ifexp_result13 = efp->ifexp_result13;
        argv = efp->argv;
        argc = efp->argc;
        new_esp = esp + 1;
      }
    }
  }
  pcmd = recv_command();
  {
    if ((*pcmd).c > 0) {
      if (0 != strcmp(((*pcmd).v)[0], "exit")) {
        ifexp_result15 = 1;
      } else {
        ifexp_result15 = 0;
      }
      ifexp_result14 = ifexp_result15;
    } else {
      ifexp_result14 = 0;
    }
    if (ifexp_result14) {
      while (1) {
        if ((*pcmd).c > 0) {
          if (0 != strcmp(((*pcmd).v)[0], "exit")) {
            ifexp_result17 = 1;
          } else {
            ifexp_result17 = 0;
          }
          ifexp_result16 = ifexp_result17;
        } else {
          ifexp_result16 = 0;
        }
        if (!ifexp_result16)
          goto loop_end;
        if (0 == strcmp(((*pcmd).v)[0], "task")) {
          recv_task(pcmd, dummy);
        } else if (0 == strcmp(((*pcmd).v)[0], "rslt")) {
          recv_rslt(pcmd, dummy);
        } else if (0 == strcmp(((*pcmd).v)[0], "treq")) {
          recv_treq(pcmd);
        } else if (0 == strcmp(((*pcmd).v)[0], "none")) {
          recv_none(pcmd);
        } else if (0 == strcmp(((*pcmd).v)[0], "rack")) {
          recv_rack(pcmd);
        } else {
          proto_error("wrong cmd", pcmd);
        }
        pcmd = recv_command();
      }
  loop_end:;
    }
  }
  exit(0);
}
