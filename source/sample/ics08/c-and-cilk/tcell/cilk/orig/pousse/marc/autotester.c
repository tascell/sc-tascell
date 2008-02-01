/* file: "referee.c" */

/*
 * Compile with: gcc -o referee referee.c
 */

#define HP

#define MAX_N 20
#define TIME_LIMIT_IN_MSECS 32000
#define MAX_REPLY 10000
#if 1
#define uid(n)(3092+n)
#else
#define uid(n)(3093)
#endif
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>

/*---------------------------------------------------------------------------*/

/* Errors */

void fatal_error (msg)
char *msg;
{
  fputs ("referee: ", stderr);
  fputs (msg, stderr);
  exit (1);
}

/*---------------------------------------------------------------------------*/

/* Strings */

typedef struct
  {
    int length;
    int buf_length;
    char *buf;
  } string;

void make_empty_string (s)
string *s;
{
  s->length = 0;
  s->buf_length = 0;
  s->buf = NULL;
}

void string_expand_buf (s, n)
string *s;
int n;
{
  if (n > s->buf_length)
    {
      int i;
      char *new_buf = (char*)malloc (n+1);
      if (new_buf == NULL)
        fatal_error ("heap overflow error\n");
      for (i=s->length-1; i>=0; i--)
        new_buf[i] = s->buf[i];
      s->buf_length = n;
      if (s->buf != NULL)
        free (s->buf);
      s->buf = new_buf;
    }
}

void string_add_char (s, c)
string *s;
char c;
{
  if (s->length == s->buf_length)
    string_expand_buf (s, s->buf_length*3/2+1);
  s->buf[s->length++] = c;
}

void string_add_c_string (s, cs)
string *s;
char *cs;
{
  while (*cs != '\0')
    string_add_char (s, *cs++);
}

/*---------------------------------------------------------------------------*/

/* Time */

jmp_buf timer_env;

void disable_timer ()
{
  sigset_t mask;

  sigemptyset (&mask);
  sigaddset (&mask, SIGALRM);
  sigprocmask (SIG_BLOCK, &mask, 0);
}

void enable_timer ()
{
  sigset_t mask;

  sigemptyset (&mask);
  sigaddset (&mask, SIGALRM);
  sigprocmask (SIG_UNBLOCK, &mask, 0);
}

void timer_interrupt_handler (sig)
int sig;
{
  longjmp (timer_env, 1);
}

void set_timer (msecs)
unsigned long msecs;
{
  struct sigaction act;
  struct itimerval tv;
  int secs = msecs/1000;
  int usecs = (msecs%1000)*1000;

  tv.it_interval.tv_sec  = 0;
  tv.it_interval.tv_usec = 0;
  tv.it_value.tv_sec     = secs;
  tv.it_value.tv_usec    = usecs;

  setitimer (ITIMER_REAL, &tv, 0); /* msecs==0 will turn timer off */

  if (msecs == 0)
    {
      act.sa_handler = SIG_DFL;
      act.sa_flags = 0;
      sigemptyset (&act.sa_mask);
      sigaction (SIGALRM, &act, 0);
      disable_timer ();
    }
  else
    {
      act.sa_handler = timer_interrupt_handler;
      act.sa_flags = 0;
#ifdef SA_INTERRUPT
      act.sa_flags |= SA_INTERRUPT;
#endif
      sigemptyset (&act.sa_mask);
      sigaction (SIGALRM, &act, 0);
      enable_timer ();
    }
}

unsigned long wall_time_in_msecs ()
{
  struct timeval tv;

  if (gettimeofday (&tv, NULL) == 0)
    return (unsigned long)tv.tv_sec*1000 + tv.tv_usec/1000;
  else
    return 0;
}

time_t wall_time_in_secs ()
{
  struct timeval tv;

  if (gettimeofday (&tv, NULL) == 0)
    return tv.tv_sec;
  else
    return 0;
}

/*---------------------------------------------------------------------------*/

/* Subprocesses */

typedef struct
  {
    pid_t pid;   /* pid of the subprocess */
    int in, out; /* file descriptors for stdin/stdout of the subprocess */
  } subprocess;

#define RD 0
#define WR 1

void subprocess_open (user_id, prog, home, path, subproc)
int user_id;
char *prog;
char *home;
char *path;
subprocess *subproc;
{
  int parent_to_child[2];
  int child_to_parent[2];

  if (pipe (parent_to_child) >= 0)
    {
      if (pipe (child_to_parent) >= 0)
        {
          subproc->pid = fork ();
          if (subproc->pid >= 0)
            {
              if (subproc->pid == 0)
                {
                  /* child */

                  close (parent_to_child[RD]);
                  close (child_to_parent[WR]);

                  if (child_to_parent[RD] != 0) /* setup stdin */
                    {
                      if (dup2 (child_to_parent[RD], 0) != 0)
                        exit (1);
                      close (child_to_parent[RD]);
                    }

                  if (parent_to_child[WR] != 1) /* setup stdout */
                    {
                      if (dup2 (parent_to_child[WR], 1) != 1)
                        exit (1);
                      close (parent_to_child[WR]);
                    }

                  if (chdir (home) != 0)
                    {
                      fprintf (stderr, "chdir failed on %s\n", home);
                      exit (1);
                    }

                  putenv (path);

                  setuid (user_id);

                  execl(prog, prog, (char*)0);

                  fprintf (stderr, "execl failed on %s (errno=%d)\n", prog, errno);

                  exit (1);
                }
              else
                {
                  /* parent */

                  close (parent_to_child[WR]);
                  close (child_to_parent[RD]);

                  subproc->in = child_to_parent[WR];
                  subproc->out = parent_to_child[RD];

                  return;
                }
            }

          close (child_to_parent[RD]);
          close (child_to_parent[WR]);
        }

      close (parent_to_child[RD]);
      close (parent_to_child[WR]);
    }

  fatal_error ("can't start subprocess\n");
}

int killuid (id, sig)
int id;
int sig;
{
  pid_t pid, parent, child;
  int status;

  disable_timer ();

  parent = getpid ();

  child = fork ();

  if (child < 0)
    {
      fprintf (stderr, "fork failed\n");
      exit (1);
    }

  if (child == 0)
    {
      int alive = 0, redo = 1;

      child = getpid ();

      if (setuid (id) != 0)
        {
          fprintf (stderr, "setuid failed\n");
          exit (0);
        }

      while (redo)
        {
          /*
           * Give me a better interface to processes and I will do a
           * cleaner job...  Note that this code is not totally
           * correct because there is a race condition between killing
           * a process and that process creating new processes (which
           * won't get killed).
           */

          redo = 0;
          for (pid=32767; pid>1; pid--) /* scan all possible process ids */
            if (pid != child && pid != parent)
              {
                if (kill (pid, sig) == 0)
                  if (sig == 0)
                    exit (1);
                  else
                    {
                      alive = 1;
                      redo = 1;
                    }
              }
        }

      exit (alive);
    }

  enable_timer ();

  waitpid (child, &status, 0);

  if (WIFEXITED(status))
    return WEXITSTATUS(status);
  else
    return 0;
}

/*---------------------------------------------------------------------------*/

/* Sending mail */

void send_mail (address, message)
char *address;
char *message;
{
  FILE *f;
  char cmd[1000];

  sprintf (cmd, "mail %s", address);

  f = popen (cmd, "w");
  if (f != NULL)
    {
      fputs (message, f);
      fclose (f);
    }
}

/*---------------------------------------------------------------------------*/

/* Game board */

enum { x_marker, o_marker, empty };
enum { top, bottom, left, right };

typedef struct
  {
    int side;
    int index;
  } move_t;

typedef struct
  {
    int n;
    unsigned char *squares;
  } board_t;

typedef struct board_list_cell
  {
    board_t board;
    struct board_list_cell *next;
  } board_list_cell;

typedef struct board_list_cell *board_list;

int equal_boards (b1, b2)
board_t b1;
board_t b2;
{
  if (b1.n != b2.n)
    return 0;
  else
    {
      int i;
      for (i=b1.n*b1.n-1; i>=0; i--)
        if (b1.squares[i] != b2.squares[i])
          return 0;
      return 1;
    }
}

void make_empty_board (b, n)
board_t *b;
int n;
{
  int i;
  unsigned char *sq;

  sq = (unsigned char*)malloc (n*n);
  if (sq == NULL)
    fatal_error ("heap overflow error\n");

  for (i=n*n-1; i>=0; i--)
    sq[i] = empty;

  b->n = n;
  b->squares = sq;
}

void copy_board (b1, b2)
board_t *b1;
board_t *b2;
{
  int i;

  make_empty_board (b2, b1->n);

  for (i=b1->n*b1->n-1; i>=0; i--)
    b2->squares[i] = b1->squares[i];
}

board_list cons_board (b, bl)
board_t b;
board_list bl;
{
  struct board_list_cell *blc;

  blc = (struct board_list_cell*)malloc (sizeof (struct board_list_cell));
  if (blc == NULL)
    fatal_error ("heap overflow error\n");

  blc->board = b;
  blc->next = bl;

  return blc;
}
  
void init_board_list (bl, n)
board_list *bl;
int n;
{
  board_t empty_board;

  make_empty_board (&empty_board, n);

  *bl = cons_board (empty_board, NULL);
}

int board_exists (b, bl)
board_t b;
board_list bl;
{
  int after_me = 0;
  while (bl != NULL)
    {
      if (after_me && equal_boards (b, bl->board))
        return 1;
      after_me = !after_me;
      bl = bl->next;
    }
  return 0;
}

#define indx2d(n,i,j)((i)*(n)+j)

int nb_straights (b, marker)
board_t b;
int marker;
{
  int i, j, n;
  unsigned char *sq;
  int nb_s;

  n = b.n;
  sq = b.squares;

  nb_s = 0;

  for (i=n-1; i>=0; i--)
    {
      for (j=n-1; j>=0; j--)
        if (sq[indx2d(n,i,j)] != marker)
          break;
      if (j<0)
        nb_s++;
    }

  for (j=n-1; j>=0; j--)
    {
      for (i=n-1; i>=0; i--)
        if (sq[indx2d(n,i,j)] != marker)
          break;
      if (i<0)
        nb_s++;
    }

  return nb_s;
}

char move_buf[MAX_REPLY+1];

char *format_move (m)
move_t m;
{
  char s;
  switch (m.side)
    {
      case top:    s = 'T'; break;
      case bottom: s = 'B'; break;
      case left:   s = 'L'; break;
      case right:  s = 'R'; break;
    }
  sprintf (move_buf, "%c%d", s, m.index+1);
  return move_buf;
}

void remove_linebreaks (moves)
string moves;
{
  int i;
  for (i=0; i<moves.length; i++)
    if (moves.buf[i] == '\n')
      moves.buf[i] = ' ';
}

void print_board (f, b)
FILE *f;
board_t b;
{
  int i, j, n;
  unsigned char *sq;

  n = b.n;
  sq = b.squares;

  for (i=0; i<n; i++)
    {
      fputs ("    ", f);
      for (j=0; j<n; j++)
        {
          switch (sq[indx2d(n,i,j)])
            {
            case x_marker: fputs ("X", f); break;
            case o_marker: fputs ("O", f); break;
            case empty:    fputs (".", f); break;
            }
        }
      fputs ("\n", f);
    }
}

void make_move (b, m, marker)
board_t b;
move_t m;
int marker;
{
  int i, j, di, dj, n;
  unsigned char *sq;

  n = b.n;
  sq = b.squares;

  switch (m.side)
    {
      case top:    i = 0;   j = m.index; di = 1;  dj = 0;  break;
      case bottom: i = n-1; j = m.index; di = -1; dj = 0;  break;
      case left:   i = m.index; j = 0;   di = 0;  dj = 1;  break;
      case right:  i = m.index; j = n-1; di = 0;  dj = -1; break;
    }

  do
    {
      unsigned char *p = &sq[indx2d(n,i,j)];
      int old = *p;
      *p = marker;
      marker = old;
      i += di;
      j += dj;
    } while ((marker!=empty) && (i>=0)&&(i<n)&&(j>=0)&&(j<n));
}

/*---------------------------------------------------------------------------*/

int parse_reply (n, reply, move_ptr)
int n;
char *reply;
move_t *move_ptr;
{
  int i;

  while (*reply == ' ')
    reply++;

  if (*reply == 'T' || *reply == 't')
    {
      move_ptr->side = top;
      reply++;
    }
  else if (*reply == 'B' || *reply == 'b')
    {
      move_ptr->side = bottom;
      reply++;
    }
  else if (*reply == 'L' || *reply == 'l')
    {
      move_ptr->side = left;
      reply++;
    }
  else if (*reply == 'R' || *reply == 'r')
    {
      move_ptr->side = right;
      reply++;
    }
  else
    return 1;

  if (!(*reply >= '0' && *reply <= '9'))
    return 1;

  i = 0;
  do
    {
      i = i*10 + (*reply++ - '0');
      if (i > n)
        return 1;
    } while (*reply >= '0' && *reply <= '9');

  while (*reply == ' ')
    reply++;

  if (*reply != '\n' && *reply != '\0')
    return 1;

  move_ptr->index = i-1; /* index is 0 based */

  return 0;
}

#define OK                   0
#define TIME_LIMIT_EXPIRED   1
#define PROCESSES_STILL_LIVE 2
#define BAD_REPLY            3
#define OTHER_ERROR          4
#define UNCAUGHT_SIGNAL      5

int turn (n, id, input, move_ptr, elapsed_ptr)
int n;
int id;
string input;
move_t *move_ptr;
long *elapsed_ptr;
{
  subprocess subp;
  char reply[MAX_REPLY+1];
  int reply_length;
  int status;
  int phase;
  int time_limit_exceeded, processes_left;
  char cwd[PATH_MAX+1], home[1000], prog[1000], support[1000], path[10000];
#ifdef HP
  long tmptime, tmptime_last;
#endif

  getcwd (cwd, PATH_MAX);
  sprintf (home, "%s/contestants/%d", cwd, id);
  sprintf (prog, "%s/runme", home);
  sprintf (support, "%s/support", home);
  sprintf (path, "PATH=.:%s:/usr/bin:/bin", support);

  subprocess_open (uid(id), prog, home, path, &subp);

  if (write (subp.in, input.buf, input.length) != input.length)
    fatal_error ("error!\n");

  close (subp.in);

  *elapsed_ptr = wall_time_in_msecs ();

  time_limit_exceeded = 0;

  phase = 1;

  if (setjmp (timer_env) == 0)
    {
      set_timer (TIME_LIMIT_IN_MSECS);

      reply_length = read (subp.out, reply, MAX_REPLY);
#ifdef HP      
      tmptime = wall_time_in_msecs () - *elapsed_ptr;

      fprintf (stderr,
               "up to phase 1a took %ld msecs: delta %ld\n",
               tmptime, tmptime);

      tmptime_last = tmptime;

#endif
      close (subp.out);
#ifdef HP
      tmptime = wall_time_in_msecs () - *elapsed_ptr;

      fprintf (stderr,
               "up to phase 1b (close) took %ld msecs: delta %ld\n",
               tmptime, tmptime-tmptime_last);

      tmptime_last = tmptime;
#endif
      phase = 2;

      waitpid (subp.pid, &status, 0); /* wait for subprocess to end normally */
#ifdef HP
      tmptime = wall_time_in_msecs () - *elapsed_ptr;

      fprintf (stderr,
               "up to phase 2 (waitpid) took %ld msecs: delta %ld\n",
               tmptime, tmptime-tmptime_last);

      tmptime_last = tmptime;
#endif
      phase = 3;

      *elapsed_ptr = wall_time_in_msecs () - *elapsed_ptr;

      phase = 4;

      while (killuid (uid(id), 0)) /* wait for subprocesses to terminate */
        /*empty*/;

      phase = 5;
    }

  set_timer (0);

  time_limit_exceeded = 0;
  processes_left = 0;

  switch (phase)
    {
    case 1:
    case 2:
      waitpid (subp.pid, &status, WNOHANG);
      time_limit_exceeded = 1;
    case 3:
      *elapsed_ptr = wall_time_in_msecs () - *elapsed_ptr;
    case 4:
      processes_left = killuid (uid(id), SIGKILL);
      break;
    default:
      killuid (uid(id), SIGKILL);
    }

  if (WIFSIGNALED(status))
    return UNCAUGHT_SIGNAL+WTERMSIG(status);

  if (time_limit_exceeded)
    return TIME_LIMIT_EXPIRED;

  if (processes_left)
    return PROCESSES_STILL_LIVE;

  if (reply_length < 0)
    return OTHER_ERROR;

  reply[reply_length] = '\0';

  if (parse_reply (n, reply, move_ptr) != 0)
    return BAD_REPLY;

  return OK;
}

void init_program_input (s, n)
string *s;
int n;
{
  char n_str[10];

  make_empty_string (s);

  sprintf (n_str, "%d\n", n);
  string_add_c_string (s, n_str);
}

void play_round (n, x_id, o_id)
int n;
int x_id;
int o_id;
{
  int nb_moves;
  string input;
  board_list boards;
  board_t new_board;
  move_t move;
  int winner, player, other_player;
  int x_straights, o_straights;
  long elapsed, start, end;
  int err;
  char *player_name;
  char end_msg[100];
  char summary[10000];
  long game_time;
  char *date;

  sprintf (end_msg, "game ended normally");

  nb_moves = 0;
  init_program_input (&input, n);
  init_board_list (&boards, n);

  fprintf (stderr,
           "------------------------------------------------ N=%d X=%d O=%d\n",
           n,
           x_id,
           o_id);

  game_time = wall_time_in_secs ();
  date = ctime (&game_time);
  date[strlen(date)-1] = '\0';

  /* dry run */

  err = turn (n, x_id, input, &move, &elapsed);
  err = turn (n, o_id, input, &move, &elapsed);

  start = wall_time_in_msecs ();

  do
    {
      nb_moves++;
      player = (nb_moves&1) ? x_marker : o_marker;
      other_player = (player==x_marker) ? o_marker : x_marker;
      player_name = (player==x_marker) ? "X" : "O";
      err = turn (n,
                  (player==x_marker)?x_id:o_id,
                  input,
                  &move,
                  &elapsed);
      fprintf (stderr,
               "move %d (took %ld msecs):",
               nb_moves,
               elapsed);
      if (err != OK)
        {
          switch (err)
            {
            case TIME_LIMIT_EXPIRED:
              sprintf (end_msg,
                       "%s exceeded time limit",
                       player_name);
              break;
            case PROCESSES_STILL_LIVE:
              sprintf (end_msg,
                       "%s did not kill all its processes",
                       player_name);
              break;
            case BAD_REPLY:
              sprintf (end_msg,
                       "%s made an invalid move",
                       player_name);
              break;
            default:
              if (err >= UNCAUGHT_SIGNAL)
                sprintf (end_msg,
                         "%s raised signal %d",
                         player_name,
                         err-UNCAUGHT_SIGNAL);
              else
                sprintf (end_msg,
                         "%s caused an unknown error",
                         player_name);
              break;
            }
          fprintf (stderr, " %s\n", end_msg);
          winner = other_player;
          break;
        }
      fprintf (stderr, " %s played %s\n", player_name, format_move (move));
      string_add_c_string (&input, format_move (move));
      string_add_char (&input, '\n');
      copy_board (&boards->board, &new_board);
      make_move (new_board, move, player);
      print_board (stderr, new_board);
      if (board_exists (new_board, boards))
        {
          winner = other_player;
          break;
        }
      x_straights = nb_straights (new_board, x_marker);
      o_straights = nb_straights (new_board, o_marker);
      if (x_straights > o_straights)
        {
          winner = x_marker;
          break;
        }
      if (o_straights > x_straights)
        {
          winner = o_marker;
          break;
        }
      boards = cons_board (new_board, boards);
    } while (1);

  end = wall_time_in_msecs ();

  remove_linebreaks (input);
  input.buf[input.length] = '\0';
  sprintf (summary,
           "(\"%s\" X: %d O: %d winner: %s end: \"%s\" time: %d game: (%s))\n",
           date,
           x_id,
           o_id,
           (winner==x_marker) ? "X" : "O",
           end_msg,
           end-start,
           input.buf);

  fputs (summary, stderr);
  fputs (summary, stdout);
  /*  send_mail ("icfp98", summary); */
}

int main (argc, argv)
int argc;
char **argv;
{
  int n;
  int x_id, o_id;

  if (argc != 4)
    fatal_error ("usage: referee N program_X program_O\n");

  n = atoi (argv[1]);
  if (n <= 0 || n > MAX_N)
    {
      char msg[100];
      sprintf (msg, "n must be in the range 1..%d\n", MAX_N);
      fatal_error (msg);
    }

  x_id = atoi (argv[2]);
  if (x_id <= 0)
    {
      char msg[100];
      sprintf (msg, "program_X must be >= 1\n");
      fatal_error (msg);
    }

  o_id = atoi (argv[3]);
  if (o_id <= 0)
    {
      char msg[100];
      sprintf (msg, "program_O must be >= 1\n");
      fatal_error (msg);
    }

  play_round (n, x_id, o_id);

  return 0;
}


