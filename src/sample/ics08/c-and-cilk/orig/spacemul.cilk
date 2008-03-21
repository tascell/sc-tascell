/*
 * Matrix Multiply: A dag-consistent shared memory example.
 */

/*
 * Copyright (c) 1996 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to use, copy, modify, and distribute the Software without
 * restriction, provided the Software, including any modified copies made
 * under this license, is not distributed for a fee, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the Massachusetts
 * Institute of Technology shall not be used in advertising or otherwise
 * to promote the sale, use or other dealings in this Software without
 * prior written authorization from the Massachusetts Institute of
 * Technology.
 *  
 */

static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/spacemul.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

#include <cilk-lib.cilkh>
#include <stdlib.h>
#include <stdio.h>
#include <getoptions.h>

/*
 * Define a block as a BLOCK_EDGE x BLOCK_EDGE array of doubles.
 */
#define BLOCK_EDGE 16
#define BLOCK_SIZE (BLOCK_EDGE*BLOCK_EDGE)
typedef double block[BLOCK_SIZE];

/* compute R = AB, where R,A,B are BLOCK_EDGE x BLOCK_EDGE matricies */
long long serialmul(block *A, block *B, block *R)
{
     int i, j;
     long long flops = 0LL;

     for (j = 0; j < 16; j += 2) {	/* 2 columns at a time */
	  double *bp = &((double *) B)[j];
	  for (i = 0; i < 16; i += 2) {		/* 2 rows at a time */
	       double *ap = &((double *) A)[i * 16];
	       double *rp = &((double *) R)[j + i * 16];
	       register double s0_0, s0_1;
	       register double s1_0, s1_1;
	       s0_0 = ap[0] * bp[0];
	       s0_1 = ap[0] * bp[1];
	       s1_0 = ap[16] * bp[0];
	       s1_1 = ap[16] * bp[1];
	       s0_0 += ap[1] * bp[16];
	       s0_1 += ap[1] * bp[17];
	       s1_0 += ap[17] * bp[16];
	       s1_1 += ap[17] * bp[17];
	       s0_0 += ap[2] * bp[32];
	       s0_1 += ap[2] * bp[33];
	       s1_0 += ap[18] * bp[32];
	       s1_1 += ap[18] * bp[33];
	       s0_0 += ap[3] * bp[48];
	       s0_1 += ap[3] * bp[49];
	       s1_0 += ap[19] * bp[48];
	       s1_1 += ap[19] * bp[49];
	       s0_0 += ap[4] * bp[64];
	       s0_1 += ap[4] * bp[65];
	       s1_0 += ap[20] * bp[64];
	       s1_1 += ap[20] * bp[65];
	       s0_0 += ap[5] * bp[80];
	       s0_1 += ap[5] * bp[81];
	       s1_0 += ap[21] * bp[80];
	       s1_1 += ap[21] * bp[81];
	       s0_0 += ap[6] * bp[96];
	       s0_1 += ap[6] * bp[97];
	       s1_0 += ap[22] * bp[96];
	       s1_1 += ap[22] * bp[97];
	       s0_0 += ap[7] * bp[112];
	       s0_1 += ap[7] * bp[113];
	       s1_0 += ap[23] * bp[112];
	       s1_1 += ap[23] * bp[113];
	       s0_0 += ap[8] * bp[128];
	       s0_1 += ap[8] * bp[129];
	       s1_0 += ap[24] * bp[128];
	       s1_1 += ap[24] * bp[129];
	       s0_0 += ap[9] * bp[144];
	       s0_1 += ap[9] * bp[145];
	       s1_0 += ap[25] * bp[144];
	       s1_1 += ap[25] * bp[145];
	       s0_0 += ap[10] * bp[160];
	       s0_1 += ap[10] * bp[161];
	       s1_0 += ap[26] * bp[160];
	       s1_1 += ap[26] * bp[161];
	       s0_0 += ap[11] * bp[176];
	       s0_1 += ap[11] * bp[177];
	       s1_0 += ap[27] * bp[176];
	       s1_1 += ap[27] * bp[177];
	       s0_0 += ap[12] * bp[192];
	       s0_1 += ap[12] * bp[193];
	       s1_0 += ap[28] * bp[192];
	       s1_1 += ap[28] * bp[193];
	       s0_0 += ap[13] * bp[208];
	       s0_1 += ap[13] * bp[209];
	       s1_0 += ap[29] * bp[208];
	       s1_1 += ap[29] * bp[209];
	       s0_0 += ap[14] * bp[224];
	       s0_1 += ap[14] * bp[225];
	       s1_0 += ap[30] * bp[224];
	       s1_1 += ap[30] * bp[225];
	       s0_0 += ap[15] * bp[240];
	       s0_1 += ap[15] * bp[241];
	       s1_0 += ap[31] * bp[240];
	       s1_1 += ap[31] * bp[241];
	       rp[0] = s0_0;
	       rp[1] = s0_1;
	       rp[16] = s1_0;
	       rp[17] = s1_1;
	       flops += 124;
	  }
     }

     return flops;
}

/* compute R += AB, where R,A,B are BLOCK_EDGE x BLOCK_EDGE matricies */
long long serialmul_add(block *A, block *B, block *R)
{
     int i, j;
     long long flops = 0LL;

     for (j = 0; j < 16; j += 2) {	/* 2 columns at a time */
	  double *bp = &((double *) B)[j];
	  for (i = 0; i < 16; i += 2) {		/* 2 rows at a time */
	       double *ap = &((double *) A)[i * 16];
	       double *rp = &((double *) R)[j + i * 16];
	       register double s0_0, s0_1;
	       register double s1_0, s1_1;
	       s0_0 = rp[0];
	       s0_1 = rp[1];
	       s1_0 = rp[16];
	       s1_1 = rp[17];
	       s0_0 += ap[0] * bp[0];
	       s0_1 += ap[0] * bp[1];
	       s1_0 += ap[16] * bp[0];
	       s1_1 += ap[16] * bp[1];
	       s0_0 += ap[1] * bp[16];
	       s0_1 += ap[1] * bp[17];
	       s1_0 += ap[17] * bp[16];
	       s1_1 += ap[17] * bp[17];
	       s0_0 += ap[2] * bp[32];
	       s0_1 += ap[2] * bp[33];
	       s1_0 += ap[18] * bp[32];
	       s1_1 += ap[18] * bp[33];
	       s0_0 += ap[3] * bp[48];
	       s0_1 += ap[3] * bp[49];
	       s1_0 += ap[19] * bp[48];
	       s1_1 += ap[19] * bp[49];
	       s0_0 += ap[4] * bp[64];
	       s0_1 += ap[4] * bp[65];
	       s1_0 += ap[20] * bp[64];
	       s1_1 += ap[20] * bp[65];
	       s0_0 += ap[5] * bp[80];
	       s0_1 += ap[5] * bp[81];
	       s1_0 += ap[21] * bp[80];
	       s1_1 += ap[21] * bp[81];
	       s0_0 += ap[6] * bp[96];
	       s0_1 += ap[6] * bp[97];
	       s1_0 += ap[22] * bp[96];
	       s1_1 += ap[22] * bp[97];
	       s0_0 += ap[7] * bp[112];
	       s0_1 += ap[7] * bp[113];
	       s1_0 += ap[23] * bp[112];
	       s1_1 += ap[23] * bp[113];
	       s0_0 += ap[8] * bp[128];
	       s0_1 += ap[8] * bp[129];
	       s1_0 += ap[24] * bp[128];
	       s1_1 += ap[24] * bp[129];
	       s0_0 += ap[9] * bp[144];
	       s0_1 += ap[9] * bp[145];
	       s1_0 += ap[25] * bp[144];
	       s1_1 += ap[25] * bp[145];
	       s0_0 += ap[10] * bp[160];
	       s0_1 += ap[10] * bp[161];
	       s1_0 += ap[26] * bp[160];
	       s1_1 += ap[26] * bp[161];
	       s0_0 += ap[11] * bp[176];
	       s0_1 += ap[11] * bp[177];
	       s1_0 += ap[27] * bp[176];
	       s1_1 += ap[27] * bp[177];
	       s0_0 += ap[12] * bp[192];
	       s0_1 += ap[12] * bp[193];
	       s1_0 += ap[28] * bp[192];
	       s1_1 += ap[28] * bp[193];
	       s0_0 += ap[13] * bp[208];
	       s0_1 += ap[13] * bp[209];
	       s1_0 += ap[29] * bp[208];
	       s1_1 += ap[29] * bp[209];
	       s0_0 += ap[14] * bp[224];
	       s0_1 += ap[14] * bp[225];
	       s1_0 += ap[30] * bp[224];
	       s1_1 += ap[30] * bp[225];
	       s0_0 += ap[15] * bp[240];
	       s0_1 += ap[15] * bp[241];
	       s1_0 += ap[31] * bp[240];
	       s1_1 += ap[31] * bp[241];
	       rp[0] = s0_0;
	       rp[1] = s0_1;
	       rp[16] = s1_0;
	       rp[17] = s1_1;
	       flops += 128;
	  }
     }

     return flops;
}

long long serialadd(block *T, block *R)
{
     long i;

     for (i = 0; i < BLOCK_SIZE; i += 4) {
	  ((double *) R)[i] += ((double *) T)[i];
	  ((double *) R)[i + 1] += ((double *) T)[i + 1];
	  ((double *) R)[i + 2] += ((double *) T)[i + 2];
	  ((double *) R)[i + 3] += ((double *) T)[i + 3];
     }

     return BLOCK_SIZE;
}


/* add matrix T into matrix R */
cilk long long matrixadd(long num, block *T, block *R)
{
     if (num == 1) {
	  /* add block T to block R */
	  return serialadd(T, R);
     } else {
	  long long flops = 0LL;
	  flops += spawn matrixadd(num / 2, T, R);
	  flops += spawn matrixadd(num / 2, T + num / 2, R + num / 2);
	  sync;
	  return flops;
     }
}

/*
 * Blocked matrix multiply is done as follows:
 *
 *      A      x      B      =          R
 *
 *  | C | D |     | G | H |     | CG+DI | CH+DJ |
 *  |---+---|  x  |---+---|  =  |-------+-------|
 *  | E | F |     | I | J |     | EG+FI | EH+FJ |
 *
 */
/* compute R = AB, where A,B,R are nb x nb blocks in size */
/* if add==1, compute R += AB                             */
cilk long long matrixmul(long nb, block *A, block *B, block *R, int add)
{
     if (nb == 1) {
          if (add)
	       return serialmul_add(A, B, R);
	  else
	       return serialmul(A, B, R);
     } else {
	  long nb2 = nb / 2;
	  long nb22 = nb2 * nb2;
	  block *C, *D, *E, *F, *G, *H, *I, *J;
	  block *CG, *CH, *EH, *EG, *DI, *DJ, *FJ, *FI;
	  long long flops = 0LL;

	  /* names for portions of original matricies */

	  C = A;
	  D = A + nb22;
	  E = A + 2 * nb22;
	  F = A + 3 * nb22;
	  G = B;
	  H = B + nb22;
	  I = B + 2 * nb22;
	  J = B + 3 * nb22;

	  /* places to put results */
	  CG = R;
	  CH = CG + nb22;
	  EH = CH + nb22;
	  EG = EH + nb22;

	  flops += spawn matrixmul(nb2, C, G, CG, add);
	  flops += spawn matrixmul(nb2, C, H, CH, add);
	  flops += spawn matrixmul(nb2, E, H, EH, add);
	  flops += spawn matrixmul(nb2, E, G, EG, add);

	  if (SYNCHED)
	    {
	      DI = R;
	      DJ = DI + nb22;
	      FJ = DJ + nb22;
	      FI = FJ + nb22;
	      
	      flops += spawn matrixmul(nb2, D, I, DI, 1);
	      flops += spawn matrixmul(nb2, D, J, DJ, 1);
	      flops += spawn matrixmul(nb2, F, J, FJ, 1);
	      flops += spawn matrixmul(nb2, F, I, FI, 1);
	      sync;
	    }
	  else
	    {
	      block *tmp = Cilk_alloca(nb * nb * sizeof(block));
	      /* printf("allocating more storage %ld\n", nb*16); */
	      DI = tmp;
	      DJ = DI + nb22;
	      FJ = DJ + nb22;
	      FI = FJ + nb22;
	      
	      flops += spawn matrixmul(nb2, D, I, DI, 0);
	      flops += spawn matrixmul(nb2, D, J, DJ, 0);
	      flops += spawn matrixmul(nb2, F, J, FJ, 0);
	      flops += spawn matrixmul(nb2, F, I, FI, 0);
	      sync;
	      
	      flops += spawn matrixadd(nb * nb, tmp, R);
	      sync;
	    }
	  
	  return flops;
     }
}

cilk void init_matrix(block *A, long num)
{
     if (num == 1) {
	  int i;

	  for (i = 0; i < BLOCK_SIZE; i++)
	       ((double *) A)[i] = 1.0;
     } else {
	  spawn init_matrix(A, num / 2);
	  spawn init_matrix(A + num / 2, num / 2);
	  sync;
     }

     return;
}

cilk int check_result(block *R, double val, long num)
{
     if (num == 1) {
	  int i;

	  for (i = 0; i < BLOCK_SIZE; i++)
	       if (((double *) R)[i] != val)
		    return 1;

     } else {
	  int a, b;
	  a = spawn check_result(R, val, num / 2);
	  b = spawn check_result(R + num / 2, val, num / 2);
	  sync;
	  return a+b;
     }

     return 0;
}

int usage(void) {
  fprintf(stderr, "\nUsage: spacemul [<cilk-options>] [-n #] [-c]\n\n");
  fprintf(stderr, "Multiplies two randomly generated n x n matrices. To check for\n");
  fprintf(stderr, "correctness use -c.\n\n");
  return 1;
}

char *specifiers[] = { "-n", "-c", "-benchmark", "-h", 0};
int opt_types[] = {INTARG, BOOLARG, BENCHMARK, BOOLARG, 0 };


cilk int cilk_main(int argc, char *argv[])
{
     long n;
     int verify, benchmark, help;
     long nblocks;
     block *A, *B, *R;
     long long flops;
     Cilk_time tm_begin, tm_elapsed;
     Cilk_time wk_begin, wk_elapsed;
     Cilk_time cp_begin, cp_elapsed;

    /* standard benchmark options*/
     n = 512;  
     verify = 0;  

     get_options(argc, argv, specifiers, opt_types, &n, &verify, &benchmark, &help);

     if (help) return usage();

     if (benchmark) {
          switch (benchmark) {
	  case 1:      /* short benchmark options -- a little work*/
	    n = 256;
	    break;
	  case 2:      /* standard benchmark options*/
	    n = 512;
	    break;
	  case 3:      /* long benchmark options -- a lot of work*/
	    n = 2048;
	    break;
	  }
     }

     if ((n & (n - 1)) != 0 || (n % BLOCK_EDGE) != 0) {
	  printf("%ld: matrix size must be a power of 2"
		 " and a multiple of %d\n", n, BLOCK_EDGE);
	  return 1;
     }
     nblocks = (n * n) / BLOCK_SIZE;

     A = malloc(nblocks * sizeof(block));
     B = malloc(nblocks * sizeof(block));
     R = malloc(nblocks * sizeof(block));

     spawn init_matrix(A, nblocks);
     spawn init_matrix(B, nblocks);
     sync;

     /* Timing. "Start" timers */
     sync;			
     cp_begin = Cilk_user_critical_path;
     wk_begin = Cilk_user_work;
     tm_begin = Cilk_get_wall_time();
      
     /* do multiplication! */
     flops = spawn matrixmul(n / BLOCK_EDGE, A, B, R, 0);
     sync;
     
     /* Timing. "Stop" timers */
     tm_elapsed = Cilk_get_wall_time() - tm_begin;
     wk_elapsed = Cilk_user_work - wk_begin;
     cp_elapsed = Cilk_user_critical_path - cp_begin;
  
     if (verify) {
	  verify = spawn check_result(R, n, nblocks);
	  sync;
     }

     if (verify)
           printf("WRONG RESULT!\n");
     else {	
	   printf("\nCilk Example: spacemul\n");
	   printf("	      running on %d processor%s\n\n", Cilk_active_size, Cilk_active_size > 1 ? "s" : "");
	   printf("Options: n = %ld\n\n", n);
	   printf("Mflops        = %4f \n", (double) flops / 1.0E6 / Cilk_time_to_sec(tm_elapsed));
	   printf("Running time  = %4f s\n", Cilk_wall_time_to_sec(tm_elapsed));
	   printf("Work          = %4f s\n", Cilk_time_to_sec(wk_elapsed));
	   printf("Critical path = %4f s\n\n", Cilk_time_to_sec(cp_elapsed));
     }
     return 0;
}
