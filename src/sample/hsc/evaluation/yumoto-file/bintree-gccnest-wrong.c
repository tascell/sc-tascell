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
typedef void    (*sht) (void);

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

extern void    *getmem(sht, desc_t);

extern void     gc(sht);

void 
gc_breadth_first(sht scan)
{
  int             i;
  char           *tmp;
  desc_t          d;
  char           *p;
  void          **link;
  char           *s;
  if (params.gcv)
    printf("BREADTH_FIRST_GC start\n");
  b = new_memory;
  s = b;
  scan();
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
  if (params.gcv)
    printf("GC complete (%d)\n", allocated_size);
}

void 
gc(sht scan)
{
  struct timeval  tp1;
  struct timeval  tp2;
  gettimeofday(&tp1, 0);
  switch (params.gctype) {
  case 0:
    gc_breadth_first(scan);
    break;
  }
  gettimeofday(&tp2, 0);
  gc_ttime += tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
}

void           *
getmem(sht scan, desc_t d)
{
  void           *p;
  p = try_getmem(d);
  if (p == 0) {
    gc(scan);
    p = try_getmem(d);
    if (p == 0)
      error("getmem: Not enough memory.");
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

Bintree        *
newBintree(sht scan0, int k, int v)
{
  struct _Bintree *tmp_object;
  Bintree        *p = 0;

  void            scan1(void){
    p = move(p);
    scan0();
  }
                  p = (((((tmp_object = (struct _Bintree *) getmem(scan1, &_Bintree_d), (*tmp_object).key = k), (*tmp_object).val = v), (*tmp_object).left = 0), (*tmp_object).right = 0), tmp_object);
  return p;
}

void 
insert(sht scan0, Bintree * x, int k, int v)
{
  Bintree        *y = 0;

  void            scan1(void){
    x = move(x);
    y = move(y);
    scan0();
  }
  do {
    if ((*x).key == k) {
      (*x).val = v;
      return;
    } else if (k < (*x).key) {
      y = (*x).left;
      if (!y) {
	(*x).left = newBintree(scan1, k, v);
	return;
      }
    } else {
      y = (*x).right;
      if (!y) {
	(*x).right = newBintree(scan1, k, v);
	return;
      }
    }
    x = y;
  } while (1);
}

int 
search(sht scan0, Bintree * x, int k, int v0)
{

  void            scan1(void){
    x = move(x);
    scan0();
  }
  if              (x)
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

void 
randinsert(sht scan0, Bintree * this, int n)
{
  int             i;
  int             k;
  unsigned short  seed[3];

  void            scan1(void){
    this = move(this);
    scan0();
  }
                  seed[0] = 3;
  seed[1] = 4;
  seed[2] = 5;
  {
    i = 0;
    if (i < n)
      do {
	k = nrand48(seed);
	insert(scan1, this, k, k);
      } while (i++, i < n);
  }
}

void 
randsearch(sht scan0, Bintree * this, int n)
{
  int             i;
  int             k;
  unsigned short  seed[3];

  void            scan1(void){
    this = move(this);
    scan0();
  }
                  seed[0] = 8;
  seed[1] = 9;
  seed[2] = 10;
  {
    i = 0;
    if (i < n)
      do {
	k = nrand48(seed);
	search(scan1, this, k, 0);
      } while (i++, i < n);
  }
}

int 
main(int argc, char **argv)
{
  struct _Bintree *tmp_object2;
  struct timeval  tp1;
  struct timeval  tp2;
  int             searchskip = 0;
  double          ttime;
  Bintree        *root;
  int             gctp;

  void            scan1(void){
    root = move(root);
    return;
  }
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
  root = (((((tmp_object2 = (struct _Bintree *) getmem(scan1, &_Bintree_d), (*tmp_object2).key = 0), (*tmp_object2).val = 0), (*tmp_object2).left = 0), (*tmp_object2).right = 0), tmp_object2);
  gettimeofday(&tp1, 0);
  randinsert(scan1, root, maxins);
  (*root).key = 0;
  (*root).val = 0;
  (*root).left = 0;
  (*root).right = 0;
  randinsert(scan1, root, maxins);
  gettimeofday(&tp2, 0);
  if (!searchskip)
    randsearch(scan1, root, maxsearch);
  ttime = tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
  printf("total=%f gc=%f other=%f\n", ttime, gc_ttime, ttime - gc_ttime);
  return 0;
}
