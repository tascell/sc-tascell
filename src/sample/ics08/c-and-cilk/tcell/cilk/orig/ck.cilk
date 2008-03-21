/*
 * This program plays rudimentary checkers, without double jumps. It is
 * not interactive, both colors are played by the computer. Also a good
 * test for aborts.
 *
 * Author: Don Dailey, drd@supertech.lcs.mit.edu
 */

static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/ck.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

/*
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cilk-lib.cilkh>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <getoptions.h>

extern int Cilk_rand(void);
extern void Cilk_srand(unsigned int seed);

#define PAWN    1
#define KING    2
#define WHITE   4
#define BLACK   8

#define WP  WHITE+PAWN
#define BP  BLACK+PAWN
#define WK  WHITE+KING
#define BK  BLACK+KING

#define INF       900000
#define WIN_SCORE 100000

#define MAX_PLY 128

int root_move;

int eval_tab[12] =
{
     0, 0, 0, 0,
     0, 10, 12, 0,
     0, 10, 12, 0
};

int color_eval_tab[12] =
{
     0, 0, 0, 0,
     0, -10, -12, 0,
     0, 10, 12, 0,
};

int dirtab[2][4] =
{
     15, 17, -17, -15,
     -15, -17, 15, 17
};

int opening[0x80] =
{
     WP, 00, WP, 00, WP, 00, WP, 00, 0, 0, 0, 0, 0, 0, 0, 0,
     00, WP, 00, WP, 00, WP, 00, WP, 0, 0, 0, 0, 0, 0, 0, 0,
     WP, 00, WP, 00, WP, 00, WP, 00, 0, 0, 0, 0, 0, 0, 0, 0,
     00, 00, 00, 00, 00, 00, 00, 00, 0, 0, 0, 0, 0, 0, 0, 0,
     00, 00, 00, 00, 00, 00, 00, 00, 0, 0, 0, 0, 0, 0, 0, 0,
     00, BP, 00, BP, 00, BP, 00, BP, 0, 0, 0, 0, 0, 0, 0, 0,
     BP, 00, BP, 00, BP, 00, BP, 00, 0, 0, 0, 0, 0, 0, 0, 0,
     00, BP, 00, BP, 00, BP, 00, BP, 0, 0, 0, 0, 0, 0, 0, 0,
};

typedef struct {
     int ply_of_game;		/* must be even for black, odd for white */
     int ply;			/* actual ply of search                  */
     int board[0x80];
     int inc_eval;		/* current static evaluation             */
     int depth;
     int alpha;
     int beta;
} position;

int killer[MAX_PLY];

/* ---- PROTOTYPES ---- */

cilk int srch(position * oldp, int mv);
void make(position * p, int mv);
void showbd(position * p);
cilk void play(int blev, int wlev);

int usage()
{
     printf("\n");
     printf("This program plays rudimentary checkers, without double jumps. Both\n");
     printf("colors are played by the computer.\n\n");
     printf("Command: ck [<cilk-options>] [<options>] \n\n");
     printf("Options: -d #  specifies how deep the game tree is evaluated\n");
     printf("               for black and white.");
     printf("         -w #  search depth for white in plys.\n");
     printf("         -b #  search depth for black in plys.\n");
     printf("         -h    for help\n");
     printf("         -benchmark [ short | medium | long ]\n\n");
     printf("Default: ck -b 7 -w 8\n");
     printf("Author:  Don Dailey, drd@supertech.lcs.mit.edu\n\n");
     return 1;
}

char *specifiers[] =
{"-d", "-b", "-w", "-benchmark", "-h", 0};
int opt_types[] =
{INTARG, INTARG, INTARG, BENCHMARK, BOOLARG, 0};

cilk int cilk_main(int argc, char *argv[])
{
     int level;
     int blev;
     int wlev;
     int benchmark;
     int help;
     Cilk_time tm_begin, tm_elapsed;
     Cilk_time wk_begin, wk_elapsed;
     Cilk_time cp_begin, cp_elapsed;

     /* standard benchmark options */
     wlev = 8;
     blev = 7;
     level = 0;

     get_options(argc, argv, specifiers, opt_types,
		 &level, &blev, &wlev, &benchmark, &help);

     if (level != 0)
	  wlev = blev = level;

     if (help)
	  return usage();

     if (benchmark) {
	  switch (benchmark) {
	      case 1:		/* short benchmark options -- a little work */
		   wlev = 3;
		   blev = 3;
		   break;
	      case 2:		/* standard benchmark options */
		   wlev = 8;
		   blev = 7;
		   break;
	      case 3:		/* long benchmark options -- a lot of work */
		   wlev = 10;
		   blev = 10;
		   break;
	  }
     }
     if (wlev < 1)
	  wlev = 1;
     if (wlev > 99)
	  wlev = 99;
     if (blev < 1)
	  blev = 1;
     if (blev > 99)
	  blev = 99;

     /* Timing. "Start" timers */
     sync;
     cp_begin = Cilk_user_critical_path;
     wk_begin = Cilk_user_work;
     tm_begin = Cilk_get_wall_time();

     spawn play(blev, wlev);
     sync;

     /* Timing. "Stop" timers */
     tm_elapsed = Cilk_get_wall_time() - tm_begin;
     wk_elapsed = Cilk_user_work - wk_begin;
     cp_elapsed = Cilk_user_critical_path - cp_begin;

     printf("\nCilk Example: ck\n");
     printf("	      running on %d processor%s\n\n",
	    Cilk_active_size, Cilk_active_size > 1 ? "s" : "");
     printf("Options: black search plys = %d\n", blev);
     printf("         wite search plys  = %d\n\n", wlev);
     printf("Running time  = %4f s\n", Cilk_wall_time_to_sec(tm_elapsed));
     printf("Work          = %4f s\n", Cilk_time_to_sec(wk_elapsed));
     printf("Critical path = %4f s\n\n", Cilk_time_to_sec(cp_elapsed));

     return (0);
}

cilk void play(int blev, int wlev)
{
     position gme[200];		/* record of game */
     int ply;
     int i;
     int sd;
     int score = 0;		/* to get rid of warning error */
     int gameover = 0;
     int level;
     /* initialize the position state */

     /* 
      * NOTE: everything may look a little backwards because the
      * search immediately starts negamaxing                  
      */
     memcpy(gme[0].board, opening, sizeof(gme[0]));

     Cilk_srand(42);

     /* calculate static score */

     gme[0].inc_eval = 0;
     for (i = 0; i < 0x78; i++) {
	  if (i & 0x88)
	       i += 8;		/* skip through board edge */
	  gme[0].inc_eval += color_eval_tab[gme[0].board[i]];
     }

     gme[0].inc_eval = -gme[0].inc_eval;

     printf("Starting inc_eval = %8d\n", gme[0].inc_eval);

     /* cycle through a game */

     showbd(&gme[0]);

     for (ply = 0; ply < 120; ply++) {
	  /* initialize search */

	  gme[ply].ply_of_game = ply - 1;
	  gme[ply].ply = 0;

	  if (ply & 1)
	       level = wlev;
	  else
	       level = blev;

	  score = 0;

	  for (sd = 1; sd <= level; sd++) {
	       if (abs(score) > 1000 && sd > 1) {
		    printf("\n");
		    continue;
	       }
	       gme[ply].depth = sd;
	       gme[ply].alpha = -INF;
	       gme[ply].beta = INF;

	       score = spawn srch(&gme[ply], 0);
	       sync;

	       printf("%4d)   sc: %-8d  mv=%5x\n",
		      sd,
		      score,
		      root_move
		   );
	       fflush(stdout);
	       if (score > 1000 && sd == 1)
		    gameover = 1;

	  }
	  printf("\n");
	  fflush(stdout);

	  gme[ply + 1] = gme[ply];
	  make(&gme[ply + 1], root_move);
	  showbd(&gme[ply + 1]);

	  if (gameover)
	       break;

	  /* next position */
	  gme[ply + 1].inc_eval = -gme[ply + 1].inc_eval;

     }
}

/* move generator */
int gen(position * p, int *mvl)
{
     int i;
     int ctm;			/* color to move  4 = black,   8 = white */
     int cix;			/* color index  0=black  1=white         */
     int count = 0;
     int dir;
     int dest;
     int cap = 0;		/* flag for captures */

     if (p->ply_of_game & 1) {
	  ctm = WHITE;
	  cix = 0;
     } else {
	  ctm = BLACK;
	  cix = 1;
     }

     for (i = 0; i < 0x78; i++) {
	  if (i & 0x88)
	       i += 8;		/* skip through board edge */

	  if (p->board[i] & ctm)
	       for (dir = 0; dir < 4; dir++) {
		    if (dir == 2 && (p->board[i] & PAWN))
			 break;
		    dest = i + dirtab[cix][dir];
		    if (dest & 0x88)
			 continue;
		    if (p->board[dest] == 0) {
			 mvl[count++] = (i << 8) + dest;
		    } else if (!(p->board[dest] & ctm)) {	/* jump
								 * move */
			 if ((dest + dirtab[cix][dir]) & 0x88)
			      continue;

			 if (p->board[dest + dirtab[cix][dir]] == 0) {
			      mvl[count++] = (i << 8) + dest;
			      cap++;
			 }
		    }
	       }
     }

     if (cap) {			/* cull out non-jump moves */
	  cap = 0;

	  for (i = 0; i < count; i++)
	       if (p->board[mvl[i] & 0xff])
		    mvl[cap++] = mvl[i];

	  count = cap;
     }
     /* randomize list at ply 1 */
     /* ----------------------- */

     if (p->ply == 1) {
	  /* 
	   * shuffle routine - forgive me for my blatant re-use of 
	   * variables here!  drd 
	   */

	  for (i = 0; i < count; i++) {
	       do
		    cix = (Cilk_rand() & 31);
	       while (cix >= count);

	       cap = mvl[i];
	       mvl[i] = mvl[cix];
	       mvl[cix] = cap;
	  }

     }
     /* put killer move at front of list */

     cix = killer[p->ply];
     for (i = 0; i < count; i++)
	  if (cix == mvl[i]) {
	       cap = mvl[i];
	       mvl[i] = mvl[0];
	       mvl[0] = cix;
	       break;
	  }
     return (count);

}

/*
 * this is the negamax search code, with alpha-beta pruning.
 * Notice the usage of the inlet and of abort when beta cutoff occurs.
 */
cilk int srch(position * oldp, int mv)
{
     position p;
     int best_score = -INF;
     int move_list[32];
     int count;
     int x;
     int beta_cutoff = 0;
     int cur_score = 0;

     inlet void catch(int eval, int choice_ix) {
	  eval = -eval;
	  if (eval > best_score) {
	       best_score = eval;
	       killer[p.ply] = move_list[choice_ix];
	       if (p.ply == 1)
		    root_move = move_list[choice_ix];
	       if (eval > p.alpha)
		    p.alpha = eval;
	  }
	  if (eval >= p.beta) {
	       beta_cutoff = 1;
	       abort;
	  }
     }

     /* fix up new state */
     p = *oldp;

     /* nega-max everything */
     p.inc_eval = -p.inc_eval;
     x = p.alpha;
     p.alpha = -p.beta;
     p.beta = -x;

     p.ply_of_game++;
     p.depth--;
     p.ply++;

     make(&p, mv);		/* make the move */

     /* generate all legal moves from this position */
     count = gen(&p, move_list);

     if (count == 0)
	  return (-WIN_SCORE + p.ply);	/* game over */

     /* exhausted quies search? */

     if (p.depth < 0 && (p.board[move_list[0] & 0xff] == 0))
	  return (p.inc_eval);

     /* this loops tries all possible moves */
     for (x = 0; x < count; x++) {

	  /* search 1st move and root moves serially */

	  if (x == 0 || p.ply == 1) {
	       cur_score = spawn srch(&p, move_list[x]);
	       sync;

	       cur_score = -cur_score;

	       if (cur_score > best_score) {
		    killer[p.ply] = move_list[x];
		    best_score = cur_score;
		    if (p.ply == 1)
			 root_move = move_list[x];
	       }
	       if (cur_score >= p.beta) {
		    killer[p.ply] = move_list[x];
		    return (best_score);
	       }
	       if (cur_score > p.alpha)
		    p.alpha = cur_score;

	       continue;
	  }
	  /* search all other moves in parallel */
	  catch(spawn srch(&p, move_list[x]), x);
	  if (beta_cutoff)
	       break;
     }

     sync;

     return (best_score);
}

void showbd(position * p)
{
     int i;

     for (i = 0; i < 0x78; i++) {
	  if (i & 0x88) {
	       printf("\n");
	       fflush(stdout);
	       i += 8;
	  }
	  switch (p->board[i]) {
	      case 0:
		   printf(" -- ");
		   break;
	      case WP:
		   printf(" WP ");
		   break;
	      case WK:
		   printf(" WK ");
		   break;
	      case BP:
		   printf(" BP ");
		   break;
	      case BK:
		   printf(" BK ");
		   break;
	  }

     }

     printf("\n\n");
     fflush(stdout);

}

void make(position * p, int mv)
{
     int mv_from;
     int mv_to;

     if (mv) {			/* else we are root  level */
	  mv_from = mv >> 8;
	  mv_to = mv & 0xff;

	  if (p->board[mv_to]) {	/* then its a capture */
	       p->inc_eval -= eval_tab[p->board[mv_to]];
	       p->board[mv_to] = 0;
	       mv_to = mv_to + (mv_to - mv_from);
	       p->board[mv_to] = p->board[mv_from];
	       p->board[mv_from] = 0;
	  } else {
	       p->board[mv_to] = p->board[mv_from];
	       p->board[mv_from] = 0;
	  }

	  /* detect promotion to KING  */
	  /* ------------------------  */

	  if (p->board[mv_to] == BP && mv_to < 8) {
	       p->board[mv_to] = BK;
	       p->inc_eval -= eval_tab[BK];
	       p->inc_eval += eval_tab[BP];
	  } else if (p->board[mv_to] == WP && mv_to >= 0x70) {
	       p->board[mv_to] = WK;
	       p->inc_eval -= eval_tab[WK];
	       p->inc_eval += eval_tab[WP];
	  }
     }
     return;
}
