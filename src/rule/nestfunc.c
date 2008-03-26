/*
Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

/* Library for LW-SC */

#include<stdlib.h>
#include<pthread.h>
typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef double	Align_t;

char           *lw_call(char *esp)
{
  closure_t      *clos = *((closure_t **) esp);

  return ((*clos).fun) (esp, (*clos).fr);
}

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

struct tho_frame {
  char           *tmp_esp;
};

void           *thread_origin(void *farg)
{
  struct tho_frame *efp;
  struct func_arg *x = farg;
  void           *(*func) (char *, void *)= (*x).func;
  void           *arg = (*x).arg;
  void           *ret;
  char           *new_esp;
  char		  estack  [16000];
  char           *esp = estack;

  pthread_cleanup_push(free, farg);
  efp = (struct tho_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct tho_frame) + sizeof(Align_t) - 1) / sizeof(Align_t));
  *((char **)esp) = 0;
  new_esp = esp;
  while ((ret = func(new_esp, arg)) == (void *)0 - 1 && ((*efp).tmp_esp = *((char **)esp)) != 0) {
    lw_call((*efp).tmp_esp);
    new_esp = esp + 1;
  }
  pthread_cleanup_pop(1);
  return ret;
}
