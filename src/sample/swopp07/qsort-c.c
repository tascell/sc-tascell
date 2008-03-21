#ifndef CT
#define CT
#endif

typedef long size_t;
typedef long WORD;

void *malloc (size_t size);

int printf (char const *, ...);

int atoi (char const *);

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

static void
swapfunc (char *a, char *b, size_t n, int swaptype)
{
  if (swaptype <= 1)
    {
      WORD t;

      {
	0;
	if (n > 0)
	  do
	    {
	      t = *((WORD *) a), *((WORD *) a) = *((WORD *) b), *((WORD *) b) =
		  t;
	    }
	  while ((a += sizeof (WORD), b += sizeof (WORD), n -=
		  sizeof (WORD)), n > 0);
      }
    }
  else
    {
      char t2;

      if (n > 0)
	do
	  {
	    t2 = *a, *a = *b, *b = t2;
	  }
	while ((a += 1, b += 1, n -= 1), n > 0);
    }
}

static char *
med3 (char *a, char *b, char *c, CT int (*cmp) (void const *, void const *))
{
  return cmp (a,
	      b) < 0 ? (cmp (b,
			     c) < 0 ? b : (cmp (a, c) < 0 ? c : a)) : (cmp (b,
									    c) >
								       0 ? b
								       : (cmp
									  (a,
									   c) >
									  0 ? c
									  : a));
}

void
print_array (int *a, int n)
{
  int i;

  {
    i = 0;
    if (i < n)
      do
	{
	  printf ("%d%c", a[i], i + 1 < n ? ' ' : '\n');
	}
      while (i++, i < n);
  }
}

void
quicksort (char *a, size_t n, size_t es,
	   CT int (*cmp) (void const *, void const *))
{
  char *pa;
  char *pb;
  char *pc;
  char *pd;
  char *pl;
  char *pm;
  char *pn;
  char *pv;
  int r;
  int swaptype;
  WORD t;
  WORD v;
  size_t s;

  swaptype =
      (a - (char *) 0 | es) % sizeof (WORD) ? 2 : (es > sizeof (WORD) ? 1 : 0);
  if (n < 7)
    {
      pl = a;
      {
	pm = a + es;
	if (pm < a + n * es)
	  do
	    {
	      {
		pl = pm;
		if (pl > a && cmp (pl - es, pl) > 0)
		  do
		    {
		      swaptype != 0 ? swapfunc (pl, pl - es, es,
						swaptype) : (void) (t =
								    *((WORD *)
								      pl),
								    *((WORD *)
								      pl) =
								    *((WORD
								       *) (pl -
									   es)),
								    *((WORD
								       *) (pl -
									   es))
								    = t);
		    }
		  while (pl -= es, pl > a && cmp (pl - es, pl) > 0);
	      }
	    }
	  while (pm += es, pm < a + n * es);
      }
      return;
    }
  pm = a + n / 2 * es;
  if (n > 7)
    {
      pl = a;
      pn = a + (n - 1) * es;
      if (n > 40)
	{
	  s = n / 8 * es;
	  pl = med3 (pl, pl + s, pl + 2 * s, cmp);
	  pm = med3 (pm - s, pm, pm + s, cmp);
	  pn = med3 (pn - 2 * s, pn - s, pn, cmp);
	}
      pm = med3 (pl, pm, pn, cmp);
    }
  if (swaptype != 0)
    {
      pv = a;
      swaptype != 0 ? swapfunc (pv, pm, es, swaptype) : (void) (t =
								*((WORD *) pv),
								*((WORD *) pv) =
								*((WORD *) pm),
								*((WORD *) pm) =
								t);
    }
  else
    {
      pv = (char *) &v;
      v = *((WORD *) pm);
    }
  pa = pb = a;
  pc = pd = a + (n - 1) * es;
  do
    {
      if (pb <= pc && (r = cmp (pb, pv)) <= 0)
	do
	  {
	    if (r == 0)
	      {
		swaptype != 0 ? swapfunc (pa, pb, es, swaptype) : (void) (t =
									  *((WORD *) pa), *((WORD *) pa) = *((WORD *) pb), *((WORD *) pb) = t);
		pa += es;
	      }
	    pb += es;
	  }
	while (pb <= pc && (r = cmp (pb, pv)) <= 0);
      if (pc >= pb && (r = cmp (pc, pv)) >= 0)
	do
	  {
	    if (r == 0)
	      {
		swaptype != 0 ? swapfunc (pc, pd, es, swaptype) : (void) (t =
									  *((WORD *) pc), *((WORD *) pc) = *((WORD *) pd), *((WORD *) pd) = t);
		pd -= es;
	      }
	    pc -= es;
	  }
	while (pc >= pb && (r = cmp (pc, pv)) >= 0);
      if (pb > pc)
	break;
      swaptype != 0 ? swapfunc (pb, pc, es, swaptype) : (void) (t =
								*((WORD *) pb),
								*((WORD *) pb) =
								*((WORD *) pc),
								*((WORD *) pc) =
								t);
      pb += es;
      pc -= es;
    }
  while (1);
  pn = a + n * es;
  s = pa - a < pb - pa ? pa - a : pb - pa;
  if (s > 0)
    swapfunc (a, pb - s, s, swaptype);
  s = pd - pc < pn - pd - es ? pd - pc : pn - pd - es;
  if (s > 0)
    swapfunc (pb, pn - s, s, swaptype);
  if ((s = pb - pa) > es)
    quicksort (a, s / es, es, cmp);
  if ((s = pd - pc) > es)
    quicksort (pn - s, s / es, es, cmp);
}

#ifdef NCOMP
static int ncomp=0;
#endif

static int em;
CT int comp_mod (int *pp, int *pq)
{
#ifdef NCOMP
  ncomp++;
#endif
  return *pp % em < *pq % em ? 1 : (*pp % em == *pq % em ? 0 : -1);
}

void
modsort (int *a, int n, int m)
{
  em = m;
  quicksort (a, n, sizeof (int), comp_mod);
}

int *aa;
int nn;
int mm;

void
foo (int n)
{
  if (n)
    foo (n - 1);
  else
    modsort (aa, nn, mm);
}

int
main (int argc, char **argv)
{
  int n = 30;
  int m = 10;
  int i;
  int nfoo = 0;
  int *a;
  struct timeval tp1;
  struct timeval tp2;
  double ttime;

  if (argc > 1)
    n = atoi (argv[1]);
  if (argc > 2)
    m = atoi (argv[2]);
  if (argc > 3)
    nfoo = atoi (argv[3]);
  a = malloc (sizeof (int) * n);
  {
    i = 0;
    if (i < n)
      do
	{
	  a[i] = i;
	}
      while (i++, i < n);
  }
  aa = a;
  nn = n;
  mm = m;
  if (argc > 4)
    print_array (a, n);
  gettimeofday (&tp1, 0);
  if (nfoo)
    foo (nfoo);
  else
    modsort (a, n, m);
  gettimeofday (&tp2, 0);
  if (argc > 4)
    print_array (a, n);
  ttime = tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
  printf ("time: %f\n", ttime);
#ifdef NCOMP
  printf ("ncomp=%d",ncomp);
#endif
  return 0;
}
