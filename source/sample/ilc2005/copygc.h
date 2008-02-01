#ifndef COPYGC_H
#define COPYGC_H

#pragma c2sc_typename size_t

#include <stddef.h>


#ifndef offsetof
#define offsetof(s, m)  (size_t)(&(((s *)0)->m))
#endif

typedef double align_t;
#define ALIGN_UPWARD(size) \
  (sizeof(align_t)*(((size)+sizeof(align_t)-1)/sizeof(align_t)))

typedef struct {
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
} descriptor, *desc_t;

#define DESC(type, fli) \
  {sizeof(type), ALIGN_UPWARD(sizeof(type)), sizeof(fli)/sizeof(size_t), fli}

typedef struct {
  int gcv;                  /* verbose */
  int gctype;               /* GC type */
  size_t tosize;
  size_t stack_size;
  size_t limited_stack_max; /* less than 256 */
} gc_params;

#ifndef CT
#define CT
#endif
typedef CT void (*sht)(void);

extern void getmem_init(gc_params);

extern void *getmem(sht, desc_t);

extern void *move(void *vp);

extern void gc(sht);

extern double gc_ttime;
#endif /* COPYGC_H */
