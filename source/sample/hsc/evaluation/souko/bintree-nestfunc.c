typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t        fun;
  void           *fr;
}               closure_t;
typedef double  Align_t;

char           *lw_call(char *esp);
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short   __int16_t;
typedef unsigned short __uint16_t;
typedef int     __int32_t;
typedef unsigned int __uint32_t;
typedef int     __int64_t;
typedef unsigned int __uint64_t;
typedef unsigned long __clock_t;
typedef unsigned int __cpumask_t;
typedef __int32_t __critical_t;
typedef double  __double_t;
typedef double  __float_t;
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
typedef long    __key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int     __nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;
typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long    __suseconds_t;
typedef __int32_t __timer_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;
typedef int     __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;
typedef __uint32_t __dev_t;
typedef __uint32_t __fixpt_t;

union unnamed0 {
  char            __mbstate8[128];
  __int64_t       _MBSTATEl;
};
typedef union unnamed0 __mbstate_t;
typedef __ptrdiff_t ptrdiff_t;
typedef __rune_t rune_t;
typedef __size_t size_t;
typedef __wchar_t wchar_t;
typedef double  align_t;

struct unnamed1 {
  size_t          size;
  size_t          asize;
  int             fli_len;
  size_t         *fli;
};
typedef struct unnamed1 descriptor;
typedef struct unnamed1 *desc_t;

struct unnamed2 {
  int             gcv;
  int             gctype;
  size_t          tosize;
  size_t          stack_size;
  size_t          limited_stack_max;
};
typedef struct unnamed2 gc_params;
typedef closure_t *sht;

extern void     getmem_init(gc_params);

extern void    *move(void *vp);
extern double   gc_ttime;

long            nrand48(unsigned short xseed[3]);

int             strcmp(char const *s1, char const *s2);

void           *memset(void *b, int c, size_t len);

void           *memcpy(void *dest, void const *src, size_t len);

int             atoi(char const *nptr);

int             printf(char const *format,...);

void           *malloc(size_t size);

void           *alloca(size_t size);

void           *realloc(void *pt, size_t size);

void            free(void *pt);

struct timeval {
  long            tv_sec;
  long            tv_usec;
};

struct timezone {
  desc_t          d;
  int             tz_minuteswest;
  int             tz_dsttime;
};
size_t          timezone_node[] = {};
descriptor      timezone_d = {sizeof(struct timezone), sizeof(align_t) * ((sizeof(struct timezone) + sizeof(align_t) - 1) / sizeof(align_t)), sizeof(timezone_node) / sizeof(size_t), timezone_node};

int             gettimeofday(struct timeval * tp, struct timezone * tzp);

void           *
MEMCPY(void *d, void const *s, size_t sz)
{
  long           *_des = (long *)d;
  long           *_src = (long *)s;
  long           *_til = (long *)((char *)_src + sz);
  do {
    *_des++ = *_src++;
  } while (_src < _til);
}

void            exit(int);

static void 
error(char *s)
{
  printf("ERROR: %s\n", s);
  exit(1);
}

static void    *
myalloc(size_t size)
{
  void           *p;
  p = malloc(size);
  if (p == (void *)0)
    error("Not enough memory.");
  return p;
}

static void    *
myrealloc(void *p, size_t size)
{
  void           *q;
  q = realloc(p, size);
  if (q == (void *)0)
    error("Not enough memory.");
  return q;
}
gc_params       params;
static int      allocated_size;
static char    *old_memory;
static char    *new_memory;
static char    *old_memory_end;
static char    *new_memory_end;
static char    *b;

void           *
move(void *vp)
{
  char           *p = vp;
  desc_t          d;
  char           *np;
  char           *nb;
  if (!((unsigned int)((char *)p - old_memory) < params.tosize))
    return vp;
  if ((unsigned int)((char *)*((void **)p) - new_memory) < params.tosize)
    return *((void **)p);
  d = *((desc_t *) p);
  np = b;
  nb = np + (*d).asize;
  if (nb >= new_memory_end)
    error("buffer overrun.");
  MEMCPY(np, p, (*d).size);
  b = nb;
  *((void **)p) = np;
  return np;
}
double          gc_ttime;

void 
getmem_init(gc_params p)
{
  char           *mp;
  static int      called = 0;
  if (called)
    return;
  called = 1;
  gc_ttime = 0.0;
  params = p;
  if (params.tosize == 0)
    params.tosize = 5 * 1024 * 1024;
  params.tosize += 3;
  params.tosize -= params.tosize & 3;
  if (params.stack_size == 0)
    params.stack_size = params.tosize / sizeof(double);
  if (params.limited_stack_max == 0)
    params.limited_stack_max = 32;
  printf("tosize=%d, stack=%d, limit=%d\n", params.tosize, params.stack_size, params.limited_stack_max);
  old_memory = myalloc(params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = myalloc(params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
}

void           *
try_getmem(desc_t d)
{
  size_t          size;
  char           *p;
  size = (*d).asize;
  if (allocated_size + size > params.tosize)
    return 0;
  p = old_memory + allocated_size;
  allocated_size += size;
  memset(p, 0, size);
  *((desc_t *) p) = d;
  return p;
}

extern void    *getmem(char *esp, sht, desc_t);

extern void     gc(char *esp, sht);

struct gc_breadth_first_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  char           *s;
  void          **link;
  char           *p;
  desc_t          d;
  char           *tmp;
  int             i;
  sht             scan;
};

void 
gc_breadth_first(char *esp, sht scan)
{
  char           *argp;
  struct gc_breadth_first_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  int             i;
  char           *tmp;
  desc_t          d;
  char           *p;
  void          **link;
  char           *s;
  closure_t      *tmp_fp;
  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct gc_breadth_first_frame *) esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct gc_breadth_first_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
lgoto:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL;
    }
    goto L_CALL;
  }
  efp = (struct gc_breadth_first_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct gc_breadth_first_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (params.gcv) {
    printf("BREADTH_FIRST_GC start\n");
  }
  b = new_memory;
  s = b;
  {
    tmp_fp = scan;
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = tmp_fp;
    efp->s = s;
    efp->link = link;
    efp->p = p;
    efp->d = d;
    efp->tmp = tmp;
    efp->i = i;
    efp->scan = scan;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return;
L_CALL:
    ;
    s = efp->s;
    link = efp->link;
    p = efp->p;
    d = efp->d;
    tmp = efp->tmp;
    i = efp->i;
    scan = efp->scan;
  }
  if (s < b)
    do {
      d = *((desc_t *) s);
      p = (char *)s;
      {
	i = 0;
	if (i < (*d).fli_len)
	  do {
	    link = (void **)(p + ((*d).fli)[i]);
	    *link = move(*link);
	  } while (++i, i < (*d).fli_len);
      }
      s += (*d).asize;
    } while (s < b);
  allocated_size = b - new_memory;
  tmp = new_memory;
  new_memory = old_memory;
  old_memory = tmp;
  tmp = new_memory_end;
  new_memory_end = old_memory_end;
  old_memory_end = tmp;
  if (params.gcv) {
    printf("GC complete (%d)\n", allocated_size);
  }
}

struct gc_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct timeval  tp2;
  struct timeval  tp1;
  sht             scan;
};

void 
gc(char *esp, sht scan)
{
  struct gc_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct gc_frame *) esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct gc_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
lgoto:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL2;
    }
    goto L_CALL2;
  }
  efp = (struct gc_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct gc_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  gettimeofday(&efp->tp1, 0);
  switch (params.gctype) {
  case 0:
    new_esp = esp;
    while (gc_breadth_first(new_esp, scan), (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->scan = scan;
      efp->call_id = 0;
      return;
  L_CALL2:
      ;
      scan = efp->scan;
      new_esp = esp + 1;
    }
    break;
  }
  gettimeofday(&efp->tp2, 0);
  gc_ttime += (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec - (efp->tp1).tv_usec) * 1.0e-6;
}

struct getmem_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  void           *p;
  desc_t          d;
  sht             scan;
};

void           *
getmem(char *esp, sht scan, desc_t d)
{
  struct getmem_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  void           *p;
  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct getmem_frame *) esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct getmem_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
lgoto:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL3;
    }
    goto L_CALL3;
  }
  efp = (struct getmem_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct getmem_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  p = try_getmem(d);
  if (p == 0) {
    new_esp = esp;
    while (gc(new_esp, scan), (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->p = p;
      efp->d = d;
      efp->scan = scan;
      efp->call_id = 0;
      return (void *)0 - 1;
  L_CALL3:
      ;
      p = efp->p;
      d = efp->d;
      scan = efp->scan;
      new_esp = esp + 1;
    }
    p = try_getmem(d);
    if (p == 0) {
      error("getmem: Not enough memory.");
    }
  }
  return p;
}

void 
gc_init(int tp, int tosize, int stack_size, int limited_max)
{
  gc_params       p;
  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init(p);
}

struct _Bintree {
  desc_t          d;
  int             key;
  int             val;
  struct _Bintree *left;
  struct _Bintree *right;
};
size_t          _Bintree_node[] = {(size_t) & (*((struct _Bintree *) 0)).left, (size_t) & (*((struct _Bintree *) 0)).right};
descriptor      _Bintree_d = {sizeof(struct _Bintree), sizeof(align_t) * ((sizeof(struct _Bintree) + sizeof(align_t) - 1) / sizeof(align_t)), sizeof(_Bintree_node) / sizeof(size_t), _Bintree_node};
typedef struct _Bintree Bintree;
static int      maxins;
static int      maxsearch;

struct newBintree_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  Bintree        *p;
  struct _Bintree *tmp_object;
  int             v;
  int             k;
  sht             scan0;
  closure_t       scan1;
};

struct scan1_in_newBintree_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct newBintree_frame *xfp;
};

char           *
scan1_in_newBintree(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_newBintree_frame *efp;
  struct newBintree_frame *xfp = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  char           *argp;
  closure_t      *tmp_fp2;
lgoto:
  ;
  efp = (struct scan1_in_newBintree_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_newBintree_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->p = move(xfp->p);
  {
    char           *goto_fr;
    tmp_fp2 = xfp->scan0;
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = tmp_fp2;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto lgoto;
      }
    xfp = efp->xfp;
  }
  return 0;
}

Bintree        *
newBintree(char *esp, sht scan0, int k, int v)
{
  struct newBintree_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  struct _Bintree *tmp_object;
  Bintree        *p;
lgoto:
  ;
  efp = (struct newBintree_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct newBintree_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  p = 0;
  p = (((((tmp_object = (struct _Bintree *) getmem(&efp->scan1, &_Bintree_d), (*tmp_object).key = k), (*tmp_object).val = v), (*tmp_object).left = 0), (*tmp_object).right = 0), tmp_object);
  return p;
}

struct insert_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  Bintree        *y;
  int             v;
  int             k;
  Bintree        *x;
  sht             scan0;
  closure_t       scan1;
};

struct scan1_in_insert_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct insert_frame *xfp;
};

char           *
scan1_in_insert(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_insert_frame *efp;
  struct insert_frame *xfp = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  char           *argp;
  closure_t      *tmp_fp3;
lgoto:
  ;
  efp = (struct scan1_in_insert_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_insert_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->x = move(xfp->x);
  xfp->y = move(xfp->y);
  {
    char           *goto_fr;
    tmp_fp3 = xfp->scan0;
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = tmp_fp3;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto lgoto;
      }
    xfp = efp->xfp;
  }
  return 0;
}

void 
insert(char *esp, sht scan0, Bintree * x, int k, int v)
{
  struct insert_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  Bintree        *y;
  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct insert_frame *) esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct insert_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
lgoto:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL4;
    case 1:
      goto L_CALL5;
    }
    goto L_CALL4;
  }
  efp = (struct insert_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct insert_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  y = 0;
  do {
    if ((*x).key == k) {
      (*x).val = v;
      return;
    } else if (k < (*x).key) {
      y = (*x).left;
      if (!y) {
	new_esp = esp;
	while (((*x).left = newBintree(new_esp, &efp->scan1, k, v)) == (struct _Bintree *) 0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	  efp->y = y;
	  efp->v = v;
	  efp->k = k;
	  efp->x = x;
	  efp->scan0 = scan0;
	  efp->scan1.fun = scan1_in_insert;
	  efp->scan1.fr = (void *)efp;
	  efp->call_id = 0;
	  return;
      L_CALL4:
	  ;
	  y = efp->y;
	  v = efp->v;
	  k = efp->k;
	  x = efp->x;
	  scan0 = efp->scan0;
	  new_esp = esp + 1;
	}
	return;
      }
    } else {
      y = (*x).right;
      if (!y) {
	new_esp = esp;
	while (((*x).right = newBintree(new_esp, &efp->scan1, k, v)) == (struct _Bintree *) 0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	  efp->y = y;
	  efp->v = v;
	  efp->k = k;
	  efp->x = x;
	  efp->scan0 = scan0;
	  efp->scan1.fun = scan1_in_insert;
	  efp->scan1.fr = (void *)efp;
	  efp->call_id = 1;
	  return;
      L_CALL5:
	  ;
	  y = efp->y;
	  v = efp->v;
	  k = efp->k;
	  x = efp->x;
	  scan0 = efp->scan0;
	  new_esp = esp + 1;
	}
	return;
      }
    }
    x = y;
  } while (1);
}

struct search_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  int             v0;
  int             k;
  Bintree        *x;
  sht             scan0;
  closure_t       scan1;
};

struct scan1_in_search_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct search_frame *xfp;
};

char           *
scan1_in_search(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_search_frame *efp;
  struct search_frame *xfp = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  char           *argp;
  closure_t      *tmp_fp4;
lgoto:
  ;
  efp = (struct scan1_in_search_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_search_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->x = move(xfp->x);
  {
    char           *goto_fr;
    tmp_fp4 = xfp->scan0;
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = tmp_fp4;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto lgoto;
      }
    xfp = efp->xfp;
  }
  return 0;
}

int 
search(char *esp, sht scan0, Bintree * x, int k, int v0)
{
  struct search_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
lgoto:
  ;
  efp = (struct search_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct search_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (x)
    do {
      if ((*x).key == k) {
	return (*x).val;
      } else if (k < (*x).key) {
	x = (*x).left;
      } else {
	x = (*x).right;
      }
    } while (x);
  return v0;
}

struct randinsert_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  unsigned short  seed[3];
  int             k;
  int             i;
  int             n;
  Bintree        *this;
  sht             scan0;
  closure_t       scan1;
};

struct scan1_in_randinsert_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct randinsert_frame *xfp;
};

char           *
scan1_in_randinsert(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_randinsert_frame *efp;
  struct randinsert_frame *xfp = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  char           *argp;
  closure_t      *tmp_fp5;
lgoto:
  ;
  efp = (struct scan1_in_randinsert_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_randinsert_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->this = move(xfp->this);
  {
    char           *goto_fr;
    tmp_fp5 = xfp->scan0;
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = tmp_fp5;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto lgoto;
      }
    xfp = efp->xfp;
  }
  return 0;
}

void 
randinsert(char *esp, sht scan0, Bintree * this, int n)
{
  struct randinsert_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  int             i;
  int             k;
  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct randinsert_frame *) esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct randinsert_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
lgoto:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL6;
    }
    goto L_CALL6;
  }
  efp = (struct randinsert_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct randinsert_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  (efp->seed)[0] = 3;
  (efp->seed)[1] = 4;
  (efp->seed)[2] = 5;
  {
    i = 0;
    if (i < n)
      do {
	k = nrand48(efp->seed);
	new_esp = esp;
	while (insert(new_esp, &efp->scan1, this, k, k), (efp->tmp_esp = *((char **)esp)) != 0) {
	  efp->k = k;
	  efp->i = i;
	  efp->n = n;
	  efp->this = this;
	  efp->scan0 = scan0;
	  efp->scan1.fun = scan1_in_randinsert;
	  efp->scan1.fr = (void *)efp;
	  efp->call_id = 0;
	  return;
      L_CALL6:
	  ;
	  k = efp->k;
	  i = efp->i;
	  n = efp->n;
	  this = efp->this;
	  scan0 = efp->scan0;
	  new_esp = esp + 1;
	}
      } while (i++, i < n);
  }
}

struct randsearch_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  unsigned short  seed[3];
  int             k;
  int             i;
  int             n;
  Bintree        *this;
  sht             scan0;
  closure_t       scan1;
};

struct scan1_in_randsearch_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct randsearch_frame *xfp;
};

char           *
scan1_in_randsearch(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_randsearch_frame *efp;
  struct randsearch_frame *xfp = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  char           *argp;
  closure_t      *tmp_fp6;
lgoto:
  ;
  efp = (struct scan1_in_randsearch_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_randsearch_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->this = move(xfp->this);
  {
    char           *goto_fr;
    tmp_fp6 = xfp->scan0;
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = tmp_fp6;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto lgoto;
      }
    xfp = efp->xfp;
  }
  return 0;
}

void 
randsearch(char *esp, sht scan0, Bintree * this, int n)
{
  struct randsearch_frame *efp;
  char           *new_esp;
  size_t          esp_flag = (size_t) esp & 3;
  int             i;
  int             k;
  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct randsearch_frame *) esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct randsearch_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
lgoto:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL7;
    }
    goto L_CALL7;
  }
  efp = (struct randsearch_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct randsearch_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  (efp->seed)[0] = 8;
  (efp->seed)[1] = 9;
  (efp->seed)[2] = 10;
  {
    i = 0;
    if (i < n)
      do {
	k = nrand48(efp->seed);
	new_esp = esp;
	while (search(new_esp, &efp->scan1, this, k, 0), (efp->tmp_esp = *((char **)esp)) != 0) {
	  efp->k = k;
	  efp->i = i;
	  efp->n = n;
	  efp->this = this;
	  efp->scan0 = scan0;
	  efp->scan1.fun = scan1_in_randsearch;
	  efp->scan1.fr = (void *)efp;
	  efp->call_id = 0;
	  return;
      L_CALL7:
	  ;
	  k = efp->k;
	  i = efp->i;
	  n = efp->n;
	  this = efp->this;
	  scan0 = efp->scan0;
	  new_esp = esp + 1;
	}
      } while (i++, i < n);
  }
}

struct main_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  int             gctp;
  Bintree        *root;
  double          ttime;
  int             searchskip;
  struct timeval  tp2;
  struct timeval  tp1;
  struct _Bintree *tmp_object2;
  char          **argv;
  int             argc;
  closure_t       scan1;
};

struct scan1_in_main_frame {
  char           *tmp_esp;
  char           *argp;
  int             call_id;
  struct main_frame *xfp;
};

char           *
scan1_in_main(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
lgoto:
  ;
  efp = (struct scan1_in_main_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->root = move(xfp->root);
  return 0;
  return 0;
}

int 
main(int argc, char **argv)
{
  struct main_frame *efp;
  char           *new_esp;
  char            estack[65536];
  char           *esp = estack;
  struct _Bintree *tmp_object2;
  int             searchskip;
  double          ttime;
  Bintree        *root;
  int             gctp;
lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  searchskip = 0;
  gctp = argc > 3 ? atoi(argv[3]) : 0;
  gc_init(gctp < 0 ? 0 : gctp, argc > 4 ? atoi(argv[4]) : 0, argc > 5 ? atoi(argv[5]) : 0, argc > 6 ? atoi(argv[6]) : 0);
  if (argc > 1 && strcmp(argv[1], "-s") == 0) {
    --argc;
    ++argv;
    ++searchskip;
  }
  maxins = argc > 1 ? atoi(argv[1]) : 100000;
  maxsearch = argc > 2 ? atoi(argv[2]) : 300000;
  printf("Bintree=%d, masins=%d, maxsearch=%d\n", sizeof(Bintree), maxins, maxsearch);
  root = (((((tmp_object2 = (struct _Bintree *) getmem(&efp->scan1, &_Bintree_d), (*tmp_object2).key = 0), (*tmp_object2).val = 0), (*tmp_object2).left = 0), (*tmp_object2).right = 0), tmp_object2);
  gettimeofday(&efp->tp1, 0);
  new_esp = esp;
  while (randinsert(new_esp, &efp->scan1, root, maxins), (efp->tmp_esp = *((char **)esp)) != 0) {
    char           *goto_fr;
    *((char **)esp) = 0;
    efp->gctp = gctp;
    efp->root = root;
    efp->ttime = ttime;
    efp->searchskip = searchskip;
    efp->tmp_object2 = tmp_object2;
    efp->argv = argv;
    efp->argc = argc;
    efp->scan1.fun = scan1_in_main;
    efp->scan1.fr = (void *)efp;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto lgoto;
    gctp = efp->gctp;
    root = efp->root;
    ttime = efp->ttime;
    searchskip = efp->searchskip;
    tmp_object2 = efp->tmp_object2;
    argv = efp->argv;
    argc = efp->argc;
    new_esp = esp + 1;
  }
  gettimeofday(&efp->tp2, 0);
  if (!searchskip) {
    new_esp = esp;
    while (randsearch(new_esp, &efp->scan1, root, maxsearch), (efp->tmp_esp = *((char **)esp)) != 0) {
      char           *goto_fr;
      *((char **)esp) = 0;
      efp->gctp = gctp;
      efp->root = root;
      efp->ttime = ttime;
      efp->searchskip = searchskip;
      efp->tmp_object2 = tmp_object2;
      efp->argv = argv;
      efp->argc = argc;
      efp->scan1.fun = scan1_in_main;
      efp->scan1.fr = (void *)efp;
      goto_fr = lw_call(efp->tmp_esp);
      if ((char *)goto_fr == (char *)efp)
	goto lgoto;
      gctp = efp->gctp;
      root = efp->root;
      ttime = efp->ttime;
      searchskip = efp->searchskip;
      tmp_object2 = efp->tmp_object2;
      argv = efp->argv;
      argc = efp->argc;
      new_esp = esp + 1;
    }
  }
  ttime = (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec - (efp->tp1).tv_usec) * 1.0e-6;
  printf("total=%f gc=%f other=%f\n", ttime, gc_ttime, ttime - gc_ttime);
  return 0;
}
