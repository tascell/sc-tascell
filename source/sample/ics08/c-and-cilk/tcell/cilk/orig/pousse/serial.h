/*
 * This should allow us to compile a serial verson of the program
 * without too much hacking.
 *
 */

#ifndef _CILK_SEQUENTIAL_H
#define _CILK_SEQUENTIAL_H


static inline void C_abort(void)
{
     abort();
}

#define cilk
#define spawn
#define sync
#define inlet
#define abort
#define private
#define cilk_main main
#define Cilk_malloc malloc
#define Cilk_free free
#define Cilk_memalign memalign
#define Cilk_calloc calloc
#define Cilk_valloc valloc
#define Cilk_realloc realloc
#define Cilk_rand rand
#define Cilk_srand srand
#define Cilk_assert assert

#define Cilk_exit exit
#define CILK_ASSERT assert

/* no work/cp, of course */
#define Cilk_user_work ((Cilk_time) 0)
#define Cilk_user_critical_path ((Cilk_time) 0)
#define Cilk_active_size (1)

#define Self (0)

#endif	
