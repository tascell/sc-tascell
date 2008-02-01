typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef int __int64_t;
typedef unsigned int __uint64_t;
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
typedef __builtin_va_list __va_list;
typedef __va_list __gnuc_va_list;
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
typedef int __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;
typedef __uint32_t __dev_t;
typedef __uint32_t __fixpt_t;

union unnamed0
{
  char __mbstate8[128];
  __int64_t _MBSTATEl;
};
typedef union unnamed0 __mbstate_t;
typedef __ptrdiff_t ptrdiff_t;
typedef __rune_t rune_t;
typedef __size_t size_t;
typedef __wchar_t wchar_t;
typedef double align_t;

struct unnamed1
{
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
};
typedef struct unnamed1 descriptor;
typedef struct unnamed1 *desc_t;

struct unnamed2
{
  int gcv;
  int gctype;
  size_t tosize;
  size_t stack_size;
  size_t limited_stack_max;
};
typedef struct unnamed2 gc_params;
typedef closure_t *sht;

extern void getmem_init (gc_params);

extern void *getmem (char *esp, sht, desc_t);

extern void *move (void *vp);

extern void gc (char *esp, sht);
extern double gc_ttime;

long nrand48 (unsigned short xseed[3]);

int strcmp (char const *s1, char const *s2);

void *memset (void *b, int c, size_t len);

void *memcpy (void *dest, void const *src, size_t len);

int atoi (char const *nptr);

int printf (char const *format, ...);

void *malloc (size_t size);

void *alloca (size_t size);

void *realloc (void *pt, size_t size);

void free (void *pt);

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

int gettimeofday (struct timeval *tp, struct timezone *tzp);

void *
MEMCPY (void *d, void const *s, size_t sz)
{
  long *_des = (long *) d;
  long *_src = (long *) s;
  long *_til = (long *) ((char *) _src + sz);

  ;
  ;
  ;
  do
    {
      *_des++ = *_src++;
    }
  while (_src < _til);
}

void exit (int);

static void
error (char *s)
{
  printf ("ERROR: %s\n", s);
  exit (1);
}

static void *
myalloc (size_t size)
{
  void *p;

  ;
  p = malloc (size);
  if (p == (void *) 0)
    error ("Not enough memory.");
  else;
  return p;
}

static void *
myrealloc (void *p, size_t size)
{
  void *q;

  ;
  q = realloc (p, size);
  if (q == (void *) 0)
    error ("Not enough memory.");
  else;
  return q;
}

gc_params params;
static int allocated_size;
static char *old_memory;
static char *new_memory;
static char *old_memory_end;
static char *new_memory_end;
static char *b;

void *
move (void *vp)
{
  char *p = vp;
  desc_t d;
  char *np;
  char *nb;

  ;
  ;
  ;
  ;
  if (!((unsigned int) ((char *) p - old_memory) < params.tosize))
    return vp;
  else;
  if ((unsigned int) ((char *) *((void **) p) - new_memory) < params.tosize)
    return *((void **) p);
  else;
  d = *((desc_t *) p);
  np = b;
  nb = np + (*d).asize;
  if (nb >= new_memory_end)
    error ("b overrun.");
  else;
  MEMCPY (np, p, (*d).size);
  b = nb;
  *((void **) p) = np;
  return np;
}

struct gc_breadth_first_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp2;
  char *s;
  void **link;
  char *p;
  desc_t d;
  char *tmp;
  int i;
  sht scan;
};

void
gc_breadth_first (char *esp, sht scan)
{
  char *argp;
  struct gc_breadth_first_frame *efp;
  closure_t *tmp_fp;

lgoto:
  ;
  efp = (struct gc_breadth_first_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct gc_breadth_first_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan = scan;
  if (params.gcv)
    {
      printf ("BREADTH_FIRST_GC start\n");
    }
  b = new_memory;
  efp->s = b;
  {
    char *goto_fr;

    tmp_fp = efp->scan;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	{
	  efp->tmp_esp = goto_fr;
	  return;
	}
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
  }
  if (efp->s < b)
    do
      {
	efp->d = *((desc_t *) efp->s);
	efp->p = (char *) efp->s;
	{
	  efp->i = 0;
	  if (efp->i < (*efp->d).fli_len)
	    do
	      {
		efp->link = (void **) (efp->p + ((*efp->d).fli)[efp->i]);
		efp->tmp2 = move (*efp->link);
		*efp->link = efp->tmp2;
		++efp->i;
	      }
	    while (efp->i < (*efp->d).fli_len);
	}
	efp->s += (*efp->d).asize;
      }
    while (efp->s < b);
  allocated_size = b - new_memory;
  efp->tmp = new_memory;
  new_memory = old_memory;
  old_memory = efp->tmp;
  efp->tmp = new_memory_end;
  new_memory_end = old_memory_end;
  old_memory_end = efp->tmp;
  if (params.gcv)
    {
      printf ("GC complete (%d)\n", allocated_size);
    }
}
double gc_ttime;
struct timeval tp1;
struct timeval tp2;

struct gc_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  sht scan;
};

void
gc (char *esp, sht scan)
{
  struct gc_frame *efp;

lgoto:
  ;
  efp = (struct gc_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct gc_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan = scan;
  gettimeofday (&tp1, 0);
  switch (params.gctype)
    {
    case 0:
      while (gc_breadth_first (esp, efp->scan),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
      break;
    }
  gettimeofday (&tp2, 0);
  gc_ttime += tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
}

void
getmem_init (gc_params p)
{
  char *mp;
  static int called = 0;

  ;
  ;
  if (called)
    return;
  else;
  called = 1;
  gc_ttime = 0.0;
  params = p;
  if (params.tosize == 0)
    params.tosize = 5 * 1024 * 1024;
  else;
  params.tosize += 3;
  params.tosize -= params.tosize & 3;
  if (params.stack_size == 0)
    params.stack_size = params.tosize / sizeof (double);
  else;
  if (params.limited_stack_max == 0)
    params.limited_stack_max = 32;
  else;
  printf ("tosize=%d, stack=%d, limit=%d\n", params.tosize, params.stack_size,
	  params.limited_stack_max);
  old_memory = myalloc (params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = myalloc (params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
  {
    mp = new_memory;
    if (mp < new_memory_end)
      do
	{
	  *mp = 1;
	  mp += 4096;
	}
      while (mp < new_memory_end);
    else;
  }
  {
    mp = old_memory;
    if (mp < old_memory_end)
      do
	{
	  *mp = 1;
	  mp += 4096;
	}
      while (mp < old_memory_end);
    else;
  }
}

void *
try_getmem (desc_t d)
{
  size_t size;
  char *p;

  ;
  ;
  size = (*d).asize;
  if (allocated_size + size > params.tosize)
    return 0;
  else;
  p = old_memory + allocated_size;
  allocated_size += size;
  memset (p, 0, size);
  *((desc_t *) p) = d;
  return p;
}

struct getmem_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *p;
  desc_t d;
  sht scan;
};

void *
getmem (char *esp, sht scan, desc_t d)
{
  struct getmem_frame *efp;

lgoto:
  ;
  efp = (struct getmem_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct getmem_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan = scan;
  efp->d = d;
  efp->p = try_getmem (efp->d);
  if (efp->p == 0)
    {
      while (gc (esp, efp->scan), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return (void *) 0 - 1;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
      efp->p = try_getmem (efp->d);
      if (efp->p == 0)
	{
	  error ("getmem: Not enough memory.");
	}
    }
  return efp->p;
}

struct _Bintree
{
  desc_t d;
  int key;
  int val;
  struct _Bintree *left;
  struct _Bintree *right;
};
typedef struct _Bintree Bintree;
size_t Bintree_node[] =
    { (unsigned int) &(*((Bintree *) 0)).left,
(unsigned int) &(*((Bintree *) 0)).right };
descriptor Bintree_d =
    { sizeof (Bintree),
sizeof (align_t) * ((sizeof (Bintree) + sizeof (align_t) - 1) / sizeof (align_t)),
sizeof (Bintree_node) / sizeof (size_t), Bintree_node };
static int maxins;
static int maxsearch;

struct newBintree_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *tmp3;
  Bintree *p;
  int v;
  int k;
  sht scan0;
  struct unnamed1 *tmp_arg;
};

Bintree *
newBintree (char *esp, sht scan0, int k, int v)
{
  struct newBintree_frame *efp;
  struct unnamed1 *tmp_arg;

lgoto:
  ;
  efp = (struct newBintree_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct newBintree_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan0 = scan0;
  efp->k = k;
  efp->v = v;
  tmp_arg = &Bintree_d;
  while ((efp->tmp3 = getmem (esp, efp->scan0, tmp_arg)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return (Bintree *) 0 - 1;
      efp->tmp_esp = 0;
      goto lgoto;
    }
  efp->p = (Bintree *) efp->tmp3;
  (*efp->p).key = efp->k;
  (*efp->p).val = efp->v;
  return efp->p;
}

struct insert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct _Bintree *tmp5;
  struct _Bintree *tmp4;
  Bintree *y;
  int v;
  int k;
  Bintree *x;
  sht scan0;
  closure_t scan1;
};

struct scan1_in_insert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct insert_frame *xfp;
};

char *
scan1_in_insert (char *esp, void *xfp0)
{
  struct scan1_in_insert_frame *efp;
  struct insert_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
  closure_t *tmp_fp2;

lgoto:
  ;
  efp = (struct scan1_in_insert_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct scan1_in_insert_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  xfp->x = move (xfp->x);
  xfp->y = move (xfp->y);
  {
    char *goto_fr;

    tmp_fp2 = xfp->scan0;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp2;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
  }
  return 0;
}

void
insert (char *esp, sht scan0, Bintree * x, int k, int v)
{
  struct insert_frame *efp;

lgoto:
  ;
  efp = (struct insert_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct insert_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan0 = scan0;
  efp->x = x;
  efp->k = k;
  efp->v = v;
  efp->scan1.fun = scan1_in_insert;
  efp->scan1.fr = (void *) efp;
  efp->y = 0;
  do
    {
      if ((*efp->x).key == efp->k)
	{
	  (*efp->x).val = efp->v;
	  return;
	}
      else if (efp->k < (*efp->x).key)
	{
	  efp->y = (*efp->x).left;
	  if (!efp->y)
	    {
	      while ((efp->tmp4 =
		      newBintree (esp, &efp->scan1, efp->k,
				  efp->v)) == (struct _Bintree *) 0 - 1
		     && (efp->tmp_esp = *((char **) esp)) != 0)
		{
		  if ((char *) efp->tmp_esp < (char *) efp)
		    return;
		  efp->tmp_esp = 0;
		  goto lgoto;
		}
	      (*efp->x).left = efp->tmp4;
	      return;
	    }
	}
      else
	{
	  efp->y = (*efp->x).right;
	  if (!efp->y)
	    {
	      while ((efp->tmp5 =
		      newBintree (esp, &efp->scan1, efp->k,
				  efp->v)) == (struct _Bintree *) 0 - 1
		     && (efp->tmp_esp = *((char **) esp)) != 0)
		{
		  if ((char *) efp->tmp_esp < (char *) efp)
		    return;
		  efp->tmp_esp = 0;
		  goto lgoto;
		}
	      (*efp->x).right = efp->tmp5;
	      return;
	    }
	}
      efp->x = efp->y;
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
  Bintree *x;
  sht scan0;
};

int
search (char *esp, sht scan0, Bintree * x, int k, int v0)
{
  struct search_frame *efp;

lgoto:
  ;
  efp = (struct search_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct search_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan0 = scan0;
  efp->x = x;
  efp->k = k;
  efp->v0 = v0;
  if (efp->x)
    do
      {
	if ((*efp->x).key == efp->k)
	  {
	    return (*efp->x).val;
	  }
	else if (efp->k < (*efp->x).key)
	  {
	    efp->x = (*efp->x).left;
	  }
	else
	  {
	    efp->x = (*efp->x).right;
	  }
      }
    while (efp->x);
  return efp->v0;
}

struct randinsert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  unsigned short seed[3];
  int k;
  int i;
  int n;
  Bintree *this;
  sht scan0;
  closure_t scan1;
};

struct scan1_in_randinsert_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct randinsert_frame *xfp;
};

char *
scan1_in_randinsert (char *esp, void *xfp0)
{
  struct scan1_in_randinsert_frame *efp;
  struct randinsert_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
  closure_t *tmp_fp3;

lgoto:
  ;
  efp = (struct scan1_in_randinsert_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct scan1_in_randinsert_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  xfp->this = move (xfp->this);
  {
    char *goto_fr;

    tmp_fp3 = xfp->scan0;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp3;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
  }
  return 0;
}

void
randinsert (char *esp, sht scan0, Bintree * this, int n)
{
  struct randinsert_frame *efp;

lgoto:
  ;
  efp = (struct randinsert_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct randinsert_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan0 = scan0;
  efp->this = this;
  efp->n = n;
  efp->scan1.fun = scan1_in_randinsert;
  efp->scan1.fr = (void *) efp;
  (efp->seed)[0] = 3;
  (efp->seed)[1] = 4;
  (efp->seed)[2] = 5;
  {
    efp->i = 0;
    if (efp->i < efp->n)
      do
	{
	  efp->k = nrand48 (efp->seed);
	  while (insert (esp, &efp->scan1, efp->this, efp->k, efp->k),
		 (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	  (efp->i)++;
	}
      while (efp->i < efp->n);
  }
}

struct randsearch_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  unsigned short seed[3];
  int k;
  int i;
  int n;
  Bintree *this;
  sht scan0;
  closure_t scan1;
};

struct scan1_in_randsearch_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct randsearch_frame *xfp;
};

char *
scan1_in_randsearch (char *esp, void *xfp0)
{
  struct scan1_in_randsearch_frame *efp;
  struct randsearch_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
  closure_t *tmp_fp4;

lgoto:
  ;
  efp = (struct scan1_in_randsearch_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct scan1_in_randsearch_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  xfp->this = move (xfp->this);
  {
    char *goto_fr;

    tmp_fp4 = xfp->scan0;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp4;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
  }
  return 0;
}

void
randsearch (char *esp, sht scan0, Bintree * this, int n)
{
  struct randsearch_frame *efp;

lgoto:
  ;
  efp = (struct randsearch_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct randsearch_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan0 = scan0;
  efp->this = this;
  efp->n = n;
  efp->scan1.fun = scan1_in_randsearch;
  efp->scan1.fr = (void *) efp;
  (efp->seed)[0] = 8;
  (efp->seed)[1] = 9;
  (efp->seed)[2] = 10;
  {
    efp->i = 0;
    if (efp->i < efp->n)
      do
	{
	  efp->k = nrand48 (efp->seed);
	  while (search (esp, &efp->scan1, efp->this, efp->k, 0),
		 (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	  (efp->i)++;
	}
      while (efp->i < efp->n);
  }
}

void
init (int tp, int tosize, int stack_size, int limited_max)
{
  gc_params p;

  ;
  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init (p);
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int ifexp_result9;
  int tmp12;
  int ifexp_result8;
  int tmp11;
  int ifexp_result7;
  int tmp10;
  int ifexp_result5;
  int tmp9;
  int ifexp_result4;
  int tmp8;
  int ifexp_result3;
  int tmp7;
  int ifexp_result2;
  int tmp6;
  double ttime;
  Bintree *root;
  int searchskip;
  int gctp;
  int i;
  struct timeval tp2;
  struct timeval tp1;
  char **argv;
  int argc;
  struct unnamed1 *tmp_arg2;
  closure_t scan1;
};

struct scan1_in_main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct main_frame *xfp;
};

char *
scan1_in_main (char *esp, void *xfp0)
{
  struct scan1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  char *parmp = (char *) esp;

lgoto:
  ;
  efp = (struct scan1_in_main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct scan1_in_main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  xfp->root = move (xfp->root);
  return 0;
}

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char estack[65536];
  char *esp = estack;
  struct unnamed1 *tmp_arg2;

lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->argc = argc;
  efp->argv = argv;
  efp->scan1.fun = scan1_in_main;
  efp->scan1.fr = (void *) efp;
  efp->searchskip = 0;
  if (efp->argc > 1)
    {
      efp->tmp6 = strcmp ((efp->argv)[1], "-s");
      efp->ifexp_result2 = efp->tmp6 == 0 ? 1 : 0;
    }
  else
    {
      efp->ifexp_result2 = 0;
    }
  if (efp->ifexp_result2)
    {
      --efp->argc;
      ++efp->argv;
      ++efp->searchskip;
    }
  if (efp->argc > 1)
    {
      efp->tmp7 = atoi ((efp->argv)[1]);
      efp->ifexp_result3 = efp->tmp7;
    }
  else
    {
      efp->ifexp_result3 = 100000;
    }
  maxins = efp->ifexp_result3;
  if (efp->argc > 2)
    {
      efp->tmp8 = atoi ((efp->argv)[2]);
      efp->ifexp_result4 = efp->tmp8;
    }
  else
    {
      efp->ifexp_result4 = 300000;
    }
  maxsearch = efp->ifexp_result4;
  printf ("Bintree=%d, masins=%d, maxsearch=%d\n", sizeof (Bintree), maxins,
	  maxsearch);
  if (efp->argc > 3)
    {
      efp->tmp9 = atoi ((efp->argv)[3]);
      efp->ifexp_result5 = efp->tmp9;
    }
  else
    {
      efp->ifexp_result5 = 0;
    }
  efp->gctp = efp->ifexp_result5;
  if (efp->argc > 4)
    {
      efp->tmp10 = atoi ((efp->argv)[4]);
      efp->ifexp_result7 = efp->tmp10;
    }
  else
    {
      efp->ifexp_result7 = 0;
    }
  if (efp->argc > 5)
    {
      efp->tmp11 = atoi ((efp->argv)[5]);
      efp->ifexp_result8 = efp->tmp11;
    }
  else
    {
      efp->ifexp_result8 = 0;
    }
  if (efp->argc > 6)
    {
      efp->tmp12 = atoi ((efp->argv)[6]);
      efp->ifexp_result9 = efp->tmp12;
    }
  else
    {
      efp->ifexp_result9 = 0;
    }
  init (efp->gctp < 0 ? 0 : efp->gctp, efp->ifexp_result7, efp->ifexp_result8,
	efp->ifexp_result9);
  tmp_arg2 = &Bintree_d;
  while ((efp->root = getmem (esp, &efp->scan1, tmp_arg2)) == (void *) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return (int) 0 - 1;
      efp->tmp_esp = 0;
      goto lgoto;
    }
  (*efp->root).key = 0;
  (*efp->root).val = 0;
  gettimeofday (&efp->tp1, 0);
  while (randinsert (esp, &efp->scan1, efp->root, maxins),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return (int) 0 - 1;
      efp->tmp_esp = 0;
      goto lgoto;
    }
  if (efp->gctp >= 0)
    while (gc (esp, &efp->scan1), (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return (int) 0 - 1;
	efp->tmp_esp = 0;
	goto lgoto;
      }
  gettimeofday (&efp->tp2, 0);
  if (!efp->searchskip)
    while (randsearch (esp, &efp->scan1, efp->root, maxsearch),
	   (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return (int) 0 - 1;
	efp->tmp_esp = 0;
	goto lgoto;
      }
  efp->ttime =
      (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec -
					       (efp->tp1).tv_usec) * 1.0e-6;
  printf ("total=%f gc=%f other=%f\n", efp->ttime, gc_ttime,
	  efp->ttime - gc_ttime);
  return 0;
}
