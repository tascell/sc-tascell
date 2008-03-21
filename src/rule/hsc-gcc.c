#include <sys/time.h>

void *malloc(size_t);

void *realloc(void *, size_t);

char *strncpy(char *, char const *, size_t);

int printf(char const *, ...);
typedef void (*sht) (void);
typedef double align_t;
typedef unsigned int size_t;

struct desc_struct {
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
};
typedef struct desc_struct descriptor;
typedef descriptor *desc_t;

struct array_object {
  desc_t d;
  int length;
  size_t el_size;
  size_t size;
  size_t asize;
  char body[0];
};

extern void *getmem(sht scan, size_t size);

extern void *move(void *vp);

void *MEMCPY(void *d, void const *s, size_t sz)
{
  long *_des = (long *)d;
  long *_src = (long *)s;
  long *_til = (long *)((char *)_src + sz);

  do {
    *_des++ = *_src++;
  } while (_src < _til);
}

static void error(char *s)
{
  printf("ERROR: %s\n", s);
  exit(1);
}

static void *myalloc(size_t size)
{
  void *p;

  p = malloc(size);
  if (p == (void *)0)
    error("Not enough memory.");
  return p;
}

static void *myrealloc(void *p, size_t size)
{
  void *q;

  q = realloc(p, size);
  if (q == (void *)0)
    error("Not enough memory.");
  return q;
}

struct gc_params_struct {
  int gcv;
  int gctype;
  size_t tosize;
  size_t stack_size;
  size_t limited_stack_max;
};
typedef struct gc_params_struct gc_params;
static gc_params params;
static int allocated_size;
static char *old_memory;
static char *new_memory;
static char *old_memory_end;
static char *new_memory_end;
static char *b;

void *move(void *vp)
{
  char *p = vp;
  void *fwp;
  size_t tag;
  desc_t d;
  struct array_object *ao;
  size_t size;
  size_t asize;
  char *np;
  char *nb;

  if (!((unsigned int)((char *)p - old_memory) < params.tosize))
    return vp;
  tag = 3 & *((size_t *) p);
  fwp = (void *)(tag ^ *((size_t *) p));
  if ((unsigned int)((char *)fwp - new_memory) < params.tosize)
    return fwp;
  switch (tag) {
  case 0:
    d = (desc_t) fwp;
    size = (*d).size;
    asize = (*d).asize;
    break;
  case 1:
  case 3:
    ao = (struct array_object *)vp;
    size = (*ao).size;
    asize = (*ao).asize;
    break;
  default:
    error("Illegal type ID!");
  }
  np = b;
  nb = np + asize;
  if (nb >= new_memory_end)
    error("buffer overrun.");
  MEMCPY(np, p, size);
  b = nb;
  *((void **)p) = np;
  return np;
}

void gc_breadth_first(sht scan)
{
  int i;
  char *tmp;
  size_t tag;
  int len;
  void **link;
  desc_t d;
  struct array_object *ao;
  size_t el_size;
  char *p;
  char *s;

  if (params.gcv)
    printf("BREADTH-FIRST-GC start\n");
  b = new_memory;
  s = b;
  scan();
  while (s < b) {
    tag = 3 & *((size_t *) s);
    switch (tag) {
    case 0:
      d = (desc_t) (tag ^ *((size_t *) s));
      p = (char *)s;
      len = (*d).fli_len;
      for (i = 0; i < len; ++i) {
	link = (void **)(p + ((*d).fli)[i]);
	*link = move(*link);
      }
      s += (*d).asize;
      break;
    case 1:
      ao = (struct array_object *)s;
      p = (*ao).body;
      len = (*ao).length;
      el_size = (*ao).el_size;
      for (i = 0; i < len; ++i) {
	link = (void **)p;
	*link = move(*link);
	p += el_size;
      }
      s += (*ao).asize;
      break;
    case 3:
      ao = (struct array_object *)s;
      s += (*ao).asize;
      break;
    default:
      error("Illegal type ID!");
    }
  }
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

double gc_ttime;
struct timeval tp1;
struct timeval tp2;

void gc(sht scan)
{
  gettimeofday(&tp1, 0);
  switch (params.gctype) {
  case 0:
    gc_breadth_first(scan);
    break;
  }
  gettimeofday(&tp2, 0);
  gc_ttime += tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
}

void getmem_init(gc_params p)
{
  char *mp;
  static int called = 0;

  if (called)
    return;
  else
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
  printf("tosize=%d, stack=%d, limit=%d\n", params.tosize, params.stack_size,
      params.limited_stack_max);
  old_memory = myalloc(params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = myalloc(params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
}

void gc_init(int tp, int tosize, int stack_size, int limited_max)
{
  gc_params p;

  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init(p);
}

void *try_getmem(size_t size)
{
  char *p;

  if (allocated_size + size > params.tosize)
    return 0;
  p = old_memory + allocated_size;
  allocated_size += size;
  memset(p, 0, size);
  return p;
}

void *getmem(sht scan, size_t size)
{
  void *p;

  p = try_getmem(size);
  if (p == 0) {
    gc(scan);
    p = try_getmem(size);
    if (p == 0)
      error("getmem: No enough memory.");
  }
  return p;
}

int main(int argc, char **argv)
{
  struct array_object *hsc_argv;
  size_t hsc_argv_size;
  size_t hsc_argv_asize;
  int i;
  struct array_object **body;
  int len;
  size_t strobj_size;
  size_t strobj_asize;

  void scan1(void) {
    argv = move(argv);
    return;
  }
  gc_init(0, 0, 0, 0);
  hsc_argv_size =
      sizeof(struct array_object) + argc * sizeof(struct array_object *);
  hsc_argv_asize =
      (hsc_argv_size + sizeof(align_t) +
      -1) / sizeof(align_t) * sizeof(align_t);
  hsc_argv = (struct array_object *)getmem(scan1, hsc_argv_asize);
  hsc_argv->d = (desc_t) 1;
  hsc_argv->length = argc;
  hsc_argv->el_size = sizeof(struct array_object *);
  hsc_argv->size = hsc_argv_size;
  hsc_argv->asize = hsc_argv_asize;
  body = (struct array_object **)hsc_argv->body;
  for (i = 0; i < argc; ++i) {
    len = 1 + strlen(argv[i]);
    strobj_size = sizeof(struct array_object) + len * sizeof(char);
    strobj_asize =
	(strobj_size + sizeof(align_t) +
	-1) / sizeof(align_t) * sizeof(align_t);
    body[i] = (struct array_object *)getmem(scan1, strobj_asize);
    (body[i])->d = (desc_t) 3;
    (body[i])->length = len;
    (body[i])->el_size = sizeof(char);
    (body[i])->size = strobj_size;
    (body[i])->asize = strobj_asize;
    strncpy((char *)(body[i])->body, argv[i], len);
  }
  return hsc_main(scan1, argc, hsc_argv);
}
