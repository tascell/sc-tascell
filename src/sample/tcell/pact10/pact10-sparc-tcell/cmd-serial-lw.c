#include<stdio.h>
typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);

struct func_arg
{
  void *(*func) (char *, void *);
  void *arg;
};

void *thread_origin (void *farg);

#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
void (*task_doers[256]) (char *, struct thread_data *, void *);
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

void *wait_rslt (char *esp, struct thread_data *thr);

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

char *skip_whitespace (char *str)
{
  char ch;

  ch = *str;
  for (; ch == ' ' || ch == '\n'; ch = *(++str))
    {
    }
  return str;
}

char *skip_notwhitespace (char *str)
{
  char ch;

  ch = *str;
  for (; !(ch == ' ' || ch == '\n'); ch = *(++str))
    {
    }
  return str;
}

void print_cmd (struct cmd *pcmd)
{
  int i;
  int j;

  fprintf (stderr, "cmd.w: %d\n", (*pcmd).w);
  fprintf (stderr, "cmd.c: %d\n", (*pcmd).c);
  fprintf (stderr, "cmd.node: %d\n", (*pcmd).node);
  {
    i = 0;
    for (; i < 4; i++)
      {
        fprintf (stderr, "cmd.v[%d]:", i);
        {
          j = 0;
          for (; j < 16; j++)
            {
              fprintf (stderr, "%3d ", ((*pcmd).v)[i][j]);
            }
        }
        fprintf (stderr, "\n");
      }
  }
}
char *cmd_strings[] =
  { "task", "rslt", "treq", "none", "back", "rack", "dreq", "data", "stat",
"verb", "exit", 0 };

int serialize_cmdname (char *buf, enum command w)
{
  char *p;

  p = buf;
  if (w >= 0 && w < WRNG)
    {
      strcpy (p, cmd_strings[w]);
      p += strlen (cmd_strings[w]);
      return p - buf;
    }
  else
    {
      *p = '\x0';
      return 0;
    }
}

int deserialize_cmdname (enum command *buf, char *str)
{
  int i;
  char *p;
  char *cmdstr;

  p = str;
  {
    switch (*p++)
      {
      case 't':
        switch (*p++)
          {
          case 'a':
            *buf = TASK;
            break;
          case 'r':
            *buf = TREQ;
            break;
          default:
            *buf = WRNG;
            return 0;
          }
        break;
      case 'd':
        switch (*p++)
          {
          case 'r':
            *buf = DREQ;
            break;
          case 'a':
            *buf = DATA;
            break;
          default:
            *buf = WRNG;
            return 0;
          }
        break;
      case 'r':
        switch (*p++)
          {
          case 's':
            *buf = RSLT;
            break;
          case 'a':
            *buf = RACK;
            break;
          default:
            *buf = WRNG;
            return 0;
          }
        break;
      case 'n':
        *buf = NONE;
        break;
      case 'b':
        *buf = BACK;
        break;
      case 's':
        *buf = STAT;
        break;
      case 'v':
        *buf = VERB;
        break;
      case 'e':
        *buf = EXIT;
        break;
      default:
        *buf = WRNG;
        return 0;
      }
    p = skip_notwhitespace (p);
    p = skip_whitespace (p);
    return p - str;
  }
}

int serialize_arg (char *buf, enum node *arg)
{
  char *p;
  enum node node2;
  int i;

  p = buf;
  {
    i = 0;
    for (; TERM != (node2 = arg[i]); i++)
      {
        if (ANY == node2)
          {
            strcpy (p, "any");
            p += 3;
          }
        else if (PARENT == node2)
          {
            *p++ = 'p';
          }
        else if (FORWARD == node2)
          {
            *p++ = 'f';
          }
        else
          {
            p += sprintf (p, "%d", node2);
          }
        *p++ = ':';
      }
  }
  if (i == 0)
    p++;
  else;
  *(--p) = '\x0';
  return p - buf;
}

enum node deserialize_node (char *str)
{
  if ('p' == str[0])
    return PARENT;
  else if ('f' == str[0])
    return FORWARD;
  else if (0 == strncmp (str, "any", 3))
    return ANY;
  else
    return atoi (str);
}

int deserialize_arg (enum node *buf, char *str)
{
  char *p0;
  char *p1;
  int ch;
  enum node *pnode;

  p0 = str;
  p1 = str;
  pnode = buf;
  for (; 1; p1++)
    {
      ch = *p1;
      if (ch == ':' || ch == ' ' || ch == '\n' || ch == '\x0')
        {
          *p1 = '\x0';
          *pnode++ = deserialize_node (p0);
          *p1 = ch;
          if (ch != ':')
            break;
          else;
          p0 = 1 + p1;
        }
      else;
    }
  *pnode = TERM;
  p1 = skip_whitespace (p1);
  return p1 - str;
}

int serialize_cmd (char *buf, struct cmd *pcmd)
{
  char *p;
  int ret;
  int i;

  p = buf;
  if (!(ret = serialize_cmdname (p, (*pcmd).w)))
    {
      fprintf (stderr, "Serialize-cmd failed.\n");
      print_cmd (pcmd);
      exit (1);
    }
  else;
  p += ret;
  *p++ = ' ';
  {
    i = 0;
    for (; i < (*pcmd).c; i++)
      {
        p += serialize_arg (p, ((*pcmd).v)[i]);
        *p++ = ' ';
      }
  }
  *(--p) = '\x0';
  return p - buf;
}

int deserialize_cmd (struct cmd *pcmd, char *str)
{
  char *p;
  int i;

  p = str;
  p += deserialize_cmdname (&(*pcmd).w, p);
  if ((*pcmd).w == WRNG)
    return p - str;
  else;
  {
    i = 0;
    for (; *p && i < 4; i++)
      {
        p += deserialize_arg (((*pcmd).v)[i], p);
      }
  }
  (*pcmd).c = i;
  return p - str;
}

int copy_address (enum node *dst, enum node *src)
{
  int i;

  {
    i = 0;
    for (; TERM != src[i]; i++)
      {
        dst[i] = src[i];
      }
  }
  dst[i] = TERM;
  return i;
}

int address_equal (enum node *adr1, enum node *adr2)
{
  int i;

  {
    i = 0;
    for (; i < 4; i++)
      {
        if (adr1[i] != adr2[i])
          return 0;
        else;
        if (TERM == adr1[i])
          return 1;
        else;
      }
  }
  return 1;
}
