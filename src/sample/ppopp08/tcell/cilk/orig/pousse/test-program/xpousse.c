/*
 * Pousse auto-test program.
 *
 * Usage: test-pousse [-n <n>] <first program> <second program>
 *
 * where <first program> and <second program> are names of
 * *directories* where the programs reside.
 * This test program will run `runme' in those directories.
 */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>

#include <forms.h>

#define  XPOUSSE  1


/* for getopt */
extern char *optarg;
extern int optind;

char *programs[2];
int n;
int human_mode;
int nrand;			/* number of random moves to produce */
int ngames;			/* number of pairs of games to play */
int do_display_board;
int timeout;
char *reason;            /* reason of victory */

pid_t child_pid;
int timed_out;

int  activity;
char human_move[8];

#define MAX_N 20


/*  A good choice is to make MOVE_BUTTON_WIDTH 3/4 size of SQ_SIZE,
    try 40,30 or 50,40 for SQ_SIZE and MOVE_BUTTON_WIDTH */
#define SQ_SIZE           50
#define MOVE_BUTTON_WIDTH 40


/* define forms and object */

FL_FORM *form;
FL_OBJECT *quit;
FL_OBJECT *back;
FL_OBJECT *square[MAX_N][MAX_N];
FL_OBJECT *move_button[4][MAX_N];
FL_OBJECT *token[MAX_N][MAX_N];

/* for move buttons : 
         0 = top (T)
	 1 = left (L)
	 2 = right (R)
	 3 = bottom (B)
*/




/* ---------- prototypes --------- */
void x_display_board();
void start_x_stuff( int argc, char *argv[] );
void end_x_stuff();


void complain(char *irritants)
{
     fprintf(stderr, "%s", irritants);
     if (!human_mode)
	  exit(-1);
}

void usage(void)
{
     complain("Usage: xpousse [-n <n>] [-h] [-d] [-r <#rand>] [-g <#games>] [-t <timeout>] program1 program2\n");
}

unsigned int create_seed(void)
{
     struct timeval tv;
     gettimeofday(&tv, NULL);
     return tv.tv_sec * 1000000L + tv.tv_usec;
}

void parse_command_line(int argc, char *argv[])
{
     int c;
     int dflag = 0;

     n = 10;
     ngames = 0;
     human_mode = 0;
     do_display_board = 1;
     timeout = 30;
     while ((c = getopt(argc, argv, "n:r:g:t:hd")) != -1) {
	  switch (c) {
	      case 'n':
		   n = atoi(optarg);
		   if (n < 4 || n > MAX_N)
			complain("<n> must be between 1 and 20\n");
		   break;
	      case 'r':
		   nrand = atoi(optarg);
		   if (nrand < 0)
			complain("<#rand> must be nonnegative\n");
		   break;
	      case 't':
		   timeout = atoi(optarg);
		   break;
	      case 'g':
		   ngames = atoi(optarg);
		   if (ngames < 0)
			complain("<#games> must be nonnegative\n");
		   if (!dflag) 
			do_display_board = 0;
		   break;
	      case 'h':
		   human_mode = 1;
		   break;
	      case 'd':
		   do_display_board = 1;
		   dflag = 1;
		   break;
	      default:
		   usage();
	  }
     }

     if (human_mode) {
	  if (optind + 1 != argc) {
	       complain("Only only one program must be specified in human mode\n");
	       exit(1);
	  }
	  programs[0] = argv[optind++];
     } else {
	  if (optind + 2 != argc)
	       usage();

	  programs[0] = argv[optind++];
	  programs[1] = argv[optind++];
     }
}

enum piece {
     NONE, XMARK, OMARK
};
enum piece board[MAX_N][MAX_N];

/* list of all previous moves */
struct move {
     char kind;
     int where;
     struct move *next;
};

struct move *move_list;

/* list of all previous boards */
struct board {
     enum piece board[MAX_N][MAX_N];
     enum piece turn;   /* 
			 * guy who made the last move, used for repetition
			 * check 
			 */
     struct board *next;
};

struct board *board_list;

void empty_board(void)
{
     int i, j;
     for (i = 0; i < n; ++i)
	  for (j = 0; j < n; ++j)
	       board[i][j] = 0;
}

void moveT(int column, enum piece p)
{
     int row = 0;
     enum piece q;

     do {
	  q = board[row][column];
	  board[row][column] = p;
	  p = q;
	  row++;
     } while (row < n && p != NONE);
}

void moveB(int column, enum piece p)
{
     int row = n;
     enum piece q;

     do {
	  row--;
	  q = board[row][column];
	  board[row][column] = p;
	  p = q;
     } while (row > 0 && p != NONE);
}

void moveR(int row, enum piece p)
{
     int column = n;
     enum piece q;

     do {
	  column--;
	  q = board[row][column];
	  board[row][column] = p;
	  p = q;
     } while (column > 0 && p != NONE);
}

void moveL(int row, enum piece p)
{
     int column = 0;
     enum piece q;

     do {
	  q = board[row][column];
	  board[row][column] = p;
	  p = q;
	  column++;
     } while (column < n && p != NONE);
}

void record_move(char kind, int where)
{
     struct move *new;

     new = malloc(sizeof(*new));
     if (!new)
	  complain("malloc failed\n");

     new->kind = kind;
     new->where = where;
     new->next = move_list;
     move_list = new;
}

void dispose_moves()
{
     struct move *p, *q;
     
     for (p = move_list; p; p = q) {
	  q = p->next;
	  free(p);
     }
}

/* create a new element in the board list and append it */
void record_board(enum piece who)
{
     struct board *new;

     new = malloc(sizeof(*new));
     if (!new)
	  complain("malloc failed\n");

     memcpy(new->board, board, sizeof(board));
     new->next = board_list;
     new->turn = who;
     board_list = new;
}

void dispose_boards()
{
     struct board *p, *q;
     
     for (p = board_list; p; p = q) {
	  q = p->next;
	  free(p);
     }
}


void  quitcb(FL_OBJECT *obj, long cbarg )
{
  end_x_stuff();

}


void  backcb(FL_OBJECT *obj, long cbarg )
{
     if (!human_mode) return;
     if (activity) return;
     activity = 1;
     strcpy( human_move, "back" );
}



void  make_press(FL_OBJECT *obj, long cbarg )
{
     char  move[4];
     char  *mdef = { "TLRB" };
     char kind;
     int where;
     char buf[64];
     static enum piece who;
     int  x;
     int  y;
     int  count_x = 0;
     int  count_y = 0;

     if (!human_mode) return;

     if (activity) return;

     activity = 1;

     move[0] = mdef[ cbarg/MAX_N ];
     sprintf( move+1, "%d", 1 + ((int) cbarg % MAX_N) );

     strcpy( human_move, move );

     if (human_mode) return;

     /* the code below this is pretty much useless */

     /* determine who's turn to move (the hard way) */

     for (y = 0; y < n; ++y) {
       for (x = 0; x < n; ++x) {
	 switch (board[y][x]) {
	 case NONE:
	   break;
	 case XMARK:
	   count_x++;
	   break;
	 case OMARK:
	   count_y++;
	   break;
	 }
       }
     }

     if (count_x <= count_y) 
       who = XMARK;
     else 
       who = OMARK;

     /*     printf("move = %s\n", move );  */

     /* make it look like matteo's routine */

     switch (toupper(move[0])) {
	 case 'T':
	 case 'B':
	 case 'L':
	 case 'R':
	      kind = toupper(move[0]);
	      break;

	 default:
	      sprintf(buf, "Illegal move %s\n", move);
	      complain(buf);
	      return;
     }

     where = atoi(&move[1]) - 1;
     if (where < 0 || where >= n) {
	  sprintf(buf, "Illegal move %s\n", move);
	  complain(buf);
	  return;
     }
     switch (kind) {
	 case 'T':
	      moveT(where, who);
	      break;
	 case 'B':
	      moveB(where, who);
	      break;
	 case 'L':
	      moveL(where, who);
	      break;
	 case 'R':
	      moveR(where, who);
	      break;
     }
     record_move(kind, where);

     x_display_board();

     return;
}




int parse_and_make_move(char *move, enum piece who)
{
     char kind;
     int where;
     char buf[64];

     switch (toupper(move[0])) {
	 case 'T':
	 case 'B':
	 case 'L':
	 case 'R':
	      kind = toupper(move[0]);
	      break;

	 default:
	      sprintf(buf, "Illegal move %s\n", move);
	      complain(buf);
	      return 0;
     }

     where = atoi(&move[1]) - 1;
     if (where < 0 || where >= n) {
	  sprintf(buf, "Illegal move %s\n", move);
	  complain(buf);
	  return 0;
     }
     switch (kind) {
	 case 'T':
	      moveT(where, who);
	      break;
	 case 'B':
	      moveB(where, who);
	      break;
	 case 'L':
	      moveL(where, who);
	      break;
	 case 'R':
	      moveR(where, who);
	      break;
     }
     record_move(kind, where);
     return 1;
}


void x_display_board()
{
    int x, y;

    for (y = 0; y < n; ++y) {
      for (x = 0; x < n; ++x) {
        switch (board[y][x]) {
        case NONE:
	  fl_hide_object( token[y][x] );
	  break;
	case XMARK:
	  fl_set_object_color( token[y][x], FL_BLUE, 0); 
	  fl_show_object( token[y][x] );
	  break;
	case OMARK:
	  fl_set_object_color( token[y][x], FL_RED, 0); 
	  fl_show_object( token[y][x] );
	  break;
	}
      }
    }
    
    fl_check_forms();

}



void display_board(void)
{
     int i, j;

#ifdef XPOUSSE
     x_display_board();
#endif

     printf("  ");
     for (j = 0; j < n; ++j)
	  printf(" %2d", j + 1);
     printf("\n");

     for (i = 0; i < n; ++i) {
	  printf("%2d ", i + 1);
	  for (j = 0; j < n; ++j) {
	       switch (board[i][j]) {
		   case NONE:
			printf("   ");
			break;
		   case XMARK:
			printf(" X ");
			break;
		   case OMARK:
			printf(" O ");
			break;
	       }
	  }
	  printf("\n");
     }

     printf("\n");
     fflush(stdout);
}

void display_move_list(int fd, struct move *m)
{
     char buf[32];
     if (m) {
	  display_move_list(fd, m->next);
	  sprintf(buf, "%c%d\n", m->kind, m->where + 1);
	  write(fd, buf, strlen(buf));
     }
}

void feed_input_to_program(int fd)
{
     char buf[32];

     sprintf(buf, "%d\n", n);
     write(fd, buf, strlen(buf));

     display_move_list(fd, move_list);
}

int is_straight_row(int row, enum piece who)
{
     int i;
     for (i = 0; i < n; ++i)
	  if (board[row][i] != who)
	       return 0;

     return 1;
}

int is_straight_col(int col, enum piece who)
{
     int i;
     for (i = 0; i < n; ++i)
	  if (board[i][col] != who)
	       return 0;

     return 1;
}

enum piece opponent(enum piece who)
{
     return (who == OMARK) ? XMARK : OMARK;
}

char mark_of(enum piece who)
{
     return (who == OMARK) ? 'O' : 'X';
}

/* return true if board already seen */
int board_already_seen(enum piece who)
{
     struct board *b;

     for (b = board_list; b; b = b->next) {
	  int i, j;
	  if (b->turn == who) {
	       for (i = 0; i < n; ++i)
		    for (j = 0; j < n; ++j)
			 if (board[i][j] != b->board[i][j])
			      goto again;
	       /* boards are the same */
	       return 1;
	  }
	again:
     }
     return 0;
}

enum piece winner_p(enum piece player)
{
     int i;
     int straightX = 0;
     int straightO = 0;

     if (board_already_seen(player)) {
	  reason = "(by repetition)";
	  return opponent(player);
     }

     for (i = 0; i < n; ++i) {
	  straightX += is_straight_row(i, XMARK);
	  straightX += is_straight_col(i, XMARK);
	  straightO += is_straight_row(i, OMARK);
	  straightO += is_straight_col(i, OMARK);
     }

     if (straightX > straightO) {
	  reason = "(more straights)";
	  return XMARK;
     }
     if (straightO > straightX) {
	  reason = "(more straights)";
	  return OMARK;
     }
     return NONE;
}

/*
 * play a random move.
 * Return the winner if any.
 */
enum piece play_random(char *name, enum piece who)
{
     char buf[32];
     enum piece winner;

     sprintf(buf, "%c%d",
	     "TBRL"[(random() >> 16) & 0x3],
	     (int)(1 + (random() % n)));

     parse_and_make_move(buf, who);
     winner = winner_p(who);
     record_board(who);
     return winner;
}

void timeout_handler( int signum )
{
     int status;
     kill( child_pid, SIGTERM );
     wait(&status);
     timed_out = 1;
}

/*
 * fork, set up the program, execute it, wait.
 * Return the winner if any.
 */
enum piece play_program(char *program, enum piece who)
{
     int status;
     int output_pipe[2];
     int input_pipe[2];
     char buf[32];
     enum piece winner;
     pid_t pid;
     int r;

     if (pipe(output_pipe) == -1)
	  complain("can't open pipe\n");
     if (pipe(input_pipe) == -1)
	  complain("can't open pipe\n");

     switch (pid = fork()) {
	 case 0: /* child */
	      if (chdir(program) == -1)
		   complain("can't chdir()\n");
	      /* set up stdin and stdout of the child program */
	      dup2(output_pipe[0], 0);
	      dup2(input_pipe[1], 1);

	      close(output_pipe[0]);
	      close(output_pipe[1]);
	      close(input_pipe[0]);
	      close(input_pipe[1]);

	      {
		   const char *new_argv[4];
		   new_argv[0] = "/bin/sh";
		   new_argv[1] = "-c";
		   new_argv[2] = "./runme";
		   new_argv[3] = NULL;

		   /* Exec the shell.  */
		   execv("/bin/sh", (char *const *) new_argv);

		   /* die if exec() returns */
		   complain("exec returned!\n");
	      }

	 case -1:
	      complain("can't fork\n");
	      break;

	 default:		/* parent */
	      child_pid = pid;

	      close(output_pipe[0]);
	      close(input_pipe[1]);

	      timed_out = 0; 
	      {
		   struct sigaction new_action, old_action;
		   new_action.sa_handler = timeout_handler;
		   sigemptyset (&new_action.sa_mask);
		   new_action.sa_flags = 0;
		   sigaction(SIGALRM, &new_action, &old_action);
	      }

     	      alarm(timeout);

	      /* write data to child */
	      feed_input_to_program(output_pipe[1]);
	      close(output_pipe[1]);

	      /* read answer */
	      buf[0] = 0;

              r = read(input_pipe[0], buf, 32);
	      close(input_pipe[0]);

	      alarm(0);

	      if (timed_out) {
		   printf( "Player (%c) timed out\n", mark_of(who) );
		   winner = opponent(who);
		   reason = "(timeout of opponent)";
	      } else {
		   wait(&status);
		   parse_and_make_move(buf, who);
		   winner = winner_p(who);
		   record_board(who);
		   buf[r] = 0;
		   if (human_mode) 
			printf("Computer move was %s\n", buf);
	      }

	      return winner;
     }
     return NONE;		/* unreached */
}


void proclaim_winner(char *prog1, enum piece p1, char *prog2, enum piece p2,
		     enum piece winner)
{
     char mark = mark_of(winner);
     char *s = (winner == p1) ? prog1 : prog2;

     display_board();
     printf("Program %s (%c) wins %s.\n", s, mark, reason);
}

void proclaim_winner_human(enum piece winner, enum piece human)
{
     display_board();
     if (winner == human)
	  printf("Human wins %s.\n", reason);
     else 
	  printf("Computer wins %s.\n", reason);
}

/* play a game.  return the winner */
enum piece play_game(char *prog1, enum piece p1, int victories1,
		     char *prog2, enum piece p2, int victories2)
{
     enum piece winner;
     enum piece (*player_function) (char *name, enum piece who);
     int nmove;

     move_list = (struct move *) 0;
     board_list = (struct board *) 0;
     reason = NULL;
     empty_board();

     for (nmove = 0; ; ++nmove) {
	  player_function = (nmove < nrand) ? play_random : play_program;

	  if (do_display_board) {
	       printf("%s's turn (%c)  %d victories \n",
		      prog1, mark_of(p1), victories1);
	  }
	  winner = player_function(prog1, p1);
	  if (do_display_board)
	       display_board();
	  if (winner != NONE)
	       return winner;

	  if (do_display_board) {
	       printf("%s's turn (%c)  %d victories \n",
		      prog2, mark_of(p2), victories2);
	  }
	  winner = player_function(prog2, p2);
	  if (do_display_board)
	       display_board();
	  if (winner != NONE)
	       return winner;
     }

     dispose_moves();
     dispose_boards();
}

void increment_victories(enum piece p1, int *victories1,
			 enum piece p2, int *victories2,
			 enum piece winner)
{
     if (winner == p1) 
	  ++*victories1;
     else
     	  ++*victories2;
}

void play_pair(char *prog1, enum piece p1, int *victories1,
	       char *prog2, enum piece p2, int *victories2)
{
     unsigned int seed;
     enum piece winner;

     /* get random seed */
     seed = create_seed();

     /* play prog1 vs prog2 */
     srandom(seed);
     winner = play_game(prog1, p1, *victories1, prog2, p2, *victories2);
     proclaim_winner(prog1, p1, prog2, p2, winner);
     increment_victories(p1, victories1, p2, victories2, winner);

     /* play prog2 vs prog1 */
     srandom(seed);
     winner = play_game(prog2, p1, *victories2, prog1, p2, *victories1);
     proclaim_winner(prog1, p2, prog2, p1, winner);
     increment_victories(p2, victories1, p1, victories2, winner);
}

void undo_one(void) 
{
     struct move *m;
     struct board *b;
     if (move_list) {
	  m = move_list->next;
	  free(move_list);
	  move_list = m;
     }
     if (board_list) {
	  b = board_list->next;
	  free(board_list);
	  board_list = b;
	  if (b) 
	       memcpy(board, b->board, sizeof(board));
	  else 
	       empty_board();
     }
}

void undo(void) {
     undo_one();
     undo_one();
}

void play_human(void)
{
     enum piece winner, human, turn;
     int ply;
     char buf[32];
     

     move_list = (struct move *) 0;
     board_list = (struct board *) 0;
     reason = NULL;
     empty_board();
     human = XMARK;

     turn = human;
     ply = 0;

     for (;;) {
	  if (turn == human) {
	       strcpy(buf, "");
	       display_board();
	       {
		 int  i; 
		 for (i=0; i<n; i++)
		 { 
		   fl_activate_object( move_button[0][i] );
		   fl_activate_object( move_button[1][i] );
		   fl_activate_object( move_button[2][i] );
		   fl_activate_object( move_button[3][i] );
		 }

		 activity = 0;
		 while ( activity == 0 ) fl_check_forms();
		 activity = 0;

		 for (i=0; i<n; i++)
		 { 
		   fl_deactivate_object( move_button[0][i] );
		   fl_deactivate_object( move_button[1][i] );
		   fl_deactivate_object( move_button[2][i] );
		   fl_deactivate_object( move_button[3][i] );
		 }

	       }
	  retry:
	       /*
	       printf("Human's turn (%c), ply %d, enter move, or `flip', or `back'\n",
		      mark_of(human), ply);
	       fgets(buf, 32, stdin);
	       */
	       strcpy( buf, human_move );
	       printf("HUMAN PLAYS %s\n", human_move );
	       strcpy( human_move, "" );


	       /* crappy unix */
	       if (buf[strlen(buf)-1] == '\n') 
		    buf[strlen(buf)-1] = 0;
	  
	       if (strcmp(buf, "back") == 0) {
		    undo();
		    ply -= 2;
		    if (ply < 0) ply = 0;
	       } else if (strcmp(buf, "flip") == 0) {
		    human = opponent(human);
	       } else {
		    if (parse_and_make_move(buf, turn) == 0) goto retry;
		    winner = winner_p(turn);
		    record_board(turn);
		    display_board();

		    if (winner != NONE) {
		      proclaim_winner_human(winner, human);
		      
		      activity = 0;
		      while ( activity == 0 ) fl_check_forms();
		      activity = 0;
		      break; 
		    }
		    turn = opponent(turn);
		    ++ply;
	       }
	  } else {
	       winner = play_program(programs[0], turn);
	       if (winner != NONE) {
		 proclaim_winner_human(winner, human);
		 activity = 0;
		 while ( activity == 0 ) fl_check_forms();
		 activity = 0;
		 break; 
	       }

	       turn = opponent(turn);
	       ++ply;
	  }
     }

     display_board();
     dispose_moves();
     dispose_boards();
}


int main(int argc, char *argv[])
{
     parse_command_line(argc, argv);

     start_x_stuff( argc, argv );


     srandom(create_seed());
     if (human_mode) {
	  play_human();
     } else
     if (ngames == 0) {
	  enum piece winner;
	  winner = play_game(programs[0], XMARK, 0, programs[1], OMARK, 0);
	  proclaim_winner(programs[0], XMARK, programs[1], OMARK, winner);
     } else {
	  int i;
	  int v0 = 0, v1 = 0;
	  double percent;
	  for (i = 0; i < ngames; ++i)
	       play_pair(programs[0], XMARK, &v0,
			 programs[1], OMARK, &v1);
	  
	  percent = (100.0 * v0) / (v0 + v1);
	  printf("Program %s wins %d times (%3.2f%%)\n", programs[0], v0,
		 percent);
	  printf("Program %s wins %d times (%3.2f%%)\n", programs[1], v1,
		 100.0 - percent);
     }

     end_x_stuff();
     return 0;
}




void  start_x_stuff( int argc, char *argv[] )
{

  fl_initialize(&argc, argv, "Xpousse for linux", 0, 0); 

  form = fl_bgn_form(FL_UP_BOX, (n+2)*SQ_SIZE, (n+4)*SQ_SIZE );


  quit = fl_add_button( 
		       FL_PUSH_BUTTON,   /* (n_2*SQ_SIZE, 60, 80, 30, "Quit" );  */
		       (n-1)*SQ_SIZE+2,  
		       ((n+2.5)*SQ_SIZE) + 2,
		       (SQ_SIZE * 2)-4,
		       MOVE_BUTTON_WIDTH,
		       "Quit" );

  fl_set_object_callback(quit, quitcb, 0 );

  back = fl_add_button( 
		       FL_NORMAL_BUTTON,   /* (n_2*SQ_SIZE, 60, 80, 30, "Quit" );  */
		       (1)*SQ_SIZE+2,  
		       ((n+2.5)*SQ_SIZE) + 2,
		       (SQ_SIZE * 2)-4,
		       MOVE_BUTTON_WIDTH,
		       "Take back" );

  fl_set_object_callback(back, backcb, 0 );


  /* routine to draw a baord based on size of n */
  {
    int  x;
    int  y;
    int  i;

    for (y=0; y<n; y++)
      for (x=0; x<n; x++)
      {
	square[y][x] = 
	  fl_add_box( FL_FRAME_BOX, (x+1)*SQ_SIZE, (y+1)*SQ_SIZE, SQ_SIZE, SQ_SIZE, "");
	fl_set_object_color( square[y][x], FL_WHITE, 0); 

	token[y][x] = 
	    fl_add_roundbutton( 
			       FL_NORMAL_BUTTON,  /* ROUNDED3D_UPBOX,  */
			       (x+1)*SQ_SIZE+2,
			       (y+1)*SQ_SIZE+1,
			       SQ_SIZE-2,
			       SQ_SIZE-2,
			       "");
	fl_hide_object( token[y][x] );
     }

    for (i=0; i<n; i++) 
    {
      char lab[8];

      /* top */
      sprintf( lab, "T%d", i+1 );
      move_button[0][i] = 
	fl_add_button( 
		      FL_NORMAL_BUTTON, 
		      (i+1)*SQ_SIZE+2, 
		      SQ_SIZE-MOVE_BUTTON_WIDTH-2,
		      SQ_SIZE-4,
		      MOVE_BUTTON_WIDTH, lab );
      fl_set_object_callback(move_button[0][i], make_press, i + (MAX_N * 0) );

      /* left */
      sprintf( lab, "L%d", i+1 );
      move_button[1][i] = 
	fl_add_button( 
		      FL_NORMAL_BUTTON, 
		      SQ_SIZE-MOVE_BUTTON_WIDTH-2,
		      (i+1)*SQ_SIZE+2,
		      MOVE_BUTTON_WIDTH,
		      SQ_SIZE-4,
		      lab );
      fl_set_object_callback(move_button[1][i], make_press, i + (MAX_N * 1) );

      /* right */
      sprintf( lab, "R%d", i+1 );
      move_button[2][i] = 
	fl_add_button( 
		      FL_NORMAL_BUTTON, 
		      ((n+1)*SQ_SIZE) + 2,
		      (i+1)*SQ_SIZE+2,
		      MOVE_BUTTON_WIDTH,
		      SQ_SIZE-4,
		      lab );
      fl_set_object_callback(move_button[2][i], make_press, i + (MAX_N * 2) );

      /* bottom */
      sprintf( lab, "B%d", i+1 );
      move_button[3][i] = 
	fl_add_button( 
		      FL_NORMAL_BUTTON,
		      (i+1)*SQ_SIZE+2,  
		      ((n+1)*SQ_SIZE) + 2,
		      SQ_SIZE-4,
		      MOVE_BUTTON_WIDTH,
		      lab );
     fl_set_object_callback(move_button[3][i], make_press, i + (MAX_N * 3) );

    }

  }


  fl_end_form();
  fl_show_form(form, FL_PLACE_CENTER, FL_FULLBORDER, "CILK-POUSSE");


}



void end_x_stuff()
{
  /* fl_do_forms(); */
  fl_finish();
  exit(0);

}

