/* Interactive Kalah game, written by Reid Barton. */

static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/kalah.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

/*
 * Copyright (c) 2003 Massachusetts Institute of Technology
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PLYTYPE 1

#define nil 0L
#define NUMHOLES 6
#define BOARDSIZE (NUMHOLES + 1)
#define POSSIZE (2*BOARDSIZE)
#define INFINITY (10000)
#define WINSCORE 1000
/* #define HASHSIZE (1 << 16) */
/* #define HASHMASK (HASHSIZE - 1) */
#define EXACTSCORE 1
#define LOWERBOUND 2
#define EMPTY 0
#define DEFAULT_DEPTH 10
#define PROC_OFFSET 64

#define ALPHA_BETA 1
#define BEST_VAR 1

#define POM(x) ((x)*(-2)+1)
#define sign(x) (((x)>0)?(1):(((x)==0)?(0):(-1)))

typedef int move;

struct position {
     int p[POSSIZE + 1];
     unsigned int again;
};
typedef struct position position;

struct movelist {
     move m;
     struct movelist *next;
};
typedef struct movelist movelist;

struct poslist {
     position p[NUMHOLES];
     move m[NUMHOLES];
     int nextAvail;
};
typedef struct poslist poslist;

struct bestres {
     move m;
     int score;
};
typedef struct bestres bestres;

struct hashentry {
     position p;
     short score;
     unsigned int bound;
     short depth;
     move best;
     long code;
     struct hashentry *next;
};
typedef struct hashentry hashentry, *hashentryptr;

struct search_state {
     position p;
     short alpha, beta;
     short ply_of_search;
     short sub_ply;
     unsigned int cont;
};
typedef struct search_state search_state;

hashentryptr hashtab[64 * PROC_OFFSET];
int hashing;
int stones;

int maxPly;

long numnodes[64 * PROC_OFFSET];

move pc[30][30][30];
move mymoves[30];
move bestMoves[30][30];
int dmax;

long hashsize;
long hashmask;

unsigned rnd_table[73][14];

move get_human_move(position p);
void clearhashtab();
void clearpc();
void printpc();
void display(position p);
poslist generatemoves(position p, move best, int cont);
poslist generatemoves0(position p, move best, int cont);
poslist generatemoves1(position p, move best, int cont);
void addpos(position p, move m, poslist * list);
position makemove(position p, move m);
void print(poslist list);
void emptylist(poslist * list);
int isempty(poslist list);
long hash(position p);
cilk int main_srch(position p, int depth);
cilk int srch(search_state ss);
int ismemequal(char *p, char *q, int length);
cilk hashentry get_hash(position p);
cilk void set_hash(hashentry he, position p);

cilk int main(int argc, char *argv[])
{
     position pos;

     move humanmove;
     move best;
     int score;
     Cilk_time workstart, workend;
     int movenum;
     int n = 0;
     int j;
     int depth;

     depth = DEFAULT_DEPTH;
     hashing = 1;
     stones = 6;
     hashsize = 1 << 16;
     for (j = 1; j < argc;) {
	  if (!strcmp(argv[j], "-depth")) {
	       j++;
	       if (j == argc)
		    printf("Option '-depth' requires numeric argument\n");
	       else {
		    depth = atoi(argv[j]);
		    j++;
	       }
	  } else if (!strcmp(argv[j], "-hash")) {
	       j++;
	       hashing = 1;
	  } else if (!strcmp(argv[j], "-nohash")) {
	       j++;
	       hashing = 0;
	  } else if (!strcmp(argv[j], "-hashsize")) {
	       j++;
	       if (j == argc)
		    printf("Option '-hashsize' requires numeric argument\n");
	       else {
		    hashsize = 1 << atoi(argv[j]);
		    j++;
	       }
	  } else if (!strcmp(argv[j], "-stones")) {
	       j++;
	       if (j == argc)
		    printf("Option '-stones' requires numeric argument\n");
	       else {
		    stones = atoi(argv[j]);
		    j++;
	       }
	  } else
	       printf("Invalid option '%s' ignored\n", argv[j++]);
     }
     hashmask = hashsize - 1;

     for (j = 0; j < POSSIZE; j++)
	  if (j % BOARDSIZE == 0)
	       pos.p[j] = 0;
	  else
	       pos.p[j] = stones;
     pos.p[POSSIZE] = 0;

     hashtab[0] = malloc(sizeof(hashentry) * hashsize);
     for (j = 0; j < 64; j++)
	  hashtab[j * PROC_OFFSET] = hashtab[0];
     clearhashtab();

     printf("Welcome to Kalah!  You are playing the lower row of stones.\n"
	  "To make a move, enter a number from 1 to 6, where 1 is your\n"
	  "leftmost pit and 6 the rightmost pit.  Your kalah is on the\n"
	  "right, and the stones move counterclockwise.  To quit, type\n"
	    "'quit'.  Good luck!\n\n");

     display(pos);
     assert(hashtab);
     movenum = 0;

     while ((pos.p[POSSIZE] >= 0) && (pos.p[0] <= NUMHOLES * stones) && (pos.p[BOARDSIZE] <= NUMHOLES * stones)) {
	  if (pos.p[POSSIZE] == 0) {
	       if (movenum > 0) {
		    printf(movenum > 1 ? "My moves: " : "My move: ");
		    for (n = 1; n <= movenum; n++)
			 printf("%d\t", mymoves[n]);
	       }
	       printf("\n");
	       movenum = 0;
	       humanmove = get_human_move(pos);
	       printf("\n");
	       pos = makemove(pos, humanmove);
	  } else if (pos.p[POSSIZE] == 1) {
	       clearpc();
	       movenum++;
	       printf("ply\t sco\t    nodes   n/ms\t    ttotal\n");
	       sync;
	       workstart = Cilk_user_work;
	       for (dmax = 1; dmax <= depth; dmax++) {
		    int i;
		    long totalnodes;
		    for (i = 0; i < 64; i++)
			 numnodes[i * PROC_OFFSET] = 0;
		    score = spawn main_srch(pos, dmax);
		    sync;
		    best = bestMoves[0][0];
		    workend = Cilk_user_work;
#if PROC_OFFSET
		    totalnodes = 0;
		    for (i = 0; i < 64; i++)
			 totalnodes += numnodes[i * PROC_OFFSET];
#else
		    totalnodes = numnodes[0];
#endif
		    printf("%2d\t%4d\t%9ld%7.2f\t%10.2f",
			   dmax,
			   score,
			   totalnodes,
			   totalnodes / (Cilk_time_to_sec(workend - workstart) * 1000),
			   Cilk_time_to_sec(workend - workstart) * 1000);
#if BEST_VAR
		    printf("  ");
		    printpc();
#else
		    printf("\n");
#endif
	       }
	       pos = makemove(pos, best);
	       mymoves[movenum] = best;
	  }
	  display(pos);
     }
     return 0;
}

move get_human_move(position p)
{
     char entry[128];
     int humanmove;

     printf("Enter move: ");
     fgets(entry, 128, stdin);
     if (entry[0] == 'q')
	  _exit(1);
     humanmove = atoi(entry);
     if (humanmove == -1)
	  _exit(1);
     while ((humanmove <= 0) || (humanmove > NUMHOLES) || !p.p[humanmove]) {
	  printf("Invalid move\nEnter move: ");
	  fgets(entry, 128, stdin);
	  humanmove = atoi(entry);
	  if (humanmove == -1)
	       _exit(1);
     }
     return (move) humanmove;
}

void clearhashtab()
{
     int i;

     for (i = 0; i < hashsize; i++)
	  hashtab[0][i].bound = EMPTY;
}

void clearpc()
{
     int i, j, k;

     for (i = 0; i < 30; i++)
	  for (j = 0; j < 30; j++)
	       for (k = 0; k < 30; k++)
		    pc[i][j][k] = 0;
}

void printpc()
{
     int i, j;

     for (i = 0; i < dmax; i++) {
	  for (j = 0; (j < 30) && (pc[0][i][j] != 0); j++)
	       printf("%2d,", pc[0][i][j]);
	  printf("\b ");
     }
     printf("\n");
}

void display(position p)
{
     int i;

     /* top row */
     printf(" ");
     for (i = POSSIZE - 1; i > BOARDSIZE; i--)
	  printf("%3d", p.p[i]);
     printf("\n");

     /* middle row */
     printf("%d", p.p[0]);
     for (i = 1; i < BOARDSIZE; i++)
	  printf("   ");
     printf(" ");
     printf("%d\n", p.p[BOARDSIZE]);

     /* bottom row */
     printf(" ");
     for (i = 1; i < BOARDSIZE; i++)
	  printf("%3d", p.p[i]);
     printf("\n");

     if (p.p[POSSIZE] >= 0)
	  printf("Player %d%s\n", p.p[POSSIZE] + 1, p.again ? " (again)" : "");
     else
	  printf("Game Over\n");
}

poslist generatemoves(position p, move best, int cont)
{
     assert(!((p.p[POSSIZE] > 1) || (p.p[POSSIZE] < 0)));
     if (p.p[POSSIZE] == 0)
	  return generatemoves0(p, best, cont);
     else
	  return generatemoves1(p, best, cont);
}

poslist generatemoves0(position p, move best, int cont)
{
     int i, j, k;
     int donemove[6] =
     {1, 1, 1, 1, 1, 1};
     poslist list;

     emptylist(&list);
     for (i = 1; i < BOARDSIZE; i++) {
	  if (p.p[i] > 0) {
	       donemove[i - 1] = 0;
	  }
     }
     if (best && !donemove[best - 1]) {
	  addpos(makemove(p, best), best, &list);
	  donemove[best - 1] = 1;
     }
     for (j = 1; j < BOARDSIZE; j++) {
	  k = j + p.p[j];
	  if (!donemove[j - 1] && ((p.p[j] == 2 * BOARDSIZE - 1) || ((k < BOARDSIZE) && (p.p[k] == 0) && (p.p[POSSIZE - k] != 0)))) {
	       addpos(makemove(p, j), j, &list);
	       donemove[j - 1] = 1;
	  }
     }
     for (i = 1; i < BOARDSIZE; i++)
	  if (((i + p.p[i]) % (POSSIZE - 1) == BOARDSIZE) && !donemove[i - 1]) {
	       addpos(makemove(p, i), i, &list);
	       donemove[i - 1] = 1;
	  }
     if (!cont)
	  for (i = 1; i < BOARDSIZE; i++)
	       if (!donemove[i - 1])
		    addpos(makemove(p, i), i, &list);
     return list;
}

poslist generatemoves1(position p, move best, int cont)
{
     int i, j, k;
     int donemove[6] =
     {1, 1, 1, 1, 1, 1};
     poslist list;

     emptylist(&list);
     for (i = BOARDSIZE + 1; i < 2 * BOARDSIZE; i++) {
	  if (p.p[i] > 0) {
	       donemove[i - BOARDSIZE - 1] = 0;
	  }
     }
     if (best && !donemove[best - BOARDSIZE - 1]) {
	  addpos(makemove(p, best), best, &list);
	  donemove[best - BOARDSIZE - 1] = 1;
     }
     for (j = BOARDSIZE + 1; j < 2 * BOARDSIZE; j++) {
	  k = j + p.p[j];
	  if (!donemove[j - BOARDSIZE - 1] && ((p.p[j] == 2 * BOARDSIZE - 1) || ((k < 2 * BOARDSIZE) && (p.p[k] == 0) && (p.p[POSSIZE - k] != 0)))) {
	       addpos(makemove(p, j), j, &list);
	       donemove[j - BOARDSIZE - 1] = 1;
	  }
     }
     for (i = BOARDSIZE + 1; i < 2 * BOARDSIZE; i++)
	  if (((i + p.p[i]) % (POSSIZE - 1) == 2 * BOARDSIZE) && !donemove[i - BOARDSIZE - 1]) {
	       addpos(makemove(p, i), i, &list);
	       donemove[i - BOARDSIZE - 1] = 1;
	  }
     if (!cont)
	  for (i = BOARDSIZE + 1; i < 2 * BOARDSIZE; i++)
	       if (!donemove[i - BOARDSIZE - 1])
		    addpos(makemove(p, i), i, &list);

     return list;
}

void emptylist(poslist * list)
{
     list->nextAvail = 0;
}

int isempty(poslist list)
{
     return (list.nextAvail == 0);
}

void addpos(position p, move m, poslist * list)
{
     list->p[list->nextAvail] = p;
     list->m[list->nextAvail] = m;
     list->nextAvail++;
}

position makemove(position p, move m)
{
     int i, j, num, empty;
     position newp;

     newp = p;
     num = newp.p[m];		/* count how many stones */
     newp.p[m] = 0;		/* empty pit i */
     for (i = num, j = m + 1; i > 1; i--, j++) {	/* for each stone except the last */
	  if (j == POSSIZE)
	       j = 0;		/* loop around */
	  if (j == newp.p[POSSIZE] * BOARDSIZE) {
	       i++;		/* one more stone */
	       continue;	/* skip over opponent's store */
	  }
	  newp.p[j]++;		/* put a stone in pit j */
     }
     /* last stone */
     if (j == POSSIZE)
	  j = 0;
     if (j == newp.p[POSSIZE] * BOARDSIZE)
	  j++;
     if (j == POSSIZE)
	  j = 0;
     if ((((newp.p[POSSIZE] == 1) && (j > BOARDSIZE)) || ((newp.p[POSSIZE] == 0) && (j > 0) && (j < BOARDSIZE))) && (newp.p[j] == 0) && (newp.p[POSSIZE - j] != 0)) {
	  newp.p[(1 - newp.p[POSSIZE]) * BOARDSIZE] += newp.p[POSSIZE - j];
	  newp.p[(1 - newp.p[POSSIZE]) * BOARDSIZE]++;
	  newp.p[j]--;
	  newp.p[POSSIZE - j] = 0;
     }
     newp.p[j]++;
     empty = 1;
     for (i = 1 + p.p[POSSIZE] * BOARDSIZE; (i < (1 + p.p[POSSIZE]) * BOARDSIZE) && empty; i++)
	  if (newp.p[i])
	       empty = 0;
     if (empty) {
	  for (i = 1 + (1 - p.p[POSSIZE]) * BOARDSIZE; i < (2 - p.p[POSSIZE]) * BOARDSIZE; i++) {
	       newp.p[p.p[POSSIZE] * BOARDSIZE] += newp.p[i];
	       newp.p[i] = 0;
	  }
	  newp.p[POSSIZE] = -1 - newp.p[POSSIZE];
	  newp.again = 0;
     } else if ((newp.p[0] > NUMHOLES * stones) || (newp.p[BOARDSIZE] > NUMHOLES * stones)) {
	  newp.p[POSSIZE] = -1 - newp.p[POSSIZE];
	  newp.again = 0;
     } else if (!(j == (1 - newp.p[POSSIZE]) * BOARDSIZE)) {
	  newp.p[POSSIZE] = 1 - newp.p[POSSIZE];
	  newp.again = 0;
     } else
	  newp.again = 1;
     return newp;
}

void print(poslist list)
{
     int i;

     for (i = 0; i < list.nextAvail; i++)
	  display(list.p[i]);
}

long hash(position p)
{
     long result = 0;
     int i;

     for (i = 0; i < POSSIZE; i++)
	  result ^= rnd_table[p.p[i]][i];
     if (p.p[POSSIZE] == 1)
	  result ^= 0xFFFFFFFF;
     return result;
}

cilk int main_srch(position p, int depth)
{
     search_state ss;
     int result;

     ss.p = p;
     ss.alpha = -INFINITY;
     ss.beta = INFINITY;
     ss.ply_of_search = 0;
     ss.sub_ply = 0;
     ss.cont = 0;
     maxPly = depth;
     result = spawn srch(ss);
     sync;
     return result;
}

cilk int srch(search_state ss)
{
     int best = -INFINITY;
     int beta_cutoff = 0;
     search_state ss2;
     poslist list;
     int i;
     hashentry he;
     int bestMove;

     /* INLET getResult */
     /* Gets result from spawned search and updates best, beta_cutoff, ss.alpha, bestMoves
      * accordingly. */
     inlet void getResult(int result, int again, move m) {
	  if (!again)
	       result = -result;
	  if (result > best) {
#if BEST_VAR
	       int j, k;
#endif
	        best = result;
	        bestMoves[ss.ply_of_search][ss.sub_ply] = m;
	        bestMove = m;
#if BEST_VAR
	        pc[ss.ply_of_search][ss.ply_of_search][ss.sub_ply] = m;
	       if (!again)
		    for (k = ss.sub_ply + 1; k < 30; k++)
			  pc[ss.ply_of_search][ss.ply_of_search][k] = 0;
	       for (j = ss.ply_of_search + 1; j < dmax; j++)
		    for (k = 0; k < 30; k++)
			  pc[ss.ply_of_search][j][k] = pc[ss.ply_of_search + 1][j][k];
#endif
	  }
#if ALPHA_BETA
	  if (result >= ss.beta) {
	       beta_cutoff = 1;
	       abort;
	  }
	  if (result > ss.alpha)
	       ss.alpha = result;
#endif
     }
     /* END INLET getResult */

     numnodes[Self * PROC_OFFSET]++;
     if (ss.p.p[POSSIZE] < 0) {
	  return (sign(ss.p.p[BOARDSIZE] - ss.p.p[0]) * POM(2 + ss.p.p[POSSIZE]) * (WINSCORE - ss.ply_of_search));
     }
     if (ss.ply_of_search >= maxPly) {
	  list = generatemoves(ss.p, 0, 1);	/* get capture/kalah moves */
	  if (isempty(list))
	       return (ss.p.p[BOARDSIZE] - ss.p.p[0]) * POM(ss.p.p[POSSIZE]);
	  else {
	       ss2.cont = 1;
	  }
     } else
	  ss2.cont = 0;

     if (hashing) {
	  he = spawn get_hash(ss.p);
	  sync;
	  if ((he.bound != EMPTY) && (he.depth >= (maxPly - ss.ply_of_search)) && (he.code == hash(ss.p)) && ((he.bound == EXACTSCORE) || ((he.bound == LOWERBOUND) && (he.score >= ss.beta)))) {
	       bestMoves[ss.ply_of_search][ss.sub_ply] = he.best;
	       return he.score;
	  }
	  if ((he.bound != EMPTY) && (he.code == hash(ss.p)) && ismemequal((char *) (&he.p.p), (char *) (&ss.p.p), sizeof(he.p.p)))
	       list = generatemoves(ss.p, he.best, ss.cont);
	  else
	       list = generatemoves(ss.p, 0, ss.cont);
     } else
	  list = generatemoves(ss.p, 0, ss.cont);
     if (isempty(list))
	  return 0;

     for (i = 0; (i < list.nextAvail) && !beta_cutoff; i++) {
	  int ag, mo;

	  ag = list.p[i].again;
	  mo = list.m[i];
	  ss2.p = list.p[i];
	  if (ss2.p.again) {
	       ss2.alpha = ss.alpha;
	       ss2.beta = ss.beta;
#if PLYTYPE
	       ss2.ply_of_search = ss.ply_of_search;
	       ss2.sub_ply = ss.sub_ply + 1;
#else
	       ss2.ply_of_search = ss.ply_of_search + 1;
#endif
	  } else {
	       ss2.alpha = -ss.beta;
	       ss2.beta = -ss.alpha;
	       ss2.ply_of_search = ss.ply_of_search + 1;
	       ss2.sub_ply = 0;
	  }
	  getResult(spawn srch(ss2), ag, mo);
	  if (i == 0)
	       sync;
     }
     sync;

     if (hashing) {
	  he.p = ss.p;
	  he.score = best;
	  he.bound = beta_cutoff ? LOWERBOUND : EXACTSCORE;
	  he.depth = maxPly - ss.ply_of_search;
	  he.best = bestMove;
	  he.code = hash(ss.p);
	  spawn set_hash(he, ss.p);
	  sync;
     }
     return best;
}

int ismemequal(char *p, char *q, int length)
{
     if (memcmp(p, q, length) != 0)
	  return 0;
     else
	  return 1;
}

cilk hashentry get_hash(position p)
{
     hashentry he;
     he = hashtab[Self * PROC_OFFSET][hash(p) & hashmask];
     return he;
}

cilk void set_hash(hashentry he, position p)
{
     hashentry h = hashtab[Self * PROC_OFFSET][hash(p) & hashmask];
     if ((h.bound == EMPTY) || (h.depth < he.depth)) {
	  hashtab[Self * PROC_OFFSET][hash(p) & hashmask] = he;
     }
}

unsigned rnd_table[73][14] =
{
     0xe7bdc877, 0xf876d508, 0x3b6a9981, 0x04a35bea,
     0x71b445c0, 0x6eb7be49, 0x54b3250f, 0xf34da304,
     0x19f664e2, 0xf5f6a192, 0x497f301c, 0x19b99b7e,
     0x8868d7a5, 0x4dd25fd9, 0x5f3b2305, 0xc26887e0,
     0x0c783a48, 0x6925150b, 0xb71eb98c, 0xa44c6902,
     0x6132680e, 0x1fd0849b, 0x1610d8ca, 0xbf163a83,
     0xc36a9bfb, 0x859a8aa2, 0x4501e638, 0xd83cd47b,
     0xf2673286, 0x2eb55929, 0x2790d371, 0x6312c7fd,
     0x1cdfcb12, 0xa58bd498, 0x7f26d066, 0x853accf2,
     0x1244fffa, 0x6f795ec6, 0xcc644894, 0xe1b73734,
     0x905a46e1, 0xf7f21789, 0x11b1addd, 0x0cc2fd8c,
     0xb63edf62, 0x2b2f396d, 0x26706d14, 0x545c5c04,
     0xb4c6cbdd, 0x9e5ce1bc, 0xf8bae068, 0x7bdecb10,
     0x68c93afc, 0x68d1fefb, 0xbcb5854f, 0x0f83726d,
     0x0d7236d0, 0xe5a4676d, 0xd95c53d4, 0x201a05f7,
     0x90249b86, 0x375c3877, 0xa57ecaf2, 0x3c282ea4,
     0x40a45dfd, 0x0636f29a, 0x9f999935, 0xe5e2dfa6,
     0xc4952a4f, 0x7e169406, 0xabf40336, 0x271cf690,
     0xa3559020, 0x094b9334, 0xfb29cfbf, 0x880716aa,
     0x87b8876b, 0xbcc20813, 0x7c44ec68, 0x1fcfad17,
     0x77114611, 0x1a11eb22, 0x646eb710, 0xdf582571,
     0x1612f627, 0xf66c157a, 0xfbc73761, 0xaa62f7e8,
     0xfd46a3c5, 0xd0ba0098, 0x03dd5f81, 0x98e69123,
     0xd4b1266f, 0xb0c0c229, 0xbaa362f0, 0xd4ceac9a,
     0x82f312ab, 0x28622674, 0xfeadc7a2, 0xdb0432b6,
     0xea863b90, 0x3649700f, 0xecce524f, 0x00a72845,
     0x1a6a9994, 0xbd4bf502, 0xff4234c6, 0x44a8ffdb,
     0xa1c0b228, 0x10215f10, 0x0a9467ec, 0xdb800f6d,
     0xe23834f7, 0x83898059, 0x8337002d, 0x1b8d709c,
     0x83a48e73, 0x77455e35, 0x163c9a15, 0x11d8102b,
     0x6979326b, 0xc0b13fc0, 0x56548b3a, 0x48df45b9,
     0x7123659e, 0xc71858f5, 0xdf761b52, 0xe2343661,
     0x37e17497, 0x9fd2ddbd, 0xcdedfb0c, 0x7dd49808,
     0x603effa8, 0xfccb3b8a, 0xa5c2ced5, 0xb9329ae3,
     0xe25e5f79, 0xaac4e075, 0xb53e0c8b, 0x10f3e2a3,
     0x516bfd67, 0x57ff918a, 0x3b50987d, 0x3104d500,
     0x2fdc6b71, 0x96ff00d4, 0x21e66215, 0x7bf1e095,
     0x976c3bac, 0xec6dc96f, 0xe0175d30, 0xd2f21f64,
     0x215b8298, 0x76eb693b, 0xa655a72d, 0x2fb0df0e,
     0x68f94bb0, 0x01480124, 0xfe74916c, 0x84f4e644,
     0x1e5c6505, 0x7885d294, 0xa609d057, 0x6d879441,
     0x63b777ae, 0x24d429f7, 0x7ae1cf90, 0x51b6d20b,
     0xe04f9173, 0x85b3ef99, 0xb41073a9, 0xb067dc4a,
     0x98faf94d, 0xd92484d2, 0x14c1999d, 0xb79584e5,
     0x50d48217, 0xa6edb182, 0xb110ebf6, 0x86551925,
     0x5b4a0606, 0xad7f31d3, 0xe2d9dee9, 0xdbde9491,
     0x7d55174c, 0x1ea2265d, 0x47a569d3, 0xaa3306ae,
     0xc8977031, 0x3daa721f, 0xb208c34d, 0xb917f45c,
     0x59c392de, 0xa14f77b0, 0xa0e7f908, 0xa4b797f9,
     0x488bc6f7, 0x4e3db33e, 0x5b1d9a06, 0x67b021df,
     0x525dfa50, 0xfaba2b07, 0x42ffe49b, 0xb352a86f,
     0xf510dc25, 0x3abbab75, 0xe945e3e2, 0x1f560189,
     0x93159452, 0x330cb70e, 0x2a7731db, 0x3447ec23,
     0xa1e289c1, 0xdeee9479, 0x24c49008, 0xf91678fe,
     0xedb6a4bc, 0xe6ce49b7, 0xdf53661d, 0x9df9f50d,
     0xf3124c67, 0x39bad5cf, 0x04c2cb14, 0x917ede9f,
     0x9b4c2abd, 0x91a50775, 0x4122556d, 0x6ed5f414,
     0x228c3001, 0xa8cac69f, 0x4b08eec8, 0x00e068db,
     0x20e2bcc2, 0x34bc59c9, 0xa57f0b7c, 0xf83df126,
     0xc4468b12, 0x467cd98c, 0x92bb3cff, 0x24967c5f,
     0x505ad689, 0xc2f1ffc3, 0xca0bb7e2, 0x4c5d93f0,
     0xb039ccb8, 0x394ec56d, 0xcb5c5de8, 0x04dbed38,
     0xcc089862, 0x0f122754, 0x5db9b2f2, 0x85a90d4a,
     0xcdaad41b, 0x5c0b9817, 0x3b02d6f3, 0x03375c80,
     0x1673c4e3, 0xb9a88fc2, 0x0a2ada09, 0xe72c25c4,
     0xdc5d8855, 0x8643a7af, 0xcd9ef096, 0xc5eabe58,
     0x52973ae6, 0x01324e35, 0xba704962, 0x598f7f25,
     0xa8a5cd70, 0x5d41845f, 0x1fa15e5e, 0xa77cd69c,
     0xdc2c0144, 0xa12f58a0, 0x69dfb145, 0x7b1ca3d8,
     0xd2b2ad15, 0x0b83d2be, 0xe6cdf740, 0xae363cd1,
     0xe0ba81c9, 0x495dafae, 0x2db44db1, 0xa55f1cc5,
     0x2fdc13f8, 0x9642444c, 0x99b60a36, 0xed0b9a40,
     0xbc4f50dc, 0x703757c1, 0x5af80772, 0xab12d311,
     0x8be3bdbd, 0x24e94f4c, 0xd4815aed, 0x28bc8a9c,
     0x2d9ae037, 0xa4f53536, 0xfbec793c, 0xfd702dfd,
     0x0ce22794, 0x9db6a995, 0x97fd2fd4, 0x08e74240,
     0xc6d2d825, 0x6f419596, 0xd43c0fae, 0x9456e320,
     0xaf648ffb, 0x749536d6, 0xea726737, 0x0b928748,
     0xa74cb5cf, 0xce712685, 0x302bf965, 0x0d07b3ce,
     0x435911d2, 0x22ee605f, 0xcffd1a1b, 0x9e78d54a,
     0x7e8a78df, 0x579b7b99, 0x01dc5f36, 0x9c1a5cdf,
     0x19e8c725, 0xae6fedbe, 0xcdb415eb, 0x1f8a49b5,
     0x299cf377, 0x4e7d9cf9, 0xd9b18eff, 0xa16b84b8,
     0xf5ae1f23, 0xd128b2a6, 0xe4338321, 0x2ab86cd3,
     0x2d7dbe9f, 0x172daeae, 0x32339559, 0x9e47c790,
     0xe0b47710, 0x5f9e5cb3, 0xade3fc43, 0xb5cec51f,
     0x106e7480, 0x73554e31, 0xdd834dd9, 0x8de5ac57,
     0x92b0c118, 0x1103e5ed, 0x7ef9f876, 0x0afff10e,
     0xa1b031ca, 0x94fe3dc2, 0x11440b5b, 0x0c07393f,
     0x834682a3, 0xf6464f7a, 0x34b3956d, 0x9b530681,
     0xe575080d, 0x7c342e5d, 0x4fafeb2e, 0x9421d861,
     0xfafb03dd, 0x9e53ec93, 0x2e6a3705, 0x03d4e17f,
     0xd235436f, 0x09a9bfe3, 0xb85d074e, 0x9c13469e,
     0x19646b68, 0xb3672425, 0x6b222f71, 0x94b1f383,
     0x640501f0, 0xf128f649, 0xbcb2fddf, 0x0b379b8a,
     0x3344c91c, 0xc28cd510, 0xa7462a39, 0x18b002ac,
     0xf80b8b4e, 0x5dff82a6, 0x05359583, 0x6c3c12cd,
     0xb71a0c50, 0x0f2a4060, 0x1b2f9ec6, 0x0934f05a,
     0xc40f911d, 0x130fe844, 0x21296393, 0x4b9c646a,
     0x9192065e, 0x07dd2b71, 0x3c0a9628, 0xc66766d9,
     0x0cbc23bf, 0x99081b94, 0x78eb5579, 0x8c9916fc,
     0x306a29be, 0xc8c8fb63, 0x27fc5afb, 0x559496a8,
     0xa0447acb, 0x0b51b7bf, 0xbf9a1cce, 0xecdef744,
     0x183f4f58, 0x769ddaee, 0xe57b356f, 0x77434454,
     0xb728d7f6, 0x7696042d, 0xb59afe3f, 0x0711f4b9,
     0x08566fac, 0xd102d375, 0xbd9a2e1e, 0xe8e3b4b1,
     0x28bf3a0f, 0xedac8494, 0x618d853d, 0xfadaf292,
     0xed07e922, 0x87402581, 0xd905de17, 0x203e1949,
     0xf11fd3ac, 0x3a39e25e, 0xb035ca42, 0xab1eead2,
     0x2ae0de8c, 0x111ea0ce, 0x28675816, 0xd5e0573e,
     0x0b635bc7, 0xb0b2ef4b, 0x7ea705f0, 0x9d1d6aa7,
     0x7ac4a39b, 0xaaf01070, 0x87a1c0db, 0x64752de5,
     0x82433562, 0x58d909c0, 0x0b9441ef, 0xc6df099a,
     0x13192df8, 0x8b9cf24f, 0xc74922ef, 0xf24dc38b,
     0x8436bb32, 0x0e34421c, 0x69e0b2f1, 0xb13d706b,
     0x8efccaf3, 0xe802ecff, 0x25941214, 0x0770151c,
     0x2412fac8, 0xc355171b, 0x43e3a241, 0x6892d190,
     0x47323170, 0x444549f4, 0xc485c4f3, 0x93898787,
     0x6a924ac1, 0x6f685db7, 0xa1dbe594, 0x3b5a03cf,
     0xa8550631, 0xae096cb7, 0x6d7ce580, 0x2ee7eb14,
     0x7d253091, 0x7a4ec0b0, 0xb6d88c66, 0x10c9d772,
     0x331b1c67, 0xca333a48, 0x9ac732af, 0xe278c1b5,
     0xfb82b3e5, 0x48464c4f, 0x21442a60, 0x8b0a1684,
     0xfcd9d4d5, 0x208ed151, 0xefe01870, 0x619ea83c,
     0xcdd77bf5, 0x30cf1e87, 0x5c5d32af, 0xbccef7ba,
     0xf52be88c, 0x779c5b2e, 0x0d0e4557, 0xec821fee,
     0xe91e1e2f, 0x0ad6c58c, 0x7aba1576, 0x61760262,
     0x2838fb13, 0xc0945589, 0x24a747a5, 0x52f81aef,
     0xa62b2f68, 0x344e0887, 0xf0e4c4dd, 0x6c738aa5,
     0x17dbcc2b, 0x3b718c13, 0x9b360dbd, 0x586160cd,
     0xcb5c7666, 0x2441edf4, 0x35f2d0e3, 0x82d1af9d,
     0x85e50916, 0xe9e1bdf7, 0xa7cef6d2, 0xc8e5f687,
     0x81dcc185, 0xbef74fa1, 0x6b0900e5, 0x0c92fcf1,
     0x67ede28c, 0x200cc718, 0x88bafbfc, 0x35759d6d,
     0xc262772c, 0xf48179f6, 0xbb89b4d7, 0xf9611b66,
     0xfe936d09, 0xa2fd9eae, 0x6b8465f9, 0xac490880,
     0xb76d48d4, 0x286e64ca, 0x8d271247, 0xfc7906cc,
     0x3352943f, 0x2af4fa23, 0xc70e7e62, 0x627f30ac,
     0x4aaf2458, 0x4f933198, 0x32763010, 0x9fd383d4,
     0x340f2008, 0x14e1d409, 0xd1e4aaf6, 0xcd6308e6,
     0xd0560ba3, 0xa5efbc4d, 0x824c66bc, 0xaeeca326,
     0x7525ce81, 0xaf2013fc, 0x6452110c, 0x4d40c0ef,
     0xc8ff24f4, 0x2cf4c1ed, 0x63e9d7ad, 0x5d659a6b,
     0x34bb1f35, 0xa6e8733a, 0x51070b65, 0xd9014b79,
     0xdb30f394, 0x34927965, 0xa2bd257a, 0x43ff9ead,
     0x0d42de3e, 0x666db0a6, 0xf528346b, 0x61d5b111,
     0x44f7f261, 0x4631d83d, 0x97d97de7, 0x53ba5a70,
     0x2add9a58, 0x30e8f4dd, 0x3570a55a, 0xc1c4c366,
     0x2f01b122, 0x9e2d8798, 0xd6bedad4, 0xdea9996b,
     0x6bbb9ae0, 0x6a51b70d, 0x26bc6f2a, 0x71605663,
     0x39c2e0dd, 0xb21d4323, 0x37cb8779, 0xdea3ed23,
     0xbedb9ccd, 0x5cf41a48, 0x677cc8c3, 0x75ae530a,
     0x2bf96668, 0x815c22fc, 0x68e081ec, 0x93a1ea57,
     0xb0732952, 0x251eaa69, 0xa1df3cb9, 0x905e4228,
     0xba3711de, 0x105468c7, 0x6a73c631, 0xcdcc4eca,
     0xd68c0a0d, 0xb0c2a36e, 0x1acab2d4, 0xdd3f9c2c,
     0x8aaab9ed, 0xb0de72a7, 0x5d24d739, 0x41c7dbb9,
     0xeb3c965c, 0xd3a97d6c, 0x8a5e7e25, 0xaf776b26,
     0xa6d5f3d0, 0xfc255809, 0xa69ebd19, 0xf3553c50,
     0xd8050889, 0xbd6f09d6, 0xfff1b8f6, 0xd3012ebc,
     0x21f75b8a, 0xe366af3c, 0xa9ca87c6, 0xf6079433,
     0x901af123, 0xe3c5bd91, 0x5013a48c, 0x68edbb1b,
     0xf2dd72d4, 0x04e63636, 0x5fe79a5f, 0x9ce9f641,
     0x6d9fd29e, 0x0a9b2a15, 0xc5a47777, 0xecaf5dca,
     0xaaa2a512, 0x5b00c590, 0x56ce2ef4, 0xdf713df5,
     0xf0b47edf, 0x606264dd, 0x82c99168, 0xeeb08f31,
     0xfcb82c15, 0x6fd518c4, 0x291a19f6, 0x0a8894c6,
     0x50bad6bb, 0x0cc027be, 0x64577d10, 0x50f8c45b,
     0xac027a35, 0xfd284f1e, 0x17f928a1, 0x4c87ad40,
     0x3a0732e0, 0x9d5dcf61, 0x4a336e37, 0x453bd8f7,
     0xcac36c2c, 0x37d56535, 0x4b180e73, 0xf7c11846,
     0xf0a450b1, 0x987d6cf6, 0x3b841ef2, 0xfb151cd1,
     0xae742130, 0xd9a45634, 0xf92e4d28, 0x2d8d09c8,
     0xbe1ad760, 0x5c7019bf, 0xcab41bea, 0xa9860092,
     0x6010e594, 0x367b0cf2, 0x21f25be7, 0x60e5a16f,
     0xe92c81e9, 0x0f967388, 0x69b44206, 0xcbd445d3,
     0xc6a71765, 0xd7fbe09d, 0x7e84721a, 0x80d9aa8d,
     0xe0299797, 0xea7a5c32, 0xc27c79ca, 0xec12aaf0,
     0xd59a1b1f, 0x84fa7e0a, 0x4829cdfc, 0xb58bd4a4,
     0xc58d6710, 0xc7305abe, 0x49401064, 0x564ed1c6,
     0xd854b8fd, 0x391edb1d, 0x8a82c8db, 0x56532461,
     0x6f54e581, 0xa9576dbc, 0x3019972b, 0x55129a0f,
     0x58fd6bb8, 0x06e6d964, 0x32a064fb, 0xc8911900,
     0x543addbf, 0xc1318a72, 0xcd057d5e, 0x69a5fafc,
     0x9ed5748f, 0xe89e72ef, 0xbb628fd9, 0x284ef9e0,
     0x76e13ec0, 0x704488ba, 0xce4019c3, 0x12901e56,
     0x5500660b, 0xc8778df3, 0x89e0b440, 0xa811685b,
     0x5275cae5, 0x332d39ae, 0xd1cb84d3, 0x0ee487db,
     0x42fe2f90, 0x82f75e0f, 0x941fa0ad, 0xe150f7cf,
     0x35c46256, 0x60096460, 0x09b9d299, 0xb2e32893,
     0x43c32940, 0x55b597cd, 0x76bbf228, 0x5bba7ef1,
     0xd319adc6, 0xa4f5b47a, 0xcdf1b230, 0x315cfa6e,
     0xba9ebf57, 0x4ad7eec4, 0xe5c6c20d, 0xd35bb87a,
     0x74d6a720, 0xb0e871a0, 0x0da2581b, 0x187387fa,
     0x57cbf74d, 0x1fc3d237, 0x004cf328, 0xd4f8c2b3,
     0x9fd280a1, 0x431a1b96, 0xcbaaa6a4, 0x0d4fa3c5,
     0x09f9a4ac, 0xf17250c0, 0x5c9bb443, 0x8d4dedd5,
     0x0ede0197, 0xf3df39b6, 0xbc797e1f, 0xd46dee83,
     0x5c692925, 0xa3d40ed4, 0x8c30a4bf, 0x98ae4e0f,
     0xbca02060, 0xbc462faf, 0xb48c2802, 0x37a5f436,
     0xfd314041, 0x8145d6bc, 0x22d3cf61, 0x644b3427,
     0x54356307, 0x55003eb7, 0x9b62cdd2, 0xd01ed1e5,
     0x048632ea, 0x2d66126c, 0xa4be5041, 0x4876c124,
     0x8a849757, 0x655fe7b1, 0x0a4a7654, 0x14eab708,
     0x639e758b, 0xe24e5da9, 0x2a912bdd, 0xe71e7d1e,
     0xe3a11236, 0x7e32e718, 0x9d36a772, 0xec93154a,
     0x29808892, 0xb56694dd, 0xaabae838, 0xbed109ba,
     0x613f670f, 0x10dafe1f, 0x7d77ba21, 0x8191a560,
     0xa14bef32, 0x9c06b4d0, 0x69e449ab, 0xde365f3a,
     0x8c9a2e07, 0xcb16a1c7, 0xc544c771, 0x69a2e795,
     0xe2aef03b, 0x598698f5, 0xf5dda05f, 0x2e9f0481,
     0xf088297f, 0xdcfa623c, 0x988d215f, 0xdc15fb27,
     0x23cf3ee3, 0xa69ca51b, 0x6545bbb8, 0xbeb0cc8a,
     0xc86f74ed, 0x1965748c, 0xde3c4b78, 0xa6f07a0b,
     0x9f763ddd, 0x6d43b1d2, 0x0b1c2caa, 0x2176da60,
     0x1a8b88ab, 0x345914d5, 0xb5d0b299, 0x4b696c51,
     0xb2ee9fce, 0x5beaae89, 0xe6dea394, 0x4fab0bb8,
     0xb19217b4, 0x0170d527, 0x7bff1367, 0xd2e71f8c,
     0x82166f8f, 0x3a7ae788, 0x794a6b2c, 0xf62a9387,
     0x0397c734, 0x905aaa20, 0x20147dd2, 0x6437c803,
     0xe9659eec, 0x2dc8e38f, 0x980ef5db, 0xc1e16ffb,
     0x4bfeba5f, 0xd74d7684, 0x0d44ae42, 0x9ab5c997,
     0x7b5e688e, 0xfbdf6702, 0x1117c0a6, 0x974cd169,
     0xba25286f, 0xd72ba90b, 0xf09f4484, 0x6a5cda9b,
     0x830f6087, 0xb01fe00f, 0xdd308545, 0x09b272f5,
     0xb3f63fdf, 0x05c77abb, 0x567113de, 0x1761e139,
     0xa305f171, 0x869ca279, 0x67e0f9f4, 0x1e08bb6d,
     0x3ea154ab, 0x8e1c5e8b, 0xf5ba91d1, 0x2a5ef426,
     0x79227f0b, 0x4d041477, 0x10174138, 0xc2a021b8,
     0xa8ffed19, 0x2f9fcc95, 0x55805d9c, 0xaa7b55b4,
     0x8748dee1, 0xa4def120, 0xb145fbdf, 0xbbd2a8de,
     0x561ddb23, 0x1b648f7e, 0xe806a72a, 0x6e16fea5,
     0xebee7ffb, 0x245b4fb9, 0x4c2694df, 0x4714cef0,
     0xe7e8d22f, 0x40906de6, 0xd0de1c82, 0x1bb5db4f,
     0x8104c8ce, 0x85b4e1ac, 0xe0233b6e, 0x5dbe9a53,
     0x4ca7bfee, 0x2f297e2a, 0x6b16c0c7, 0x8e47b330,
     0x0674ef4d, 0x9cf1bdee, 0xf3a0b992, 0x755cfcf0,
     0x8150ee42, 0x716f6206, 0x49054121, 0x33bca914,
     0x516bcfbf, 0x26bf1cbe, 0x1222a2f2, 0xbc622889,
     0xdd49833e, 0x70a963ce, 0x4fbda453, 0x9dd759c3,
     0xb9806af6, 0x9edd1fc9, 0x8045fe42, 0x33053873,
     0x96bfdaa8, 0x997ae9e3, 0x4201430d, 0x4cdd2fe9,
     0xee9cfadb, 0x031b385d, 0xc02fc934, 0x596da69a,
     0x53e92dda, 0x503f8722, 0xbf84284a, 0x8b2b3e99,
     0xf27f7b33, 0xefda8e39, 0xcabaa17d, 0x8bafaf6d,
     0x373e6ceb, 0xcd4fa8a1, 0x48bcd88a, 0x5649ef63,
     0xce7272ab, 0xccca384d, 0x229e83c0, 0x233a6398,
     0x53aef9f2, 0x976eabad, 0x47d50cd9, 0x405ce8d7,
     0x0834345e, 0x3c8fcb7d, 0x4efcffd8, 0x5d768387,
     0x3cb7ab68, 0x4ab61a87, 0x9f42649e, 0x987256eb,
     0x643f1122, 0x91a2ec4c, 0x89b7dad6, 0x2810c328,
     0x77766207, 0x928a8285, 0xde5be5ad, 0xb53eba1b,
     0xb58a32bf, 0x6ba1fa3e, 0x3cdea2b0, 0xdad1e2b5,
     0x4e4fd125, 0xe03e5a04, 0xfcef7536, 0x9e3d3626,
     0x00c54517, 0x55bb518c, 0x71a9296d, 0xd3e47f9f,
     0xdf0d0c76, 0xa9ccd515};
