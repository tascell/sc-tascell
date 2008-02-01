#include<stdio.h>
#include<stdlib.h>

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
void            (*task_doers[256]) (struct thread_data *, void *);
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

void           *wait_rslt(struct thread_data *thr);

int             serialize_cmdname(char *buf, enum command w);

int             deserialize_cmdname(enum command *buf, char *str);

int             serialize_arg(char *buf, enum node *arg);

enum node       deserialize_node(char *str);

int             deserialize_arg(enum node *buf, char *str);

int             serialize_cmd(char *buf, struct cmd *pcmd);

int             deserialize_cmd(struct cmd *pcmd, char *str);

int             copy_address(enum node *dst, enum node *src);
char           *cmd_strings[] = {"task", "rslt", "treq", "none", "rack", "exit", 0};

char           *skip_whitespace(char *str)
{
  char            ch;

  ch = *str;
  for (; ch == ' ' || ch == '\n'; ch = *(++str)) {
  }
  return str;
}

int             serialize_cmdname(char *buf, enum command w)
{
  char           *p = buf;

  if (w >= 0 && w < WRNG) {
    p = stpcpy(p, cmd_strings[w]);
    return p - buf;
  } else {
    *p = '\x0';
    return 0;
  }
}

int             deserialize_cmdname(enum command *buf, char *str)
{
  int             i;
  char           *p;
  char           *cmdstr;

  p = str;
  for (i = 0; cmdstr = cmd_strings[i]; i++) {
    if (0 == strncmp(p, cmdstr, 4)) {
      *buf = i;
      p += 4;
      p = skip_whitespace(p);
      return p - str;
      return i;
    }
  }
  *buf = WRNG;
  return 0;
}

int             serialize_arg(char *buf, enum node *arg)
{
  char           *p = buf;
  enum node       node;
  int             i;

  for (i = 0; TERM != (node = arg[i]); i++) {
    if (ANY == node) {
      p = stpcpy(p, "any");
    } else if (PARENT == node) {
      *p++ = 'p';
    } else {
      p += sprintf(p, "%d", node);
    }
    *p++ = ':';
  }
  *(--p) = '\x0';
  return p - buf;
}

enum node       deserialize_node(char *str)
{
  if ('p' == str[0])
    return PARENT;
  else if (0 == strncmp(str, "any", 3))
    return ANY;
  else
    return atoi(str);
}

int             deserialize_arg(enum node *buf, char *str)
{
  char           *p0;
  char           *p1;
  int             ch;
  enum node      *pnode;

  p0 = str;
  p1 = str;
  pnode = buf;
  for (; 1; p1++) {
    ch = *p1;
    if (ch == ':' || ch == ' ' || ch == '\n' || ch == '\x0') {
      *p1 = '\x0';
      *pnode++ = deserialize_node(p0);
      *p1 = ch;
      if (ch != ':')
        break;
      p0 = p1;
    }
  }
  *pnode = TERM;
  p1 = skip_whitespace(p1);
  return p1 - str;
}

int             serialize_cmd(char *buf, struct cmd *pcmd)
{
  char           *p;
  int             ret;
  int             i;

  p = buf;
  if (!(ret = serialize_cmdname(p, pcmd->w))) {
    fprintf(stderr, "Serialize-cmd failed.\n");
    exit(1);
  }
  p += ret;
  *p++ = ' ';
  for (i = 0; i < pcmd->c; i++) {
    p += serialize_arg(p, (pcmd->v)[i]);
    *p++ = ' ';
  }
  *(--p) = '\x0';
  return p - buf;
}

int             deserialize_cmd(struct cmd *pcmd, char *str)
{
  char           *p;
  int             i;

  p = str;
  p += deserialize_cmdname(&pcmd->w, p);
  if (pcmd->w == WRNG)
    return p - str;
  for (i = 0; *p && i < 4; i++) {
    p += deserialize_arg((pcmd->v)[i], p);
  }
  pcmd->c = i;
  return p - str;
}

int             copy_address(enum node *dst, enum node *src)
{
  int             i;

  for (i = 0; TERM != src[i]; i++) {
    dst[i] = src[i];
  }
  dst[i] = TERM;
  return i;
}
