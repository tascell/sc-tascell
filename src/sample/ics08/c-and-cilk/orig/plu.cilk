/*
 *  Parallel LU Decomposition with Partial Pivoting
 *  lup.cilk
 *  Svetoslav Tzvetkov   <svetkins@supertech.lcs.mit.edu>
 *  6/1/98
 */
static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/plu.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <getoptions.h>
#include <assert.h>

typedef double Real;

#define BLOCK_SIZE 16
/* Must be a power of 2. */

/* We are using a block row major memory. */
typedef Real Block[BLOCK_SIZE][BLOCK_SIZE];
typedef Block *Matrix;

int *P;
Matrix A;
int N;
int K;				/* size in blocks = N/BLOCK_SIZE */

#define BL(i,j) (A[(i)*K+(j)])
#define A(i,j) ((BL((i)/BLOCK_SIZE, (j)/BLOCK_SIZE))[(i)%BLOCK_SIZE][(j)%BLOCK_SIZE])

/* find the maximal elelement (by absolute value) in the column
   from rows r1..r2 */
cilk int column_max(int c, int r1, int r2)
{
     int j = 0, k;
     int r;
     Real max = -1.0;
     Real *R;

     if (r2 - r1 < BLOCK_SIZE) {	/* can relax spawning here */
	  R = &A(r1, c);
	  for (r = r1; r <= r2;) {
	       if (fabs(*R) >= max) {
		    max = fabs(*R);
		    j = r;
	       }
	       r++;
	       R += BLOCK_SIZE;
	       if (r % BLOCK_SIZE == 0)
		    R += BLOCK_SIZE * BLOCK_SIZE * (K - 1);
	  }
     } else {
	  r = (r2 + r1) / 2;
	  j = spawn column_max(c, r1, r);
	  k = spawn column_max(c, r + 1, r2);
	  sync;
	  if (fabs(A(j, c)) < fabs(A(k, c)))
	       j = k;
     }

     return j;
}

/* update the lower rows after pivoting */
cilk void update_lower_rows(Real * Ri, int cb, int k, int t, int b)
{
     int l, m, j;
     Real a;
     Real *Rj;

     if (b - t < BLOCK_SIZE) {	/* can relax spawning here */
	  for (j = t; j <= b; j++) {
	       Rj = BL(j / BLOCK_SIZE, cb)[j % BLOCK_SIZE];
	       Rj[k] /= Ri[k];
	       a = Rj[k];
	       for (l = k + 1; l < BLOCK_SIZE; l++)
		    Rj[l] -= a * Ri[l];
	  }
     } else {
	  m = (t + b) / 2;
	  spawn update_lower_rows(Ri, cb, k, t, m);
	  spawn update_lower_rows(Ri, cb, k, m + 1, b);
     }
}

/* block LUP
   factors the block (r,c)-(r+m-1,c+m-1) and updates the
   lower rows: (r+m,c)-(N-1,c+m-1)
   performs permutations on rows r..N-1 if needed */
cilk void block_lup(int r, int c)
{
     Real *Ri, *Rj, *R;
     Real t;
     int i, j, k, l;

     assert(r % BLOCK_SIZE == 0);
     assert(c % BLOCK_SIZE == 0);

     Ri = (Real *) BL(r / BLOCK_SIZE, c / BLOCK_SIZE);
     for (k = 0; k < BLOCK_SIZE; k++) {
	  i = r + k;

	  /* find maximum element in column */
	  j = spawn column_max(c + k, i, N - 1);
	  sync;

	  /* swap rows if necessary */
	  if (i != j) {
	       l = P[i];
	       P[i] = P[j];
	       P[j] = l;
	       R = (Real *) BL(r / BLOCK_SIZE, 0) + k * BLOCK_SIZE;
	       Rj =
		   (Real *) BL(j / BLOCK_SIZE,
			       0) + (j % BLOCK_SIZE) * BLOCK_SIZE;
	       for (l = 0; l < N;) {
		    t = *R;
		    *R = *Rj;
		    *Rj = t;
		    l++;
		    R++;
		    Rj++;
		    if (l % BLOCK_SIZE == 0) {
			 R += (BLOCK_SIZE - 1) * BLOCK_SIZE;
			 Rj += (BLOCK_SIZE - 1) * BLOCK_SIZE;
		    }
	       }
	  }

	  /* update lower rows */
	  spawn update_lower_rows(Ri, c / BLOCK_SIZE, k, i + 1, N - 1);
	  sync;
	  Ri += BLOCK_SIZE;
     }
}

/* block schur's complement
   computes A-=B*C where
   A is (ar,ac)-(ar+m-1,ac+m-1)
   B is (br,bc)-(br+m-1,bc+m-1)
   C is (cr,cc)-(cr+m-1,cc+m-1) */
cilk void block_schur(int ar, int ac, int br, int bc, int cr, int cc)
{
     int i, j, k;
     Real a;
     Real *RA, *R, *RB, *RC;

     assert(ar % BLOCK_SIZE == 0);
     assert(ac % BLOCK_SIZE == 0);
     assert(br % BLOCK_SIZE == 0);
     assert(bc % BLOCK_SIZE == 0);
     assert(cr % BLOCK_SIZE == 0);
     assert(cc % BLOCK_SIZE == 0);

     RB = (Real *) BL(br / BLOCK_SIZE, bc / BLOCK_SIZE);
     RA = (Real *) BL(ar / BLOCK_SIZE, ac / BLOCK_SIZE);
     for (i = 0; i < BLOCK_SIZE; i++) {
	  RC = (Real *) BL(cr / BLOCK_SIZE, cc / BLOCK_SIZE);
	  for (k = 0; k < BLOCK_SIZE; k++) {
	       R = RA;
	       a = *RB;
	       for (j = 0; j < BLOCK_SIZE; j++) {
		    *R -= *RC * a;
		    R++;
		    RC++;
	       }
	       RB++;
	  }
	  RA += BLOCK_SIZE;
     }
}

/* compute schur's complement: A-=B*C */
cilk void schur(int ar, int ac, int br, int bc, int cr, int cc, int n,
		int m)
{
     if (m == BLOCK_SIZE) {
	  int s;
	  for (s = 0; s < n; s += m)
	       spawn block_schur(ar + s, ac, br + s, bc, cr, cc);
     } else {
	  m /= 2;
	  n /= 2;

	  spawn schur(ar, ac, br, bc, cr, cc, n, m);
	  spawn schur(ar, ac + m, br, bc + m, cr + m, cc + m, n, m);
	  spawn schur(ar + n, ac, br + n, bc + m, cr + m, cc, n, m);
	  spawn schur(ar + n, ac + m, br + n, bc, cr, cc + m, n, m);
	  sync;

	  spawn schur(ar, ac, br, bc + m, cr + m, cc, n, m);
	  spawn schur(ar, ac + m, br, bc, cr, cc + m, n, m);
	  spawn schur(ar + n, ac, br + n, bc, cr, cc, n, m);
	  spawn schur(ar + n, ac + m, br + n, bc + m, cr + m, cc + m, n,
		      m);
     }
}

/* solve: L*B=A (for B) */
cilk void lower_solve(int lr, int lc, int ar, int ac, int m);

void block_lower_solve(int lr, int lc, int ar, int ac)
{
     int i, j, k;
     Real a;
     Real *RL, *Ri, *R, *Rk;

     assert(lr % BLOCK_SIZE == 0);
     assert(lc % BLOCK_SIZE == 0);
     assert(ar % BLOCK_SIZE == 0);
     assert(ac % BLOCK_SIZE == 0);

     RL = (Real *) BL(lr / BLOCK_SIZE, lc / BLOCK_SIZE);
     Ri = (Real *) BL(ar / BLOCK_SIZE, ac / BLOCK_SIZE);
     for (i = 1; i < BLOCK_SIZE; i++) {
	  RL += BLOCK_SIZE;
	  Ri += BLOCK_SIZE;
	  Rk = (Real *) BL(ar / BLOCK_SIZE, ac / BLOCK_SIZE);
	  for (k = 0; k < i; k++) {
	       R = Ri;
	       a = RL[k];
	       for (j = 0; j < BLOCK_SIZE; j++) {
		    *R -= *Rk * a;
		    R++;
		    Rk++;
	       }
	  }
     }
}

/* solve [L00  0 ] * [X0] = [A0]
         [L10 L11]   [X1]   [A1]  */
cilk void lower_solve_aux(int lr, int lc, int ar, int ac, int m)
{
     spawn lower_solve(lr, lc, ar, ac, m);
     sync;

     spawn schur(ar + m, ac, lr + m, lc, ar, ac, m, m);
     sync;

     spawn lower_solve(lr + m, lc + m, ar + m, ac, m);
}

/* solve [L00  0 ] * [X00 X01] = [A00 A01]
         [L10 L11]   [X10 X11]   [A10 A11] */
cilk void lower_solve(int lr, int lc, int ar, int ac, int m)
{
     if (m == BLOCK_SIZE) {
	  block_lower_solve(lr, lc, ar, ac);
     } else {
	  m /= 2;
	  spawn lower_solve_aux(lr, lc, ar, ac, m);
	  spawn lower_solve_aux(lr, lc, ar, ac + m, m);
     }
}

/* solve a LUP subproblem of size nxm */
cilk void lup(int r, int c, int n, int m)
{
     if (m == BLOCK_SIZE) {
	  spawn block_lup(r, c);
     } else {

	  m /= 2;

	  spawn lup(r, c, n, m);
	  sync;

	  spawn lower_solve(r, c, r, c + m, m);
	  sync;

	  n -= m;

	  spawn schur(r + m, c + m, r + m, c, r, c + m, n, m);
	  sync;

	  spawn lup(r + m, c + m, n, m);
     }
}

/* the entry point: factors A */
cilk void LUP(void)
{
     int i;
     for (i = 0; i < N; i++)
	  P[i] = i;
     spawn lup(0, 0, N, N);
}

/* allocate and initialize memory, etc. */
void init(int n)
{
     int i;

     /* complement n to a power of 2 */
     for (N = BLOCK_SIZE; N < n; N *= 2);
     K = N / BLOCK_SIZE;

     P = malloc(sizeof(int) * N);
     A = malloc(sizeof(Real) * N * N);
     assert(P != NULL);
     assert(A != NULL);

     /* initialize the permutation vector */
     for (i = 0; i < N; i++)
	  P[i] = i;
}

/* generate a random matrix */
void randomize_A(void)
{
     int i, j;
     for (i = 0; i < N; i++)
	  for (j = 0; j < N; j++)
	       A(i, j) = ((double)rand()) / (double)RAND_MAX;
}

/* read a matrix from file. the format is:
   n
   A(0,0)   ... A(0,n-1)
   .
   .
   A(n-1,0) ... A(n-1,n-1) */
void read_matrix(char *filename)
{
     FILE *fp;
     int i, j;
     int n;

     fp = fopen(filename, "rt");
     assert(fp != NULL);

     fscanf(fp, "%d", &n);
     init(n);

     for (i = 0; i < n; i++) {
	  for (j = 0; j < n; j++)
	       fscanf(fp, "%lf", &A(i, j));
	  for (; j < N; j++)
	       A(i, j) = 0.0;
     }
     for (; i < N; i++) {
	  for (j = 0; j < i; j++)
	       A(i, j) = 0.0;
	  A(i, i) = 1.0;
	  for (j = i + 1; j < N; j++)
	       A(i, j) = 0.0;
     }

     fclose(fp);
}

int usage(void)
{
     printf("\nUsage: plu <options>\n\n");
     printf("Options:\n");
     printf
	 ("  -n N : Decompose NxN matrix, where N is at least 16 and power of 2.\n");

     printf("Default: lup -n %d\n\n", 1024);

     return 1;
}

char *specifiers[] = { "-n", "-benchmark", "-h", 0 };
int opt_types[] = { INTARG, BENCHMARK, BOOLARG, 0 };

/* main */
cilk main(int argc, char **argv)
{
     int n, benchmark, help;
     Cilk_time tm_begin, tm_elapsed;
     Cilk_time wk_begin, wk_elapsed;
     Cilk_time cp_begin, cp_elapsed;

     n = 1024;

     /* Parse arguments. */
     get_options(argc, argv, specifiers, opt_types, &n, &benchmark, &help);

     if (help)
	  return usage();

     if (benchmark) {
	  switch (benchmark) {
	      case 1:		/* short benchmark options -- a little work */
	       n = 16;
	       break;
	      case 2:		/* standard benchmark options */
	       n = 2048;
	       break;
	      case 3:		/* long benchmark options -- a lot of work */
	       n = 2048;
	       break;
	  }
     }

     init(n);
     randomize_A();

     sync;
     cp_begin = Cilk_user_critical_path;
     wk_begin = Cilk_user_work;
     tm_begin = Cilk_get_wall_time();

     spawn LUP();
     sync;

     /* Timing. "Stop" timers */
     tm_elapsed = Cilk_get_wall_time() - tm_begin;
     wk_elapsed = Cilk_user_work - wk_begin;
     cp_elapsed = Cilk_user_critical_path - cp_begin;

     printf("\nCilk Example: plu\n");
     printf("              running on %d processor%s\n\n",
	    Cilk_active_size, Cilk_active_size > 1 ? "s" : "");
     printf("Options: (n x n matrix) n = %d\n\n", n);
     printf("Running time  = %4f s\n", Cilk_wall_time_to_sec(tm_elapsed));
     printf("Work          = %4f s\n", Cilk_time_to_sec(wk_elapsed));
     printf("Critical path = %4f s\n\n", Cilk_time_to_sec(cp_elapsed));

     /* Free matrix. */
     free(P);
     free(A);

     return 0;
}
