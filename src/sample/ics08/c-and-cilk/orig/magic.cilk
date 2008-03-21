/*
 * This program computes the number of N x N magic squares,
 * where N is given as a command line argument.
 *
 * Matteo Frigo, Sep '96
 */
static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/magic.cilk $ $LastChangedBy: bradley $ $Rev: 517 $ $Date: 2004-09-21 16:43:48 -0400 (Tue, 21 Sep 2004) $";
/*
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 1996,2000 Matteo Frigo
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
#include <getoptions.h>

#include "config.h"

#if HAVE_ALLOCA_H
#include <alloca.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

static int n;			/* the size of the grid */
static int nsquare;		/* n * n */
static int magic_sum;		/* the sum of rows, columns etc. */
/* the magic sum is always n * (n*n + 1) / 2 */

static int print_solution = 0;	/* flag; print each solution if true */

/* 
 * definition of a (nondeterministic) virtual machine that
 * solves the problem.
 */

typedef int INSTR;

/* instructions of the nondeterministic machine */
enum {
     I_GUESS,			/* 
				 * non-deterministically guess a number 
				 * (for cell arg) 
				 */
     I_FIRST_GUESS,		/* 
				 * guess a number, but only between
				 * 1 and n/2.
				 */
     I_DEDUCE,			/* 
				 * deduce as a linear combination of
				 * existing cells
				 */
     I_GUESS_MOD,		/* 
				 * deduce modulo some integer; that is,
				 * non-deterministically try all
				 * numbers congruent to the solution
				 * of the equation, modulo something
				 */
     I_PRUNE_ROW,		/* 
				 * do some test to ensure that the
				 * values in row arg1 are not 
				 * incompatible (too big, too low) 
				 */
     I_PRUNE_COL,		/* same for column */
     I_PRUNE_DIAG0,		/* same for diagonal */
     I_PRUNE_DIAG1,		/* same for anti-diagonal */
     I_ACCEPT			/* valid solution found */
};

/*
 * done; print the solution.
 */
void done(int *grid)
{
     int i, j;

     /* print the solution */
     if (print_solution) {
	  for (i = 0; i < n; ++i) {
	       for (j = 0; j < n; ++j) {
		    printf("%4d", grid[i * n + j]);
	       }
	       printf("\n");
	  }
	  printf("\n");
     }
}

/*
 * Compilation of the nondeterministic program.
 * 
 * First of all, we must generate a linear system Ax = b
 * containing all the constraints on rows, columns, etc.
 * Then, the system is reduced to upper-triangular form
 * by Gauss-Jordan elimination.  This leaves some free variabled
 * that the program must guess.  The order of the guesses is
 * chosen so to maximize the number of deductions.
 */
void compile_program(INSTR * pc)
{
     /* A has 2 * n + 2 equations and nsquare columns (variables) */
     int *A = alloca(nsquare * (2 * n + 2) * sizeof(int));
     int *b = alloca((2 * n + 2) * sizeof(int));
     int neq;
     void generate_program(INSTR * pc, int *A, int *b, int r, int c);
     void gauss_jordanize(int *A, int *b, int r, int c);
     int generate_equations(int *A, int *b);

     neq = generate_equations(A, b);

     gauss_jordanize(A, b, neq, nsquare);

     generate_program(pc, A, b, neq, nsquare);
}

/*
 * generate the set of contraints (linear equations,
 * stored in the matrix A).
 */
int generate_equations(int *A, int *b)
{
     int i, j, neq;

     neq = 0;

     /* make sure A is empty */
     for (i = 0; i < nsquare * (2 * n + 2); ++i)
	  A[i] = 0;

     /* put equation for the main diagonal */
     for (i = 0; i < n; ++i)
	  A[neq * nsquare + (i * n + i)] = 1;
     b[neq] = magic_sum;
     ++neq;

     /* put equation for the anti-diagonal */
     for (i = 0; i < n; ++i)
	  A[neq * nsquare + (i * n + n - 1 - i)] = 1;
     b[neq] = magic_sum;
     ++neq;

     /* put equations for the rows */
     for (i = 0; i < n; ++i) {
	  for (j = 0; j < n; ++j)
	       A[neq * nsquare + (i * n + j)] = 1;
	  b[neq] = magic_sum;
	  ++neq;
     }

     /* put equations for the columns */
     for (j = 0; j < n; ++j) {
	  for (i = 0; i < n; ++i)
	       A[neq * nsquare + (i * n + j)] = 1;
	  b[neq] = magic_sum;
	  ++neq;
     }

     return neq;
}

int gcd(int i, int j)
{
     int r;
     if (i == 0)
	  return j;
     if (j == 0)
	  return i;

     if (i < 0)
	  i = -i;

     if (j < 0)
	  j = -j;

     if (i < j)
	  r = i, i = j, j = r;

     r = i % j;
     while (r) {
	  i = j;
	  j = r;
	  r = i % j;
     }

     return j;
}

void pivot(int *A, int *b, int i, int v, int r, int c)
{
     int mi, mk, ic;
     int j, k, g;

     ic = i * c;
     mi = A[ic + v];

     for (k = 0; k < r; ++k)
	  if (i != k && (mk = A[k * c + v]) != 0) {
	       int kc = k * c;
	       for (j = 0; j < c; ++j)
		    A[kc + j] =
			mi * A[kc + j] - mk * A[ic + j];
	       b[k] = mi * b[k] - mk * b[i];

	       /* 
	        * now compute the gcd of all the elements and
	        * normalize
	        */
	       g = b[k];
	       for (j = 0; g != 1 && j < c; ++j)
		    g = gcd(g, A[kc + j]);

	       if (g != 1 && g != 0) {
		    b[k] /= g;
		    for (j = 0; j < c; ++j)
			 A[kc + j] /= g;
	       }
	  }
}

/* 
 * perform Gauss-Jordan elimination on the linear system Ax = b.
 * Notice that the system is singular, thus we cannot solve it
 * by Gaussian elimination + backsubstitution, and Gauss Jordan
 * is necessary.
 */
void gauss_jordanize(int *A, int *b, int r, int c)
{
     int i, j, k, v;
     int t;

     for (i = v = 0; i < r; ++i, ++v) {
	  int ic = i * c;

	  /* 
	   * find a pivot row k; if found, exchange row i with
	   * row j and eliminate the variable v from other equations;
	   * otherwise try another variable.
	   */
	next_variable:
	  for (k = i; k < r; ++k)
	       if (A[k * c + v])
		    break;

	  if (k == r) {
	       if (++v == c)
		    break;
	       goto next_variable;
	  }
	  /* 
	   * now we have a pivot element A[k, v].
	   * if k != i, echange rows i and k
	   */
	  if (k != i) {
	       /* 
	        * start at column v, since elemnts before that
	        * are null.
	        */
	       for (j = v; j < c; ++j) {
		    t = A[ic + j];
		    A[ic + j] = A[k * c + j];
		    A[k * c + j] = t;
	       }

	       t = b[i];
	       b[i] = b[k];
	       b[k] = t;
	  }
	  /* 
	   * now eliminate this variable from all other equations,
	   * using Gauss-Jordan
	   */
	  pivot(A, b, i, v, r, c);
     }
}

INSTR *emit_deduce(INSTR * pc, int var, int row, int *thisrow, int b, int c)
{
     int j;

     *pc++ = I_DEDUCE;
     *pc++ = var;
     *pc++ = b;
     *pc++ = thisrow[var];

     for (j = 0; j < c; ++j)
	  if (j != var && thisrow[j]) {
	       *pc++ = j;
	       *pc++ = -thisrow[j];
	  }
     *pc++ = -1;

     return pc;
}

INSTR *emit_guess(INSTR * pc, int var)
{
     *pc++ = I_GUESS;
     *pc++ = var;
     return pc;
}

INSTR *emit_guess_mod(INSTR * pc, int var, int mod,
		      int row, int *thisrow, int b, int c)
{
     int j;

     *pc++ = I_GUESS_MOD;
     *pc++ = var;
     *pc++ = mod;
     *pc++ = b;
     *pc++ = thisrow[var];

     for (j = 0; j < c; ++j)
	  if (j != var && thisrow[j]) {
	       *pc++ = j;
	       *pc++ = -thisrow[j];
	  }
     *pc++ = -1;

     return pc;
}

INSTR *emit_first_guess(INSTR * pc, int var)
{
     *pc++ = I_FIRST_GUESS;
     *pc++ = var;
     return pc;
}

INSTR *emit_accept(INSTR * pc)
{
     *pc++ = I_ACCEPT;
     return pc;
}

#define PRUNE_THRESHOLD (2)

INSTR *emit_prune_row(INSTR * pc, int row, int *known)
{
     /* 
      * heuristics: check only if there are enough variables 
      * in this row 
      */
     int j, s;
     for (s = 0, j = row * n; j < nsquare; j++)
	  s += known[j];

     if (s >= PRUNE_THRESHOLD) {
	  *pc++ = I_PRUNE_ROW;
	  *pc++ = row;
     }
     return pc;
}

INSTR *emit_prune_col(INSTR * pc, int col, int *known)
{
     int j, s;
     for (s = 0, j = col; j < nsquare; j += n)
	  s += known[j];

     if (s >= PRUNE_THRESHOLD) {
	  *pc++ = I_PRUNE_COL;
	  *pc++ = col;
     }
     return pc;
}

INSTR *emit_prune_diag0(INSTR * pc, int *known)
{
     int j, s;
     for (s = 0, j = 0; j < nsquare; j += n + 1)
	  s += known[j];

     if (s >= PRUNE_THRESHOLD) {
	  *pc++ = I_PRUNE_DIAG0;
     }
     return pc;
}

INSTR *emit_prune_diag1(INSTR * pc, int *known)
{
     int j, s;
     for (s = 0, j = n - 1; j < nsquare; j += n - 1)
	  s += known[j];

     if (s >= PRUNE_THRESHOLD) {
	  *pc++ = I_PRUNE_DIAG1;
     }
     return pc;
}

static long global_min_so_far = 0;

#define COST_FUNCTION(depth, var, m) \
          ((depth) * (depth) * (depth) * (depth) / (m) )

#define NMODULOS 5

/* possible results of minimize */
enum {
     DEDUCE,
     BLIND_GUESS,
     GUESS_MODULO
};

/*
 * try to find an optimal sequence of moves, that minimizes the cost
 * function.  The cost of a move is
 *
 * 0 for a deduction
 * COST_FUNCTION(ndepth, variable, m)  for a guess modulo m
 *       (m is 1 for a blind guess)
 * 
 */
long minimize(int *A, int r, int c, int *known, int depth, int *where,
	      int *eq, int *mod, int *result, int score_so_far)
{
     int *nknown = alloca(c * sizeof(int));

     /* # of unknowns in this equation */
     int *unknowns = alloca(r * sizeof(int));
     int *unknowns_modulo = alloca(NMODULOS * r * sizeof(int));

     int i, j, ic, v, m;
     int min, mineq;
     long score, cost;
     int dummy1, dummy2, dummy3, dummy4;

     /* To get rid of compiler warnings */
     v = 0;
     mineq = 0;

     if (depth < 0) {
	printf("BUG in minimize!\n");
	exit(1);
	/*  Cilk_die("BUG in minimize!\n"); */
     }
     if (depth == 0) {
	  if (score_so_far < global_min_so_far)
	       global_min_so_far = score_so_far;
	  return score_so_far;
     }
     if (score_so_far >= global_min_so_far)
	  return score_so_far;	/* pruning */

     for (i = 0; i < r; ++i)
	  unknowns[i] = 0;
     for (i = 0; i < r * NMODULOS; ++i)
	  unknowns_modulo[i] = 0;

     /* 
      * for each row, compute the number of unknowns,
      * and the number of unknowns for the equation modulo
      * something.
      */
     for (i = 0; i < r; ++i) {
	  ic = i * c;
	  for (j = 0; j < c; ++j) {
	       if (A[ic + j] && !known[j])
		    ++unknowns[i];
	       for (m = 2; m < NMODULOS; ++m)
		    if ((A[ic + j] % m) && !known[j])
			 ++unknowns_modulo[i * NMODULOS + m];
	  }
     }

     min = 100000;
     for (i = 0; i < r; ++i)
	  if (unknowns[i] > 0 && unknowns[i] < min) {
	       min = unknowns[i];
	       mineq = i;
	  }
     /* easy case: there is a variable that can be deduced */
     if (min == 1) {
	  int ndepth = depth;
	  memcpy(nknown, known, c * sizeof(int));
	  ic = mineq * c;
	  for (j = 0; j < c; ++j)
	       if (A[ic + j] && !nknown[j]) {
		    nknown[j] = 1;
		    v = j;
		    ndepth--;
	       }
	  if (ndepth == depth) {
	    printf("BUG 1\n");
	    exit(1);
	    /*Cilk_die("BUG 1\n");*/
	  }
	  score = minimize(A, r, c, nknown, ndepth,
			   &dummy1, &dummy2, &dummy3, &dummy4,
			   score_so_far);
	  *where = v;
	  *eq = mineq;
	  *result = DEDUCE;
	  return score;
     }
     /* 
      * try to look for a deduction modulo something, starting with the
      * greatest modulos
      */
     min = 100000000;
     *result = BLIND_GUESS;

     for (m = NMODULOS - 1; m > 1; --m)
	  for (i = 0; i < r; ++i)
	       if (unknowns_modulo[i * NMODULOS + m] == 1) {
		    int ndepth = depth;
		    memcpy(nknown, known, c * sizeof(int));
		    ic = i * c;
		    for (j = 0; j < c; ++j)
			 if (A[ic + j] % m && !nknown[j]) {
			      nknown[j] = 1;
			      v = j;
			      ndepth--;
			 }
		    if (ndepth != depth - 1) {
		      printf("BUG 2\n");
		      exit(1);
		      /*Cilk_die("BUG 2\n");*/
		    }
		    cost = COST_FUNCTION(ndepth, j, m);
		    score = minimize(A, r, c, nknown, ndepth,
				     &dummy1, &dummy2, &dummy3, &dummy4,
				     score_so_far + cost);

		    if (score < min) {
			 *eq = i;
			 *mod = m;
			 *result = GUESS_MODULO;
			 min = score;
			 *where = v;
		    }
	       }
     if (*result == GUESS_MODULO)
	  return min;

     /* 
      * else, we must guess everything. Try all possibilities
      */
     min = 100000000;

     for (m = NMODULOS - 1; m > 1; --m) {
	  for (i = 0; i < r; ++i)
	       if (unknowns_modulo[i * NMODULOS + m] &&
		   unknowns_modulo[i * NMODULOS + m] != unknowns[i]) {
		    /* if this equation is useful at all */
		    int ndepth = depth;
		    ic = i * c;
		    memcpy(nknown, known, c * sizeof(int));
		    cost = 0;
		    for (j = 0; j < c; ++j)
			 if (A[ic + j] % m && !nknown[j]) {
			      nknown[j] = 1;
			      v = j;
			      ndepth--;
			      cost += COST_FUNCTION(ndepth, j, 1);
			 }
		    if (ndepth == depth) {
		      printf("BUG 3\n");
		      exit(1);
		      /*Cilk_die("BUG 3\n");*/
		    }
		    /* the last cost was actually a guess_mod. Adjust */
		    cost -= COST_FUNCTION(ndepth, j, 1);
		    cost += COST_FUNCTION(ndepth, j, m);
		    score = minimize(A, r, c, nknown, ndepth,
				     &dummy1, &dummy2, &dummy3, &dummy4,
				     score_so_far + cost);
		    if (score < min) {
			 min = score;
			 *where = v;
		    }
	       }
     }

     for (i = 0; i < r; ++i)
	  if (unknowns[i]) {
	       /* if this equation is useful at all */
	       int ndepth = depth;
	       ic = i * c;
	       memcpy(nknown, known, c * sizeof(int));
	       cost = 0;
	       for (j = 0; j < c; ++j)
		    if (A[ic + j] && !nknown[j]) {
			 nknown[j] = 1;
			 v = j;
			 ndepth--;
			 cost += COST_FUNCTION(ndepth, j, 1);
		    }
	       if (ndepth == depth) {
		 printf("BUG 4\n");
		 exit(1);
		 /*Cilk_die("BUG 4\n");*/
	       }
	       /* 
	        * the last one was actually a deduce, thus don't
	        * pay for it
	        */
	       cost -= COST_FUNCTION(ndepth, j, 1);
	       score = minimize(A, r, c, nknown, ndepth,
				&dummy1, &dummy2, &dummy3, &dummy4,
				score_so_far + cost);
	       if (score < min) {
		    min = score;
		    *where = v;
	       }
	  }
     *result = BLIND_GUESS;
     return min;
}

/*
 * generate a non-deterministic program that tries to
 * deduce things from the equations as early as possible
 */
void generate_program(INSTR * pc, int *A, int *b, int r, int c)
{
     int nvar = c;
     int *known = alloca(c * sizeof(int));
     int i, j, current_var;
     int last_guessed = -1;
     /*     long score; */
     int first_guess = 1;
     int result, mod;

     for (j = 0; j < c; ++j)
	  known[j] = 0;

     for (nvar = c; nvar; nvar--) {
	  global_min_so_far = 1000000000;
	  /* score = */
	  minimize(A, r, c, known, nvar, &current_var, &i,
		   &mod, &result, 0);
	  
	  switch (result) {
	      case DEDUCE:
		   /* we can deduce the variable */
		   pc = emit_deduce(pc, current_var, i, A + i * c,
				    b[i], c);
		   last_guessed = -1;
		   break;

	      case BLIND_GUESS:
		   /* we have to guess it */
		   if (last_guessed != -1) {
			pc = emit_prune_col(pc, last_guessed % n, known);
			pc = emit_prune_row(pc, last_guessed / n, known);
			if (last_guessed % (n + 1) == 0)
			     pc = emit_prune_diag0(pc, known);
			if (last_guessed % (n - 1) == 0)
			     pc = emit_prune_diag1(pc, known);

		   }
		   if (first_guess)
			pc = emit_first_guess(pc, current_var);
		   else
			pc = emit_guess(pc, current_var);
		   first_guess = 0;
		   last_guessed = current_var;
		   break;

	      case GUESS_MODULO:
		   if (last_guessed != -1) {
			pc = emit_prune_col(pc, last_guessed % n, known);
			pc = emit_prune_row(pc, last_guessed / n, known);
			if (last_guessed % (n + 1) == 0)
			     pc = emit_prune_diag0(pc, known);
			if (last_guessed % (n - 1) == 0)
			     pc = emit_prune_diag1(pc, known);

		   }
		   pc = emit_guess_mod(pc, current_var, mod, i,
				       A + i * c, b[i], c);
		   first_guess = 0;
		   last_guessed = current_var;

		   break;
	  }

	  known[current_var] = 1;
     }

     pc = emit_accept(pc);
}

/*
 * executor of the nondeterministic program 
 */
cilk long execute(int *grid, int *used, INSTR * pc, int m)
{
     int r, c;			/* row and column */
     int q;			/* cell */
     int i;			/* generic index */
     int *ngrid, *nused;
     long count;
     int s, t;			/* sum */
     int *p;
     int z, mod;

     count = 0;
   again:
     switch (*pc++) {
	 case I_GUESS:
	      q = *pc++;
	      for (i = 0; i < nsquare; ++i) {
		   if (!used[i]) {
			ngrid = Cilk_alloca(nsquare * sizeof(int));
			nused = Cilk_alloca(nsquare * sizeof(int));
			memcpy(ngrid, grid, nsquare * sizeof(int));
			memcpy(nused, used, nsquare * sizeof(int));
			nused[i] = 1;
			ngrid[q] = i + 1;
			count += spawn execute(ngrid, nused, pc, m);
		   }
	      }
	      sync;
	      break;

	 case I_FIRST_GUESS:
	      q = *pc++;

	      for (i = 0; i < nsquare / 2; ++i) {
		   if (!used[i]) {
			ngrid = Cilk_alloca(nsquare * sizeof(int));
			memcpy(ngrid, grid, nsquare * sizeof(int));
			nused = Cilk_alloca(nsquare * sizeof(int));
			memcpy(nused, used, nsquare * sizeof(int));
			nused[i] = 1;
			ngrid[q] = i + 1;

			count += spawn execute(ngrid, nused, pc,
					       (i + i == nsquare - 1 ?
						1 : 2));
		   }
	      }
	      sync;
	      break;

	 case I_DEDUCE:
	      q = *pc++;
	      s = *pc++;
	      i = *pc++;	/* 
				 * i is the divisor, i.e. coeff of this
				 * variable in the equation
				 */
	      while ((t = *pc++) != -1) {
		   /* t is the variable, z the coefficient */
		   z = *pc++;
		   s += z * grid[t];
	      }

	      /* if i does not divide s, wrong */
	      t = s / i;
	      if (i * t != s)
		   break;	/* reject */

	      /* now, t is the value this cell should have */
	      if (t < 1 || t > nsquare || used[t - 1])
		   break;	/* reject */

	      grid[q] = t;
	      used[t - 1] = 1;
	      goto again;

	 case I_GUESS_MOD:
	      q = *pc++;
	      mod = *pc++;
	      s = *pc++;
	      i = *pc++;	/* 
				 * i is the divisor, i.e. coeff of this
				 * variable in the equation
				 */
	      while ((t = *pc++) != -1) {
		   /* t is the variable, z the coefficient */
		   z = *pc++;
		   s += z * grid[t];
	      }

	      /* if i does not divide s, wrong */
	      t = s / i;
	      if (i * t != s)
		   break;	/* reject */

	      t = (t + nsquare * mod - 1) % mod;

	      for (i = t; i < nsquare; i += mod) {
		   if (!used[i]) {
			ngrid = Cilk_alloca(nsquare * sizeof(int));
			nused = Cilk_alloca(nsquare * sizeof(int));
			memcpy(ngrid, grid, nsquare * sizeof(int));
			memcpy(nused, used, nsquare * sizeof(int));
			nused[i] = 1;
			ngrid[q] = i + 1;
			count += spawn execute(ngrid, nused, pc, m);
		   }
	      }

	      sync;
	      break;

	 case I_PRUNE_ROW:
	      s = 0;
	      z = 0;		/* number of empty cells */
	      r = *pc++;
	      p = grid + n * r;
	      for (i = 0; i < n; ++i, p++) {
		   s += (t = *p);
		   if (!t)
			z++;
	      }

	      goto continue_pruning;

	 case I_PRUNE_COL:
	      s = 0;
	      z = 0;		/* number of empty cells */
	      c = *pc++;
	      p = grid + c;
	      for (i = 0; i < n; ++i, p += n) {
		   s += (t = *p);
		   if (!t)
			z++;
	      }
	      goto continue_pruning;

	 case I_PRUNE_DIAG0:
	      s = 0;
	      z = 0;		/* number of empty cells */
	      p = grid;
	      for (i = 0; i < n; ++i, p += n + 1) {
		   s += (t = *p);
		   if (!t)
			z++;
	      }
	      goto continue_pruning;

	 case I_PRUNE_DIAG1:
	      s = 0;
	      z = 0;		/* number of empty cells */
	      p = grid + n - 1;
	      for (i = 0; i < n; ++i, p += n - 1) {
		   s += (t = *p);
		   if (!t)
			z++;
	      }

	    continue_pruning:
	      /* 
	       * try to see what happens if we add the biggest numbers
	       * to s; if the sum is still below magic_sum, reject
	       */
	      t = s;
	      i = nsquare - 1;
	      while (z) {
		   if (!used[i]) {
			--z;
			t += i + 1;
		   }
		   --i;
	      }

	      if (t < magic_sum)
		   break;	/* and prune */

	      /* 
	       * try to see what happens if we add the smallest numbers
	       * to s; if the sum is still above magic_sum, reject
	       */
	      t = s;
	      i = 0;
	      while (z) {
		   if (!used[i]) {
			--z;
			t += i + 1;
		   }
		   ++i;
	      }

	      if (t > magic_sum)
		   break;	/* and prune */

	      goto again;	/* test passed */

	 case I_ACCEPT:
	      done(grid);
	      count = m;
	      break;
	 default:
	      Cilk_die("BUG: unrecognized instruction\n");
     }

     return count;
}

int usage(void)
{
     fprintf(stderr, "\nUsage: magic [<cilk-options>] [-n #] [-benchmark] [-h]\n\n");
     fprintf(stderr, "The program computes the number of magic squares of size n x n, in an\n");
     fprintf(stderr, "original way. In a magic square the sum along all horizontal,\n");
     fprintf(stderr, "vertical, and diagonal lines is the same. For example this is a magic\n");
     fprintf(stderr, "square:\n");
     fprintf(stderr, "           8 1 6\n");
     fprintf(stderr, "           3 5 7\n");
     fprintf(stderr, "           4 9 2\n\n");
     return -1;
}

INSTR program[1024];

char *specifiers[] =
{"-n", "-benchmark", "-h", 0};
int opt_types[] =
{INTARG, BENCHMARK, BOOLARG, 0};

cilk int main(int argc, char *argv[])
{
     int *grid;
     int *used;
     long count;
     int i, benchmark, help;
     Cilk_time tm_begin, tm_elapsed;
     Cilk_time wk_begin, wk_elapsed;
     Cilk_time cp_begin, cp_elapsed;

     /* standard benchmark options */
     n = 4;

     get_options(argc, argv, specifiers, opt_types, &n, &benchmark, &help);

     if (help)
	  return usage();

     if (benchmark) {
	  switch (benchmark) {
	      case 1:		/* short benchmark options -- a little work */
		   n = 2;
		   break;
	      case 2:		/* standard benchmark options */
		   n = 4;
		   break;
	      case 3:		/* long benchmark options -- a lot of work */
		   n = 5;
		   break;
	  }
     }
     if (n <= 0) {
	  fprintf(stderr, "error: n must be positive\n");
	  return 1;
     }
     nsquare = n * n;
     magic_sum = n * (nsquare + 1) / 2;

     grid = Cilk_alloca(nsquare * sizeof(int));
     used = Cilk_alloca(nsquare * sizeof(int));
     for (i = 0; i < nsquare; ++i)
	  used[i] = grid[i] = 0;

     /* Timing. "Start" timers */
     sync;
     cp_begin = Cilk_user_critical_path;
     wk_begin = Cilk_user_work;
     tm_begin = Cilk_get_wall_time();

     compile_program(program);
     count = spawn execute(grid, used, program, 1);
     sync;

     /* Timing. "Stop" timers */
     tm_elapsed = Cilk_get_wall_time() - tm_begin;
     wk_elapsed = Cilk_user_work - wk_begin;
     cp_elapsed = Cilk_user_critical_path - cp_begin;

     printf("\nCilk Example: magic\n");
     printf("	      running on %d processor%s\n\n", Cilk_active_size, Cilk_active_size > 1 ? "s" : "");
     printf("options: <n> = %d\n\n", n);
     printf("Result is %ld\n\n", count);
     printf("Running time  = %4f s\n", Cilk_wall_time_to_sec(tm_elapsed));
     printf("Work          = %4f s\n", Cilk_time_to_sec(wk_elapsed));
     printf("Critical path = %4f s\n\n", Cilk_time_to_sec(cp_elapsed));

     return 0;
}
