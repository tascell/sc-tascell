typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef unsigned int size_t;
typedef double	Align_t;

char           *lw_call(char *esp);

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

void           *thread_origin(void *farg);

#include <sys/time.h>

void           *malloc(size_t);

void           *realloc(void *, size_t);

char           *strncpy(char *, char const *, size_t);

int		printf     (char const *,...);

struct desc_struct {
  unsigned int	  size;
  unsigned int	  asize;
  int		  fli_len;
  unsigned int   *fli;
};

struct array_object {
  struct desc_struct *d;
  int		  length;
  unsigned int	  el_size;
  unsigned int	  size;
  unsigned int	  asize;
  char		  body    [0];
};

extern void    *getmem(char *esp, closure_t * scan, unsigned int size);

extern void    *move(void *vp);

void           *MEMCPY(void *d, void const *s, unsigned int sz)
{
  long           *_des = (long *)d;
  long           *_src = (long *)s;
  long           *_til = (long *)((char *)_src + sz);

  do {
    *_des++ = *_src++;
  } while (_src < _til);
}

static void	error(char *s)
{
  printf("ERROR: %s\n", s);
  exit(1);
}

static void    *myalloc(unsigned int size)
{
  void           *p;

  p = malloc(size);
  if (p == (void *)0)
    error("Not enough memory.");
  return p;
}

static void    *myrealloc(void *p, unsigned int size)
{
  void           *q;

  q = realloc(p, size);
  if (q == (void *)0)
    error("Not enough memory.");
  return q;
}

struct gc_params_struct {
  int		  gcv;
  int		  gctype;
  unsigned int	  tosize;
  unsigned int	  stack_size;
  unsigned int	  limited_stack_max;
};
static struct gc_params_struct params;
static int	allocated_size;
static char    *old_memory;
static char    *new_memory;
static char    *old_memory_end;
static char    *new_memory_end;
static char    *b;

void           *move(void *vp)
{
  char           *p = vp;
  void           *fwp;
  unsigned int	  tag;
  struct desc_struct *d;
  struct array_object *ao;
  unsigned int	  size;
  unsigned int	  asize;
  char           *np;
  char           *nb;

  if (!((unsigned int)((char *)p - old_memory) < params.tosize))
    return vp;
  tag = 3 & *((unsigned int *)p);
  fwp = (void *)(tag ^ *((unsigned int *)p));
  if ((unsigned int)((char *)fwp - new_memory) < params.tosize)
    return fwp;
  switch (tag) {
  case 0:
    d = (struct desc_struct *)fwp;
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

struct gc_breadth_first_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  char           *s;
  char           *p;
  unsigned int	  el_size;
  struct array_object *ao;
  struct desc_struct *d;
  void          **link;
  int		  len;
  unsigned int	  tag;
  char           *tmp;
  int		  i;
  closure_t      *scan;
};

void		gc_breadth_first(char *esp, closure_t * scan)
{
  char           *argp;
  struct gc_breadth_first_frame *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  int		  i;
  char           *tmp;
  unsigned int	  tag;
  int		  len;
  void          **link;
  struct desc_struct *d;
  struct array_object *ao;
  unsigned int	  el_size;
  char           *p;
  char           *s;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct gc_breadth_first_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct gc_breadth_first_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL;
    }
    goto L_CALL;
  }
  efp = (struct gc_breadth_first_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct gc_breadth_first_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (params.gcv) {
    printf("BREADTH-FIRST-GC start\n");
  }
  b = new_memory;
  s = b;
  {
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = scan;
    efp->s = s;
    efp->p = p;
    efp->el_size = el_size;
    efp->ao = ao;
    efp->d = d;
    efp->link = link;
    efp->len = len;
    efp->tag = tag;
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
    p = efp->p;
    el_size = efp->el_size;
    ao = efp->ao;
    d = efp->d;
    link = efp->link;
    len = efp->len;
    tag = efp->tag;
    tmp = efp->tmp;
    i = efp->i;
    scan = efp->scan;
  }
  while (s < b) {
    tag = 3 & *((unsigned int *)s);
    switch (tag) {
    case 0:
      d = (struct desc_struct *)(tag ^ *((unsigned int *)s));
      p = (char *)s;
      len = (*d).fli_len;
      {
	i = 0;
	for (; i < len; ++i) {
	  link = (void **)(p + ((*d).fli)[i]);
	  *link = move(*link);
	}
      }
      s += (*d).asize;
      break;
    case 1:
      ao = (struct array_object *)s;
      p = (*ao).body;
      len = (*ao).length;
      el_size = (*ao).el_size;
      {
	i = 0;
	for (; i < len; ++i) {
	  link = (void **)p;
	  *link = move(*link);
	  p += el_size;
	}
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
  if (params.gcv) {
    printf("GC complete (%d)\n", allocated_size);
  }
}
double		gc_ttime;
struct timeval	tp1;
struct timeval	tp2;

struct gc_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  closure_t      *scan;
};

void		gc        (char *esp, closure_t * scan)
{
  struct gc_frame *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct gc_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct gc_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL2;
    }
    goto L_CALL2;
  }
  efp = (struct gc_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct gc_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  gettimeofday(&tp1, 0);
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
  gettimeofday(&tp2, 0);
  gc_ttime += tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
}

void		getmem_init(struct gc_params_struct p)
{
  char           *mp;
  static int	  called = 0;

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
  printf("tosize=%d, stack=%d, limit=%d\n", params.tosize, params.stack_size, params.limited_stack_max);
  old_memory = myalloc(params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = myalloc(params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
}

void		gc_init   (int tp, int tosize, int stack_size, int limited_max)
{
  struct gc_params_struct p;

  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init(p);
}

void           *try_getmem(unsigned int size)
{
  char           *p;

  if (allocated_size + size > params.tosize)
    return 0;
  p = old_memory + allocated_size;
  allocated_size += size;
  memset(p, 0, size);
  return p;
}

struct getmem_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  void           *p;
  unsigned int	  size;
  closure_t      *scan;
};

void           *getmem(char *esp, closure_t * scan, unsigned int size)
{
  struct getmem_frame *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  void           *p;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct getmem_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct getmem_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL3;
    }
    goto L_CALL3;
  }
  efp = (struct getmem_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct getmem_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  p = try_getmem(size);
  if (p == 0) {
    new_esp = esp;
    while (gc(new_esp, scan), (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->p = p;
      efp->size = size;
      efp->scan = scan;
      efp->call_id = 0;
      return (void *)0 - 1;
  L_CALL3:
      ;
      p = efp->p;
      size = efp->size;
      scan = efp->scan;
      new_esp = esp + 1;
    }
    p = try_getmem(size);
    if (p == 0) {
      error("getmem: No enough memory.");
    }
  }
  return p;
}

int		hsc_main   (char *esp, closure_t *, int, struct array_object *);

struct main_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  void           *tmp3;
  unsigned int	  strobj_asize;
  unsigned int	  strobj_size;
  int		  len;
  struct array_object **body;
  int		  i;
  unsigned int	  hsc_argv_asize;
  unsigned int	  hsc_argv_size;
  struct array_object *hsc_argv;
  int		  tmp4;
  void           *tmp2;
  char          **argv;
  int		  argc;
  closure_t	  scan1;
};

struct scan1_in_main_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  struct main_frame *xfp;
};

char           *scan1_in_main(char *esp, void *xfp0)
{
  char           *new_esp;
  struct scan1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  size_t	  esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);

LGOTO:
  ;
  efp = (struct scan1_in_main_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct scan1_in_main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->argv = move(xfp->argv);
  return 0;
  return 0;
}

int		main       (int argc, char **argv)
{
  struct main_frame *efp;
  char           *new_esp;
  char		  estack  [8192];
  char           *esp = estack;
  void           *tmp2;
  int		  tmp4;
  struct array_object *hsc_argv;
  unsigned int	  hsc_argv_size;
  unsigned int	  hsc_argv_asize;
  int		  i;
  struct array_object **body;
  int		  len;
  unsigned int	  strobj_size;
  unsigned int	  strobj_asize;
  void           *tmp3;

LGOTO:
  ;
  efp = (struct main_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  gc_init(0, 0, 0, 0);
  hsc_argv_size = sizeof(struct array_object) + argc * sizeof(struct array_object *);
  hsc_argv_asize = (hsc_argv_size + sizeof(double) + -1) / sizeof(double) * sizeof(double);
  {
    new_esp = esp;
    while ((tmp2 = getmem(new_esp, &efp->scan1, hsc_argv_asize)) == (void *)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
      char           *goto_fr;

      *((char **)esp) = 0;
      efp->strobj_asize = strobj_asize;
      efp->strobj_size = strobj_size;
      efp->len = len;
      efp->body = body;
      efp->i = i;
      efp->hsc_argv_asize = hsc_argv_asize;
      efp->hsc_argv_size = hsc_argv_size;
      efp->hsc_argv = hsc_argv;
      efp->tmp4 = tmp4;
      efp->tmp2 = tmp2;
      efp->argv = argv;
      efp->argc = argc;
      efp->scan1.fun = scan1_in_main;
      efp->scan1.fr = (void *)efp;
      goto_fr = lw_call(efp->tmp_esp);
      if ((char *)goto_fr == (char *)efp)
	goto LGOTO;
      strobj_asize = efp->strobj_asize;
      strobj_size = efp->strobj_size;
      len = efp->len;
      body = efp->body;
      i = efp->i;
      hsc_argv_asize = efp->hsc_argv_asize;
      hsc_argv_size = efp->hsc_argv_size;
      hsc_argv = efp->hsc_argv;
      tmp4 = efp->tmp4;
      tmp2 = efp->tmp2;
      argv = efp->argv;
      argc = efp->argc;
      new_esp = esp + 1;
    }
    hsc_argv = (struct array_object *)tmp2;
  }
  (*hsc_argv).d = (struct desc_struct *)1;
  (*hsc_argv).length = argc;
  (*hsc_argv).el_size = sizeof(struct array_object *);
  (*hsc_argv).size = hsc_argv_size;
  (*hsc_argv).asize = hsc_argv_asize;
  body = (struct array_object **)(*hsc_argv).body;
  {
    i = 0;
    for (; i < argc; ++i) {
      len = 1 + strlen(argv[i]);
      strobj_size = sizeof(struct array_object) + len * sizeof(char);
      strobj_asize = (strobj_size + sizeof(double) + -1) / sizeof(double) * sizeof(double);
      {
	new_esp = esp;
	while ((tmp3 = getmem(new_esp, &efp->scan1, strobj_asize)) == (void *)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
	  char           *goto_fr;

	  *((char **)esp) = 0;
	  efp->tmp3 = tmp3;
	  efp->strobj_asize = strobj_asize;
	  efp->strobj_size = strobj_size;
	  efp->len = len;
	  efp->body = body;
	  efp->i = i;
	  efp->hsc_argv_asize = hsc_argv_asize;
	  efp->hsc_argv_size = hsc_argv_size;
	  efp->hsc_argv = hsc_argv;
	  efp->tmp4 = tmp4;
	  efp->tmp2 = tmp2;
	  efp->argv = argv;
	  efp->argc = argc;
	  efp->scan1.fun = scan1_in_main;
	  efp->scan1.fr = (void *)efp;
	  goto_fr = lw_call(efp->tmp_esp);
	  if ((char *)goto_fr == (char *)efp)
	    goto LGOTO;
	  tmp3 = efp->tmp3;
	  strobj_asize = efp->strobj_asize;
	  strobj_size = efp->strobj_size;
	  len = efp->len;
	  body = efp->body;
	  i = efp->i;
	  hsc_argv_asize = efp->hsc_argv_asize;
	  hsc_argv_size = efp->hsc_argv_size;
	  hsc_argv = efp->hsc_argv;
	  tmp4 = efp->tmp4;
	  tmp2 = efp->tmp2;
	  argv = efp->argv;
	  argc = efp->argc;
	  new_esp = esp + 1;
	}
	body[i] = (struct array_object *)tmp3;
      }
      (*body[i]).d = (struct desc_struct *)3;
      (*body[i]).length = len;
      (*body[i]).el_size = sizeof(char);
      (*body[i]).size = strobj_size;
      (*body[i]).asize = strobj_asize;
      strncpy((char *)(*body[i]).body, argv[i], len);
    }
  }
  {
    new_esp = esp;
    while ((tmp4 = hsc_main(new_esp, &efp->scan1, argc, hsc_argv)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
      char           *goto_fr;

      *((char **)esp) = 0;
      efp->tmp3 = tmp3;
      efp->strobj_asize = strobj_asize;
      efp->strobj_size = strobj_size;
      efp->len = len;
      efp->body = body;
      efp->i = i;
      efp->hsc_argv_asize = hsc_argv_asize;
      efp->hsc_argv_size = hsc_argv_size;
      efp->hsc_argv = hsc_argv;
      efp->tmp4 = tmp4;
      efp->tmp2 = tmp2;
      efp->argv = argv;
      efp->argc = argc;
      efp->scan1.fun = scan1_in_main;
      efp->scan1.fr = (void *)efp;
      goto_fr = lw_call(efp->tmp_esp);
      if ((char *)goto_fr == (char *)efp)
	goto LGOTO;
      tmp3 = efp->tmp3;
      strobj_asize = efp->strobj_asize;
      strobj_size = efp->strobj_size;
      len = efp->len;
      body = efp->body;
      i = efp->i;
      hsc_argv_asize = efp->hsc_argv_asize;
      hsc_argv_size = efp->hsc_argv_size;
      hsc_argv = efp->hsc_argv;
      tmp4 = efp->tmp4;
      tmp2 = efp->tmp2;
      argv = efp->argv;
      argc = efp->argc;
      new_esp = esp + 1;
    }
    return tmp4;
  }
}
