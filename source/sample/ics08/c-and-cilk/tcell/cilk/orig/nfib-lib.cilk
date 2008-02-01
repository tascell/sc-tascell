/* nfib <n> <m>  computes fib <n>   <m> times */
/*
 * This program is used to understand the behavior of the memory
 * allocator.  In Cilk 5.[012], the memory allocator seems to use an
 * unbounded amount of space as <m> grows.
 */

static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/nfib-lib.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

cilk int fib(int n)
{
     if (n < 2)
	  return (n);
     else {
	  int x, y;
	  x = spawn fib(n - 1);
	  y = spawn fib(n - 2);
	  sync;
	  return (x + y);
     }
}

extern EXPORT(fib)(CilkContext *const, int n);

int main(int argc, char *argv[])
{
     int n, result = -1;
     int repeat, i;
     CilkContext *context;

     assert (argc>1);

     context = Cilk_init(&argc, argv);
     if( !context)
	return -1;

     n = atoi(argv[1]);

     if (argc > 2)
	  repeat = atoi(argv[2]);
     else
	  repeat = 1;

     for (i = 0; i < repeat; ++i) {
	  result = EXPORT(fib)(context,n);
     	  printf("%d: n=%d Result: %d\n", i, n, result);
     }

     Cilk_terminate(context);

     return 0;
}
