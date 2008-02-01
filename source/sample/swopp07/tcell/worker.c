#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>
enum node
{ OUTSIDE, INSIDE, ANY };

struct cmd
{
  int c;
  enum node node;
  char *v[4];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  struct cmd_list *next;
};

void recv_rslt (struct cmd, void *);

void recv_task (struct cmd, void *);

void recv_treq (struct cmd);

void recv_rack (struct cmd);

void recv_none (struct cmd);
struct task;
struct thread_data;

void do_task_body (struct thread_data *, void *);

void send_task_body (struct thread_data *, void *);

void *recv_task_body (struct thread_data *);

void send_rslt_body (struct thread_data *, void *);

void recv_rslt_body (struct thread_data *, void *);
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
  void *body;
  int ndiv;
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  enum node req_from;
  struct task_home *next;
  void *body;
  char task_head[256];
};

struct thread_data
{
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
  char id_str[32];
  char buf[1024];
};
extern int divisibility_flag;

void send_int (int n);

void recv_int (int *n);

int
systhr_create (void *(*start_func) (void *), void *arg)
{
  int status = 0;
  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init (&attr);
  status = pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
  if (status == 0)
    status = pthread_create (&tid, &attr, start_func, arg);
  else
    status = pthread_create (&tid, 0, start_func, arg);
  return status;
}

void
mem_error (char const *str)
{
  fputs (str, stderr);
  fputc ('\n', stderr);
}
struct cmd_list cmd_queue[512];
char cmd_v_buf[512][4][256];
struct cmd_list *cmd_in;
struct cmd_list *cmd_out;
char buf[1024];
int divisibility_flag = 0;

struct cmd
recv_command ()
{
  struct cmd r;
  char p;
  char c;
  char *b = buf;

  fgets (b, 1024, stdin);
  r.c = 0;
  r.node = OUTSIDE;
  {
    p = '\x0', c = *b;
    if (c)
      do
	{
	  if (c == ' ' || c == '\n')
	    {
	      c = '\x0';
	      *b = '\x0';
	    }
	  else if (p == '\x0' && r.c < 4)
	    (r.v)[(r.c)++] = b;
	}
      while ((p = c, c = *(++b)), c);
  }
  return r;
}

pthread_mutex_t send_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_q = PTHREAD_COND_INITIALIZER;

void
send_command (struct cmd cmd)
{
  int i;

  {
    i = 0;
    if (i < cmd.c - 1)
      do
	{
	  fputs ((cmd.v)[i], stdout);
	  fputc (' ', stdout);
	}
      while (i++, i < cmd.c - 1);
  }
  if (cmd.c > 0)
    {
      fputs ((cmd.v)[cmd.c - 1], stdout);
      fputc ('\n', stdout);
    }
}

void
proto_error (char const *str, struct cmd cmd)
{
  int i;

  fputs (str, stderr);
  fputc ('\n', stderr);
  {
    i = 0;
    if (i < cmd.c - 1)
      do
	{
	  fputs ((cmd.v)[i], stderr);
	  fputc ('\n', stderr);
	}
      while (i++, i < cmd.c - 1);
  }
  if (cmd.c > 0)
    {
      fputs ((cmd.v)[cmd.c - 1], stderr);
      fputc ('\n', stderr);
    }
}

void
enqueue_command (struct cmd cmd, void *body)
{
  int i;
  size_t len;
  struct cmd_list *q;

  q = cmd_in;
  if ((*cmd_in).next == cmd_out)
    {
      perror ("cmd-queue overflow\n");
      exit (0);
    }
  cmd_in = (*cmd_in).next;
  (*q).cmd.c = cmd.c;
  (*q).cmd.node = cmd.node;
  {
    i = 0;
    if (i < cmd.c)
      do
	{
	  len = strlen ((cmd.v)[i]);
	  if (len > 254)
	    proto_error ("too long cmd", cmd);
	  strncpy (((*q).cmd.v)[i], (cmd.v)[i], len + 1);
	}
      while (i++, i < cmd.c);
  }
  (*q).body = body;
}

void *
exec_queue_cmd (void *arg)
{
  struct cmd cmd;
  void *body;

  do
    {
      pthread_mutex_lock (&queue_mut);
      if (cmd_in == cmd_out)
	do
	  {
	    pthread_cond_wait (&cond_q, &queue_mut);
	  }
	while (cmd_in == cmd_out);
      cmd = (*cmd_out).cmd;
      body = (*cmd_out).body;
      pthread_mutex_unlock (&queue_mut);
      if (0 == strcmp ((cmd.v)[0], "task"))
	recv_task (cmd, body);
      else if (0 == strcmp ((cmd.v)[0], "rslt"))
	recv_rslt (cmd, body);
      else if (0 == strcmp ((cmd.v)[0], "treq"))
	recv_treq (cmd);
      else if (0 == strcmp ((cmd.v)[0], "none"))
	recv_none (cmd);
      else if (0 == strcmp ((cmd.v)[0], "rack"))
	recv_rack (cmd);
      else
	{
	  proto_error ("wrong cmd", cmd);
	  exit (0);
	}
      cmd_out = (*cmd_out).next;
    }
  while (1);
}

void
read_to_eol (void)
{
  int c;

  if (EOF != (c = getc (stdin)))
    do
      {
	if (c == '\n')
	  break;
      }
    while (EOF != (c = getc (stdin)));
}

void
write_eol (void)
{
  putc ('\n', stdout);
}

void
flush_send (void)
{
  fflush (stdout);
}

pthread_mutex_t snr_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_snr = PTHREAD_COND_INITIALIZER;
struct cmd_list snr_queue[32];
char snr_v_buf[32][2][256];
struct cmd_list *snr_in;
struct cmd_list *snr_out;

void
enqueue_snr (struct cmd cmd)
{
  int i;
  size_t len;
  struct cmd_list *q;

  q = snr_in;
  if ((*snr_in).next == snr_out)
    {
      perror ("snr-queue overflow\n");
      exit (0);
    }
  snr_in = (*snr_in).next;
  (*q).cmd.c = cmd.c;
  (*q).cmd.node = cmd.node;
  {
    i = 0;
    if (i < cmd.c)
      do
	{
	  len = strlen ((cmd.v)[i]);
	  if (len > 254)
	    proto_error ("too long cmd", cmd);
	  strncpy (((*q).cmd.v)[i], (cmd.v)[i], len + 1);
	}
      while (i++, i < cmd.c);
  }
}

void *
send_none_rack (void *arg)
{
  struct cmd cmd;

  do
    {
      pthread_mutex_lock (&snr_mut);
      if (snr_in == snr_out)
	do
	  {
	    pthread_cond_wait (&cond_snr, &snr_mut);
	  }
	while (snr_in == snr_out);
      cmd = (*snr_out).cmd;
      pthread_mutex_unlock (&snr_mut);
      pthread_mutex_lock (&send_mut);
      send_command (cmd);
      flush_send ();
      if (0 == strcmp ((cmd.v)[0], "none"))
	divisibility_flag = 1;
      pthread_mutex_unlock (&send_mut);
      snr_out = (*snr_out).next;
    }
  while (1);
}
struct thread_data threads[64];
unsigned int num_thrs;

void
flush_treq_with_none_1 (struct thread_data *thr)
{
  struct cmd rcmd;
  struct task_home *hx;

  if (hx = (*thr).treq_top)
    do
      {
	rcmd.c = 2;
	rcmd.node = (*hx).req_from;
	(rcmd.v)[0] = "none";
	(rcmd.v)[1] = (*hx).task_head;
	if (rcmd.node == INSIDE)
	  {
	    pthread_mutex_lock (&queue_mut);
	    enqueue_command (rcmd, NULL);
	    pthread_cond_signal (&cond_q);
	    pthread_mutex_unlock (&queue_mut);
	  }
	else if (rcmd.node == OUTSIDE)
	  {
	    send_command (rcmd);
	    divisibility_flag = 1;
	  }
	else
	  {
	    perror ("Invalid rcmd.node in flush-treq-with-none-1\n");
	    fprintf (stderr, "%d\n", rcmd.node);
	    exit (0);
	  }
	(*thr).treq_top = (*hx).next;
	(*hx).next = (*thr).treq_free;
	(*thr).treq_free = hx;
      }
    while (hx = (*thr).treq_top);
}

void
flush_treq_with_none (struct thread_data *thr)
{
  if ((*thr).treq_top)
    {
      pthread_mutex_lock (&send_mut);
      flush_treq_with_none_1 (thr);
      flush_send ();
      pthread_mutex_unlock (&send_mut);
    }
}

void
recv_exec_send (struct thread_data *thr, char *treq_head, int req_to)
{
  struct cmd rcmd;
  struct task *tx;
  long delay;
  int old_ndiv;

  if ((*thr).w_none > 0)
    do
      {
	pthread_cond_wait (&(*thr).cond, &(*thr).mut);
	if ((*thr).sub && (*(*thr).sub).stat == TASK_HOME_DONE)
	  return;
      }
    while ((*thr).w_none > 0);
  tx = (*thr).task_free;
  if (!tx)
    mem_error ("Not enough task memory");
  (*thr).task_top = tx;
  (*thr).task_free = (*tx).prev;
  delay = 2 * 1000 * 1000;
  do
    {
      flush_treq_with_none (thr);
      (*tx).stat = TASK_ALLOCATED;
      pthread_mutex_unlock (&(*thr).mut);
      rcmd.c = 3;
      if (num_thrs > 1)
	rcmd.node = req_to;
      else
	rcmd.node = OUTSIDE;
      (rcmd.v)[0] = "treq";
      (rcmd.v)[1] = (*thr).id_str;
      (rcmd.v)[2] = treq_head;
      if (rcmd.node != OUTSIDE)
	{
	  pthread_mutex_lock (&queue_mut);
	  enqueue_command (rcmd, NULL);
	  pthread_cond_signal (&cond_q);
	  pthread_mutex_unlock (&queue_mut);
	}
      else
	{
	  pthread_mutex_lock (&send_mut);
	  send_command (rcmd);
	  flush_send ();
	  divisibility_flag = 1;
	  pthread_mutex_unlock (&send_mut);
	}
      pthread_mutex_lock (&(*thr).mut);
      do
	{
	  if ((*tx).stat != TASK_INITIALIZED && (*thr).sub
	      && (*(*thr).sub).stat == TASK_HOME_DONE)
	    {
	      if ((*tx).stat == TASK_NONE)
		goto Lnone;
	      ((*thr).w_none)++;
	      goto Lnone;
	    }
	  if ((*tx).stat != TASK_ALLOCATED)
	    break;
	  pthread_cond_wait (&(*thr).cond, &(*thr).mut);
	}
      while (1);
      if ((*tx).stat == TASK_NONE)
	{
	  struct timespec t1;
	  struct timeval now;
	  long nsec;

	  gettimeofday (&now, 0);
	  nsec = now.tv_usec * 1000;
	  nsec += delay;
	  delay += delay;
	  if (delay > 40 * 1000 * 1000)
	    delay = 40 * 1000 * 1000;
	  t1.tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
	  t1.tv_sec = now.tv_sec + (nsec > 999999999 ? 1 : 0);
	  pthread_cond_timedwait (&(*thr).cond_r, &(*thr).mut, &t1);
	  if ((*thr).sub && (*(*thr).sub).stat == TASK_HOME_DONE)
	    goto Lnone;
	}
    }
  while ((*tx).stat != TASK_INITIALIZED);
  (*tx).stat = TASK_STARTED;
  old_ndiv = (*thr).ndiv;
  (*thr).ndiv = (*tx).ndiv;
  pthread_mutex_unlock (&(*thr).mut);
  do_task_body (thr, (*tx).body);
  pthread_mutex_lock (&(*thr).mut);
  rcmd.c = 2;
  rcmd.node = (*tx).rslt_to;
  (rcmd.v)[0] = "rslt";
  (rcmd.v)[1] = (*tx).rslt_head;
  if (rcmd.node == INSIDE)
    {
      pthread_mutex_lock (&queue_mut);
      enqueue_command (rcmd, (*tx).body);
      pthread_cond_signal (&cond_q);
      pthread_mutex_unlock (&queue_mut);
      pthread_mutex_lock (&send_mut);
    }
  else if (rcmd.node == OUTSIDE)
    {
      pthread_mutex_lock (&send_mut);
      pthread_mutex_unlock (&(*thr).mut);
      send_command (rcmd);
      send_rslt_body (thr, (*tx).body);
      write_eol ();
      pthread_mutex_lock (&(*thr).mut);
    }
  else
    {
      perror ("Invalid rcmd.node in recv-exec-send\n");
      fprintf (stderr, "%d\n", rcmd.node);
      exit (0);
    }
  flush_treq_with_none_1 (thr);
  flush_send ();
  pthread_mutex_unlock (&send_mut);
  pthread_mutex_lock (&(*thr).rack_mut);
  ((*thr).w_rack)++;
  pthread_mutex_unlock (&(*thr).rack_mut);
  (*thr).ndiv = old_ndiv;
Lnone:
  {
    (*thr).task_free = tx;
    (*thr).task_top = (*tx).next;
  }
}

void *
worker (void *arg)
{
  struct thread_data *thr = arg;

  pthread_mutex_lock (&(*thr).mut);
  do
    {
      recv_exec_send (thr, "any", ANY);
    }
  while (1);
  pthread_mutex_unlock (&(*thr).mut);
}

void
recv_task (struct cmd cmd, void *body)
{
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  size_t len;

  if (cmd.c < 4)
    proto_error ("wrong-task", cmd);
  id = atoi ((cmd.v)[3]);
  if (!(id < num_thrs))
    proto_error ("wrong task-head", cmd);
  thr = threads + id;
  pthread_mutex_lock (&(*thr).mut);
  tx = (*thr).task_top;
  (*tx).rslt_to = cmd.node;
  len = strlen ((cmd.v)[2]);
  if (len > 254)
    proto_error ("Too long rslt-head for task", cmd);
  strncpy ((*tx).rslt_head, (cmd.v)[2], len + 1);
  (*tx).ndiv = atoi ((cmd.v)[1]);
  if (cmd.node == INSIDE)
    (*tx).body = body;
  else if (cmd.node == OUTSIDE)
    {
      (*tx).body = recv_task_body (thr);
      read_to_eol ();
    }
  else
    {
      perror ("Invalid cmd.node in recv-task\n");
      fprintf (stderr, "%d\n", cmd.node);
      exit (0);
    }
  (*tx).stat = TASK_INITIALIZED;
  pthread_cond_broadcast (&(*thr).cond);
  pthread_mutex_unlock (&(*thr).mut);
}

void
recv_none (struct cmd cmd)
{
  struct thread_data *thr;
  unsigned int id;
  size_t len;

  if (cmd.c < 2)
    proto_error ("Wrong none", cmd);
  id = atoi ((cmd.v)[1]);
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", cmd);
  thr = threads + id;
  pthread_mutex_lock (&(*thr).mut);
  if ((*thr).w_none > 0)
    ((*thr).w_none)--;
  else
    (*(*thr).task_top).stat = TASK_NONE;
  pthread_cond_broadcast (&(*thr).cond);
  pthread_mutex_unlock (&(*thr).mut);
}

void
recv_rslt (struct cmd cmd, void *rbody)
{
  struct cmd rcmd;
  struct thread_data *thr;
  struct task_home *hx;
  unsigned int tid;
  unsigned int sid;
  char *b;
  char h_buf[256];

  if (cmd.c < 2)
    proto_error ("Wrong rslt", cmd);
  b = (cmd.v)[1];
  tid = atoi (b);
  b = strchr (b, ':');
  if (!b)
    proto_error ("Wrong rslt-head", cmd);
  sid = atoi (b + 1);
  thr = threads + tid;
  pthread_mutex_lock (&(*thr).mut);
  hx = (*thr).sub;
  if (hx && (*hx).id != sid)
    do
      {
	hx = (*hx).next;
      }
    while (hx && (*hx).id != sid);
  if (!hx)
    proto_error ("Wrong rslt-head", cmd);
  if (cmd.node == INSIDE)
    (*hx).body = rbody;
  else if (cmd.node == OUTSIDE)
    {
      recv_rslt_body (thr, (*hx).body);
      read_to_eol ();
    }
  else
    {
      perror ("Invalid cmd.node in recv-rslt\n");
      fprintf (stderr, "%d\n", cmd.node);
      exit (0);
    }
  rcmd.c = 2;
  rcmd.node = cmd.node;
  (rcmd.v)[0] = "rack";
  strncpy (h_buf, (*hx).task_head, 1 + strlen ((*hx).task_head));
  (rcmd.v)[1] = h_buf;
  (*hx).stat = TASK_HOME_DONE;
  if (hx == (*thr).sub)
    {
      pthread_cond_broadcast (&(*thr).cond_r);
      pthread_cond_broadcast (&(*thr).cond);
    }
  pthread_mutex_unlock (&(*thr).mut);
  if (cmd.node)
    {
      pthread_mutex_lock (&queue_mut);
      enqueue_command (rcmd, NULL);
      pthread_cond_signal (&cond_q);
      pthread_mutex_unlock (&queue_mut);
    }
  else
    {
      pthread_mutex_lock (&snr_mut);
      enqueue_snr (rcmd);
      pthread_cond_signal (&cond_snr);
      pthread_mutex_unlock (&snr_mut);
    }
}

int
try_treq (struct cmd cmd, unsigned int id)
{
  struct task_home *hx;
  struct thread_data *thr;
  size_t len;
  int avail = 0;

  thr = threads + id;
  pthread_mutex_lock (&(*thr).mut);
  pthread_mutex_lock (&(*thr).rack_mut);
  if ((*thr).task_top
      && ((*(*thr).task_top).stat == TASK_STARTED
	  || (*(*thr).task_top).stat == TASK_INITIALIZED) && (*thr).w_rack == 0)
    avail = 1;
  pthread_mutex_unlock (&(*thr).rack_mut);
  if (avail)
    {
      hx = (*thr).treq_free;
      if (!hx)
	mem_error ("Not enough task-home memory");
      (*thr).treq_free = (*hx).next;
      (*hx).next = (*thr).treq_top;
      (*hx).stat = TASK_HOME_ALLOCATED;
      len = strlen ((cmd.v)[1]);
      if (len > 254)
	proto_error ("Too long task-head for treq", cmd);
      strncpy ((*hx).task_head, (cmd.v)[1], len + 1);
      if (cmd.node != OUTSIDE)
	(*hx).req_from = INSIDE;
      else
	(*hx).req_from = OUTSIDE;
      (*thr).treq_top = hx;
      (*thr).req = hx;
    }
  pthread_mutex_unlock (&(*thr).mut);
  return avail;
}

void
recv_treq (struct cmd cmd)
{
  struct cmd rcmd;
  unsigned int id;

  if (cmd.c < 3)
    proto_error ("Wrong treq", cmd);
  if (strcmp ((cmd.v)[2], "any") == 0)
    {
      int myid;

      myid = atoi ((cmd.v)[1]);
      {
	id = 0;
	if (id < num_thrs)
	  do
	    {
	      if (cmd.node != OUTSIDE && id == myid)
		continue;
	      if (try_treq (cmd, id))
		break;
	    }
	  while (id++, id < num_thrs);
      }
      if (id != num_thrs)
	return;
    }
  else
    {
      id = atoi ((cmd.v)[2]);
      if (!(id < num_thrs))
	proto_error ("Wrong task-head", cmd);
      if (try_treq (cmd, id))
	return;
    }
  if (cmd.node == ANY)
    if (atoi ((cmd.v)[1]) == 0)
      {
	pthread_mutex_lock (&send_mut);
	send_command (cmd);
	flush_send ();
	divisibility_flag = 1;
	pthread_mutex_unlock (&send_mut);
	return;
      }
    else
      cmd.node = INSIDE;
  rcmd.c = 2;
  rcmd.node = cmd.node;
  (rcmd.v)[0] = "none";
  (rcmd.v)[1] = (cmd.v)[1];
  if (rcmd.node == INSIDE)
    {
      pthread_mutex_lock (&queue_mut);
      enqueue_command (rcmd, NULL);
      pthread_cond_signal (&cond_q);
      pthread_mutex_unlock (&queue_mut);
    }
  else if (rcmd.node == OUTSIDE)
    {
      pthread_mutex_lock (&snr_mut);
      enqueue_snr (rcmd);
      pthread_cond_signal (&cond_snr);
      pthread_mutex_unlock (&snr_mut);
    }
  else
    {
      perror ("Invalid rcmd.node in recv-treq\n");
      fprintf (stderr, "%d\n", rcmd.node);
      exit (0);
    }
}

void
recv_rack (struct cmd cmd)
{
  struct task *tx;
  struct thread_data *thr;
  unsigned int id;
  size_t len;

  if (cmd.c < 2)
    proto_error ("Wrong rack", cmd);
  id = atoi ((cmd.v)[1]);
  if (!(id < num_thrs))
    proto_error ("Wrong task-head", cmd);
  thr = threads + id;
  pthread_mutex_lock (&(*thr).rack_mut);
  ((*thr).w_rack)--;
  pthread_mutex_unlock (&(*thr).rack_mut);
}

void
send_divisible (void)
{
  struct cmd cmd;

  if (pthread_mutex_trylock (&send_mut))
    return;
  divisibility_flag = 0;
  cmd.c = 1;
  (cmd.v)[0] = "dvbl";
  send_command (cmd);
  flush_send ();
  pthread_mutex_unlock (&send_mut);
}

void
handle_req (int (*_bk) (void), struct thread_data *_thr)
{
  pthread_mutex_lock (&(*_thr).mut);
  if ((*_thr).req)
    {
      _bk ();
      (*_thr).req = (*_thr).treq_top;
    }
  pthread_mutex_unlock (&(*_thr).mut);
}

void
make_and_send_task (struct thread_data *thr, void *body)
{
  struct cmd tcmd;
  struct task_home *hx = (*thr).treq_top;

  (*thr).treq_top = (*hx).next;
  (*hx).next = (*thr).sub;
  (*thr).sub = hx;
  (*hx).body = body;
  (*hx).id = (*hx).next ? (*(*hx).next).id + 1 : 0;
  (*hx).stat = TASK_HOME_INITIALIZED;
  tcmd.c = 4;
  tcmd.node = (*hx).req_from;
  (tcmd.v)[0] = "task";
  sprintf ((*thr).ndiv_buf, "%d", ++(*thr).ndiv);
  sprintf ((*thr).buf, "%s:%d", (*thr).id_str, (*hx).id);
  (tcmd.v)[1] = (*thr).ndiv_buf;
  (tcmd.v)[2] = (*thr).buf;
  (tcmd.v)[3] = (*hx).task_head;
  if (tcmd.node == INSIDE)
    {
      pthread_mutex_lock (&queue_mut);
      enqueue_command (tcmd, body);
      pthread_cond_signal (&cond_q);
      pthread_mutex_unlock (&queue_mut);
    }
  else if (tcmd.node == OUTSIDE)
    {
      pthread_mutex_lock (&send_mut);
      pthread_mutex_unlock (&(*thr).mut);
      send_command (tcmd);
      send_task_body (thr, body);
      write_eol ();
      flush_send ();
      pthread_mutex_unlock (&send_mut);
      pthread_mutex_lock (&(*thr).mut);
    }
  else
    {
      perror ("Invalid tcmd.node in make-and-send-task\n");
      fprintf (stderr, "%d\n", tcmd.node);
      exit (0);
    }
}

void *
wait_rslt (struct thread_data *thr)
{
  void *body;
  struct timespec t1;
  struct timeval now;
  long nsec;
  struct task_home *sub;

  pthread_mutex_lock (&(*thr).mut);
  sub = (*thr).sub;
  if ((*sub).stat != TASK_HOME_DONE)
    do
      {
	gettimeofday (&now, 0);
	nsec = 1000 * now.tv_usec;
	nsec += 5 * 1000 * 1000;
	t1.tv_nsec = nsec > 999999999 ? nsec - 999999999 : nsec;
	t1.tv_sec = now.tv_sec + (nsec > 999999999 ? 1 : 0);
	pthread_cond_timedwait (&(*thr).cond_r, &(*thr).mut, &t1);
	if ((*sub).stat == TASK_HOME_DONE)
	  break;
	recv_exec_send (thr, (*sub).task_head, (*sub).req_from);
      }
    while ((*sub).stat != TASK_HOME_DONE);
  body = (*sub).body;
  (*thr).sub = (*sub).next;
  (*sub).next = (*thr).treq_free;
  (*thr).treq_free = sub;
  pthread_mutex_unlock (&(*thr).mut);
  return body;
}

int
main (int argc, char **argv)
{
  int i;
  int j;
  void *dummy;
  struct cmd cmd;

  {
    i = 0;
    if (i < 512)
      do
	{
	  (cmd_queue[i]).next = &cmd_queue[(i + 1) % 512];
	  {
	    j = 0;
	    if (j < 4)
	      do
		{
		  ((cmd_queue[i]).cmd.v)[j] = cmd_v_buf[i][j];
		}
	      while (j++, j < 4);
	  }
	}
      while (i++, i < 512);
  }
  cmd_in = &cmd_queue[0];
  cmd_out = &cmd_queue[0];
  systhr_create (exec_queue_cmd, NULL);
  {
    i = 0;
    if (i < 32)
      do
	{
	  (snr_queue[i]).next = &snr_queue[(i + 1) % 32];
	  {
	    j = 0;
	    if (j < 2)
	      do
		{
		  ((snr_queue[i]).cmd.v)[j] = snr_v_buf[i][j];
		}
	      while (j++, j < 2);
	  }
	}
      while (i++, i < 32);
  }
  snr_in = &snr_queue[0];
  snr_out = &snr_queue[0];
  systhr_create (send_none_rack, NULL);
  num_thrs = argc > 1 ? atoi (argv[1]) : 1;
  {
    i = 0;
    if (i < num_thrs)
      do
	{
	  {
	    struct thread_data *thr = threads + i;
	    struct task *tx;
	    struct task_home *hx;

	    (*thr).id = i;
	    (*thr).w_rack = 0;
	    (*thr).w_none = 0;
	    (*thr).ndiv = 0;
	    pthread_mutex_init (&(*thr).mut, 0);
	    pthread_mutex_init (&(*thr).rack_mut, 0);
	    pthread_cond_init (&(*thr).cond, 0);
	    pthread_cond_init (&(*thr).cond_r, 0);
	    sprintf ((*thr).id_str, "%d", i);
	    tx = (struct task *) malloc (sizeof (struct task) * 512);
	    (*thr).task_top = 0;
	    (*thr).task_free = tx;
	    {
	      j = 0;
	      if (j < 511)
		do
		  {
		    (tx[j]).prev = &tx[j + 1];
		    (tx[j + 1]).next = &tx[j];
		  }
		while (j++, j < 511);
	    }
	    (tx[0]).next = 0;
	    (tx[511]).prev = 0;
	    hx = (struct task_home *) malloc (sizeof (struct task_home) * 512);
	    (*thr).treq_top = 0;
	    (*thr).treq_free = hx;
	    (*thr).sub = 0;
	    {
	      j = 0;
	      if (j < 511)
		do
		  {
		    (hx[j]).next = &hx[j + 1];
		  }
		while (j++, j < 511);
	    }
	    (hx[511]).next = 0;
	    systhr_create (worker, thr);
	  }
	}
      while (i++, i < num_thrs);
  }
  if (cmd = recv_command (), cmd.c > 0 && 0 != strcmp ((cmd.v)[0], "exit"))
    do
      {
	if (0 == strcmp ((cmd.v)[0], "task"))
	  recv_task (cmd, dummy);
	else if (0 == strcmp ((cmd.v)[0], "rslt"))
	  recv_rslt (cmd, dummy);
	else if (0 == strcmp ((cmd.v)[0], "treq"))
	  recv_treq (cmd);
	else if (0 == strcmp ((cmd.v)[0], "none"))
	  recv_none (cmd);
	else if (0 == strcmp ((cmd.v)[0], "rack"))
	  recv_rack (cmd);
	else
	  proto_error ("wrong cmd", cmd);
      }
    while (cmd = recv_command (), cmd.c > 0
	   && 0 != strcmp ((cmd.v)[0], "exit"));
  exit (0);
}
