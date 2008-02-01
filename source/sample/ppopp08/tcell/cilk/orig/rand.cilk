static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/rand.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

/*
 * Copyright (c) 2003 Massachusetts Institute of Technology
 * Copyright (c) 2003 Bradley C. Kuszmaul
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

/***********************************************************\
 * Random library functions for use by user
\***********************************************************/
#include <cilk-lib.cilkh>

#ifndef RAND_MAX
#define RAND_MAX 32767
#endif

static unsigned long rand_nxt = 0;
static Cilk_lockvar rand_lock;

int Cilk_rand(void)
{
     int result;
     Cilk_lock(rand_lock);
     rand_nxt = rand_nxt * 1103515245 + 12345;
     result = (rand_nxt >> 16) % ((unsigned int) RAND_MAX + 1);
     Cilk_unlock(rand_lock);
     return result;
}

void Cilk_srand(unsigned int seed)
{
     Cilk_lock(rand_lock);
     rand_nxt = seed;
     Cilk_unlock(rand_lock);
}

#if 0
static void rand_init_hook(void)
{
     Cilk_lock_init(rand_lock);
}

static void rand_init(void) __attribute__((constructor));
static void rand_init(void)
{
     Cilk_add_hook(&Cilk_init_global_hooks, rand_init_hook);
}
#endif
