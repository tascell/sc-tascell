static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/test-locks.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

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

#define N 10000000

Cilk_lockvar lock;
int n;

cilk void increment(int count)
{
     int i;
     if (count < 2) {
	  for (i = 0; i < count; i++) {
	       Cilk_lock(lock);
	       n++;
	       Cilk_unlock(lock);
	  }
     } else {
	  spawn increment(count / 2);
	  spawn increment(count - (count / 2));
	  sync;
     }
}

cilk int main(void)
{
     Cilk_lock_init(lock);
     n = 0;

     spawn increment(N);
     sync;

     if (n == N) {
	  printf("OK\n");
	  return 0;
     } else {
	  printf("BAD %d %d\n", n, N * 2);
	  return -1;
     }
}
