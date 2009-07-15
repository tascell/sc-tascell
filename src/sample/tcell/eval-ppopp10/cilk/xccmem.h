/* GCC macros for XC-cube primitives for the XC-cube shared memory model.
   version 0.44
   Copyright (C) 2001-2004,2009 Masahiro Yasugi.

   Supported platforms:
   __i386__
   __x86_64__
   __alpha__
   __sparc_v9__ (not __arch64__)
   _POWER _ARCH_PPC
   __mips__
   __ia64__

   GCC compilation options:
   for all:         -O (or -O2 ...   to expand "extern __inline__")
   for UltraSparc:  -mcpu=ultrasparc
   for PowerPC:     -mcpu=powerpc
   */

#ifndef XCCMEM_H
#define XCCMEM_H

/* *//* *//* *//* *//* */

/*
 * Assertion
 */

#define XCC_ASSERT(cond,msg) ({char msg[(cond)?1:(-1)]; 0;})

/* *//* *//* *//* *//* */

/*
 * Data Size
 */

#ifdef __x86_64__

#define XCC_LONG_TYPE_SIZE 8
#define XCC_PTR_TYPE_SIZE 8

#endif /* __x86_64__ */

#ifdef __alpha__
/* __alpha_ev4__ __alpha_ev5__ __alpha_ev6__ */

#define XCC_LONG_TYPE_SIZE 8
#define XCC_PTR_TYPE_SIZE 8

#endif /* __alpha__ */

#ifdef __mips__

#define XCC_INT_TYPE_SIZE (_MIPS_SZINT/8)
#define XCC_LONG_TYPE_SIZE (_MIPS_SZLONG/8)
#define XCC_PTR_TYPE_SIZE (_MIPS_SZPTR/8)

#endif /* __mips__ */

#ifdef __ia64__

#define XCC_LONG_TYPE_SIZE 8
#define XCC_PTR_TYPE_SIZE 8

#endif /* __ia64__ */

/*
 * Default Data Size
 */

#ifndef XCC_CHAR_TYPE_SIZE
#define XCC_CHAR_TYPE_SIZE 1
#endif
#ifndef XCC_SHORT_TYPE_SIZE
#define XCC_SHORT_TYPE_SIZE 2
#endif
#ifndef XCC_INT_TYPE_SIZE
#define XCC_INT_TYPE_SIZE 4
#endif
#ifndef XCC_LONG_TYPE_SIZE
#define XCC_LONG_TYPE_SIZE 4
#endif
#ifndef XCC_LONG_LONG_TYPE_SIZE
#define XCC_LONG_LONG_TYPE_SIZE 8
#endif
#ifndef XCC_FLOAT_TYPE_SIZE
#define XCC_FLOAT_TYPE_SIZE 4
#endif
#ifndef XCC_DOUBLE_TYPE_SIZE
#define XCC_DOUBLE_TYPE_SIZE 8
#endif
#ifndef XCC_PTR_TYPE_SIZE
#define XCC_PTR_TYPE_SIZE 4
#endif

/* *//* *//* *//* *//* */

/*
 * Tools for instructions 
 */

typedef union { int i; float f; short s[2]; char c[4];} xcc_int_float;
typedef union { long long ll; double d; } xcc_long_long_double;
#define INTO_UNION(TP,tag,x) ({TP _u ; _u.tag = (x); _u; })
#define FORCE_TYPE(TP,x) \
({ union { TP x1; typeof(x) x2; } _u ; _u.x2 = (x); _u.x1; })
#define EODEF struct _dummy /* eating a ';' */

/*
 * Common Instructions
 */

#define xcc_nnbar() __asm__ __volatile__("":::"memory")

/*
 * Instructions
 */

#if defined(__x86_64__) || defined(__i386__)

#ifdef __i386__

/* or lock add $0,(%esp) ? */
#define xcc_slbar __xcc_slbar
extern __inline__ void
__xcc_slbar(){
  int _tmp1, _tmp2;
  __asm__ __volatile__("xchgl %1,%0": "=m"(_tmp1), "=r"(_tmp2) :: "memory"); 
}

#define xcc_sabar xcc_slbar
#define xcc_albar xcc_slbar
#define xcc_aabar xcc_slbar

#endif /* __i386__ */

#ifdef __x86_64__

#define xcc_llbar() __asm__ __volatile__("lfence":::"memory")
#define xcc_aabar() __asm__ __volatile__("mfence":::"memory")

#define xcc_labar xcc_llbar
#define xcc_sabar xcc_aabar
#define xcc_albar xcc_aabar
#define xcc_slbar xcc_aabar

#endif /* __x86_64__ */

#define xcc_start_access_after_lock xcc_nnbar
#define xcc_start_read_after_lock  xcc_nnbar
#define xcc_start_write_after_lock  xcc_nnbar

#ifdef __i386__

#define DEF_xcc_rawcas_DI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN(TP *loc, TP ov, TP nv){\
  int s = 0;\
  union { TP v; int w2[2]; } u;\
  u.v = nv;\
  __asm__ __volatile__("lock; cmpxchg8b %0\n"\
		       "	jz 1f\n"\
		       "	inc %1\n"\
		       "1:"\
		       : "=m"(*loc), "=r"(s), "=A"(ov)\
		       : "m" (*loc), "1"(s), "2"(ov),\
		       "b"(u.w2[0]), "c"(u.w2[1]) : "cc");\
  return s;\
} EODEF

#endif /* __i386__ */

#ifdef __x86_64__

#define DEF_xcc_rawcas_DI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN(TP *loc, TP ov, TP nv){\
  int s = 0;\
  __asm__ __volatile__("lock; cmpxchgq %6, %0\n"\
		       "	jz 1f\n"\
		       "	inc %1\n"\
		       "1:"\
		       : "=m"(*loc), "=r"(s), "=a"(ov)\
		       : "m"(*loc), "1"(s), "2"(ov), "q"(nv) : "cc");\
  return s;\
} EODEF
#define xcc_rawcas_long __xcc_rawcas_long
#define xcc_rawcas_ptr __xcc_rawcas_ptr
DEF_xcc_rawcas_DI(long,long);
DEF_xcc_rawcas_DI(ptr,void *);

#endif /* __x86_64__ */

#define xcc_rawcas_long_long __xcc_rawcas_long_long
#define xcc_rawcas_long_long_double __xcc_rawcas_long_long_double
DEF_xcc_rawcas_DI(long_long,long long);
DEF_xcc_rawcas_DI(long_long_double,xcc_long_long_double);

#define DEF_xcc_rawcas_SI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN (TP *loc, TP ov, TP nv){\
  int s = 0;\
  __asm__ __volatile__("lock; cmpxchgl %6,%0\n"\
		       "	jz 1f\n"\
		       "	inc %1\n"\
		       "1:"\
		       : "=m"(*loc), "=r"(s), "=a"(ov)\
		       : "m"(*loc), "1"(s), "2"(ov), "q"(nv) : "cc");\
  return s;\
} EODEF
#define xcc_rawcas_int __xcc_rawcas_int
#define xcc_rawcas_int_float __xcc_rawcas_int_float
DEF_xcc_rawcas_SI(int,int);
DEF_xcc_rawcas_SI(int_float,xcc_int_float);

#ifdef __i386__

#define xcc_rawcas_long __xcc_rawcas_long
#define xcc_rawcas_ptr __xcc_rawcas_ptr
DEF_xcc_rawcas_SI(long,long);
DEF_xcc_rawcas_SI(ptr,void *);

#endif /* __i386__ */

#define xcc_rawcas_short __xcc_rawcas_short
extern __inline__ int
__xcc_rawcas_short(short *loc, short ov, short nv){
  int s = 0; 
  __asm__ __volatile__("lock; cmpxchgw %6,%0\n"
		       "	jz 1f\n"
		       "	inc %1\n"
		       "1:"
		       : "=m"(*loc), "=r"(s), "=a"(ov)
		       : "m"(*loc), "1"(s), "2"(ov), "q"(nv) : "cc");
  return s;
}

#define xcc_cas_short __xcc_cas_short
extern __inline__ int
__xcc_cas_short(short *loc, short ov, short nv){
  return ((*(volatile short *)loc) != ov) || xcc_rawcas_short(loc,ov,nv);
}

#define xcc_rawcas_char __xcc_rawcas_char
extern __inline__ int
__xcc_rawcas_char(char *loc, char ov, char nv){
  int s = 0; 
  __asm__ __volatile__("lock; cmpxchgb %6,%0\n"
		       "	jz 1f\n"
		       "	inc %1\n"
		       "1:"
		       : "=m"(*loc), "=r"(s), "=a"(ov)
		       : "m"(*loc), "1"(s), "2"(ov), "q"(nv) : "cc");
  return s;
}

#define xcc_cas_char __xcc_cas_char
extern __inline__ int
__xcc_cas_char(char *loc, char ov, char nv){
  return ((*(volatile char *)loc) != ov) || xcc_rawcas_char(loc,ov,nv);
}

#ifdef __i386__

/*
#define xcc_atomic_swap_long_long __xcc_atomic_swap_long_long
extern __inline__ long long
__xcc_atomic_swap_long_long(long long *loc, long long val){ 
  __asm__ __volatile__("movl %%eax,%%ebx\n"
		       "	movl %%edx,%%ecx\n"
		       "1:	lock\n"
		       "	cmpxchg8b %0\n"
		       "	jnz 1b"
		       : "=m"(*loc), "=A"(val) : "m"(*loc), "1"(val)
		       : "ecx", "ebx", "cc");
  return val;
}
*/

#define DEF_xcc_atomic_swap_DI(TPN,TP) \
extern __inline__ TP \
__xcc_atomic_swap_##TPN(TP *loc, TP val){\
  __asm__ __volatile__("movl %%eax,%%ebx\n"\
		       "	movl %%edx,%%ecx\n"\
		       "1:	lock\n"\
		       "	cmpxchg8b %0\n"\
		       "	jnz 1b"\
		       : "=m"(*loc), "=A"(val) : "m"(*loc), "1"(val)\
		       : "ecx", "ebx", "cc");\
  return val;\
} EODEF

#endif /* __i386__ */

#ifdef __x86_64__

#define DEF_xcc_atomic_swap_DI(TPN,TP) \
extern __inline__ TP \
__xcc_atomic_swap_##TPN(TP *loc, TP val){\
  __asm__ __volatile__("xchgq %1,%0"\
		       : "=m"(*loc), "=r"(val) : "m"(*loc), "1"(val));\
  return val;\
} EODEF
#define xcc_atomic_swap_long __xcc_atomic_swap_long
#define xcc_atomic_swap_ptr __xcc_atomic_swap_ptr
DEF_xcc_atomic_swap_DI(long,long);
DEF_xcc_atomic_swap_DI(ptr,void *);

#endif /* __x86_64__ */

#define xcc_atomic_swap_long_long __xcc_atomic_swap_long_long
#define xcc_atomic_swap_long_long_double __xcc_atomic_swap_long_long_double
DEF_xcc_atomic_swap_DI(long_long,long long);
DEF_xcc_atomic_swap_DI(long_long_double,xcc_long_long_double);

/* need lock ? */
#define DEF_xcc_atomic_swap_SI(TPN,TP) \
extern __inline__ TP \
__xcc_atomic_swap_##TPN(TP *loc, TP val){\
  __asm__ __volatile__("xchgl %1,%0"\
		       : "=m"(*loc), "=r"(val) : "m"(*loc), "1"(val));\
  return val;\
} EODEF
#define xcc_atomic_swap_int __xcc_atomic_swap_int
#define xcc_atomic_swap_int_float __xcc_atomic_swap_int_float
DEF_xcc_atomic_swap_SI(int,int);
DEF_xcc_atomic_swap_SI(int_float,xcc_int_float);

#ifdef __i386__

#define xcc_atomic_swap_long __xcc_atomic_swap_long
#define xcc_atomic_swap_ptr __xcc_atomic_swap_ptr
DEF_xcc_atomic_swap_SI(long,long);
DEF_xcc_atomic_swap_SI(ptr,void *);

#endif /* __i386__ */

#define xcc_atomic_swap_short __xcc_atomic_swap_short
extern __inline__ short
__xcc_atomic_swap_short(short *loc, short val){
  __asm__ __volatile__("xchgw %1,%0"
		       : "=m"(*loc), "=r"(val) : "m"(*loc), "1"(val));
  return val;
}

#define xcc_atomic_swap_char __xcc_atomic_swap_char
extern __inline__ char
__xcc_atomic_swap_char(char *loc, char val){
  __asm__ __volatile__("xchgb %1,%0"
		       : "=m"(*loc), "=r"(val) : "m"(*loc), "1"(val));
  return val;
}

#ifdef __i386__

#define xcc_atomic_write_long_long __xcc_atomic_write_long_long
extern __inline__ void
__xcc_atomic_write_long_long(long long *loc, long long val){
  long long tmp;
  __asm__ __volatile__("movl %%eax,%%ebx\n"
		       "	movl %%edx,%%ecx\n"
		       "1:	lock\n"
		       "	cmpxchg8b %0\n"
		       "	jnz 1b"
		       : "=m"(*loc), "=A"(tmp) : "A"(val)
		       : "ecx", "ebx", "cc");
}

#define xcc_atomic_read_long_long __xcc_atomic_read_long_long
extern __inline__ long long
__xcc_atomic_read_long_long(long long *loc){
  long long val;
  __asm__ __volatile__("movl %%eax,%%ebx\n"
		       "	movl %%edx,%%ecx\n"
		       "	lock\n"
		       "	cmpxchg8b %1"
		       : "=A"(val) : "m"(*loc)
		       : "ecx", "ebx", "cc", "memory");
  return val;
}

#endif /* __i386__ */

#endif /* defined(__x86_64__) || defined(__i386__) */

#ifdef __alpha__
/* __alpha_ev4__ __alpha_ev5__ __alpha_ev6__ */

#define xcc_aabar() __asm__ __volatile__("mb":::"memory")
#define xcc_defaultbar xcc_aabar

#define DEF_xcc_cas_DI(TPN,TP) \
extern __inline__ int \
__xcc_cas_##TPN(TP *loc, TP ov, TP nv){\
  int cmp; TP tmp;\
  __asm__ __volatile__("ldq_l %0,%5\n"\
		       "	cmpeq %0,%3,%1\n"\
		       "	beq %1,1f\n"\
		       "	mov %4,%1\n"\
		       "	stq_c %1,%2\n"\
		       "1:"\
		       : "=&r"(tmp),"=&r"(cmp),"=m"(*loc)\
		       : "r"(ov),"r"(nv),"m"(*loc));\
  return !cmp;\
} EODEF
#define xcc_rawcas_long xcc_cas_long
#define xcc_cas_long __xcc_cas_long
#define xcc_rawcas_long_long xcc_cas_long_long
#define xcc_cas_long_long __xcc_cas_long_long
#define xcc_rawcas_ptr xcc_cas_ptr
#define xcc_cas_ptr __xcc_cas_ptr
#define xcc_rawcas_long_long_double xcc_cas_long_long_double
#define xcc_cas_long_long_double __xcc_cas_long_long_double
DEF_xcc_cas_DI(long,long);
DEF_xcc_cas_DI(long_long,long long);
DEF_xcc_cas_DI(ptr,void *);
DEF_xcc_cas_DI(long_long_double,xcc_long_long_double);

#define DEF_xcc_cas_SI(TPN,TP) \
extern __inline__ int \
__xcc_cas_##TPN(TP *loc, TP ov, TP nv){\
  int cmp; TP tmp;\
  __asm__ __volatile__("ldl_l %0,%5\n"\
		       "	cmpeq %0,%3,%1\n"\
		       "	beq %1,1f\n"\
		       "	mov %4,%1\n"\
		       "	stl_c %1,%2\n"\
		       "1:"\
		       : "=&r"(tmp),"=&r"(cmp),"=m"(*loc)\
		       : "rI"(ov),"rI"(nv),"m"(*loc));\
  return !cmp;\
} EODEF
#define xcc_rawcas_int xcc_cas_int
#define xcc_cas_int __xcc_cas_int
#define xcc_rawcas_int_float xcc_cas_int_float
#define xcc_cas_int_float __xcc_cas_int_float
DEF_xcc_cas_SI(int,int);
DEF_xcc_cas_SI(int_float,xcc_int_float);

#define xcc_atomic_write_short __xcc_atomic_write_short
extern __inline__ void
__xcc_atomic_write_short(short *loc, short val){
  long _tmp1, _tmp2, *_tmpa, *_tmpb;
  __asm__ __volatile__("lda	%3,%0\n"
		       "	bic	%3,7,%4\n"
		       "	inswl	%6,%3,%2\n"
		       "1:	ldq_l	%1,0(%4)\n"
		       "	mskwl	%1,%3,%1\n"
		       "	bis	%1,%2,%1\n"
		       "	stq_c	%1,0(%4)\n"
		       "	bne	%1,1b"
		       : "=m"(*loc),"=r"(_tmp1),"=r"(_tmp2),
		       "=&r"(_tmpa),"=&r"(_tmpb)
		       : "m"(*loc), "r"(val)); 
}

#define xcc_atomic_write_char __xcc_atomic_write_char
extern __inline__ void
__xcc_atomic_write_char(char *loc, char val){
  long _tmp1, _tmp2, *_tmpa, *_tmpb;
  __asm__ __volatile__("lda	%3,%0\n"
		       "	bic	%3,7,%4\n"
		       "	insbl	%6,%3,%2\n"
		       "1:	ldq_l	%1,0(%4)\n"
		       "	mskbl	%1,%3,%1\n"
		       "	bis	%1,%2,%1\n"
		       "	stq_c	%1,0(%4)\n"
		       "	bne	%1,1b"
		       : "=m"(*loc),"=r"(_tmp1),"=r"(_tmp2),
		       "=&r"(_tmpa),"=&r"(_tmpb)
		       : "m"(*loc), "r"(val)); 
}

#endif /* __alpha__ */

#ifdef __sparc_v9__
#ifndef __arch64__
/*  -mcpu=ultrasparc */

/* assuming TSO */
#define xcc_slbar() __asm__("membar #StoreLoad"::: "memory")
#define xcc_sabar xcc_slbar
#define xcc_albar xcc_slbar
#define xcc_aabar xcc_slbar

#define xcc_start_access_after_lock xcc_nnbar
#define xcc_start_read_after_lock  xcc_nnbar
#define xcc_start_write_after_lock  xcc_nnbar

#define DEF_xcc_rawcas_DI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN(TP *loc, TP ov, TP nv){\
  TP tmpa, tmpb, ovm=ov, nvm=nv;\
  __asm__ __volatile__("ldx %6,%2\n"\
		       "	ldx %7,%3\n"\
		       "	casx [%5],%3,%2\n"\
		       "	stx %2,%1"\
		       : "=m"(*loc),"=m"(nvm),"=&r"(tmpa),"=&r"(tmpb)\
		       : "m"(*loc),"r"(loc),"m"(nvm),"m"(ovm) : "cc");\
  nv = nvm;\
  return (FORCE_TYPE(long long,nv) != FORCE_TYPE(long long,ov));\
} EODEF
#define xcc_rawcas_long_long __xcc_rawcas_long_long
#define xcc_rawcas_long_long_double \
 __xcc_rawcas_long_long_double
DEF_xcc_rawcas_DI(long_long,long long);
DEF_xcc_rawcas_DI(long_long_double,xcc_long_long_double);

#define DEF_xcc_rawcas_SI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN(TP *loc, TP ov, TP nv){\
  __asm__ __volatile__("cas [%3],%5,%1"\
		       : "=m"(*loc),"=&r"(nv)\
		       : "m"(*loc),"r"(loc),"1"(nv),"r"(ov) : "cc");\
  return (FORCE_TYPE(int,nv) != FORCE_TYPE(int,ov));\
} EODEF

#define xcc_rawcas_int __xcc_rawcas_int
#define xcc_rawcas_long __xcc_rawcas_long
#define xcc_rawcas_ptr __xcc_rawcas_ptr
#define xcc_rawcas_int_float __xcc_rawcas_int_float
DEF_xcc_rawcas_SI(int,int);
DEF_xcc_rawcas_SI(long,long);
DEF_xcc_rawcas_SI(ptr,void *);
DEF_xcc_rawcas_SI(int_float,xcc_int_float);

#endif /* not __arch64__ */
#endif /* __sparc_v9__ */

#ifdef _POWER
#ifdef _ARCH_PPC
/*  -mcpu=powerpc */

#define xcc_aabar() __asm__ __volatile__("sync":::"memory")
#define xcc_isync() __asm__ __volatile__("isync":::"memory")
#define xcc_ssbar() __asm__ __volatile__("eieio":::"memory")

#define xcc_defaultbar xcc_aabar
#define xcc_start_access_after_lock xcc_isync
#define xcc_start_read_after_lock  xcc_isync
#define xcc_start_write_after_lock  xcc_isync

#define xcc_rawcas_long_long xcc_cas_long_long
#define xcc_cas_long_long(loc,ov,nv) XCC_ASSERT(0,_64bit_cas_not_support)
#define xcc_atomic_swap_long_long(loc,v) XCC_ASSERT(0,_64bit_cas_not_support)
#define xcc_cas_double(loc,ov,nv) XCC_ASSERT(0,_64bit_cas_not_support)
#define xcc_atomic_swap_double(loc,v) XCC_ASSERT(0,_64bit_cas_not_support)
#define xcc_atomic_read_long_long(loc) XCC_ASSERT(0, _64bit_read_not_support)
#define xcc_atomic_write_long_long(loc,v)\
 XCC_ASSERT(0, _64bit_write_not_support)

#define DEF_xcc_cas_SI(TPN,TP) \
extern __inline__ int \
__xcc_cas_##TPN(TP *loc, TP ov, TP nv){\
  int cmp = 1;\
  TP tmp;\
  __asm__ __volatile__("lwarx %2,0,%4\n"\
		       "	cmpw %6,%2\n"\
		       "	bne- 1f\n"\
		       "	stwcx. %5,0,%4\n"\
		       "	bne- 1f\n"\
		       "	li %1,0\n"\
		       "1:"\
		       : "=m"(*loc),"=r"(cmp),"=&r"(tmp)\
		       : "m"(*loc),"r"(loc),"r"(nv),"r"(ov),"1"(cmp) : "cc");\
  return cmp;\
} EODEF
#define xcc_rawcas_int xcc_cas_int
#define xcc_cas_int __xcc_cas_int
DEF_xcc_cas_SI(int,int);
DEF_xcc_cas_SI(long,long);
DEF_xcc_cas_SI(ptr,void *);
DEF_xcc_cas_SI(int_float,xcc_int_float);

#endif /* _ARCH_PPC */
#endif /* _POWER */

#ifdef __mips__

#define DEF_xcc_cas_DI(TPN,TP) \
extern __inline__ int \
__xcc_cas_##TPN(TP *loc, TP ov, TP nv){\
  int cmp; TP tmp;\
  __asm__ __volatile__(".set	noreorder\n"\
		       "	lld %0,%5\n"\
		       "	bne %0,%3,1f\n"\
		       "	or %1,$0,$0\n"\
		       "	or %1,%4,$0\n"\
		       "	scd %1,%2\n"\
		       "1:\n"\
		       "	.set	reorder"\
		       : "=&r"(tmp),"=&r"(cmp),"=m"(*loc)\
		       : "r"(ov),"r"(nv),"m"(*loc));\
  return !cmp;\
} EODEF

#define DEF_xcc_cas_SI(TPN,TP) \
extern __inline__ int \
__xcc_cas_##TPN(TP *loc, TP ov, TP nv){\
  int cmp; TP tmp;\
  __asm__ __volatile__(".set	noreorder\n"\
		       "	ll %0,%5\n"\
		       "	bne %0,%3,1f\n"\
		       "	or %1,$0,$0\n"\
		       "	or %1,%4,$0\n"\
		       "	sc %1,%2\n"\
		       "1:\n"\
		       "	.set	reorder"\
		       : "=&r"(tmp),"=&r"(cmp),"=m"(*loc)\
		       : "r"(ov),"r"(nv),"m"(*loc));\
  return !cmp;\
} EODEF

#define xcc_rawcas_long_long xcc_cas_long_long
#define xcc_cas_long_long __xcc_cas_long_long
#define xcc_rawcas_long xcc_cas_long
#define xcc_cas_long __xcc_cas_long
#define xcc_rawcas_int xcc_cas_int
#define xcc_cas_int __xcc_cas_int
#define xcc_rawcas_ptr xcc_cas_ptr
#define xcc_cas_ptr __xcc_cas_ptr
#define xcc_rawcas_long_long_double xcc_cas_long_long_double
#define xcc_cas_long_long_double __xcc_cas_long_long_double
#define xcc_rawcas_int_float xcc_cas_int_float
#define xcc_cas_int_float __xcc_cas_int_float
DEF_xcc_cas_DI(long_long,long long);
DEF_xcc_cas_DI(long_long_double,xcc_long_long_double);
#if XCC_INT_TYPE_SIZE == 4
DEF_xcc_cas_SI(int,int);
DEF_xcc_cas_SI(int_float,xcc_int_float);
#elif XCC_INT_TYPE_SIZE == 8
/* how to implemnet xcc_cas_int_float ? */
DEF_xcc_cas_DI(int,int);
#endif /* XCC_INT_TYPE_SIZE == 8 */
#if XCC_LONG_TYPE_SIZE == 4
DEF_xcc_cas_SI(long,long);
#elif XCC_LONG_TYPE_SIZE == 8
DEF_xcc_cas_DI(long,long);
#endif /* XCC_LONG_TYPE_SIZE == 8 */
#if XCC_PTR_TYPE_SIZE == 4
DEF_xcc_cas_SI(ptr,void *);
#elif XCC_PTR_TYPE_SIZE == 8
DEF_xcc_cas_DI(ptr,void *);
#endif /* XCC_PTR_TYPE_SIZE == 8 */

#endif /* __mips__ */

#ifdef __ia64__

#define xcc_aabar() __asm__ __volatile__("mf":::"memory")
#define xcc_defaultbar xcc_aabar

#define xcc_start_access_after_lock xcc_nnbar
#define xcc_start_read_after_lock  xcc_nnbar
#define xcc_start_write_after_lock  xcc_nnbar

#define DEF_xcc_rawcas_DI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN(TP *loc, TP ov, TP nv){\
  unsigned long ov64 = FORCE_TYPE(unsigned long,ov), tmp;\
  __asm__ __volatile__("mov ar.ccv = %3\n"\
		       "	;;\n"\
		       "	cmpxchg8.acq %1 = %0, %2, ar.ccv"\
		       : "=m"(*loc), "=r"(tmp)\
		       : "r"(nv), "r"(ov64), "m"(*loc)\
		       : "ar.ccv");\
  return (ov64 != tmp);\
} EODEF
/* need "memory" ? */
#define xcc_rawcas_long __xcc_rawcas_long
#define xcc_rawcas_long_long __xcc_rawcas_long_long
#define xcc_rawcas_ptr __xcc_rawcas_ptr
#define xcc_rawcas_long_long_double __xcc_rawcas_long_long_double
DEF_xcc_rawcas_DI(long,long);
DEF_xcc_rawcas_DI(long_long,long long);
DEF_xcc_rawcas_DI(ptr,void *);
DEF_xcc_rawcas_DI(long_long_double,xcc_long_long_double);

#define DEF_xcc_rawcas_SI(TPN,TP) \
extern __inline__ int \
__xcc_rawcas_##TPN(TP *loc, TP ov, TP nv){\
  unsigned int ov32 = FORCE_TYPE(unsigned int,ov), tmp;\
  __asm__ __volatile__("mov ar.ccv = %3\n"\
		       "	;;\n"\
		       "	cmpxchg4.acq %1 = %0, %2, ar.ccv"\
		       : "=m"(*loc), "=r"(tmp)\
		       : "r"(nv), "r"((unsigned long)ov32), "m"(*loc)\
		       : "ar.ccv");\
  return (ov32 != tmp);\
} EODEF
/* need "memory" ? */
#define xcc_rawcas_int __xcc_rawcas_int
#define xcc_rawcas_int_float __xcc_rawcas_int_float
DEF_xcc_rawcas_SI(int,int);
DEF_xcc_rawcas_SI(int_float,xcc_int_float);

#define xcc_rawcas_short __xcc_rawcas_short
extern __inline__ int
__xcc_rawcas_short(short *loc, short ov, short nv){
  unsigned short ov16 = ov, tmp;
  __asm__ __volatile__("mov ar.ccv = %3\n"
		       "	;;\n"
		       "	cmpxchg2.acq %1 = %0, %2, ar.ccv"
		       : "=m"(*loc), "=r"(tmp)
		       : "r"(nv), "r"((unsigned long) ov16), "m"(*loc)
		       : "ar.ccv"); /* need "memory" ? */
  return (ov != tmp);
}

#define xcc_cas_short __xcc_cas_short
extern __inline__ int
__xcc_cas_short(short *loc, short ov, short nv){
  return ((*(volatile short *)loc) != ov) || xcc_rawcas_short(loc,ov,nv);
}

#define xcc_rawcas_char __xcc_rawcas_char
extern __inline__ int
__xcc_rawcas_char(char *loc, char ov, char nv){
  unsigned char ov8 = ov, tmp;
  __asm__ __volatile__("mov ar.ccv = %3\n"
		       "	;;\n"
		       "	cmpxchg1.acq %1 = %0, %2, ar.ccv"
		       : "=m"(*loc), "=r"(tmp)
		       : "r"(nv), "r"((unsigned long) ov8), "m"(*loc)
		       : "ar.ccv"); /* need "memory" ? */
  return (ov != tmp);
}

#define xcc_cas_char __xcc_cas_char
extern __inline__ int
__xcc_cas_char(char *loc, char ov, char nv){
  return ((*(volatile char *)loc) != ov) || xcc_rawcas_char(loc,ov,nv);
}

#endif /* __ia64__ */

/*
 * Common Emulations and/or Common (usual) GNUC case
 */

#ifndef xcc_cas_long_long
#define xcc_cas_long_long __xcc_cas_long_long
extern __inline__ int
__xcc_cas_long_long(long long *loc, long long ov, long long nv){
  return ((*(volatile long long *)loc) != ov) ||
    xcc_rawcas_long_long(loc,ov,nv);
}
#endif

#ifndef xcc_cas_long
#define xcc_cas_long __xcc_cas_long
extern __inline__ int
__xcc_cas_long(long *loc, long ov, long nv){
  return ((*(volatile long *)loc) != ov) || xcc_rawcas_long(loc,ov,nv);
}
#endif

#ifndef xcc_cas_ptr
#define xcc_cas_ptr __xcc_cas_ptr
extern __inline__ int
__xcc_cas_ptr(void **loc, void *ov, void *nv){
  return ((*(void * volatile *)loc) != ov) || xcc_rawcas_ptr(loc,ov,nv);
}
#endif

#ifndef xcc_cas_int
#define xcc_cas_int __xcc_cas_int
extern __inline__ int
__xcc_cas_int(int *loc, int ov, int nv){
  return ((*(volatile int *)loc) != ov) || xcc_rawcas_int(loc,ov,nv);
}
#endif

#ifndef xcc_cas_short
#define xcc_cas_short __xcc_cas_short
extern __inline__ int
__xcc_cas_short(short *loc, short ov, short nv){
  int ofs = (loc - (short *)(int *)0) & 1;
  volatile xcc_int_float *p = (volatile xcc_int_float *)(loc - ofs);
  xcc_int_float uov = *p, unv;
  if(uov.s[ofs] != ov) return 1;
  unv = uov; unv.s[ofs] = nv;
  return xcc_rawcas_int_float((xcc_int_float *)p,uov,unv);
}
#endif

#ifndef xcc_cas_char
#define xcc_cas_char __xcc_cas_char
extern __inline__ int
__xcc_cas_char(char *loc, char ov, char nv){
  int ofs = (loc - (char *)(int *)0) & 3;
  volatile xcc_int_float *p = (volatile xcc_int_float *)(loc - ofs);
  xcc_int_float uov = *p, unv;
  if(uov.c[ofs] != ov) return 1;
  unv = uov; unv.c[ofs] = nv;
  return xcc_rawcas_int_float((xcc_int_float *)p,uov,unv);
}
#endif

#ifndef xcc_cas_long_long_double
#define xcc_cas_long_long_double __xcc_cas_long_long_double
extern __inline__ int
__xcc_cas_long_long_double(xcc_long_long_double *loc,
			   xcc_long_long_double ov, xcc_long_long_double nv){
  return (((volatile xcc_long_long_double *)loc)->ll != ov.ll) ||
    xcc_rawcas_long_long_double(loc,ov,nv);
}
#endif

#ifndef xcc_cas_int_float
#define xcc_cas_int_float __xcc_cas_int_float
extern __inline__ int
__xcc_cas_int_float(xcc_int_float *loc, xcc_int_float ov, xcc_int_float nv){
  return (((volatile xcc_int_float *)loc)->i != ov.i) ||
    xcc_rawcas_int_float(loc,ov,nv);
}
#endif

#ifndef xcc_cas_double
#define xcc_cas_double __xcc_cas_double
extern __inline__ int
__xcc_cas_double(double *loc, double ov, double nv){
  return xcc_cas_long_long_double((xcc_long_long_double *)loc,
				  INTO_UNION(xcc_long_long_double,d,ov),
				  INTO_UNION(xcc_long_long_double,d,nv));
}
#endif

#ifndef xcc_cas_float
#define xcc_cas_float __xcc_cas_float
extern __inline__ int
__xcc_cas_float(float *loc, float ov, float nv){
  return xcc_cas_int_float((xcc_int_float *)loc,
			   INTO_UNION(xcc_int_float,f,ov),
			   INTO_UNION(xcc_int_float,f,nv));
}
#endif

#ifndef xcc_atomic_swap_long_long
#define xcc_atomic_swap_long_long __xcc_atomic_swap_long_long
extern __inline__ long long
__xcc_atomic_swap_long_long(long long *loc, long long val){
  long long ov;
  do{ ov = *loc; }while(xcc_rawcas_long_long(loc,ov,val));
  return ov;
}
#endif

#ifndef xcc_atomic_swap_long
#define xcc_atomic_swap_long __xcc_atomic_swap_long
extern __inline__ long
__xcc_atomic_swap_long(long *loc, long val){
  long ov;
  do{ ov = *loc; }while(xcc_rawcas_long(loc,ov,val));
  return ov;
}
#endif

#ifndef xcc_atomic_swap_ptr
#define xcc_atomic_swap_ptr __xcc_atomic_swap_ptr
extern __inline__ void *
__xcc_atomic_swap_ptr(void **loc, void *val){
  void *ov;
  do{ ov = *loc; }while(xcc_rawcas_ptr(loc,ov,val));
  return ov;
}
#endif

#ifndef xcc_atomic_swap_int
#define xcc_atomic_swap_int __xcc_atomic_swap_int
extern __inline__ int
__xcc_atomic_swap_int(int *loc, int val){
  int ov;
  do{ ov = *loc; }while(xcc_rawcas_int(loc,ov,val));
  return ov;
}
#endif

#ifndef xcc_atomic_swap_short
#define xcc_atomic_swap_short __xcc_atomic_swap_short
extern __inline__ short
__xcc_atomic_swap_short(short *loc, short val){
  int ofs = (loc- (short *)(int *)0) & 1;
  volatile xcc_int_float *p = (xcc_int_float *)(loc - ofs);
  xcc_int_float uov, unv;
  do{ uov = *p; unv = uov; unv.s[ofs] = val; 
  }while(xcc_rawcas_int_float((xcc_int_float *)p,uov,unv));
  return uov.s[ofs];
}
#endif

#ifndef xcc_atomic_swap_char
#define xcc_atomic_swap_char __xcc_atomic_swap_char
extern __inline__ char
__xcc_atomic_swap_char(char *loc, char val){
  int ofs = (loc- (char *)(int *)0) & 3;
  volatile xcc_int_float *p = (xcc_int_float *)(loc - ofs);
  xcc_int_float uov, unv;
  do{ uov = *p ; unv = uov; unv.c[ofs] = val;
  }while(xcc_rawcas_int_float((xcc_int_float *)p,uov,unv));
  return uov.c[ofs];
}
#endif

#ifndef xcc_atomic_swap_double
#define xcc_atomic_swap_double __xcc_atomic_swap_double
extern __inline__ double
__xcc_atomic_swap_double(double *loc, double val){
  xcc_long_long_double ov, nv = INTO_UNION(xcc_long_long_double,d,val);
  do{ ov = *(xcc_long_long_double *)loc;
  }while(xcc_rawcas_long_long_double((xcc_long_long_double *)loc, ov, nv));
  return ov.d;
}
#endif

#ifndef xcc_atomic_swap_float
#define xcc_atomic_swap_float __xcc_atomic_swap_float
extern __inline__ float
__xcc_atomic_swap_float(float *loc, float val){
  xcc_int_float ov, nv = INTO_UNION(xcc_int_float,f,val);
#ifdef xcc_atomic_swap_int_float
  ov = xcc_atomic_swap_int_float((xcc_int_float *)loc, nv);
#else
  do{ ov = *(xcc_int_float *)loc;
  }while(xcc_rawcas_int_float((xcc_int_float *)loc, ov, nv));
#endif
  return ov.f;
}
#endif

#define xcc_xread_char(loc) (*(volatile char *)(loc))
#define xcc_xread_short(loc) (*(volatile short *)(loc))
#define xcc_xread_int(loc) (*(volatile int *)(loc))
#define xcc_xread_long(loc) (*(volatile long *)(loc))
#define xcc_xread_long_long(loc) (*(volatile long long *)(loc))
#define xcc_xread_float(loc) (*(volatile float *)(loc))
#define xcc_xread_double(loc) (*(volatile double *)(loc))
#define xcc_xread_ptr(loc) (*(void * volatile *)(loc))

#define xcc_xwrite_char(loc,v) do{*(volatile char *)(loc)=(v);}while(0)
#define xcc_xwrite_short(loc,v) do{*(volatile short *)(loc)=(v);}while(0)
#define xcc_xwrite_int(loc,v) do{*(volatile int *)(loc)=(v);}while(0)
#define xcc_xwrite_long(loc,v) do{*(volatile long *)(loc)=(v);}while(0)
#define xcc_xwrite_long_long(loc,v) \
 do{*(volatile long long *)(loc)=(v);}while(0)
#define xcc_xwrite_float(loc,v) do{*(volatile float *)(loc)=(v);}while(0)
#define xcc_xwrite_double(loc,v) do{*(volatile double *)(loc)=(v);}while(0)
#define xcc_xwrite_ptr(loc,v) do{*(void * volatile *)(loc)=(v);}while(0)

#ifndef xcc_atomic_read_char
#define xcc_atomic_read_char xcc_xread_char
#endif
#ifndef xcc_atomic_read_short
#define xcc_atomic_read_short xcc_xread_short
#endif
#ifndef xcc_atomic_read_int
#define xcc_atomic_read_int xcc_xread_int
#endif
#ifndef xcc_atomic_read_long
#define xcc_atomic_read_long xcc_xread_long
#endif
/* long long: except for i386, PPC32, MIPS1/2  */
#ifndef xcc_atomic_read_long_long
#define xcc_atomic_read_long_long xcc_xread_long_long
#endif
#ifndef xcc_atomic_read_float
#define xcc_atomic_read_float xcc_xread_float
#endif
#ifndef xcc_atomic_read_double
#define xcc_atomic_read_double xcc_xread_double
#endif
#ifndef xcc_atomic_read_ptr
#define xcc_atomic_read_ptr xcc_xread_ptr
#endif

/* char: except for Alhpa  */
#ifndef xcc_atomic_write_char
#define xcc_atomic_write_char xcc_xwrite_char
#endif
#ifndef xcc_atomic_write_short
#define xcc_atomic_write_short xcc_xwrite_short
#endif
#ifndef xcc_atomic_write_int
#define xcc_atomic_write_int xcc_xwrite_int
#endif
#ifndef xcc_atomic_write_long
#define xcc_atomic_write_long xcc_xwrite_long
#endif
/* long long: except for i386, PPC32, MIPS1/2  */
#ifndef xcc_atomic_write_long_long
#define xcc_atomic_write_long_long xcc_xwrite_long_long
#endif
#ifndef xcc_atomic_write_float
#define xcc_atomic_write_float xcc_xwrite_float
#endif
#ifndef xcc_atomic_write_double
#define xcc_atomic_write_double xcc_xwrite_double
#endif
#ifndef xcc_atomic_write_ptr
#define xcc_atomic_write_ptr xcc_xwrite_ptr
#endif

#ifndef xcc_defaultbar
#define xcc_defaultbar xcc_nnbar
#endif

#ifndef xcc_ssbar
#define xcc_ssbar xcc_defaultbar
#endif
#ifndef xcc_slbar
#define xcc_slbar xcc_defaultbar
#endif
#ifndef xcc_sabar
#define xcc_sabar xcc_defaultbar
#endif
#ifndef xcc_lsbar
#define xcc_lsbar xcc_defaultbar
#endif
#ifndef xcc_llbar
#define xcc_llbar xcc_defaultbar
#endif
#ifndef xcc_labar
#define xcc_labar xcc_defaultbar
#endif
#ifndef xcc_asbar
#define xcc_asbar xcc_defaultbar
#endif
#ifndef xcc_albar
#define xcc_albar xcc_defaultbar
#endif
#ifndef xcc_aabar
#define xcc_aabar xcc_defaultbar
#endif

/* Pentium's spinlock means aabar() Alpha's spinlock does not */
#ifndef xcc_start_access_after_lock
#define xcc_start_access_after_lock xcc_aabar
#endif
#ifndef xcc_start_read_after_lock
#define xcc_start_read_after_lock xcc_albar
#endif
#ifndef xcc_start_write_after_lock
#define xcc_start_write_after_lock xcc_asbar
#endif
#ifndef xcc_finish_access_before_unlock
#define xcc_finish_access_before_unlock xcc_asbar
#endif
#ifndef xcc_finish_read_before_unlock
#define xcc_finish_read_before_unlock xcc_lsbar
#endif
#ifndef xcc_finish_write_before_unlock
#define xcc_finish_write_before_unlock xcc_ssbar
#endif

#define xcc_start_access_after_read xcc_labar
#define xcc_start_access_after_write xcc_sabar
#define xcc_start_read_after_read xcc_llbar
#define xcc_start_read_after_write xcc_slbar
#define xcc_start_write_after_read xcc_lsbar
#define xcc_start_write_after_write xcc_ssbar
#define xcc_finish_access_before_read xcc_albar
#define xcc_finish_access_before_write xcc_asbar
#define xcc_finish_read_before_read xcc_llbar
#define xcc_finish_read_before_write  xcc_lsbar
#define xcc_finish_write_before_read xcc_slbar
#define xcc_finish_write_before_write xcc_ssbar

#ifndef xcc_LOCK_INITIALIZER
#define xcc_LOCK_INITIALIZER 0
#endif
#ifndef xcc_lock_t
#define xcc_lock_t int
#endif
#ifndef xcc_try_lock
#define xcc_try_lock(loc) xcc_cas_int((loc),0,1)
#endif
#ifndef xcc_spin_lock
#define xcc_spin_lock __xcc_spin_lock
extern __inline__ void
__xcc_spin_lock(int *loc){ while(xcc_try_lock(loc)) ; }
#endif
#ifndef xcc_release_lock
#define xcc_release_lock(loc) xcc_atomic_write_int((loc),0)
#endif

#ifndef xcc_RWLOCK_INITIALIZER
#define xcc_RWLOCK_INITIALIZER 0
#endif
#ifndef xcc_rwlock_t
#define xcc_rwlock_t int
#endif
#ifndef xcc_try_rlock
#define xcc_try_rlock __xcc_try_rlock
extern __inline__ int
__xcc_try_rlock(int *loc){
  int c= *(volatile int *)(loc);
  return ((c<0) || xcc_rawcas_int(loc,c,c+1));
}
#endif
#ifndef xcc_spin_rlock
#define xcc_spin_rlock __xcc_spin_rlock
extern __inline__ void
__xcc_spin_rlock(int *loc){ while(xcc_try_rlock(loc)) ; }
#endif
#ifndef xcc_release_rlock
#define xcc_release_rlock __xcc_release_rlock
extern __inline__ void
__xcc_release_rlock(int *loc){
  int c; do{c = *loc;}while(xcc_rawcas_int(loc,c,c-1));
}
#endif
#ifndef xcc_try_wlock
#define xcc_try_wlock(loc) xcc_cas_int((loc),0,-1)
#endif
#ifndef xcc_spin_wlock
#define xcc_spin_wlock __xcc_spin_wlock
extern __inline__ void
__xcc_spin_wlock(int *loc){ while(xcc_try_wlock(loc)) ; }
#endif
#ifndef xcc_release_wlock
#define xcc_release_wlock(loc) xcc_atomic_write_int((loc),0)
#endif

/*
 * Compositions
 */

#ifndef xcc_spin_lock_to_start_access
#define xcc_spin_lock_to_start_access(loc) \
 do{ xcc_spin_lock(loc); xcc_start_access_after_lock(); }while(0)
#endif
#ifndef xcc_spin_lock_to_start_read
#define xcc_spin_lock_to_start_read(loc) \
 do{ xcc_spin_lock(loc); xcc_start_read_after_lock(); }while(0)
#endif
#ifndef xcc_spin_lock_to_start_write
#define xcc_spin_lock_to_start_write(loc) \
 do{ xcc_spin_lock(loc); xcc_start_write_after_lock(); }while(0)
#endif

#ifndef xcc_release_lock_to_finish_access
#define xcc_release_lock_to_finish_access __xcc_release_lock_to_finish_access
extern __inline__ void
__xcc_release_lock_to_finish_access(int *loc){ 
  xcc_finish_access_before_unlock(); xcc_release_lock(loc);
}
#endif
#ifndef xcc_release_lock_to_finish_read
#define xcc_release_lock_to_finish_read __xcc_release_lock_to_finish_read
extern __inline__ void
__xcc_release_lock_to_finish_read(int *loc){ 
  xcc_finish_read_before_unlock(); xcc_release_lock(loc);
}
#endif
#ifndef xcc_release_lock_to_finish_write
#define xcc_release_lock_to_finish_write __xcc_release_lock_to_finish_write
extern __inline__ void
__xcc_release_lock_to_finish_write(int *loc){ 
  xcc_finish_write_before_unlock(); xcc_release_lock(loc);
}
#endif

#ifndef xcc_spin_rlock_to_start_read
#define xcc_spin_rlock_to_start_read(loc) \
 do{ xcc_spin_rlock(loc); xcc_start_read_after_lock(); }while(0)
#endif

#ifndef xcc_release_rlock_to_finish_read
#define xcc_release_rlock_to_finish_read __xcc_release_rlock_to_finish_read
extern __inline__ void
__xcc_release_rlock_to_finish_read(int *loc){ 
  xcc_finish_read_before_unlock(); xcc_release_rlock(loc);
}
#endif

#ifndef xcc_spin_wlock_to_start_write
#define xcc_spin_wlock_to_start_write(loc) \
 do{ xcc_spin_wlock(loc); xcc_start_write_after_lock(); }while(0)
#endif

#ifndef xcc_release_wlock_to_finish_write
#define xcc_release_wlock_to_finish_write __xcc_release_wlock_to_finish_write
extern __inline__ void
__xcc_release_wlock_to_finish_write(int *loc){ 
  xcc_finish_write_before_unlock(); xcc_release_wlock(loc);
}
#endif

/* atomic_read_TP_to_.... */

#define DEF_xcc_comp_RB(TPN,TP,BN,B) \
extern __inline__ TP \
__xcc_atomic_read_##TPN##_to_start_##BN (TP *loc){\
  TP tmp = xcc_atomic_read_##TPN(loc); B(); return tmp;\
} EODEF

#ifndef xcc_atomic_read_char_to_start_access
#define xcc_atomic_read_char_to_start_access \
 __xcc_atomic_read_char_to_start_access
DEF_xcc_comp_RB(char,char,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_short_to_start_access
#define xcc_atomic_read_short_to_start_access \
 __xcc_atomic_read_short_to_start_access
DEF_xcc_comp_RB(short,short,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_int_to_start_access
#define xcc_atomic_read_int_to_start_access \
 __xcc_atomic_read_int_to_start_access
DEF_xcc_comp_RB(int,int,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_long_to_start_access
#define xcc_atomic_read_long_to_start_access \
 __xcc_atomic_read_long_to_start_access
DEF_xcc_comp_RB(long,long,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_long_long_to_start_access
#define xcc_atomic_read_long_long_to_start_access \
 __xcc_atomic_read_long_long_to_start_access
DEF_xcc_comp_RB(long_long,long long,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_ptr_to_start_access
#define xcc_atomic_read_ptr_to_start_access \
 __xcc_atomic_read_ptr_to_start_access
DEF_xcc_comp_RB(ptr,void *,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_float_to_start_access
#define xcc_atomic_read_float_to_start_access \
 __xcc_atomic_read_float_to_start_access
DEF_xcc_comp_RB(float,float,access,xcc_labar);
#endif
#ifndef xcc_atomic_read_double_to_start_access
#define xcc_atomic_read_double_to_start_access \
 __xcc_atomic_read_double_to_start_access
DEF_xcc_comp_RB(double,double,access,xcc_labar);
#endif

#ifndef xcc_atomic_read_char_to_start_read
#define xcc_atomic_read_char_to_start_read \
 __xcc_atomic_read_char_to_start_read
DEF_xcc_comp_RB(char,char,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_short_to_start_read
#define xcc_atomic_read_short_to_start_read \
 __xcc_atomic_read_short_to_start_read
DEF_xcc_comp_RB(short,short,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_int_to_start_read
#define xcc_atomic_read_int_to_start_read \
 __xcc_atomic_read_int_to_start_read
DEF_xcc_comp_RB(int,int,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_long_to_start_read
#define xcc_atomic_read_long_to_start_read \
 __xcc_atomic_read_long_to_start_read
DEF_xcc_comp_RB(long,long,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_long_long_to_start_read
#define xcc_atomic_read_long_long_to_start_read \
 __xcc_atomic_read_long_long_to_start_read
DEF_xcc_comp_RB(long_long,long long,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_ptr_to_start_read
#define xcc_atomic_read_ptr_to_start_read \
 __xcc_atomic_read_ptr_to_start_read
DEF_xcc_comp_RB(ptr,void *,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_float_to_start_read
#define xcc_atomic_read_float_to_start_read \
 __xcc_atomic_read_float_to_start_read
DEF_xcc_comp_RB(float,float,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_double_to_start_read
#define xcc_atomic_read_double_to_start_read \
 __xcc_atomic_read_double_to_start_read
DEF_xcc_comp_RB(double,double,read,xcc_llbar);
#endif

#ifndef xcc_atomic_read_char_to_start_write
#define xcc_atomic_read_char_to_start_write \
 __xcc_atomic_read_char_to_start_write
DEF_xcc_comp_RB(char,char,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_short_to_start_write
#define xcc_atomic_read_short_to_start_write \
 __xcc_atomic_read_short_to_start_write
DEF_xcc_comp_RB(short,short,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_int_to_start_write
#define xcc_atomic_read_int_to_start_write \
 __xcc_atomic_read_int_to_start_write
DEF_xcc_comp_RB(int,int,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_long_to_start_write
#define xcc_atomic_read_long_to_start_write \
 __xcc_atomic_read_long_to_start_write
DEF_xcc_comp_RB(long,long,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_long_long_to_start_write
#define xcc_atomic_read_long_long_to_start_write \
 __xcc_atomic_read_long_long_to_start_write
DEF_xcc_comp_RB(long_long,long long,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_ptr_to_start_write
#define xcc_atomic_read_ptr_to_start_write \
 __xcc_atomic_read_ptr_to_start_write
DEF_xcc_comp_RB(ptr,void *,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_float_to_start_write
#define xcc_atomic_read_float_to_start_write \
 __xcc_atomic_read_float_to_start_write
DEF_xcc_comp_RB(float,float,write,xcc_lsbar);
#endif
#ifndef xcc_atomic_read_double_to_start_write
#define xcc_atomic_read_double_to_start_write \
 __xcc_atomic_read_double_to_start_write
DEF_xcc_comp_RB(double,double,write,xcc_lsbar);
#endif

#define DEF_xcc_comp_RB2(TPN,TP,BN,B) \
extern __inline__ TP \
__xcc_atomic_read_##TPN##_to_finish_##BN (TP *loc){\
  B(); return xcc_atomic_read_##TPN(loc);\
} EODEF

#ifndef xcc_atomic_read_char_to_finish_access
#define xcc_atomic_read_char_to_finish_access \
 __xcc_atomic_read_char_to_finish_access
DEF_xcc_comp_RB2(char,char,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_short_to_finish_access
#define xcc_atomic_read_short_to_finish_access \
 __xcc_atomic_read_short_to_finish_access
DEF_xcc_comp_RB2(short,short,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_int_to_finish_access
#define xcc_atomic_read_int_to_finish_access \
 __xcc_atomic_read_int_to_finish_access
DEF_xcc_comp_RB2(int,int,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_long_to_finish_access
#define xcc_atomic_read_long_to_finish_access \
 __xcc_atomic_read_long_to_finish_access
DEF_xcc_comp_RB2(long,long,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_long_long_to_finish_access
#define xcc_atomic_read_long_long_to_finish_access \
 __xcc_atomic_read_long_long_to_finish_access
DEF_xcc_comp_RB2(long_long,long long,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_ptr_to_finish_access
#define xcc_atomic_read_ptr_to_finish_access \
 __xcc_atomic_read_ptr_to_finish_access
DEF_xcc_comp_RB2(ptr,void *,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_float_to_finish_access
#define xcc_atomic_read_float_to_finish_access \
 __xcc_atomic_read_float_to_finish_access
DEF_xcc_comp_RB2(float,float,access,xcc_albar);
#endif
#ifndef xcc_atomic_read_double_to_finish_access
#define xcc_atomic_read_double_to_finish_access \
 __xcc_atomic_read_double_to_finish_access
DEF_xcc_comp_RB2(double,double,access,xcc_albar);
#endif

#ifndef xcc_atomic_read_char_to_finish_read
#define xcc_atomic_read_char_to_finish_read \
 __xcc_atomic_read_char_to_finish_read
DEF_xcc_comp_RB2(char,char,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_short_to_finish_read
#define xcc_atomic_read_short_to_finish_read \
 __xcc_atomic_read_short_to_finish_read
DEF_xcc_comp_RB2(short,short,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_int_to_finish_read
#define xcc_atomic_read_int_to_finish_read \
 __xcc_atomic_read_int_to_finish_read
DEF_xcc_comp_RB2(int,int,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_long_to_finish_read
#define xcc_atomic_read_long_to_finish_read \
 __xcc_atomic_read_long_to_finish_read
DEF_xcc_comp_RB2(long,long,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_long_long_to_finish_read
#define xcc_atomic_read_long_long_to_finish_read \
 __xcc_atomic_read_long_long_to_finish_read
DEF_xcc_comp_RB2(long_long,long long,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_ptr_to_finish_read
#define xcc_atomic_read_ptr_to_finish_read \
 __xcc_atomic_read_ptr_to_finish_read
DEF_xcc_comp_RB2(ptr,void *,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_float_to_finish_read
#define xcc_atomic_read_float_to_finish_read \
 __xcc_atomic_read_float_to_finish_read
DEF_xcc_comp_RB2(float,float,read,xcc_llbar);
#endif
#ifndef xcc_atomic_read_double_to_finish_read
#define xcc_atomic_read_double_to_finish_read \
 __xcc_atomic_read_double_to_finish_read
DEF_xcc_comp_RB2(double,double,read,xcc_llbar);
#endif

#ifndef xcc_atomic_read_char_to_finish_write
#define xcc_atomic_read_char_to_finish_write \
 __xcc_atomic_read_char_to_finish_write
DEF_xcc_comp_RB2(char,char,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_short_to_finish_write
#define xcc_atomic_read_short_to_finish_write \
 __xcc_atomic_read_short_to_finish_write
DEF_xcc_comp_RB2(short,short,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_int_to_finish_write
#define xcc_atomic_read_int_to_finish_write \
 __xcc_atomic_read_int_to_finish_write
DEF_xcc_comp_RB2(int,int,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_long_to_finish_write
#define xcc_atomic_read_long_to_finish_write \
 __xcc_atomic_read_long_to_finish_write
DEF_xcc_comp_RB2(long,long,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_long_long_to_finish_write
#define xcc_atomic_read_long_long_to_finish_write \
 __xcc_atomic_read_long_long_to_finish_write
DEF_xcc_comp_RB2(long_long,long long,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_ptr_to_finish_write
#define xcc_atomic_read_ptr_to_finish_write \
 __xcc_atomic_read_ptr_to_finish_write
DEF_xcc_comp_RB2(ptr,void *,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_float_to_finish_write
#define xcc_atomic_read_float_to_finish_write \
 __xcc_atomic_read_float_to_finish_write
DEF_xcc_comp_RB2(float,float,write,xcc_slbar);
#endif
#ifndef xcc_atomic_read_double_to_finish_write
#define xcc_atomic_read_double_to_finish_write \
 __xcc_atomic_read_double_to_finish_write
DEF_xcc_comp_RB2(double,double,write,xcc_slbar);
#endif

/* atomic_swap_TP_to_.... */

#define DEF_xcc_comp_SB(TPN,TP,BN,B) \
extern __inline__ TP \
__xcc_atomic_swap_##TPN##_to_start_##BN (TP *loc, TP ov){\
  TP tmp = xcc_atomic_swap_##TPN(loc, ov); B(); return tmp;\
} EODEF

#ifndef xcc_atomic_swap_char_to_start_access
#define xcc_atomic_swap_char_to_start_access \
 __xcc_atomic_swap_char_to_start_access
DEF_xcc_comp_SB(char,char,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_short_to_start_access
#define xcc_atomic_swap_short_to_start_access \
 __xcc_atomic_swap_short_to_start_access
DEF_xcc_comp_SB(short,short,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_int_to_start_access
#define xcc_atomic_swap_int_to_start_access \
 __xcc_atomic_swap_int_to_start_access
DEF_xcc_comp_SB(int,int,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_long_to_start_access
#define xcc_atomic_swap_long_to_start_access \
 __xcc_atomic_swap_long_to_start_access
DEF_xcc_comp_SB(long,long,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_long_long_to_start_access
#define xcc_atomic_swap_long_long_to_start_access \
 __xcc_atomic_swap_long_long_to_start_access
DEF_xcc_comp_SB(long_long,long long,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_ptr_to_start_access
#define xcc_atomic_swap_ptr_to_start_access \
 __xcc_atomic_swap_ptr_to_start_access
DEF_xcc_comp_SB(ptr,void *,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_float_to_start_access
#define xcc_atomic_swap_float_to_start_access \
 __xcc_atomic_swap_float_to_start_access
DEF_xcc_comp_SB(float,float,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_atomic_swap_double_to_start_access
#define xcc_atomic_swap_double_to_start_access \
 __xcc_atomic_swap_double_to_start_access
DEF_xcc_comp_SB(double,double,access,xcc_start_access_after_lock);
#endif

#ifndef xcc_atomic_swap_char_to_start_read
#define xcc_atomic_swap_char_to_start_read \
 __xcc_atomic_swap_char_to_start_read
DEF_xcc_comp_SB(char,char,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_short_to_start_read
#define xcc_atomic_swap_short_to_start_read \
 __xcc_atomic_swap_short_to_start_read
DEF_xcc_comp_SB(short,short,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_int_to_start_read
#define xcc_atomic_swap_int_to_start_read \
 __xcc_atomic_swap_int_to_start_read
DEF_xcc_comp_SB(int,int,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_long_to_start_read
#define xcc_atomic_swap_long_to_start_read \
 __xcc_atomic_swap_long_to_start_read
DEF_xcc_comp_SB(long,long,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_long_long_to_start_read
#define xcc_atomic_swap_long_long_to_start_read \
 __xcc_atomic_swap_long_long_to_start_read
DEF_xcc_comp_SB(long_long,long long,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_ptr_to_start_read
#define xcc_atomic_swap_ptr_to_start_read \
 __xcc_atomic_swap_ptr_to_start_read
DEF_xcc_comp_SB(ptr,void *,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_float_to_start_read
#define xcc_atomic_swap_float_to_start_read \
 __xcc_atomic_swap_float_to_start_read
DEF_xcc_comp_SB(float,float,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_atomic_swap_double_to_start_read
#define xcc_atomic_swap_double_to_start_read \
 __xcc_atomic_swap_double_to_start_read
DEF_xcc_comp_SB(double,double,read,xcc_start_read_after_lock);
#endif

#ifndef xcc_atomic_swap_char_to_start_write
#define xcc_atomic_swap_char_to_start_write \
 __xcc_atomic_swap_char_to_start_write
DEF_xcc_comp_SB(char,char,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_short_to_start_write
#define xcc_atomic_swap_short_to_start_write \
 __xcc_atomic_swap_short_to_start_write
DEF_xcc_comp_SB(short,short,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_int_to_start_write
#define xcc_atomic_swap_int_to_start_write \
 __xcc_atomic_swap_int_to_start_write
DEF_xcc_comp_SB(int,int,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_long_to_start_write
#define xcc_atomic_swap_long_to_start_write \
 __xcc_atomic_swap_long_to_start_write
DEF_xcc_comp_SB(long,long,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_long_long_to_start_write
#define xcc_atomic_swap_long_long_to_start_write \
 __xcc_atomic_swap_long_long_to_start_write
DEF_xcc_comp_SB(long_long,long long,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_ptr_to_start_write
#define xcc_atomic_swap_ptr_to_start_write \
 __xcc_atomic_swap_ptr_to_start_write
DEF_xcc_comp_SB(ptr,void *,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_float_to_start_write
#define xcc_atomic_swap_float_to_start_write \
 __xcc_atomic_swap_float_to_start_write
DEF_xcc_comp_SB(float,float,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_atomic_swap_double_to_start_write
#define xcc_atomic_swap_double_to_start_write \
 __xcc_atomic_swap_double_to_start_write
DEF_xcc_comp_SB(double,double,write,xcc_start_write_after_lock);
#endif

#define DEF_xcc_comp_SB2(TPN,TP,BN,B) \
extern __inline__ TP \
__xcc_atomic_swap_##TPN##_to_finish_##BN (TP *loc, TP ov){\
  B(); return xcc_atomic_swap_##TPN(loc, ov); \
} EODEF

#ifndef xcc_atomic_swap_char_to_finish_access
#define xcc_atomic_swap_char_to_finish_access \
 __xcc_atomic_swap_char_to_finish_access
DEF_xcc_comp_SB2(char,char,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_short_to_finish_access
#define xcc_atomic_swap_short_to_finish_access \
 __xcc_atomic_swap_short_to_finish_access
DEF_xcc_comp_SB2(short,short,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_int_to_finish_access
#define xcc_atomic_swap_int_to_finish_access \
 __xcc_atomic_swap_int_to_finish_access
DEF_xcc_comp_SB2(int,int,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_long_to_finish_access
#define xcc_atomic_swap_long_to_finish_access \
 __xcc_atomic_swap_long_to_finish_access
DEF_xcc_comp_SB2(long,long,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_long_long_to_finish_access
#define xcc_atomic_swap_long_long_to_finish_access \
 __xcc_atomic_swap_long_long_to_finish_access
DEF_xcc_comp_SB2(long_long,long long,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_ptr_to_finish_access
#define xcc_atomic_swap_ptr_to_finish_access \
 __xcc_atomic_swap_ptr_to_finish_access
DEF_xcc_comp_SB2(ptr,void *,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_float_to_finish_access
#define xcc_atomic_swap_float_to_finish_access \
 __xcc_atomic_swap_float_to_finish_access
DEF_xcc_comp_SB2(float,float,access,xcc_asbar);
#endif
#ifndef xcc_atomic_swap_double_to_finish_access
#define xcc_atomic_swap_double_to_finish_access \
 __xcc_atomic_swap_double_to_finish_access
DEF_xcc_comp_SB2(double,double,access,xcc_asbar);
#endif

#ifndef xcc_atomic_swap_char_to_finish_read
#define xcc_atomic_swap_char_to_finish_read \
 __xcc_atomic_swap_char_to_finish_read
DEF_xcc_comp_SB2(char,char,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_short_to_finish_read
#define xcc_atomic_swap_short_to_finish_read \
 __xcc_atomic_swap_short_to_finish_read
DEF_xcc_comp_SB2(short,short,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_int_to_finish_read
#define xcc_atomic_swap_int_to_finish_read \
 __xcc_atomic_swap_int_to_finish_read
DEF_xcc_comp_SB2(int,int,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_long_to_finish_read
#define xcc_atomic_swap_long_to_finish_read \
 __xcc_atomic_swap_long_to_finish_read
DEF_xcc_comp_SB2(long,long,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_long_long_to_finish_read
#define xcc_atomic_swap_long_long_to_finish_read \
 __xcc_atomic_swap_long_long_to_finish_read
DEF_xcc_comp_SB2(long_long,long long,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_ptr_to_finish_read
#define xcc_atomic_swap_ptr_to_finish_read \
 __xcc_atomic_swap_ptr_to_finish_read
DEF_xcc_comp_SB2(ptr,void *,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_float_to_finish_read
#define xcc_atomic_swap_float_to_finish_read \
 __xcc_atomic_swap_float_to_finish_read
DEF_xcc_comp_SB2(float,float,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_swap_double_to_finish_read
#define xcc_atomic_swap_double_to_finish_read \
 __xcc_atomic_swap_double_to_finish_read
DEF_xcc_comp_SB2(double,double,read,xcc_lsbar);
#endif

#ifndef xcc_atomic_swap_char_to_finish_write
#define xcc_atomic_swap_char_to_finish_write \
 __xcc_atomic_swap_char_to_finish_write
DEF_xcc_comp_SB2(char,char,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_short_to_finish_write
#define xcc_atomic_swap_short_to_finish_write \
 __xcc_atomic_swap_short_to_finish_write
DEF_xcc_comp_SB2(short,short,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_int_to_finish_write
#define xcc_atomic_swap_int_to_finish_write \
 __xcc_atomic_swap_int_to_finish_write
DEF_xcc_comp_SB2(int,int,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_long_to_finish_write
#define xcc_atomic_swap_long_to_finish_write \
 __xcc_atomic_swap_long_to_finish_write
DEF_xcc_comp_SB2(long,long,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_long_long_to_finish_write
#define xcc_atomic_swap_long_long_to_finish_write \
 __xcc_atomic_swap_long_long_to_finish_write
DEF_xcc_comp_SB2(long_long,long long,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_ptr_to_finish_write
#define xcc_atomic_swap_ptr_to_finish_write \
 __xcc_atomic_swap_ptr_to_finish_write
DEF_xcc_comp_SB2(ptr,void *,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_float_to_finish_write
#define xcc_atomic_swap_float_to_finish_write \
 __xcc_atomic_swap_float_to_finish_write
DEF_xcc_comp_SB2(float,float,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_swap_double_to_finish_write
#define xcc_atomic_swap_double_to_finish_write \
 __xcc_atomic_swap_double_to_finish_write
DEF_xcc_comp_SB2(double,double,write,xcc_ssbar);
#endif

/* atomic_write_TP_to_.... */

#ifndef xcc_atomic_write_char_to_start_access
#define xcc_atomic_write_char_to_start_access(loc,val) \
 do{ xcc_atomic_write_char((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_short_to_start_access
#define xcc_atomic_write_short_to_start_access(loc,val) \
 do{ xcc_atomic_write_short((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_int_to_start_access
#define xcc_atomic_write_int_to_start_access(loc,val) \
 do{ xcc_atomic_write_int((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_long_to_start_access
#define xcc_atomic_write_long_to_start_access(loc,val) \
 do{ xcc_atomic_write_long((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_long_long_to_start_access
#define xcc_atomic_write_long_long_to_start_access(loc,val) \
 do{ xcc_atomic_write_long_long((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_ptr_to_start_access
#define xcc_atomic_write_ptr_to_start_access(loc,val) \
 do{ xcc_atomic_write_ptr((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_float_to_start_access
#define xcc_atomic_write_float_to_start_access(loc,val) \
 do{ xcc_atomic_write_float((loc),(val)); xcc_sabar(); }while(0)
#endif
#ifndef xcc_atomic_write_double_to_start_access
#define xcc_atomic_write_double_to_start_access(loc,val) \
 do{ xcc_atomic_write_double((loc),(val)); xcc_sabar(); }while(0)
#endif

#ifndef xcc_atomic_write_char_to_start_read
#define xcc_atomic_write_char_to_start_read(loc,val) \
 do{ xcc_atomic_write_char((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_short_to_start_read
#define xcc_atomic_write_short_to_start_read(loc,val) \
 do{ xcc_atomic_write_short((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_int_to_start_read
#define xcc_atomic_write_int_to_start_read(loc,val) \
 do{ xcc_atomic_write_int((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_long_to_start_read
#define xcc_atomic_write_long_to_start_read(loc,val) \
 do{ xcc_atomic_write_long((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_long_long_to_start_read
#define xcc_atomic_write_long_long_to_start_read(loc,val) \
 do{ xcc_atomic_write_long_long((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_ptr_to_start_read
#define xcc_atomic_write_ptr_to_start_read(loc,val) \
 do{ xcc_atomic_write_ptr((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_float_to_start_read
#define xcc_atomic_write_float_to_start_read(loc,val) \
 do{ xcc_atomic_write_float((loc),(val)); xcc_slbar(); }while(0)
#endif
#ifndef xcc_atomic_write_double_to_start_read
#define xcc_atomic_write_double_to_start_read(loc,val) \
 do{ xcc_atomic_write_double((loc),(val)); xcc_slbar(); }while(0)
#endif

#ifndef xcc_atomic_write_char_to_start_write
#define xcc_atomic_write_char_to_start_write(loc,val) \
 do{ xcc_atomic_write_char((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_short_to_start_write
#define xcc_atomic_write_short_to_start_write(loc,val) \
 do{ xcc_atomic_write_short((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_int_to_start_write
#define xcc_atomic_write_int_to_start_write(loc,val) \
 do{ xcc_atomic_write_int((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_long_to_start_write
#define xcc_atomic_write_long_to_start_write(loc,val) \
 do{ xcc_atomic_write_long((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_long_long_to_start_write
#define xcc_atomic_write_long_long_to_start_write(loc,val) \
 do{ xcc_atomic_write_long_long((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_ptr_to_start_write
#define xcc_atomic_write_ptr_to_start_write(loc,val) \
 do{ xcc_atomic_write_ptr((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_float_to_start_write
#define xcc_atomic_write_float_to_start_write(loc,val) \
 do{ xcc_atomic_write_float((loc),(val)); xcc_ssbar(); }while(0)
#endif
#ifndef xcc_atomic_write_double_to_start_write
#define xcc_atomic_write_double_to_start_write(loc,val) \
 do{ xcc_atomic_write_double((loc),(val)); xcc_ssbar(); }while(0)
#endif

#define DEF_xcc_comp_WB2(TPN,TP,BN,B) \
extern __inline__ TP \
 __xcc_atomic_write_##TPN##_to_finish_##BN (TP *loc, TP ov){	\
  B(); xcc_atomic_write_##TPN(loc, ov);				\
} EODEF

#ifndef xcc_atomic_write_char_to_finish_access
#define xcc_atomic_write_char_to_finish_access \
 __xcc_atomic_write_char_to_finish_access
DEF_xcc_comp_WB2(char,char,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_short_to_finish_access
#define xcc_atomic_write_short_to_finish_access \
 __xcc_atomic_write_short_to_finish_access
DEF_xcc_comp_WB2(short,short,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_int_to_finish_access
#define xcc_atomic_write_int_to_finish_access \
 __xcc_atomic_write_int_to_finish_access
DEF_xcc_comp_WB2(int,int,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_long_to_finish_access
#define xcc_atomic_write_long_to_finish_access \
 __xcc_atomic_write_long_to_finish_access
DEF_xcc_comp_WB2(long,long,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_long_long_to_finish_access
#define xcc_atomic_write_long_long_to_finish_access \
 __xcc_atomic_write_long_long_to_finish_access
DEF_xcc_comp_WB2(long_long,long long,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_ptr_to_finish_access
#define xcc_atomic_write_ptr_to_finish_access \
 __xcc_atomic_write_ptr_to_finish_access
DEF_xcc_comp_WB2(ptr,void *,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_float_to_finish_access
#define xcc_atomic_write_float_to_finish_access \
 __xcc_atomic_write_float_to_finish_access
DEF_xcc_comp_WB2(float,float,access,xcc_asbar);
#endif
#ifndef xcc_atomic_write_double_to_finish_access
#define xcc_atomic_write_double_to_finish_access \
 __xcc_atomic_write_double_to_finish_access
DEF_xcc_comp_WB2(double,double,access,xcc_asbar);
#endif

#ifndef xcc_atomic_write_char_to_finish_read
#define xcc_atomic_write_char_to_finish_read \
 __xcc_atomic_write_char_to_finish_read
DEF_xcc_comp_WB2(char,char,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_short_to_finish_read
#define xcc_atomic_write_short_to_finish_read \
 __xcc_atomic_write_short_to_finish_read
DEF_xcc_comp_WB2(short,short,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_int_to_finish_read
#define xcc_atomic_write_int_to_finish_read \
 __xcc_atomic_write_int_to_finish_read
DEF_xcc_comp_WB2(int,int,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_long_to_finish_read
#define xcc_atomic_write_long_to_finish_read \
 __xcc_atomic_write_long_to_finish_read
DEF_xcc_comp_WB2(long,long,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_long_long_to_finish_read
#define xcc_atomic_write_long_long_to_finish_read \
 __xcc_atomic_write_long_long_to_finish_read
DEF_xcc_comp_WB2(long_long,long long,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_ptr_to_finish_read
#define xcc_atomic_write_ptr_to_finish_read \
 __xcc_atomic_write_ptr_to_finish_read
DEF_xcc_comp_WB2(ptr,void *,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_float_to_finish_read
#define xcc_atomic_write_float_to_finish_read \
 __xcc_atomic_write_float_to_finish_read
DEF_xcc_comp_WB2(float,float,read,xcc_lsbar);
#endif
#ifndef xcc_atomic_write_double_to_finish_read
#define xcc_atomic_write_double_to_finish_read \
 __xcc_atomic_write_double_to_finish_read
DEF_xcc_comp_WB2(double,double,read,xcc_lsbar);
#endif

#ifndef xcc_atomic_write_char_to_finish_write
#define xcc_atomic_write_char_to_finish_write \
 __xcc_atomic_write_char_to_finish_write
DEF_xcc_comp_WB2(char,char,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_short_to_finish_write
#define xcc_atomic_write_short_to_finish_write \
 __xcc_atomic_write_short_to_finish_write
DEF_xcc_comp_WB2(short,short,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_int_to_finish_write
#define xcc_atomic_write_int_to_finish_write \
 __xcc_atomic_write_int_to_finish_write
DEF_xcc_comp_WB2(int,int,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_long_to_finish_write
#define xcc_atomic_write_long_to_finish_write \
 __xcc_atomic_write_long_to_finish_write
DEF_xcc_comp_WB2(long,long,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_long_long_to_finish_write
#define xcc_atomic_write_long_long_to_finish_write \
 __xcc_atomic_write_long_long_to_finish_write
DEF_xcc_comp_WB2(long_long,long long,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_ptr_to_finish_write
#define xcc_atomic_write_ptr_to_finish_write \
 __xcc_atomic_write_ptr_to_finish_write
DEF_xcc_comp_WB2(ptr,void *,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_float_to_finish_write
#define xcc_atomic_write_float_to_finish_write \
 __xcc_atomic_write_float_to_finish_write
DEF_xcc_comp_WB2(float,float,write,xcc_ssbar);
#endif
#ifndef xcc_atomic_write_double_to_finish_write
#define xcc_atomic_write_double_to_finish_write \
 __xcc_atomic_write_double_to_finish_write
DEF_xcc_comp_WB2(double,double,write,xcc_ssbar);
#endif

/* cas_TP_to_.... */

#define DEF_xcc_comp_CB(TPN,TP,BN,B) \
extern __inline__ int \
__xcc_cas_##TPN##_to_start_##BN (TP *loc, TP ov, TP nv){\
  int tmp = xcc_cas_##TPN(loc, ov, nv); B(); return tmp;\
} EODEF

#ifndef xcc_cas_char_to_start_access
#define xcc_cas_char_to_start_access \
 __xcc_cas_char_to_start_access
DEF_xcc_comp_CB(char,char,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_short_to_start_access
#define xcc_cas_short_to_start_access \
 __xcc_cas_short_to_start_access
DEF_xcc_comp_CB(short,short,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_int_to_start_access
#define xcc_cas_int_to_start_access \
 __xcc_cas_int_to_start_access
DEF_xcc_comp_CB(int,int,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_long_to_start_access
#define xcc_cas_long_to_start_access \
 __xcc_cas_long_to_start_access
DEF_xcc_comp_CB(long,long,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_long_long_to_start_access
#define xcc_cas_long_long_to_start_access \
 __xcc_cas_long_long_to_start_access
DEF_xcc_comp_CB(long_long,long long,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_ptr_to_start_access
#define xcc_cas_ptr_to_start_access \
 __xcc_cas_ptr_to_start_access
DEF_xcc_comp_CB(ptr,void *,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_float_to_start_access
#define xcc_cas_float_to_start_access \
 __xcc_cas_float_to_start_access
DEF_xcc_comp_CB(float,float,access,xcc_start_access_after_lock);
#endif
#ifndef xcc_cas_double_to_start_access
#define xcc_cas_double_to_start_access \
 __xcc_cas_double_to_start_access
DEF_xcc_comp_CB(double,double,access,xcc_start_access_after_lock);
#endif

#ifndef xcc_cas_char_to_start_read
#define xcc_cas_char_to_start_read \
 __xcc_cas_char_to_start_read
DEF_xcc_comp_CB(char,char,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_short_to_start_read
#define xcc_cas_short_to_start_read \
 __xcc_cas_short_to_start_read
DEF_xcc_comp_CB(short,short,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_int_to_start_read
#define xcc_cas_int_to_start_read \
 __xcc_cas_int_to_start_read
DEF_xcc_comp_CB(int,int,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_long_to_start_read
#define xcc_cas_long_to_start_read \
 __xcc_cas_long_to_start_read
DEF_xcc_comp_CB(long,long,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_long_long_to_start_read
#define xcc_cas_long_long_to_start_read \
 __xcc_cas_long_long_to_start_read
DEF_xcc_comp_CB(long_long,long long,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_ptr_to_start_read
#define xcc_cas_ptr_to_start_read \
 __xcc_cas_ptr_to_start_read
DEF_xcc_comp_CB(ptr,void *,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_float_to_start_read
#define xcc_cas_float_to_start_read \
 __xcc_cas_float_to_start_read
DEF_xcc_comp_CB(float,float,read,xcc_start_read_after_lock);
#endif
#ifndef xcc_cas_double_to_start_read
#define xcc_cas_double_to_start_read \
 __xcc_cas_double_to_start_read
DEF_xcc_comp_CB(double,double,read,xcc_start_read_after_lock);
#endif

#ifndef xcc_cas_char_to_start_write
#define xcc_cas_char_to_start_write \
 __xcc_cas_char_to_start_write
DEF_xcc_comp_CB(char,char,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_short_to_start_write
#define xcc_cas_short_to_start_write \
 __xcc_cas_short_to_start_write
DEF_xcc_comp_CB(short,short,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_int_to_start_write
#define xcc_cas_int_to_start_write \
 __xcc_cas_int_to_start_write
DEF_xcc_comp_CB(int,int,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_long_to_start_write
#define xcc_cas_long_to_start_write \
 __xcc_cas_long_to_start_write
DEF_xcc_comp_CB(long,long,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_long_long_to_start_write
#define xcc_cas_long_long_to_start_write \
 __xcc_cas_long_long_to_start_write
DEF_xcc_comp_CB(long_long,long long,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_ptr_to_start_write
#define xcc_cas_ptr_to_start_write \
 __xcc_cas_ptr_to_start_write
DEF_xcc_comp_CB(ptr,void *,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_float_to_start_write
#define xcc_cas_float_to_start_write \
 __xcc_cas_float_to_start_write
DEF_xcc_comp_CB(float,float,write,xcc_start_write_after_lock);
#endif
#ifndef xcc_cas_double_to_start_write
#define xcc_cas_double_to_start_write \
 __xcc_cas_double_to_start_write
DEF_xcc_comp_CB(double,double,write,xcc_start_write_after_lock);
#endif

#define DEF_xcc_comp_CB2(TPN,TP,BN,B) \
extern __inline__ int \
__xcc_cas_##TPN##_to_finish_##BN (TP *loc, TP ov, TP nv){\
  B(); return xcc_cas_##TPN(loc, ov, nv);\
} EODEF

#ifndef xcc_cas_char_to_finish_access
#define xcc_cas_char_to_finish_access \
 __xcc_cas_char_to_finish_access
DEF_xcc_comp_CB2(char,char,access,xcc_asbar);
#endif
#ifndef xcc_cas_short_to_finish_access
#define xcc_cas_short_to_finish_access \
 __xcc_cas_short_to_finish_access
DEF_xcc_comp_CB2(short,short,access,xcc_asbar);
#endif
#ifndef xcc_cas_int_to_finish_access
#define xcc_cas_int_to_finish_access \
 __xcc_cas_int_to_finish_access
DEF_xcc_comp_CB2(int,int,access,xcc_asbar);
#endif
#ifndef xcc_cas_long_to_finish_access
#define xcc_cas_long_to_finish_access \
 __xcc_cas_long_to_finish_access
DEF_xcc_comp_CB2(long,long,access,xcc_asbar);
#endif
#ifndef xcc_cas_long_long_to_finish_access
#define xcc_cas_long_long_to_finish_access \
 __xcc_cas_long_long_to_finish_access
DEF_xcc_comp_CB2(long_long,long long,access,xcc_asbar);
#endif
#ifndef xcc_cas_ptr_to_finish_access
#define xcc_cas_ptr_to_finish_access \
 __xcc_cas_ptr_to_finish_access
DEF_xcc_comp_CB2(ptr,void *,access,xcc_asbar);
#endif
#ifndef xcc_cas_float_to_finish_access
#define xcc_cas_float_to_finish_access \
 __xcc_cas_float_to_finish_access
DEF_xcc_comp_CB2(float,float,access,xcc_asbar);
#endif
#ifndef xcc_cas_double_to_finish_access
#define xcc_cas_double_to_finish_access \
 __xcc_cas_double_to_finish_access
DEF_xcc_comp_CB2(double,double,access,xcc_asbar);
#endif

#ifndef xcc_cas_char_to_finish_read
#define xcc_cas_char_to_finish_read \
 __xcc_cas_char_to_finish_read
DEF_xcc_comp_CB2(char,char,read,xcc_lsbar);
#endif
#ifndef xcc_cas_short_to_finish_read
#define xcc_cas_short_to_finish_read \
 __xcc_cas_short_to_finish_read
DEF_xcc_comp_CB2(short,short,read,xcc_lsbar);
#endif
#ifndef xcc_cas_int_to_finish_read
#define xcc_cas_int_to_finish_read \
 __xcc_cas_int_to_finish_read
DEF_xcc_comp_CB2(int,int,read,xcc_lsbar);
#endif
#ifndef xcc_cas_long_to_finish_read
#define xcc_cas_long_to_finish_read \
 __xcc_cas_long_to_finish_read
DEF_xcc_comp_CB2(long,long,read,xcc_lsbar);
#endif
#ifndef xcc_cas_long_long_to_finish_read
#define xcc_cas_long_long_to_finish_read \
 __xcc_cas_long_long_to_finish_read
DEF_xcc_comp_CB2(long_long,long long,read,xcc_lsbar);
#endif
#ifndef xcc_cas_ptr_to_finish_read
#define xcc_cas_ptr_to_finish_read \
 __xcc_cas_ptr_to_finish_read
DEF_xcc_comp_CB2(ptr,void *,read,xcc_lsbar);
#endif
#ifndef xcc_cas_float_to_finish_read
#define xcc_cas_float_to_finish_read \
 __xcc_cas_float_to_finish_read
DEF_xcc_comp_CB2(float,float,read,xcc_lsbar);
#endif
#ifndef xcc_cas_double_to_finish_read
#define xcc_cas_double_to_finish_read \
 __xcc_cas_double_to_finish_read
DEF_xcc_comp_CB2(double,double,read,xcc_lsbar);
#endif

#ifndef xcc_cas_char_to_finish_write
#define xcc_cas_char_to_finish_write \
 __xcc_cas_char_to_finish_write
DEF_xcc_comp_CB2(char,char,write,xcc_ssbar);
#endif
#ifndef xcc_cas_short_to_finish_write
#define xcc_cas_short_to_finish_write \
 __xcc_cas_short_to_finish_write
DEF_xcc_comp_CB2(short,short,write,xcc_ssbar);
#endif
#ifndef xcc_cas_int_to_finish_write
#define xcc_cas_int_to_finish_write \
 __xcc_cas_int_to_finish_write
DEF_xcc_comp_CB2(int,int,write,xcc_ssbar);
#endif
#ifndef xcc_cas_long_to_finish_write
#define xcc_cas_long_to_finish_write \
 __xcc_cas_long_to_finish_write
DEF_xcc_comp_CB2(long,long,write,xcc_ssbar);
#endif
#ifndef xcc_cas_long_long_to_finish_write
#define xcc_cas_long_long_to_finish_write \
 __xcc_cas_long_long_to_finish_write
DEF_xcc_comp_CB2(long_long,long long,write,xcc_ssbar);
#endif
#ifndef xcc_cas_ptr_to_finish_write
#define xcc_cas_ptr_to_finish_write \
 __xcc_cas_ptr_to_finish_write
DEF_xcc_comp_CB2(ptr,void *,write,xcc_ssbar);
#endif
#ifndef xcc_cas_float_to_finish_write
#define xcc_cas_float_to_finish_write \
 __xcc_cas_float_to_finish_write
DEF_xcc_comp_CB2(float,float,write,xcc_ssbar);
#endif
#ifndef xcc_cas_double_to_finish_write
#define xcc_cas_double_to_finish_write \
 __xcc_cas_double_to_finish_write
DEF_xcc_comp_CB2(double,double,write,xcc_ssbar);
#endif

/* end of Instructions */

/* *//* *//* *//* *//* */

/*
 * xccmem API primitives
 */

#define atomic_read_char(loc) xcc_atomic_read_char(&(loc))
#define atomic_read_short(loc) xcc_atomic_read_short(&(loc))
#define atomic_read_int(loc) xcc_atomic_read_int(&(loc))
#define atomic_read_long(loc) xcc_atomic_read_long(&(loc))
#define atomic_read_long_long(loc) xcc_atomic_read_long_long(&(loc))
#define atomic_read_float(loc) xcc_atomic_read_float(&(loc))
#define atomic_read_double(loc) xcc_atomic_read_double(&(loc))
#define atomic_read_ptr(loc) xcc_atomic_read_ptr(&(loc))

#define atomic_read_volatile_char atomic_read_char
#define atomic_read_volatile_short atomic_read_short
#define atomic_read_volatile_int atomic_read_int
#define atomic_read_volatile_long atomic_read_long
#define atomic_read_volatile_long_long atomic_read_long_long
#define atomic_read_volatile_float atomic_read_float
#define atomic_read_volatile_double atomic_read_double
#define atomic_read_volatile_ptr atomic_read_ptr

#define xread_char(loc) xcc_xread_char(&(loc))
#define xread_short(loc) xcc_xread_short(&(loc))
#define xread_int(loc) xcc_xread_int(&(loc))
#define xread_long(loc) xcc_xread_long(&(loc))
#define xread_long_long(loc) xcc_xread_long_long(&(loc))
#define xread_float(loc) xcc_xread_float(&(loc))
#define xread_double(loc) xcc_xread_double(&(loc))
#define xread_ptr(loc) xcc_xread_ptr(&(loc))

#define read_volatile_char xread_char
#define read_volatile_short xread_short
#define read_volatile_int xread_int
#define read_volatile_long xread_long
#define read_volatile_long_long xread_long_long
#define read_volatile_float xread_float
#define read_volatile_double xread_double
#define read_volatile_ptr xread_ptr

#define atomic_write_char(loc,v) xcc_atomic_write_char(&(loc),(v))
#define atomic_write_short(loc,v) xcc_atomic_write_short(&(loc),(v))
#define atomic_write_int(loc,v) xcc_atomic_write_int(&(loc),(v))
#define atomic_write_long(loc,v) xcc_atomic_write_long(&(loc),(v))
#define atomic_write_long_long(loc,v) xcc_atomic_write_long_long(&(loc),(v))
#define atomic_write_float(loc,v) xcc_atomic_write_float(&(loc),(v))
#define atomic_write_double(loc,v) xcc_atomic_write_double(&(loc),(v))
#define atomic_write_ptr(loc,v) xcc_atomic_write_ptr(&(loc),(v))

#define atomic_write_volatile_char atomic_write_char
#define atomic_write_volatile_short atomic_write_short
#define atomic_write_volatile_int atomic_write_int
#define atomic_write_volatile_long atomic_write_long
#define atomic_write_volatile_long_long atomic_write_long_long
#define atomic_write_volatile_float atomic_write_float
#define atomic_write_volatile_double atomic_write_double
#define atomic_write_volatile_ptr atomic_write_ptr

#define xwrite_char(loc,v) xcc_xwrite_char(&(loc),(v))
#define xwrite_short(loc,v) xcc_xwrite_short(&(loc),(v))
#define xwrite_int(loc,v) xcc_xwrite_int(&(loc),(v))
#define xwrite_long(loc,v) xcc_xwrite_long(&(loc),(v))
#define xwrite_long_long(loc,v) xcc_xwrite_long_long(&(loc),(v))
#define xwrite_float(loc,v) xcc_xwrite_float(&(loc),(v))
#define xwrite_double(loc,v) xcc_xwrite_double(&(loc),(v))
#define xwrite_ptr(loc,v) xcc_xwrite_ptr(&(loc),(v))

#define write_volatile_char xwrite_char
#define write_volatile_short xwrite_short
#define write_volatile_int xwrite_int
#define write_volatile_long xwrite_long
#define write_volatile_long_long xwrite_long_long
#define write_volatile_float xwrite_float
#define write_volatile_double xwrite_double
#define write_volatile_ptr xwrite_ptr

#define atomic_swap_char(loc,v) xcc_atomic_swap_char(&(loc),(v))
#define atomic_swap_short(loc,v) xcc_atomic_swap_short(&(loc),(v))
#define atomic_swap_int(loc,v) xcc_atomic_swap_int(&(loc),(v))
#define atomic_swap_long(loc,v) xcc_atomic_swap_long(&(loc),(v))
#define atomic_swap_long_long(loc,v) xcc_atomic_swap_long_long(&(loc),(v))
#define atomic_swap_float(loc,v) xcc_atomic_swap_float(&(loc),(v))
#define atomic_swap_double(loc,v) xcc_atomic_swap_double(&(loc),(v))
#define atomic_swap_ptr(loc,v) xcc_atomic_swap_ptr(&(loc),(v))

#define cas_char(loc,ov,nv) xcc_cas_char(&(loc),(ov),(nv))
#define cas_short(loc,ov,nv) xcc_cas_short(&(loc),(ov),(nv))
#define cas_int(loc,ov,nv) xcc_cas_int(&(loc),(ov),(nv))
#define cas_long(loc,ov,nv) xcc_cas_long(&(loc),(ov),(nv))
#define cas_long_long(loc,ov,nv) xcc_cas_long_long(&(loc),(ov),(nv))
#define cas_float(loc,ov,nv) xcc_cas_float(&(loc),(ov),(nv))
#define cas_double(loc,ov,nv) xcc_cas_double(&(loc),(ov),(nv))
#define cas_ptr(loc,ov,nv) xcc_cas_ptr(&(loc),(ov),(nv))

#define ssbar xcc_ssbar
#define slbar xcc_slbar
#define sabar xcc_sabar
#define lsbar xcc_lsbar
#define llbar xcc_llbar
#define labar xcc_labar
#define asbar xcc_asbar
#define albar xcc_albar
#define aabar xcc_aabar

#define start_access_after_lock xcc_start_access_after_lock
#define start_read_after_lock xcc_start_read_after_lock
#define start_write_after_lock xcc_start_write_after_lock

#define start_access_after_read xcc_start_access_after_read
#define start_access_after_write xcc_start_access_after_write
#define start_read_after_read xcc_start_read_after_read
#define start_read_after_write xcc_start_read_after_write
#define start_write_after_read xcc_start_write_after_read
#define start_write_after_write xcc_start_write_after_write

#define finish_access_before_unlock xcc_finish_access_before_unlock
#define finish_read_before_unlock xcc_finish_read_before_unlock
#define finish_write_before_unlock xcc_finish_write_before_unlock

#define finish_access_before_read xcc_finish_access_before_read
#define finish_access_before_write xcc_finish_access_before_write
#define finish_read_before_read xcc_finish_read_before_read
#define finish_read_before_write xcc_finish_read_before_write
#define finish_write_before_read xcc_finish_write_before_read
#define finish_write_before_write xcc_finish_write_before_write

#define LOCK_INITIALIZER xcc_LOCK_INITIALIZER
#define lock_t xcc_lock_t
#define try_lock(loc) xcc_try_lock(&(loc))
#define spin_lock(loc) xcc_spin_lock(&(loc))
#define release_lock(loc) xcc_release_lock(&(loc))

#define RWLOCK_INITIALIZER xcc_RWLOCK_INITIALIZER
#define rwlock_t xcc_rwlock_t
#define try_rlock(loc) xcc_try_rlock(&(loc))
#define spin_rlock(loc) xcc_spin_rlock(&(loc))
#define release_rlock(loc) xcc_release_rlock(&(loc))
#define try_wlock(loc) xcc_try_wlock(&(loc))
#define spin_wlock(loc) xcc_spin_wlock(&(loc))
#define release_wlock(loc) xcc_release_wlock(&(loc))

/* Composed */

#define atomic_read_char_to_start_access(loc) \
 xcc_atomic_read_char_to_start_access(&(loc))
#define atomic_read_short_to_start_access(loc) \
 xcc_atomic_read_short_to_start_access(&(loc))
#define atomic_read_int_to_start_access(loc) \
 xcc_atomic_read_int_to_start_access(&(loc))
#define atomic_read_long_to_start_access(loc) \
 xcc_atomic_read_long_to_start_access(&(loc))
#define atomic_read_long_long_to_start_access(loc) \
 xcc_atomic_read_long_long_to_start_access(&(loc))
#define atomic_read_ptr_to_start_access(loc) \
 xcc_atomic_read_ptr_to_start_access(&(loc))
#define atomic_read_float_to_start_access(loc) \
 xcc_atomic_read_float_to_start_access(&(loc))
#define atomic_read_double_to_start_access(loc) \
 xcc_atomic_read_double_to_start_access(&(loc))

#define atomic_read_char_to_start_read(loc) \
 xcc_atomic_read_char_to_start_read(&(loc))
#define atomic_read_short_to_start_read(loc) \
 xcc_atomic_read_short_to_start_read(&(loc))
#define atomic_read_int_to_start_read(loc) \
 xcc_atomic_read_int_to_start_read(&(loc))
#define atomic_read_long_to_start_read(loc) \
 xcc_atomic_read_long_to_start_read(&(loc))
#define atomic_read_long_long_to_start_read(loc) \
 xcc_atomic_read_long_long_to_start_read(&(loc))
#define atomic_read_ptr_to_start_read(loc) \
 xcc_atomic_read_ptr_to_start_read(&(loc))
#define atomic_read_float_to_start_read(loc) \
 xcc_atomic_read_float_to_start_read(&(loc))
#define atomic_read_double_to_start_read(loc) \
 xcc_atomic_read_double_to_start_read(&(loc))

#define atomic_read_char_to_start_write(loc) \
 xcc_atomic_read_char_to_start_write(&(loc))
#define atomic_read_short_to_start_write(loc) \
 xcc_atomic_read_short_to_start_write(&(loc))
#define atomic_read_int_to_start_write(loc) \
 xcc_atomic_read_int_to_start_write(&(loc))
#define atomic_read_long_to_start_write(loc) \
 xcc_atomic_read_long_to_start_write(&(loc))
#define atomic_read_long_long_to_start_write(loc) \
 xcc_atomic_read_long_long_to_start_write(&(loc))
#define atomic_read_ptr_to_start_write(loc) \
 xcc_atomic_read_ptr_to_start_write(&(loc))
#define atomic_read_float_to_start_write(loc) \
 xcc_atomic_read_float_to_start_write(&(loc))
#define atomic_read_double_to_start_write(loc) \
 xcc_atomic_read_double_to_start_write(&(loc))

#define atomic_read_char_to_finish_access(loc) \
 xcc_atomic_read_char_to_finish_access(&(loc))
#define atomic_read_short_to_finish_access(loc) \
 xcc_atomic_read_short_to_finish_access(&(loc))
#define atomic_read_int_to_finish_access(loc) \
 xcc_atomic_read_int_to_finish_access(&(loc))
#define atomic_read_long_to_finish_access(loc) \
 xcc_atomic_read_long_to_finish_access(&(loc))
#define atomic_read_long_long_to_finish_access(loc) \
 xcc_atomic_read_long_long_to_finish_access(&(loc))
#define atomic_read_ptr_to_finish_access(loc) \
 xcc_atomic_read_ptr_to_finish_access(&(loc))
#define atomic_read_float_to_finish_access(loc) \
 xcc_atomic_read_float_to_finish_access(&(loc))
#define atomic_read_double_to_finish_access(loc) \
 xcc_atomic_read_double_to_finish_access(&(loc))

#define atomic_read_char_to_finish_read(loc) \
 xcc_atomic_read_char_to_finish_read(&(loc))
#define atomic_read_short_to_finish_read(loc) \
 xcc_atomic_read_short_to_finish_read(&(loc))
#define atomic_read_int_to_finish_read(loc) \
 xcc_atomic_read_int_to_finish_read(&(loc))
#define atomic_read_long_to_finish_read(loc) \
 xcc_atomic_read_long_to_finish_read(&(loc))
#define atomic_read_long_long_to_finish_read(loc) \
 xcc_atomic_read_long_long_to_finish_read(&(loc))
#define atomic_read_ptr_to_finish_read(loc) \
 xcc_atomic_read_ptr_to_finish_read(&(loc))
#define atomic_read_float_to_finish_read(loc) \
 xcc_atomic_read_float_to_finish_read(&(loc))
#define atomic_read_double_to_finish_read(loc) \
 xcc_atomic_read_double_to_finish_read(&(loc))

#define atomic_read_char_to_finish_write(loc) \
 xcc_atomic_read_char_to_finish_write(&(loc))
#define atomic_read_short_to_finish_write(loc) \
 xcc_atomic_read_short_to_finish_write(&(loc))
#define atomic_read_int_to_finish_write(loc) \
 xcc_atomic_read_int_to_finish_write(&(loc))
#define atomic_read_long_to_finish_write(loc) \
 xcc_atomic_read_long_to_finish_write(&(loc))
#define atomic_read_long_long_to_finish_write(loc) \
 xcc_atomic_read_long_long_to_finish_write(&(loc))
#define atomic_read_ptr_to_finish_write(loc) \
 xcc_atomic_read_ptr_to_finish_write(&(loc))
#define atomic_read_float_to_finish_write(loc) \
 xcc_atomic_read_float_to_finish_write(&(loc))
#define atomic_read_double_to_finish_write(loc) \
 xcc_atomic_read_double_to_finish_write(&(loc))


#define atomic_write_char_to_start_access(loc,v) \
 xcc_atomic_write_char_to_start_access(&(loc),(v))
#define atomic_write_short_to_start_access(loc,v) \
 xcc_atomic_write_short_to_start_access(&(loc),(v))
#define atomic_write_int_to_start_access(loc,v) \
 xcc_atomic_write_int_to_start_access(&(loc),(v))
#define atomic_write_long_to_start_access(loc,v) \
 xcc_atomic_write_long_to_start_access(&(loc),(v))
#define atomic_write_long_long_to_start_access(loc,v) \
 xcc_atomic_write_long_long_to_start_access(&(loc),(v))
#define atomic_write_ptr_to_start_access(loc,v) \
 xcc_atomic_write_ptr_to_start_access(&(loc),(v))
#define atomic_write_float_to_start_access(loc,v) \
 xcc_atomic_write_float_to_start_access(&(loc),(v))
#define atomic_write_double_to_start_access(loc,v) \
 xcc_atomic_write_double_to_start_access(&(loc),(v))

#define atomic_write_char_to_start_read(loc,v) \
 xcc_atomic_write_char_to_start_read(&(loc),(v))
#define atomic_write_short_to_start_read(loc,v) \
 xcc_atomic_write_short_to_start_read(&(loc),(v))
#define atomic_write_int_to_start_read(loc,v) \
 xcc_atomic_write_int_to_start_read(&(loc),(v))
#define atomic_write_long_to_start_read(loc,v) \
 xcc_atomic_write_long_to_start_read(&(loc),(v))
#define atomic_write_long_long_to_start_read(loc,v) \
 xcc_atomic_write_long_long_to_start_read(&(loc),(v))
#define atomic_write_ptr_to_start_read(loc,v) \
 xcc_atomic_write_ptr_to_start_read(&(loc),(v))
#define atomic_write_float_to_start_read(loc,v) \
 xcc_atomic_write_float_to_start_read(&(loc),(v))
#define atomic_write_double_to_start_read(loc,v) \
 xcc_atomic_write_double_to_start_read(&(loc),(v))

#define atomic_write_char_to_start_write(loc,v) \
 xcc_atomic_write_char_to_start_write(&(loc),(v))
#define atomic_write_short_to_start_write(loc,v) \
 xcc_atomic_write_short_to_start_write(&(loc),(v))
#define atomic_write_int_to_start_write(loc,v) \
 xcc_atomic_write_int_to_start_write(&(loc),(v))
#define atomic_write_long_to_start_write(loc,v) \
 xcc_atomic_write_long_to_start_write(&(loc),(v))
#define atomic_write_long_long_to_start_write(loc,v) \
 xcc_atomic_write_long_long_to_start_write(&(loc),(v))
#define atomic_write_ptr_to_start_write(loc,v) \
 xcc_atomic_write_ptr_to_start_write(&(loc),(v))
#define atomic_write_float_to_start_write(loc,v) \
 xcc_atomic_write_float_to_start_write(&(loc),(v))
#define atomic_write_double_to_start_write(loc,v) \
 xcc_atomic_write_double_to_start_write(&(loc),(v))

#define atomic_write_char_to_finish_access(loc,v) \
 xcc_atomic_write_char_to_finish_access(&(loc),(v))
#define atomic_write_short_to_finish_access(loc,v) \
 xcc_atomic_write_short_to_finish_access(&(loc),(v))
#define atomic_write_int_to_finish_access(loc,v) \
 xcc_atomic_write_int_to_finish_access(&(loc),(v))
#define atomic_write_long_to_finish_access(loc,v) \
 xcc_atomic_write_long_to_finish_access(&(loc),(v))
#define atomic_write_long_long_to_finish_access(loc,v) \
 xcc_atomic_write_long_long_to_finish_access(&(loc),(v))
#define atomic_write_ptr_to_finish_access(loc,v) \
 xcc_atomic_write_ptr_to_finish_access(&(loc),(v))
#define atomic_write_float_to_finish_access(loc,v) \
 xcc_atomic_write_float_to_finish_access(&(loc),(v))
#define atomic_write_double_to_finish_access(loc,v) \
 xcc_atomic_write_double_to_finish_access(&(loc),(v))

#define atomic_write_char_to_finish_read(loc,v) \
 xcc_atomic_write_char_to_finish_read(&(loc),(v))
#define atomic_write_short_to_finish_read(loc,v) \
 xcc_atomic_write_short_to_finish_read(&(loc),(v))
#define atomic_write_int_to_finish_read(loc,v) \
 xcc_atomic_write_int_to_finish_read(&(loc),(v))
#define atomic_write_long_to_finish_read(loc,v) \
 xcc_atomic_write_long_to_finish_read(&(loc),(v))
#define atomic_write_long_long_to_finish_read(loc,v) \
 xcc_atomic_write_long_long_to_finish_read(&(loc),(v))
#define atomic_write_ptr_to_finish_read(loc,v) \
 xcc_atomic_write_ptr_to_finish_read(&(loc),(v))
#define atomic_write_float_to_finish_read(loc,v) \
 xcc_atomic_write_float_to_finish_read(&(loc),(v))
#define atomic_write_double_to_finish_read(loc,v) \
 xcc_atomic_write_double_to_finish_read(&(loc),(v))

#define atomic_write_char_to_finish_write(loc,v) \
 xcc_atomic_write_char_to_finish_write(&(loc),(v))
#define atomic_write_short_to_finish_write(loc,v) \
 xcc_atomic_write_short_to_finish_write(&(loc),(v))
#define atomic_write_int_to_finish_write(loc,v) \
 xcc_atomic_write_int_to_finish_write(&(loc),(v))
#define atomic_write_long_to_finish_write(loc,v) \
 xcc_atomic_write_long_to_finish_write(&(loc),(v))
#define atomic_write_long_long_to_finish_write(loc,v) \
 xcc_atomic_write_long_long_to_finish_write(&(loc),(v))
#define atomic_write_ptr_to_finish_write(loc,v) \
 xcc_atomic_write_ptr_to_finish_write(&(loc),(v))
#define atomic_write_float_to_finish_write(loc,v) \
 xcc_atomic_write_float_to_finish_write(&(loc),(v))
#define atomic_write_double_to_finish_write(loc,v) \
 xcc_atomic_write_double_to_finish_write(&(loc),(v))


#define atomic_swap_char_to_start_access(loc,ov) \
 xcc_atomic_swap_char_to_start_access(&(loc),(ov))
#define atomic_swap_short_to_start_access(loc,ov) \
 xcc_atomic_swap_short_to_start_access(&(loc),(ov))
#define atomic_swap_int_to_start_access(loc,ov) \
 xcc_atomic_swap_int_to_start_access(&(loc),(ov))
#define atomic_swap_long_to_start_access(loc,ov) \
 xcc_atomic_swap_long_to_start_access(&(loc),(ov))
#define atomic_swap_long_long_to_start_access(loc,ov) \
 xcc_atomic_swap_long_long_to_start_access(&(loc),(ov))
#define atomic_swap_ptr_to_start_access(loc,ov) \
 xcc_atomic_swap_ptr_to_start_access(&(loc),(ov))
#define atomic_swap_float_to_start_access(loc,ov) \
 xcc_atomic_swap_float_to_start_access(&(loc),(ov))
#define atomic_swap_double_to_start_access(loc,ov) \
 xcc_atomic_swap_double_to_start_access(&(loc),(ov))

#define atomic_swap_char_to_start_read(loc,ov) \
 xcc_atomic_swap_char_to_start_read(&(loc),(ov))
#define atomic_swap_short_to_start_read(loc,ov) \
 xcc_atomic_swap_short_to_start_read(&(loc),(ov))
#define atomic_swap_int_to_start_read(loc,ov) \
 xcc_atomic_swap_int_to_start_read(&(loc),(ov))
#define atomic_swap_long_to_start_read(loc,ov) \
 xcc_atomic_swap_long_to_start_read(&(loc),(ov))
#define atomic_swap_long_long_to_start_read(loc,ov) \
 xcc_atomic_swap_long_long_to_start_read(&(loc),(ov))
#define atomic_swap_ptr_to_start_read(loc,ov) \
 xcc_atomic_swap_ptr_to_start_read(&(loc),(ov))
#define atomic_swap_float_to_start_read(loc,ov) \
 xcc_atomic_swap_float_to_start_read(&(loc),(ov))
#define atomic_swap_double_to_start_read(loc,ov) \
 xcc_atomic_swap_double_to_start_read(&(loc),(ov))

#define atomic_swap_char_to_start_write(loc,ov) \
 xcc_atomic_swap_char_to_start_write(&(loc),(ov))
#define atomic_swap_short_to_start_write(loc,ov) \
 xcc_atomic_swap_short_to_start_write(&(loc),(ov))
#define atomic_swap_int_to_start_write(loc,ov) \
 xcc_atomic_swap_int_to_start_write(&(loc),(ov))
#define atomic_swap_long_to_start_write(loc,ov) \
 xcc_atomic_swap_long_to_start_write(&(loc),(ov))
#define atomic_swap_long_long_to_start_write(loc,ov) \
 xcc_atomic_swap_long_long_to_start_write(&(loc),(ov))
#define atomic_swap_ptr_to_start_write(loc,ov) \
 xcc_atomic_swap_ptr_to_start_write(&(loc),(ov))
#define atomic_swap_float_to_start_write(loc,ov) \
 xcc_atomic_swap_float_to_start_write(&(loc),(ov))
#define atomic_swap_double_to_start_write(loc,ov) \
 xcc_atomic_swap_double_to_start_write(&(loc),(ov))

#define atomic_swap_char_to_finish_access(loc,ov) \
 xcc_atomic_swap_char_to_finish_access(&(loc),(ov))
#define atomic_swap_short_to_finish_access(loc,ov) \
 xcc_atomic_swap_short_to_finish_access(&(loc),(ov))
#define atomic_swap_int_to_finish_access(loc,ov) \
 xcc_atomic_swap_int_to_finish_access(&(loc),(ov))
#define atomic_swap_long_to_finish_access(loc,ov) \
 xcc_atomic_swap_long_to_finish_access(&(loc),(ov))
#define atomic_swap_long_long_to_finish_access(loc,ov) \
 xcc_atomic_swap_long_long_to_finish_access(&(loc),(ov))
#define atomic_swap_ptr_to_finish_access(loc,ov) \
 xcc_atomic_swap_ptr_to_finish_access(&(loc),(ov))
#define atomic_swap_float_to_finish_access(loc,ov) \
 xcc_atomic_swap_float_to_finish_access(&(loc),(ov))
#define atomic_swap_double_to_finish_access(loc,ov) \
 xcc_atomic_swap_double_to_finish_access(&(loc),(ov))

#define atomic_swap_char_to_finish_read(loc,ov) \
 xcc_atomic_swap_char_to_finish_read(&(loc),(ov))
#define atomic_swap_short_to_finish_read(loc,ov) \
 xcc_atomic_swap_short_to_finish_read(&(loc),(ov))
#define atomic_swap_int_to_finish_read(loc,ov) \
 xcc_atomic_swap_int_to_finish_read(&(loc),(ov))
#define atomic_swap_long_to_finish_read(loc,ov) \
 xcc_atomic_swap_long_to_finish_read(&(loc),(ov))
#define atomic_swap_long_long_to_finish_read(loc,ov) \
 xcc_atomic_swap_long_long_to_finish_read(&(loc),(ov))
#define atomic_swap_ptr_to_finish_read(loc,ov) \
 xcc_atomic_swap_ptr_to_finish_read(&(loc),(ov))
#define atomic_swap_float_to_finish_read(loc,ov) \
 xcc_atomic_swap_float_to_finish_read(&(loc),(ov))
#define atomic_swap_double_to_finish_read(loc,ov) \
 xcc_atomic_swap_double_to_finish_read(&(loc),(ov))

#define atomic_swap_char_to_finish_write(loc,ov) \
 xcc_atomic_swap_char_to_finish_write(&(loc),(ov))
#define atomic_swap_short_to_finish_write(loc,ov) \
 xcc_atomic_swap_short_to_finish_write(&(loc),(ov))
#define atomic_swap_int_to_finish_write(loc,ov) \
 xcc_atomic_swap_int_to_finish_write(&(loc),(ov))
#define atomic_swap_long_to_finish_write(loc,ov) \
 xcc_atomic_swap_long_to_finish_write(&(loc),(ov))
#define atomic_swap_long_long_to_finish_write(loc,ov) \
 xcc_atomic_swap_long_long_to_finish_write(&(loc),(ov))
#define atomic_swap_ptr_to_finish_write(loc,ov) \
 xcc_atomic_swap_ptr_to_finish_write(&(loc),(ov))
#define atomic_swap_float_to_finish_write(loc,ov) \
 xcc_atomic_swap_float_to_finish_write(&(loc),(ov))
#define atomic_swap_double_to_finish_write(loc,ov) \
 xcc_atomic_swap_double_to_finish_write(&(loc),(ov))


#define cas_char_to_start_access(loc,ov,nv) \
 xcc_cas_char_to_start_access(&(loc),(ov),(nv))
#define cas_short_to_start_access(loc,ov,nv) \
 xcc_cas_short_to_start_access(&(loc),(ov),(nv))
#define cas_int_to_start_access(loc,ov,nv) \
 xcc_cas_int_to_start_access(&(loc),(ov),(nv))
#define cas_long_to_start_access(loc,ov,nv) \
 xcc_cas_long_to_start_access(&(loc),(ov),(nv))
#define cas_long_long_to_start_access(loc,ov,nv) \
 xcc_cas_long_long_to_start_access(&(loc),(ov),(nv))
#define cas_ptr_to_start_access(loc,ov,nv) \
 xcc_cas_ptr_to_start_access(&(loc),(ov),(nv))
#define cas_float_to_start_access(loc,ov,nv) \
 xcc_cas_float_to_start_access(&(loc),(ov),(nv))
#define cas_double_to_start_access(loc,ov,nv) \
 xcc_cas_double_to_start_access(&(loc),(ov),(nv))

#define cas_char_to_start_read(loc,ov,nv) \
 xcc_cas_char_to_start_read(&(loc),(ov),(nv))
#define cas_short_to_start_read(loc,ov,nv) \
 xcc_cas_short_to_start_read(&(loc),(ov),(nv))
#define cas_int_to_start_read(loc,ov,nv) \
 xcc_cas_int_to_start_read(&(loc),(ov),(nv))
#define cas_long_to_start_read(loc,ov,nv) \
 xcc_cas_long_to_start_read(&(loc),(ov),(nv))
#define cas_long_long_to_start_read(loc,ov,nv) \
 xcc_cas_long_long_to_start_read(&(loc),(ov),(nv))
#define cas_ptr_to_start_read(loc,ov,nv) \
 xcc_cas_ptr_to_start_read(&(loc),(ov),(nv))
#define cas_float_to_start_read(loc,ov,nv) \
 xcc_cas_float_to_start_read(&(loc),(ov),(nv))
#define cas_double_to_start_read(loc,ov,nv) \
 xcc_cas_double_to_start_read(&(loc),(ov),(nv))

#define cas_char_to_start_write(loc,ov,nv) \
 xcc_cas_char_to_start_write(&(loc),(ov),(nv))
#define cas_short_to_start_write(loc,ov,nv) \
 xcc_cas_short_to_start_write(&(loc),(ov),(nv))
#define cas_int_to_start_write(loc,ov,nv) \
 xcc_cas_int_to_start_write(&(loc),(ov),(nv))
#define cas_long_to_start_write(loc,ov,nv) \
 xcc_cas_long_to_start_write(&(loc),(ov),(nv))
#define cas_long_long_to_start_write(loc,ov,nv) \
 xcc_cas_long_long_to_start_write(&(loc),(ov),(nv))
#define cas_ptr_to_start_write(loc,ov,nv) \
 xcc_cas_ptr_to_start_write(&(loc),(ov),(nv))
#define cas_float_to_start_write(loc,ov,nv) \
 xcc_cas_float_to_start_write(&(loc),(ov),(nv))
#define cas_double_to_start_write(loc,ov,nv) \
 xcc_cas_double_to_start_write(&(loc),(ov),(nv))

#define cas_char_to_finish_access(loc,ov,nv) \
 xcc_cas_char_to_finish_access(&(loc),(ov),(nv))
#define cas_short_to_finish_access(loc,ov,nv) \
 xcc_cas_short_to_finish_access(&(loc),(ov),(nv))
#define cas_int_to_finish_access(loc,ov,nv) \
 xcc_cas_int_to_finish_access(&(loc),(ov),(nv))
#define cas_long_to_finish_access(loc,ov,nv) \
 xcc_cas_long_to_finish_access(&(loc),(ov),(nv))
#define cas_long_long_to_finish_access(loc,ov,nv) \
 xcc_cas_long_long_to_finish_access(&(loc),(ov),(nv))
#define cas_ptr_to_finish_access(loc,ov,nv) \
 xcc_cas_ptr_to_finish_access(&(loc),(ov),(nv))
#define cas_float_to_finish_access(loc,ov,nv) \
 xcc_cas_float_to_finish_access(&(loc),(ov),(nv))
#define cas_double_to_finish_access(loc,ov,nv) \
 xcc_cas_double_to_finish_access(&(loc),(ov),(nv))

#define cas_char_to_finish_read(loc,ov,nv) \
 xcc_cas_char_to_finish_read(&(loc),(ov),(nv))
#define cas_short_to_finish_read(loc,ov,nv) \
 xcc_cas_short_to_finish_read(&(loc),(ov),(nv))
#define cas_int_to_finish_read(loc,ov,nv) \
 xcc_cas_int_to_finish_read(&(loc),(ov),(nv))
#define cas_long_to_finish_read(loc,ov,nv) \
 xcc_cas_long_to_finish_read(&(loc),(ov),(nv))
#define cas_long_long_to_finish_read(loc,ov,nv) \
 xcc_cas_long_long_to_finish_read(&(loc),(ov),(nv))
#define cas_ptr_to_finish_read(loc,ov,nv) \
 xcc_cas_ptr_to_finish_read(&(loc),(ov),(nv))
#define cas_float_to_finish_read(loc,ov,nv) \
 xcc_cas_float_to_finish_read(&(loc),(ov),(nv))
#define cas_double_to_finish_read(loc,ov,nv) \
 xcc_cas_double_to_finish_read(&(loc),(ov),(nv))

#define cas_char_to_finish_write(loc,ov,nv) \
 xcc_cas_char_to_finish_write(&(loc),(ov),(nv))
#define cas_short_to_finish_write(loc,ov,nv) \
 xcc_cas_short_to_finish_write(&(loc),(ov),(nv))
#define cas_int_to_finish_write(loc,ov,nv) \
 xcc_cas_int_to_finish_write(&(loc),(ov),(nv))
#define cas_long_to_finish_write(loc,ov,nv) \
 xcc_cas_long_to_finish_write(&(loc),(ov),(nv))
#define cas_long_long_to_finish_write(loc,ov,nv) \
 xcc_cas_long_long_to_finish_write(&(loc),(ov),(nv))
#define cas_ptr_to_finish_write(loc,ov,nv) \
 xcc_cas_ptr_to_finish_write(&(loc),(ov),(nv))
#define cas_float_to_finish_write(loc,ov,nv) \
 xcc_cas_float_to_finish_write(&(loc),(ov),(nv))
#define cas_double_to_finish_write(loc,ov,nv) \
 xcc_cas_double_to_finish_write(&(loc),(ov),(nv))


#define spin_lock_to_start_access(loc) xcc_spin_lock_to_start_access(&(loc))
#define spin_lock_to_start_read(loc) xcc_spin_lock_to_start_read(&(loc))
#define spin_lock_to_start_write(loc) xcc_spin_lock_to_start_write(&(loc))
#define release_lock_to_finish_access(loc) \
 xcc_release_lock_to_finish_access(&(loc))
#define release_lock_to_finish_read(loc) \
 xcc_release_lock_to_finish_read(&(loc))
#define release_lock_to_finish_write(loc) \
 xcc_release_lock_to_finish_write(&(loc))

#define spin_rlock_to_start_read(loc) xcc_spin_rlock_to_start_read(&(loc))
#define release_rlock_to_finish_read(loc) \
 xcc_release_rlock_to_finish_read(&(loc))

#define spin_wlock_to_start_write(loc) xcc_spin_wlock_to_start_write(&(loc))
#define release_wlock_to_finish_write(loc) \
 xcc_release_wlock_to_finish_write(&(loc))

/* end of xccmem API primitives */

/* *//* *//* *//* *//* */

#endif /* not XCCMEM_H */
