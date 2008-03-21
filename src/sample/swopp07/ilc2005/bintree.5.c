#include<stdio.h>
typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double align_t;

char *
lw_call (char *esp)
{
  closure_t *clos = *((closure_t **) esp);

  return (clos->fun) (esp, clos->fr);
}

#line 1 "sample/ilc2005/copygc.h"
#line 1 "/usr/include/stddef.h"
#line 1 "/usr/include/sys/cdefs.h"
#line 1 "/usr/include/sys/_null.h"
#line 1 "/usr/include/sys/_types.h"
#line 1 "/usr/include/sys/cdefs.h"
#line 1 "/usr/include/machine/_types.h"
#line 45 "/usr/include/machine/_types.h"
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef int __int64_t;
typedef unsigned int __uint64_t;

#line 70 "/usr/include/machine/_types.h"
typedef unsigned long __clock_t;
typedef unsigned int __cpumask_t;
typedef __int32_t __critical_t;
typedef double __double_t;
typedef double __float_t;
typedef __int32_t __intfptr_t;
typedef __int64_t __intmax_t;
typedef __int32_t __intptr_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;
typedef __int32_t __ptrdiff_t;
typedef __int32_t __register_t;
typedef __int32_t __segsz_t;
typedef __uint32_t __size_t;
typedef __int32_t __ssize_t;
typedef __int32_t __time_t;
typedef __uint32_t __uintfptr_t;
typedef __uint64_t __uintmax_t;
typedef __uint32_t __uintptr_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;
typedef __uint32_t __u_register_t;
typedef __uint32_t __vm_offset_t;
typedef __int64_t __vm_ooffset_t;
typedef __uint32_t __vm_paddr_t;
typedef __uint64_t __vm_pindex_t;
typedef __uint32_t __vm_size_t;

#line 118 "/usr/include/machine/_types.h"
typedef __builtin_va_list __va_list;

#line 124 "/usr/include/machine/_types.h"
typedef __va_list __gnuc_va_list;

#line 38 "/usr/include/sys/_types.h"
typedef __int32_t __clockid_t;
typedef __uint32_t __fflags_t;
typedef __uint64_t __fsblkcnt_t;
typedef __uint64_t __fsfilcnt_t;
typedef __uint32_t __gid_t;
typedef __int64_t __id_t;
typedef __uint32_t __ino_t;
typedef long __key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int __nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;
typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long __suseconds_t;
typedef __int32_t __timer_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;

#line 78 "/usr/include/sys/_types.h"
typedef int __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;
typedef __uint32_t __dev_t;
typedef __uint32_t __fixpt_t;

#line 91 "/usr/include/sys/_types.h"

union unnamed0
{
  char __mbstate8[128];
  __int64_t _mbstateL;
};
typedef union unnamed0 __mbstate_t;

#line 45 "/usr/include/stddef.h"
typedef __ptrdiff_t ptrdiff_t;
typedef __rune_t rune_t;
typedef __size_t size_t;
typedef __wchar_t wchar_t;

#line 13 "sample/ilc2005/copygc.h"
typedef double align_t;

#line 17 "sample/ilc2005/copygc.h"

struct unnamed1
{
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
};
typedef struct unnamed1 descriptor;
typedef struct unnamed1 *desc_t;

#line 27 "sample/ilc2005/copygc.h"

struct unnamed2
{
  int gcv;
  int gctype;
  size_t tosize;
  size_t stack_size;
  size_t limited_stack_max;
};
typedef struct unnamed2 gc_params;

#line 42 "sample/ilc2005/copygc.h"

extern void getmem_init (gc_params);

extern void *getmem (sht, desc_t);

extern void *move (void *vp);

extern void gc (sht);
extern double gc_ttime;

long nrand48 (unsigned short xseed[3]);

int strcmp (char const *s1, char const *s2);

int atoi (char const *nptr);

int printf (char const *format, ...);

int gettimeofday (struct timeval *tp, struct timezone *tzp);

struct timeval
{
  long tv_sec;
  long tv_usec;
};

struct timezone
{
  int tz_minuteswest;
  int tz_dsttime;
};
typedef closure_t *sht;

struct _bINTREE
{
  desc_t d;
  int key;
  int val;
  bINTREE *left;
  bINTREE *right;
};
typedef struct _bINTREE bINTREE;
size_t bINTREE_NODE[] =
    { (unsigned int) &(*((bINTREE *) 0)).left,
(unsigned int) &(*((bINTREE *) 0)).right };
descriptor bINTREE_D =
    ({ sizeof (bINTREE),
     (sizeof (align_t) *
      (((sizeof (bINTREE)) + sizeof (align_t) - 1) / sizeof (align_t))),
     sizeof (bINTREE_NODE) / sizeof (size_t), bINTREE_NODE }
);
static int maxins;
static int maxsearch;

struct NEWbINTREE_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  bINTREE *p;
  int v;
  int k;
  sht scan0;
};

bINTREE *
NEWbINTREE (char *esp, sht scan0, int k, int v)
{
  struct NEWbINTREE_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  bINTREE *p = (bINTREE *) getmem (scan0, &efp->bINTREE_D);

lgoto:
  ;
  efp = (struct NEWbINTREE_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct NEWbINTREE_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  (*p).key = k;
  (*p).val = v;
  return p;
}

struct insert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct _bINTREE *tmp4;
  struct _bINTREE *tmp3;
  closure_t *scan1;
  bINTREE *y;
  int v;
  int k;
  bINTREE *x;
  sht scan0;
  closure_t scan10;
};

struct scan1_in_insert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp2;
  void *tmp;
  struct insert_frame *xfp;
};

char *
scan1_in_insert (char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_insert_frame *efp;
  struct insert_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;
  void *tmp;
  void *tmp2;
  closure_t *tmp_fp;

lgoto:
  ;
  efp = (struct scan1_in_insert_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct scan1_in_insert_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  while ((tmp = move (new_esp, xfp->x)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp2 = tmp2;
      efp->tmp = tmp;
      efp->xfp = xfp;
      goto_fr = lw_call (efp->tmp_esp);
      if (goto_fr && (char *) goto_fr < (char *) efp)
	return goto_fr;
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  xfp->x = tmp;
  new_esp = esp;
  while ((tmp2 = move (new_esp, xfp->y)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp2 = tmp2;
      efp->tmp = tmp;
      efp->xfp = xfp;
      goto_fr = lw_call (efp->tmp_esp);
      if (goto_fr && (char *) goto_fr < (char *) efp)
	return goto_fr;
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  xfp->y = tmp2;
  {
    char *goto_fr;

    tmp_fp = xfp->scan0;
    argp =
	(char *) ((align_t *) esp +
		  (sizeof (char *) + sizeof (align_t) + -1) / sizeof (align_t));
    *((closure_t **) argp) = tmp_fp;
    efp->tmp2 = tmp2;
    efp->tmp = tmp;
    efp->xfp = xfp;
    efp->argp = argp;
    efp->tmp_esp = argp;
    goto_fr = lw_call (efp->tmp_esp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else if ((char *) goto_fr == (char *) efp)
	goto lgoto;
    tmp2 = efp->tmp2;
    tmp = efp->tmp;
    xfp = efp->xfp;
  }
  return 0;
}

void
insert (char *esp, sht scan0, bINTREE * x, int k, int v)
{
  struct insert_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  bINTREE *y = 0;
  closure_t *scan1 = &((struct insert_frame *) esp)->scan10;
  struct _bINTREE *tmp3;
  struct _bINTREE *tmp4;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct insert_frame *) esp;
      esp =
	  (char *) ((align_t *) esp +
		    (sizeof (struct insert_frame) + sizeof (align_t) +
		     -1) / sizeof (align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_call;
	case 1:
	  goto L_call2;
	}
      goto L_call;
    }
  efp = (struct insert_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct insert_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  do
    {
      if ((*x).key == k)
	{
	  {
	    {
	      (*x).val = v;
	      return;
	    }
	  }
	}
      else
	{
	  {
	    if (k < (*x).key)
	      {
		{
		  {
		    y = (*x).left;
		    if (!y)
		      {
			{
			  new_esp = esp;
			  while ((tmp3 =
				  NEWbINTREE (new_esp, scan1, k,
					      v)) == (struct _bINTREE *) 0 - 1
				 && (efp->tmp_esp = *((char **) esp)) != 0)
			    {
			      efp->tmp4 = tmp4;
			      efp->tmp3 = tmp3;
			      efp->scan1 = scan1;
			      efp->y = y;
			      efp->v = v;
			      efp->k = k;
			      efp->x = x;
			      efp->scan0 = scan0;
			      efp->scan10.fun = scan1_in_insert;
			      efp->scan10.fr = (void *) efp;
			      efp->call_id = 0;
			      return;
			    L_call:
			      ;
			      tmp4 = efp->tmp4;
			      tmp3 = efp->tmp3;
			      scan1 = efp->scan1;
			      y = efp->y;
			      v = efp->v;
			      k = efp->k;
			      x = efp->x;
			      scan0 = efp->scan0;
			      new_esp = esp + 1;
			    }
			  (*x).left = tmp3;
			  return;
			}
		      }
		  }
		}
	      }
	    else
	      {
		{
		  {
		    y = (*x).right;
		    if (!y)
		      {
			{
			  new_esp = esp;
			  while ((tmp4 =
				  NEWbINTREE (new_esp, scan1, k,
					      v)) == (struct _bINTREE *) 0 - 1
				 && (efp->tmp_esp = *((char **) esp)) != 0)
			    {
			      efp->tmp4 = tmp4;
			      efp->tmp3 = tmp3;
			      efp->scan1 = scan1;
			      efp->y = y;
			      efp->v = v;
			      efp->k = k;
			      efp->x = x;
			      efp->scan0 = scan0;
			      efp->scan10.fun = scan1_in_insert;
			      efp->scan10.fr = (void *) efp;
			      efp->call_id = 1;
			      return;
			    L_call2:
			      ;
			      tmp4 = efp->tmp4;
			      tmp3 = efp->tmp3;
			      scan1 = efp->scan1;
			      y = efp->y;
			      v = efp->v;
			      k = efp->k;
			      x = efp->x;
			      scan0 = efp->scan0;
			      new_esp = esp + 1;
			    }
			  (*x).right = tmp4;
			  return;
			}
		      }
		  }
		}
	      }
	  }
	}
      x = y;
    }
  while (1);
}

struct search_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int v0;
  int k;
  bINTREE *x;
  sht scan0;
};

int
search (char *esp, sht scan0, bINTREE * x, int k, int v0)
{
  struct search_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;

lgoto:
  ;
  efp = (struct search_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct search_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  if (x)
    {
      do
	{
	  if ((*x).key == k)
	    {
	      {
		{
		  return (*x).val;
		}
	      }
	    }
	  else
	    {
	      {
		if (k < (*x).key)
		  {
		    {
		      {
			x = (*x).left;
		      }
		    }
		  }
		else
		  {
		    {
		      {
			x = (*x).right;
		      }
		    }
		  }
	      }
	    }
	}
      while (x);
    }
  return v0;
}

struct randinsert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  long tmp6;
  unsigned short seed[3];
  int k;
  int i;
  closure_t *scan1;
  int n;
  bINTREE *this;
  sht scan0;
  closure_t scan102;
};

struct scan1_in_randinsert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp5;
  struct randinsert_frame *xfp;
};

char *
scan1_in_randinsert (char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_randinsert_frame *efp;
  struct randinsert_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;
  void *tmp5;
  closure_t *tmp_fp2;

lgoto:
  ;
  efp = (struct scan1_in_randinsert_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct scan1_in_randinsert_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  while ((tmp5 = move (new_esp, xfp->this)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp5 = tmp5;
      efp->xfp = xfp;
      goto_fr = lw_call (efp->tmp_esp);
      if (goto_fr && (char *) goto_fr < (char *) efp)
	return goto_fr;
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  xfp->this = tmp5;
  {
    char *goto_fr;

    tmp_fp2 = xfp->scan0;
    argp =
	(char *) ((align_t *) esp +
		  (sizeof (char *) + sizeof (align_t) + -1) / sizeof (align_t));
    *((closure_t **) argp) = tmp_fp2;
    efp->tmp5 = tmp5;
    efp->xfp = xfp;
    efp->argp = argp;
    efp->tmp_esp = argp;
    goto_fr = lw_call (efp->tmp_esp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else if ((char *) goto_fr == (char *) efp)
	goto lgoto;
    tmp5 = efp->tmp5;
    xfp = efp->xfp;
  }
  return 0;
}

void
randinsert (char *esp, sht scan0, bINTREE * this, int n)
{
  struct randinsert_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  closure_t *scan1 = &((struct randinsert_frame *) esp)->scan102;
  int i;
  int k;
  unsigned short seed[3];
  long tmp6;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct randinsert_frame *) esp;
      esp =
	  (char *) ((align_t *) esp +
		    (sizeof (struct randinsert_frame) + sizeof (align_t) +
		     -1) / sizeof (align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_call3;
	}
      goto L_call3;
    }
  efp = (struct randinsert_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct randinsert_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  (efp->seed)[0] = 3;
  (efp->seed)[1] = 4;
  (efp->seed)[2] = 5;
  {
    i = 0;
    if (i < n)
      {
	do
	  {
	    backq_list;
	    x;
	    k = tmp6;
	    new_esp = esp;
	    while (insert (new_esp, scan1, this, k, k),
		   (efp->tmp_esp = *((char **) esp)) != 0)
	      {
		efp->tmp6 = tmp6;
		efp->k = k;
		efp->i = i;
		efp->scan1 = scan1;
		efp->n = n;
		efp->this = this;
		efp->scan0 = scan0;
		efp->scan102.fun = scan1_in_randinsert;
		efp->scan102.fr = (void *) efp;
		efp->call_id = 0;
		return;
	      L_call3:
		;
		tmp6 = efp->tmp6;
		k = efp->k;
		i = efp->i;
		scan1 = efp->scan1;
		n = efp->n;
		this = efp->this;
		scan0 = efp->scan0;
		new_esp = esp + 1;
	      }
	    i++;
	  }
	while (i < n);
      }
  }
}

struct randsearch_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp9;
  long tmp8;
  unsigned short seed[3];
  int k;
  int i;
  closure_t *scan1;
  int n;
  bINTREE *this;
  sht scan0;
  closure_t scan103;
};

struct scan1_in_randsearch_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp7;
  struct randsearch_frame *xfp;
};

char *
scan1_in_randsearch (char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_randsearch_frame *efp;
  struct randsearch_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  char *argp;
  void *tmp7;
  closure_t *tmp_fp3;

lgoto:
  ;
  efp = (struct scan1_in_randsearch_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct scan1_in_randsearch_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  while ((tmp7 = move (new_esp, xfp->this)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp7 = tmp7;
      efp->xfp = xfp;
      goto_fr = lw_call (efp->tmp_esp);
      if (goto_fr && (char *) goto_fr < (char *) efp)
	return goto_fr;
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  xfp->this = tmp7;
  {
    char *goto_fr;

    tmp_fp3 = xfp->scan0;
    argp =
	(char *) ((align_t *) esp +
		  (sizeof (char *) + sizeof (align_t) + -1) / sizeof (align_t));
    *((closure_t **) argp) = tmp_fp3;
    efp->tmp7 = tmp7;
    efp->xfp = xfp;
    efp->argp = argp;
    efp->tmp_esp = argp;
    goto_fr = lw_call (efp->tmp_esp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else if ((char *) goto_fr == (char *) efp)
	goto lgoto;
    tmp7 = efp->tmp7;
    xfp = efp->xfp;
  }
  return 0;
}

void
randsearch (char *esp, sht scan0, bINTREE * this, int n)
{
  struct randsearch_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  closure_t *scan1 = &((struct randsearch_frame *) esp)->scan103;
  int i;
  int k;
  unsigned short seed[3];
  long tmp8;
  int tmp9;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct randsearch_frame *) esp;
      esp =
	  (char *) ((align_t *) esp +
		    (sizeof (struct randsearch_frame) + sizeof (align_t) +
		     -1) / sizeof (align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_call4;
	case 1:
	  goto L_call5;
	}
      goto L_call4;
    }
  efp = (struct randsearch_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct randsearch_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  (efp->seed)[0] = 8;
  (efp->seed)[1] = 9;
  (efp->seed)[2] = 10;
  {
    i = 0;
    if (i < n)
      {
	do
	  {
	    new_esp = esp;
	    while ((tmp8 = nrand48 (new_esp, efp->seed)) == (long) 0 - 1
		   && (efp->tmp_esp = *((char **) esp)) != 0)
	      {
		efp->tmp9 = tmp9;
		efp->tmp8 = tmp8;
		efp->k = k;
		efp->i = i;
		efp->scan1 = scan1;
		efp->n = n;
		efp->this = this;
		efp->scan0 = scan0;
		efp->scan103.fun = scan1_in_randsearch;
		efp->scan103.fr = (void *) efp;
		efp->call_id = 0;
		return;
	      L_call4:
		;
		tmp9 = efp->tmp9;
		tmp8 = efp->tmp8;
		k = efp->k;
		i = efp->i;
		scan1 = efp->scan1;
		n = efp->n;
		this = efp->this;
		scan0 = efp->scan0;
		new_esp = esp + 1;
	      }
	    k = tmp8;
	    new_esp = esp;
	    while ((tmp9 = search (new_esp, scan1, this, k, 0)) == (int) 0 - 1
		   && (efp->tmp_esp = *((char **) esp)) != 0)
	      {
		efp->tmp9 = tmp9;
		efp->tmp8 = tmp8;
		efp->k = k;
		efp->i = i;
		efp->scan1 = scan1;
		efp->n = n;
		efp->this = this;
		efp->scan0 = scan0;
		efp->scan103.fun = scan1_in_randsearch;
		efp->scan103.fr = (void *) efp;
		efp->call_id = 1;
		return;
	      L_call5:
		;
		tmp9 = efp->tmp9;
		tmp8 = efp->tmp8;
		k = efp->k;
		i = efp->i;
		scan1 = efp->scan1;
		n = efp->n;
		this = efp->this;
		scan0 = efp->scan0;
		new_esp = esp + 1;
	      }
	    tmp9;
	    i++;
	  }
	while (i < n);
      }
  }
}

struct init_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  gc_params p;
  int limited_max;
  int stack_size;
  int tosize;
  int tp;
};

void
init (char *esp, int tp, int tosize, int stack_size, int limited_max)
{
  struct init_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  gc_params p;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct init_frame *) esp;
      esp =
	  (char *) ((align_t *) esp +
		    (sizeof (struct init_frame) + sizeof (align_t) +
		     -1) / sizeof (align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_call6;
	}
      goto L_call6;
    }
  efp = (struct init_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct init_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  new_esp = esp;
  while (getmem_init (new_esp, p), (efp->tmp_esp = *((char **) esp)) != 0)
    {
      efp->p = p;
      efp->limited_max = limited_max;
      efp->stack_size = stack_size;
      efp->tosize = tosize;
      efp->tp = tp;
      efp->call_id = 0;
      return;
    L_call6:
      ;
      p = efp->p;
      limited_max = efp->limited_max;
      stack_size = efp->stack_size;
      tosize = efp->tosize;
      tp = efp->tp;
      new_esp = esp + 1;
    }
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp22;
  int tmp21;
  int tmp20;
  void *tmp19;
  int ifexp_result9;
  int tmp18;
  int ifexp_result8;
  int tmp17;
  int ifexp_result7;
  int tmp16;
  int ifexp_result5;
  int tmp15;
  int tmp14;
  int ifexp_result4;
  int tmp13;
  int ifexp_result3;
  int tmp12;
  int ifexp_result2;
  int tmp11;
  double ttime;
  bINTREE *root;
  int searchskip;
  int gctp;
  int i;
  struct timeval tp2;
  struct timeval tp1;
  closure_t *scan1;
  char *argv[];
  int argc;
  struct unnamed1 *tmp_arg3;
  int tmp_arg2;
  char *tmp_arg;
  closure_t scan104;
};

struct scan1_in_main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp10;
  struct main_frame *xfp;
};

char *
scan1_in_main (char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  void *tmp10;

lgoto:
  ;
  efp = (struct scan1_in_main_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct scan1_in_main_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  while ((tmp10 = move (new_esp, root)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp10 = tmp10;
      efp->xfp = xfp;
      goto_fr = lw_call (efp->tmp_esp);
      if (goto_fr && (char *) goto_fr < (char *) efp)
	return goto_fr;
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  root = tmp10;
  return 0;
}

int
main (int argc, char *argv[])
{
  struct main_frame *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  closure_t *scan1 = &((struct main_frame *) esp)->scan104;
  struct timeval tp1;
  struct timeval tp2;
  int i;
  int gctp;
  int searchskip = 0;
  bINTREE *root;
  double ttime;
  int tmp11;
  int ifexp_result2;
  int tmp12;
  int ifexp_result3;
  int tmp13;
  int ifexp_result4;
  int tmp14;
  int tmp15;
  int ifexp_result5;
  int tmp16;
  int ifexp_result7;
  int tmp17;
  int ifexp_result8;
  int tmp18;
  int ifexp_result9;
  void *tmp19;
  int tmp20;
  int tmp21;
  int tmp22;

  backq_list;
  x;
  int tmp_arg2;
  struct unnamed1 *tmp_arg3;

  backq_list;
  x;
  backq_list;
  x;
  backq_list;
  x;
lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct main_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  if (argc > 1)
    {
      {
	char *tmp_arg;

	new_esp = esp;
	tmp_arg = (efp->argv)[1];
	while ((tmp11 = strcmp (new_esp, tmp_arg, "-s")) == (int) 0 - 1
	       && (efp->tmp_esp = *((char **) esp)) != 0)
	  {
	    char *goto_fr;

	    *((char **) esp) = 0;
	    efp->tmp22 = tmp22;
	    efp->tmp21 = tmp21;
	    efp->tmp20 = tmp20;
	    efp->tmp19 = tmp19;
	    efp->ifexp_result9 = ifexp_result9;
	    efp->tmp18 = tmp18;
	    efp->ifexp_result8 = ifexp_result8;
	    efp->tmp17 = tmp17;
	    efp->ifexp_result7 = ifexp_result7;
	    efp->tmp16 = tmp16;
	    efp->ifexp_result5 = ifexp_result5;
	    efp->tmp15 = tmp15;
	    efp->tmp14 = tmp14;
	    efp->ifexp_result4 = ifexp_result4;
	    efp->tmp13 = tmp13;
	    efp->ifexp_result3 = ifexp_result3;
	    efp->tmp12 = tmp12;
	    efp->ifexp_result2 = ifexp_result2;
	    efp->tmp11 = tmp11;
	    efp->ttime = ttime;
	    efp->root = root;
	    efp->searchskip = searchskip;
	    efp->gctp = gctp;
	    efp->i = i;
	    efp->scan1 = scan1;
	    efp->argc = argc;
	    efp->scan104.fun = scan1_in_main;
	    efp->scan104.fr = (void *) efp;
	    goto_fr = lw_call (efp->tmp_esp);
	    if ((char *) goto_fr == (char *) efp)
	      goto lgoto;
	    tmp_arg = efp->tmp_arg;
	    new_esp = esp + 1;
	  }
	ifexp_result2 = tmp11 == 0 ? 1 : 0;
      }
    }
  else
    {
      {
	ifexp_result2 = 0;
      }
    }
  if (ifexp_result2)
    {
      {
	--argc;
	++efp->argv;
	++searchckip;
      }
    }
  if (argc > 1)
    {
      {
	backq_list;
	x;
	ifexp_result3 = tmp12;
      }
    }
  else
    {
      {
	ifexp_result3 = 100000;
      }
    }
  maxins = ifexp_result3;
  if (argc > 2)
    {
      {
	backq_list;
	x;
	ifexp_result4 = tmp13;
      }
    }
  else
    {
      {
	ifexp_result4 = 300000;
      }
    }
  maxsearch = ifexp_result4;
  tmp14;
  if (argc > 3)
    {
      {
	backq_list;
	x;
	ifexp_result5 = tmp15;
      }
    }
  else
    {
      {
	ifexp_result5 = 0;
      }
    }
  gctp = ifexp_result5;
  if (argc > 4)
    {
      {
	backq_list;
	x;
	ifexp_result7 = tmp16;
      }
    }
  else
    {
      {
	ifexp_result7 = 0;
      }
    }
  if (argc > 5)
    {
      {
	backq_list;
	x;
	ifexp_result8 = tmp17;
      }
    }
  else
    {
      {
	ifexp_result8 = 0;
      }
    }
  if (argc > 6)
    {
      {
	backq_list;
	x;
	ifexp_result9 = tmp18;
      }
    }
  else
    {
      {
	ifexp_result9 = 0;
      }
    }
  new_esp = esp;
  tmp_arg2 = gctp < 0 ? 0 : gctp;
  while (init (new_esp, tmp_arg2, ifexp_result7, ifexp_result8, ifexp_result9),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp22 = tmp22;
      efp->tmp21 = tmp21;
      efp->tmp20 = tmp20;
      efp->tmp19 = tmp19;
      efp->ifexp_result9 = ifexp_result9;
      efp->tmp18 = tmp18;
      efp->ifexp_result8 = ifexp_result8;
      efp->tmp17 = tmp17;
      efp->ifexp_result7 = ifexp_result7;
      efp->tmp16 = tmp16;
      efp->ifexp_result5 = ifexp_result5;
      efp->tmp15 = tmp15;
      efp->tmp14 = tmp14;
      efp->ifexp_result4 = ifexp_result4;
      efp->tmp13 = tmp13;
      efp->ifexp_result3 = ifexp_result3;
      efp->tmp12 = tmp12;
      efp->ifexp_result2 = ifexp_result2;
      efp->tmp11 = tmp11;
      efp->ttime = ttime;
      efp->root = root;
      efp->searchskip = searchskip;
      efp->gctp = gctp;
      efp->i = i;
      efp->scan1 = scan1;
      efp->argc = argc;
      efp->scan104.fun = scan1_in_main;
      efp->scan104.fr = (void *) efp;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      tmp_arg2 = efp->tmp_arg2;
      new_esp = esp + 1;
    }
  new_esp = esp;
  tmp_arg3 = &efp->bINTREE_D;
  while ((tmp19 = getmem (new_esp, scan1, tmp_arg3)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp22 = tmp22;
      efp->tmp21 = tmp21;
      efp->tmp20 = tmp20;
      efp->tmp19 = tmp19;
      efp->ifexp_result9 = ifexp_result9;
      efp->tmp18 = tmp18;
      efp->ifexp_result8 = ifexp_result8;
      efp->tmp17 = tmp17;
      efp->ifexp_result7 = ifexp_result7;
      efp->tmp16 = tmp16;
      efp->ifexp_result5 = ifexp_result5;
      efp->tmp15 = tmp15;
      efp->tmp14 = tmp14;
      efp->ifexp_result4 = ifexp_result4;
      efp->tmp13 = tmp13;
      efp->ifexp_result3 = ifexp_result3;
      efp->tmp12 = tmp12;
      efp->ifexp_result2 = ifexp_result2;
      efp->tmp11 = tmp11;
      efp->ttime = ttime;
      efp->root = root;
      efp->searchskip = searchskip;
      efp->gctp = gctp;
      efp->i = i;
      efp->scan1 = scan1;
      efp->argc = argc;
      efp->scan104.fun = scan1_in_main;
      efp->scan104.fr = (void *) efp;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      tmp_arg3 = efp->tmp_arg3;
      new_esp = esp + 1;
    }
  root = tmp19;
  (*root).key = 0;
  (*root).val = 0;
  tmp20;
  new_esp = esp;
  while (randinsert (new_esp, scan1, root, maxins),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp22 = tmp22;
      efp->tmp21 = tmp21;
      efp->tmp20 = tmp20;
      efp->tmp19 = tmp19;
      efp->ifexp_result9 = ifexp_result9;
      efp->tmp18 = tmp18;
      efp->ifexp_result8 = ifexp_result8;
      efp->tmp17 = tmp17;
      efp->ifexp_result7 = ifexp_result7;
      efp->tmp16 = tmp16;
      efp->ifexp_result5 = ifexp_result5;
      efp->tmp15 = tmp15;
      efp->tmp14 = tmp14;
      efp->ifexp_result4 = ifexp_result4;
      efp->tmp13 = tmp13;
      efp->ifexp_result3 = ifexp_result3;
      efp->tmp12 = tmp12;
      efp->ifexp_result2 = ifexp_result2;
      efp->tmp11 = tmp11;
      efp->ttime = ttime;
      efp->root = root;
      efp->searchskip = searchskip;
      efp->gctp = gctp;
      efp->i = i;
      efp->scan1 = scan1;
      efp->argc = argc;
      efp->scan104.fun = scan1_in_main;
      efp->scan104.fr = (void *) efp;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  tmp21;
  if (gctp >= 0)
    {
      new_esp = esp;
      while (gc (new_esp, scan1), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->tmp22 = tmp22;
	  efp->tmp21 = tmp21;
	  efp->tmp20 = tmp20;
	  efp->tmp19 = tmp19;
	  efp->ifexp_result9 = ifexp_result9;
	  efp->tmp18 = tmp18;
	  efp->ifexp_result8 = ifexp_result8;
	  efp->tmp17 = tmp17;
	  efp->ifexp_result7 = ifexp_result7;
	  efp->tmp16 = tmp16;
	  efp->ifexp_result5 = ifexp_result5;
	  efp->tmp15 = tmp15;
	  efp->tmp14 = tmp14;
	  efp->ifexp_result4 = ifexp_result4;
	  efp->tmp13 = tmp13;
	  efp->ifexp_result3 = ifexp_result3;
	  efp->tmp12 = tmp12;
	  efp->ifexp_result2 = ifexp_result2;
	  efp->tmp11 = tmp11;
	  efp->ttime = ttime;
	  efp->root = root;
	  efp->searchskip = searchskip;
	  efp->gctp = gctp;
	  efp->i = i;
	  efp->scan1 = scan1;
	  efp->argc = argc;
	  efp->scan104.fun = scan1_in_main;
	  efp->scan104.fr = (void *) efp;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
    }
  if (!searchckip)
    {
      new_esp = esp;
      while (randsearch (new_esp, scan1, root, maxsearch),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->tmp22 = tmp22;
	  efp->tmp21 = tmp21;
	  efp->tmp20 = tmp20;
	  efp->tmp19 = tmp19;
	  efp->ifexp_result9 = ifexp_result9;
	  efp->tmp18 = tmp18;
	  efp->ifexp_result8 = ifexp_result8;
	  efp->tmp17 = tmp17;
	  efp->ifexp_result7 = ifexp_result7;
	  efp->tmp16 = tmp16;
	  efp->ifexp_result5 = ifexp_result5;
	  efp->tmp15 = tmp15;
	  efp->tmp14 = tmp14;
	  efp->ifexp_result4 = ifexp_result4;
	  efp->tmp13 = tmp13;
	  efp->ifexp_result3 = ifexp_result3;
	  efp->tmp12 = tmp12;
	  efp->ifexp_result2 = ifexp_result2;
	  efp->tmp11 = tmp11;
	  efp->ttime = ttime;
	  efp->root = root;
	  efp->searchskip = searchskip;
	  efp->gctp = gctp;
	  efp->i = i;
	  efp->scan1 = scan1;
	  efp->argc = argc;
	  efp->scan104.fun = scan1_in_main;
	  efp->scan104.fr = (void *) efp;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
    }
  ttime =
      (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec -
					       (efp->tp1).tv_usec) * 1.0e-6;
  tmp22;
  return 0;
}
