/*
 * This is a simple game to test aborts.  Rules: There are two players
 * on a n x m board.  The players alternate in putting a coin in
 * position (i,j).  When (i,j) is filled with a coin, all positions
 * (i',j') for all i' <= i and j' <= j are filled as well. (Multiple
 * coins count as one.)  The player who fills the board loses.
 *
 * Homework: prove that the first player has a winning strategy.
 *
 * I make no attempt to be clever in this program.
 *
 * Author: Matteo Frigo
 * This game was suggested by David Finberg, dfinberg@mit.edu .
 */
static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/game.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

#include <cilk-lib.cilkh>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#if HAVE_MALLOC_H
#include <malloc.h>
#endif
#include "getoptions.h"

#define MAX_PROC 16		/* max # of processors */

#define M 5
#define N 5

/* well... this is one attempt to be clever */
#define FOR_ALL_POSITIONS(i,j) \
   for (i = N - 1; i >= 0; --i) \
      for (j = M - 1; j >= 0; --j)

#define EMPTY 0
#define COIN 1

#define WIN 1000
#define LOSE (-WIN)

typedef int BOARD[N][M];
typedef int (*BOARDHACK)[M];
typedef int INDEX;

int naborts[MAX_PROC];

int usage()
/*
 * Not very useful since there are no options, but anyway to be
 * consistent with the other examples.  
 */
{
     printf("\n"
	    "This is a simple game to test aborts.\n"
	    "Rules: There are two players on a n x m board. While the board\n"
	    "is not full, each player puts a coin in position (i,j) and in\n"
	    "all positions (i',j') for all i' <= i and j' <= j.\n"
	    "(Multiple coins count as one.)\n"
	    "The player who fills the board loses.\n\n"
	    "\n"
	    "Command: game [<cilk-options>] [<options>] \n\n"
	    "Options: -h    for this help\n"
	    "Author:  Matteo Frigo, athena@theory.lcs.mit.edu\n\n");
     return 1;
}

inline void init_board(BOARD b)
{
     INDEX i, j;

     FOR_ALL_POSITIONS(i, j) {
	  b[i][j] = EMPTY;
     }
}

/* test if the move is legal */
inline int legal(BOARD b, INDEX i, INDEX j)
{
     return (b[i][j] == EMPTY);
}

/* produce a new board nb with the given move */
inline void move(BOARD b, INDEX r, INDEX c, BOARD nb)
{
     INDEX i, j;

     FOR_ALL_POSITIONS(i, j) {
	  nb[i][j] = b[i][j];
     }

     for (i = 0; i <= r; ++i)
	  for (j = 0; j <= c; ++j)
	       nb[i][j] = COIN;
}

/* return true if the position is lost */
inline int lostp(BOARD b)
{
     return (b[N - 1][M - 1] == COIN);
}

/* do the negamax search */
cilk int negamax(BOARDHACK b, INDEX *movei, INDEX *movej,
		 int alpha, int beta, int depth)
{
     INDEX i, j;
     INDEX dummyi, dummyj;	/* not used */
     int done;

     inlet void catch(int evaluation, INDEX i1, INDEX j1, INDEX test) {
	  Cilk_assert(test == i1 + j1);
	  evaluation = -evaluation;

	  if (evaluation > alpha) {
	       alpha = evaluation;
	       /* record the best so far */
	       /* TODO: how do we communicate new alpha to children? */
	       *movei = i1;
	       *movej = j1;
	  }
	  if (alpha >= beta) {
	       done = 1;
	       ++naborts[Self];
	       abort;
	  }
     }

     if (lostp(b))
	  return WIN - depth;	/* winning earlier is better */

     done = 0;

     FOR_ALL_POSITIONS(i, j) {
	  if (legal(b, i, j)) {
	       int (*nb)[M] = Cilk_alloca(sizeof(BOARD));

	       move(b, i, j, nb);
	       catch(
		       spawn negamax(nb, &dummyi, &dummyj, -beta, -alpha,
				     depth + 1), i, j, i + j
		   );

	       if (done)
		    break;
	  }
     }
     sync;

     return alpha;
}

char *specifiers[] =
{"-benchmark", "-h", 0};
int opt_types[] =
{BENCHMARK, BOOLARG, 0};

cilk int main(int argc, char *argv[])
{
     INDEX i, j;
     int (*b)[M];
     int best = 1;
     int first = 1;
     int prev_best = 1;
     int k, n, player, benchmark, help;
     Cilk_time tm_begin, tm_elapsed;
     Cilk_time wk_begin, wk_elapsed;
     Cilk_time cp_begin, cp_elapsed;

     get_options(argc, argv, specifiers, opt_types, &benchmark, &help);

     if (help)
	  return usage();

     for (k = 0; k < MAX_PROC; ++k)
	  naborts[i] = 0;

     b = malloc(sizeof(BOARD));

     init_board(b);
     player = 0;

     /* Timing. "Start" timers */
     sync;
     cp_begin = Cilk_user_critical_path;
     wk_begin = Cilk_user_work;
     tm_begin = Cilk_get_wall_time();

     while (!lostp(b)) {
	  player = !(player);
	  best = spawn negamax(b, &i, &j, -100000, 100000, 0);
	  sync;

	  printf("%s found best move at %d,%d\n",
		 ((player) ? "White" : "Black"), i, j);

	  if (first) {
	       Cilk_assert(best >= 0);
	       first = 0;
	  } else {
	       Cilk_assert(best == -(prev_best + (prev_best > 0 ? 1 : -1)));
	  }

	  prev_best = best;

	  move(b, i, j, b);
	  for (i = 0; i < N; ++i) {
	       for (j = 0; j < M; ++j)
		    printf("%d ", b[i][j]);
	       printf("\n");
	  }
	  printf("\n");
     }

     /* Timing. "Stop" timers */
     tm_elapsed = Cilk_get_wall_time() - tm_begin;
     wk_elapsed = Cilk_user_work - wk_begin;
     cp_elapsed = Cilk_user_critical_path - cp_begin;

     for (k = n = 0; k < MAX_PROC; ++k)
	  n += naborts[k];

     printf("%s wins. Total # of aborts %d\n\n",
	    ((!(player)) ? "White" : "Black"), n);

     printf("\nCilk Example: game\n");
     printf("	      running on %d processor%s\n\n", 
	    Cilk_active_size, Cilk_active_size > 1 ? "s" : "");
     printf("Running time  = %4f s\n", Cilk_wall_time_to_sec(tm_elapsed));
     printf("Work          = %4f s\n", Cilk_time_to_sec(wk_elapsed));
     printf("Critical path = %4f s\n\n", Cilk_time_to_sec(cp_elapsed));

     return 0;
}
