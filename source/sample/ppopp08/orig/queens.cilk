/*
 * A simple nqueens program to test abort
 *
 * Author: Keith Randall
 */
static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/queens.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";
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
#include <memory.h>

/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */
int ok(int n, char *a)
{
     int i, j;
     char p, q;

     for (i = 0; i < n; i++) {
	  p = a[i];

	  for (j = i + 1; j < n; j++) {
	       q = a[j];
	       if (q == p || q == p - (j - i) || q == p + (j - i))
		    return 0;
	  }
     }
     return 1;
}

/*
 * <a> is an array of <j> numbers.  The entries of <a> contain
 * queen positions already set.  If there is any extension of <a>
 * to a complete <n> queen setting, returns one of these queen
 * settings (allocated from the heap).  Otherwise, returns NULL.
 * Does not side-effect <a>.
 */

cilk char *nqueens(int n, int j, char *a)
{
     char *b;
     char *done = NULL;
     int i;

     inlet void catch(char *res) {
	  if (res != NULL) {
	       if (done == NULL)
		    done = res;
	       abort;
	  }
     } if (n == j) {
	  /* put good solution in heap, return pointer to it. */
	  b = malloc(n * sizeof(char));
	  memcpy(b, a, n * sizeof(char));
	  return b;
     }
     /* try each possible position for queen <j> */
     for (i = 0; i < n; i++) {
	  /* allocate a temporary array and copy <a> into it */
	  b = Cilk_alloca((j + 1) * sizeof(char));
	  memcpy(b, a, j * sizeof(char));
	  b[j] = i;
	  if (ok(j + 1, b))
	       catch(spawn nqueens(n, j + 1, b));

	  if (done != NULL)
	       break;
     }
     sync;

     return done;
}

cilk int cilk_main(int argc, char *argv[])
{
     int n;
     char *a;
     int i;

     if (argc < 2) {
	  printf("%s: number of queens required\n", argv[0]);
	  return 1;
     }
     if (argc > 2)
	  printf("%s: extra arguments being ignored\n", argv[0]);

     n = atoi(argv[1]);
     a = Cilk_alloca(n * sizeof(char));

     printf("running queens %d\n", n);
     a = spawn nqueens(n, 0, a);
     sync;
     if (a != NULL) {
	  if (!ok(n, a))
	       printf("RESULT IS BAD!!!\n");
	  printf("result: ");
	  for (i = 0; i < n; i++)
	       printf("%2d ", a[i]);
	  printf("\n");
     } else
	  printf("no possible result!\n");

     return 0;
}
