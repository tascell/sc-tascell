#ifndef TOSIZE
#define TOSIZE   (5*1024*1024)
#endif

#ifndef ROOTSIZEMAX
#define ROOTSIZEMAX (8*1024)
#endif

#ifndef GC_STACK_SIZE
#define GC_STACK_SIZE (params.tosize/(sizeof(double)))
#endif

#ifndef GC_LIMITED_STACK_SIZE
#define GC_LIMITED_STACK_SIZE 256
#endif

#ifndef GC_LIMITED_STACK_MAX
#define GC_LIMITED_STACK_MAX 32
#endif

#define IN_FROM(p) ((unsigned)((char *)(p) - old_memory) < params.tosize)
#define IN_TOSP(p) ((unsigned)((char *)(p) - new_memory) < params.tosize)
#define FWPTR(p) (*((void **)(p)))

#if 0
#define MEMCPY(d,s,sz) memcpy((d),(s),(sz))
#else
#define MEMCPY(d,s,sz) \
  { long *_des=(long *)(d), *_src=(long *)(s),  \
         *_til=(long *)(((char *)_src)+(sz)); \
    do{ *(_des++) = *(_src++); } while( _src < _til); }
#endif

/* util */

static void error(char *s) {
  fprintf(stderr, "ERROR: %s\n", s);
  exit(EXIT_FAILURE);
}

static void *mymalloc(size_t size) {
  void *p = malloc(size);

  if(p == NULL)
    error("Not enough memory.");
  return p;
}

static void *myrealloc(void *p, size_t size) {
  void *q = realloc(p, size);

  if(q == NULL)
    error("Not enough memory.");
  return q;
}

/* root + heap area */

gc_params params;

static int allocated_size;
static char *old_memory, *new_memory;
static char *old_memory_end, *new_memory_end;

/* gc */

static char *b;

/* *link = move(*link); */
void *move(void *vp)
{
  char *p = vp;
  if(!IN_FROM(p))
    return vp;
  if(IN_TOSP(FWPTR(p)))
    return FWPTR(p);
  {
    desc_t d = *(desc_t *)p;
    char *np = b, *nb = np + d->asize;
    if(nb >= new_memory_end)
      error("b overrun.");
    MEMCPY(np, p, d->size);
    b = nb;
    FWPTR(p) = np;
    return np;
  }
}

/* BREADTH_FIRST_GC */

void gc_breadth_first(sht scan) {
  int i;
  char *tmp;
  desc_t d;
  char *p;
  void **link;
  char *s;

  if(params.gcv)printf("BREADTH_FIRST_GC start\n");
  s = b = new_memory;
  scan();
  while(s < b) {
    d = *(desc_t *)s;
    p = (char *)s;
    for(i = 0; i < d->fli_len; i++) {
      link = (void **)(p + d->fli[i]);
      *link = move(*link);
    }
    s += d->asize;
  }
  allocated_size = b - new_memory;
  tmp = new_memory;
  new_memory = old_memory;
  old_memory = tmp;
  tmp = new_memory_end;
  new_memory_end = old_memory_end;
  old_memory_end = tmp;
  if(params.gcv)printf("GC complete (%d)\n", allocated_size);
}

/* */

double gc_ttime;

void gc(sht scan){
  struct timeval tp1, tp2;
  gettimeofday(&tp1, 0); 
  switch(params.gctype){
  case 0: gc_breadth_first(scan); break;
  }
  gettimeofday(&tp2, 0); 
  gc_ttime += ((tp2.tv_sec - tp1.tv_sec)
	       + (tp2.tv_usec - tp1.tv_usec) * 0.000001);
}

/* memory allocation interface */

void getmem_init(gc_params p) {
  char *mp;
  static int called = 0;

  if(called)
    return;
  called=1;
  gc_ttime = 0.0;
  params = p;
  if(params.tosize == 0) params.tosize = TOSIZE;
  params.tosize += 3; params.tosize -= (params.tosize & 3); 
  if(params.stack_size == 0) params.stack_size = GC_STACK_SIZE;
  if(params.limited_stack_max == 0)
    params.limited_stack_max = GC_LIMITED_STACK_MAX;
  printf("tosize=%d, stack=%d, limit=%d\n", 
	 params.tosize,
	 params.stack_size,
	 params.limited_stack_max);
  old_memory = mymalloc(params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = mymalloc(params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
  for(mp = new_memory; mp < new_memory_end; mp+= 4096) *mp = 1;
  for(mp = old_memory; mp < old_memory_end; mp+= 4096) *mp = 1;
}

void *try_getmem(desc_t d) {
  size_t size;
  char *p;

  size = d->asize;
  if(allocated_size + size > params.tosize) {
    return 0;
  }
  p = old_memory + allocated_size;
  allocated_size += size;
  memset(p, 0, size);
  *(desc_t *)p = d;
  return p;
}

void *getmem(sht scan, desc_t d) {
  void *p;
  p = try_getmem(d);
  if(p == 0) {
    gc(scan);
    p = try_getmem(d);
    if(p == 0)
      error("getmem: Not enough memory.");
  }
  return p;
}
