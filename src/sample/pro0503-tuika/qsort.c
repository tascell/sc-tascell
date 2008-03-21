typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);
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

struct swapfunc_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  char t2;
  WORD t;
  int swaptype;
  size_t n;
  char *b;
  char *a;
};

static void
swapfunc (char *esp, char *a, char *b, size_t n, int swaptype)
{
  struct swapfunc_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  WORD t;
  char t2;

lgoto:
  ;
  efp = (struct swapfunc_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct swapfunc_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (swaptype <= 1)
    {
      0;
      if (n > 0)
	do
	  {
	    t = *((WORD *) a);
	    *((WORD *) a) = *((WORD *) b);
	    *((WORD *) b) = t;
	    a += sizeof (WORD);
	    b += sizeof (WORD);
	    n -= sizeof (WORD);
	  }
	while (n > 0);
    }
  else if (n > 0)
    do
      {
	t2 = *a;
	*a = *b;
	*b = t2;
	a += 1;
	b += 1;
	n -= 1;
      }
    while (n > 0);
}

struct med3_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  char *ifexp_result5;
  char *ifexp_result4;
  int tmp5;
  int tmp4;
  char *ifexp_result2;
  int tmp3;
  int tmp2;
  int tmp;
  closure_t *cmp;
  char *c;
  char *b;
  char *a;
};

static char *
med3 (char *esp, char *a, char *b, char *c, closure_t * cmp)
{
  char *argp;
  struct med3_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int tmp;
  int tmp2;
  int tmp3;
  char *ifexp_result2;
  int tmp4;
  int tmp5;
  char *ifexp_result4;
  char *ifexp_result5;
  closure_t *tmp_fp;
  closure_t *tmp_fp2;
  closure_t *tmp_fp3;
  closure_t *tmp_fp4;
  closure_t *tmp_fp5;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct med3_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct med3_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL;
	case 1:
	  goto L_CALL2;
	case 2:
	  goto L_CALL3;
	case 3:
	  goto L_CALL4;
	case 4:
	  goto L_CALL5;
	}
      goto L_CALL;
    }
  efp = (struct med3_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct med3_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    tmp_fp = cmp;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((char **) argp) = b, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((char **) argp) = a, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp;
    efp->ifexp_result5 = ifexp_result5;
    efp->ifexp_result4 = ifexp_result4;
    efp->tmp5 = tmp5;
    efp->tmp4 = tmp4;
    efp->ifexp_result2 = ifexp_result2;
    efp->tmp3 = tmp3;
    efp->tmp2 = tmp2;
    efp->tmp = tmp;
    efp->cmp = cmp;
    efp->c = c;
    efp->b = b;
    efp->a = a;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return (char *) 0 - 1;
  L_CALL:
    ;
    ifexp_result5 = efp->ifexp_result5;
    ifexp_result4 = efp->ifexp_result4;
    tmp5 = efp->tmp5;
    tmp4 = efp->tmp4;
    ifexp_result2 = efp->ifexp_result2;
    tmp3 = efp->tmp3;
    tmp2 = efp->tmp2;
    tmp = efp->tmp;
    cmp = efp->cmp;
    c = efp->c;
    b = efp->b;
    a = efp->a;
    tmp = *((int *) efp->argp);
  }
  if (tmp < 0)
    {
      {
	tmp_fp2 = cmp;
	argp =
	    (char *) ((Align_t *) esp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = c, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = b, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((closure_t **) argp) = tmp_fp2;
	efp->ifexp_result5 = ifexp_result5;
	efp->ifexp_result4 = ifexp_result4;
	efp->tmp5 = tmp5;
	efp->tmp4 = tmp4;
	efp->ifexp_result2 = ifexp_result2;
	efp->tmp3 = tmp3;
	efp->tmp2 = tmp2;
	efp->tmp = tmp;
	efp->cmp = cmp;
	efp->c = c;
	efp->b = b;
	efp->a = a;
	efp->argp = argp;
	efp->tmp_esp = argp;
	efp->call_id = 1;
	return (char *) 0 - 1;
      L_CALL2:
	;
	ifexp_result5 = efp->ifexp_result5;
	ifexp_result4 = efp->ifexp_result4;
	tmp5 = efp->tmp5;
	tmp4 = efp->tmp4;
	ifexp_result2 = efp->ifexp_result2;
	tmp3 = efp->tmp3;
	tmp2 = efp->tmp2;
	tmp = efp->tmp;
	cmp = efp->cmp;
	c = efp->c;
	b = efp->b;
	a = efp->a;
	tmp2 = *((int *) efp->argp);
      }
      if (tmp2 < 0)
	{
	  ifexp_result2 = b;
	}
      else
	{
	  {
	    tmp_fp3 = cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = c, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = a, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp3;
	    efp->ifexp_result5 = ifexp_result5;
	    efp->ifexp_result4 = ifexp_result4;
	    efp->tmp5 = tmp5;
	    efp->tmp4 = tmp4;
	    efp->ifexp_result2 = ifexp_result2;
	    efp->tmp3 = tmp3;
	    efp->tmp2 = tmp2;
	    efp->tmp = tmp;
	    efp->cmp = cmp;
	    efp->c = c;
	    efp->b = b;
	    efp->a = a;
	    efp->argp = argp;
	    efp->tmp_esp = argp;
	    efp->call_id = 2;
	    return (char *) 0 - 1;
	  L_CALL3:
	    ;
	    ifexp_result5 = efp->ifexp_result5;
	    ifexp_result4 = efp->ifexp_result4;
	    tmp5 = efp->tmp5;
	    tmp4 = efp->tmp4;
	    ifexp_result2 = efp->ifexp_result2;
	    tmp3 = efp->tmp3;
	    tmp2 = efp->tmp2;
	    tmp = efp->tmp;
	    cmp = efp->cmp;
	    c = efp->c;
	    b = efp->b;
	    a = efp->a;
	    tmp3 = *((int *) efp->argp);
	  }
	  ifexp_result2 = tmp3 < 0 ? c : a;
	}
      ifexp_result5 = ifexp_result2;
    }
  else
    {
      {
	tmp_fp4 = cmp;
	argp =
	    (char *) ((Align_t *) esp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = c, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = b, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((closure_t **) argp) = tmp_fp4;
	efp->ifexp_result5 = ifexp_result5;
	efp->ifexp_result4 = ifexp_result4;
	efp->tmp5 = tmp5;
	efp->tmp4 = tmp4;
	efp->ifexp_result2 = ifexp_result2;
	efp->tmp3 = tmp3;
	efp->tmp2 = tmp2;
	efp->tmp = tmp;
	efp->cmp = cmp;
	efp->c = c;
	efp->b = b;
	efp->a = a;
	efp->argp = argp;
	efp->tmp_esp = argp;
	efp->call_id = 3;
	return (char *) 0 - 1;
      L_CALL4:
	;
	ifexp_result5 = efp->ifexp_result5;
	ifexp_result4 = efp->ifexp_result4;
	tmp5 = efp->tmp5;
	tmp4 = efp->tmp4;
	ifexp_result2 = efp->ifexp_result2;
	tmp3 = efp->tmp3;
	tmp2 = efp->tmp2;
	tmp = efp->tmp;
	cmp = efp->cmp;
	c = efp->c;
	b = efp->b;
	a = efp->a;
	tmp4 = *((int *) efp->argp);
      }
      if (tmp4 > 0)
	{
	  ifexp_result4 = b;
	}
      else
	{
	  {
	    tmp_fp5 = cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = c, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = a, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp5;
	    efp->ifexp_result5 = ifexp_result5;
	    efp->ifexp_result4 = ifexp_result4;
	    efp->tmp5 = tmp5;
	    efp->tmp4 = tmp4;
	    efp->ifexp_result2 = ifexp_result2;
	    efp->tmp3 = tmp3;
	    efp->tmp2 = tmp2;
	    efp->tmp = tmp;
	    efp->cmp = cmp;
	    efp->c = c;
	    efp->b = b;
	    efp->a = a;
	    efp->argp = argp;
	    efp->tmp_esp = argp;
	    efp->call_id = 4;
	    return (char *) 0 - 1;
	  L_CALL5:
	    ;
	    ifexp_result5 = efp->ifexp_result5;
	    ifexp_result4 = efp->ifexp_result4;
	    tmp5 = efp->tmp5;
	    tmp4 = efp->tmp4;
	    ifexp_result2 = efp->ifexp_result2;
	    tmp3 = efp->tmp3;
	    tmp2 = efp->tmp2;
	    tmp = efp->tmp;
	    cmp = efp->cmp;
	    c = efp->c;
	    b = efp->b;
	    a = efp->a;
	    tmp5 = *((int *) efp->argp);
	  }
	  ifexp_result4 = tmp5 > 0 ? c : a;
	}
      ifexp_result5 = ifexp_result4;
    }
  return ifexp_result5;
}

struct print_array_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  int n;
  int *a;
};

void
print_array (char *esp, int *a, int n)
{
  struct print_array_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int i;

lgoto:
  ;
  efp = (struct print_array_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct print_array_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    i = 0;
    if (i < n)
      do
	{
	  printf ("%d%c", a[i], i + 1 < n ? ' ' : '\n');
	  i++;
	}
      while (i < n);
  }
}

struct quicksort_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int ifexp_result20;
  int tmp14;
  int ifexp_result16;
  int tmp11;
  int ifexp_result18;
  int tmp13;
  int ifexp_result14;
  int tmp10;
  int ifexp_result12;
  int tmp7;
  int ifexp_result10;
  int tmp6;
  size_t s;
  WORD v;
  WORD t;
  int swaptype;
  int r;
  char *pv;
  char *pn;
  char *pm;
  char *pl;
  char *pd;
  char *pc;
  char *pb;
  char *pa;
  closure_t *cmp;
  size_t es;
  size_t n;
  char *a;
  long tmp_arg12;
  char *tmp_arg11;
  long tmp_arg10;
  char *tmp_arg9;
  char *tmp_arg8;
  char *tmp_arg7;
  char *tmp_arg6;
  char *tmp_arg5;
  char *tmp_arg4;
  char *tmp_arg3;
  char *tmp_arg2;
  char *tmp_arg;
};

void
quicksort (char *esp, char *a, size_t n, size_t es, closure_t * cmp)
{
  char *argp;
  struct quicksort_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
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
  size_t s;
  int tmp6;
  int ifexp_result10;
  closure_t *tmp_fp6;
  int tmp7;
  int ifexp_result12;
  char *tmp_arg;
  closure_t *tmp_fp7;
  char *tmp_arg2;
  char *tmp_arg3;
  char *tmp_arg4;
  char *tmp_arg5;
  char *tmp_arg6;
  char *tmp_arg7;
  int tmp10;
  int ifexp_result14;
  int tmp13;
  int ifexp_result18;
  closure_t *tmp_fp8;
  int tmp11;
  int ifexp_result16;
  closure_t *tmp_fp9;
  closure_t *tmp_fp10;
  int tmp14;
  int ifexp_result20;
  closure_t *tmp_fp11;
  char *tmp_arg8;
  char *tmp_arg9;
  long tmp_arg10;
  char *tmp_arg11;
  long tmp_arg12;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct quicksort_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct quicksort_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL6;
	case 1:
	  goto L_CALL7;
	case 2:
	  goto L_CALL8;
	case 3:
	  goto L_CALL9;
	case 4:
	  goto L_CALL10;
	case 5:
	  goto L_CALL11;
	case 6:
	  goto L_CALL12;
	case 7:
	  goto L_CALL13;
	case 8:
	  goto L_CALL14;
	case 9:
	  goto L_CALL15;
	case 10:
	  goto L_CALL16;
	case 11:
	  goto L_CALL17;
	case 12:
	  goto L_CALL18;
	case 13:
	  goto L_CALL19;
	case 14:
	  goto L_CALL20;
	case 15:
	  goto L_CALL21;
	case 16:
	  goto L_CALL22;
	case 17:
	  goto L_CALL23;
	case 18:
	  goto L_CALL24;
	}
      goto L_CALL6;
    }
  efp = (struct quicksort_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct quicksort_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
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
		if (pl > a)
		  {
		    {
		      tmp_fp6 = cmp;
		      argp =
			  (char *) ((Align_t *) esp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((char **) argp) = pl, argp =
			  (char *) ((Align_t *) argp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((char **) argp) = pl - es, argp =
			  (char *) ((Align_t *) argp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((closure_t **) argp) = tmp_fp6;
		      efp->ifexp_result10 = ifexp_result10;
		      efp->tmp6 = tmp6;
		      efp->s = s;
		      efp->t = t;
		      efp->swaptype = swaptype;
		      efp->r = r;
		      efp->pv = pv;
		      efp->pn = pn;
		      efp->pm = pm;
		      efp->pl = pl;
		      efp->pd = pd;
		      efp->pc = pc;
		      efp->pb = pb;
		      efp->pa = pa;
		      efp->cmp = cmp;
		      efp->es = es;
		      efp->n = n;
		      efp->a = a;
		      efp->argp = argp;
		      efp->tmp_esp = argp;
		      efp->call_id = 0;
		      return;
		    L_CALL6:
		      ;
		      ifexp_result10 = efp->ifexp_result10;
		      tmp6 = efp->tmp6;
		      s = efp->s;
		      t = efp->t;
		      swaptype = efp->swaptype;
		      r = efp->r;
		      pv = efp->pv;
		      pn = efp->pn;
		      pm = efp->pm;
		      pl = efp->pl;
		      pd = efp->pd;
		      pc = efp->pc;
		      pb = efp->pb;
		      pa = efp->pa;
		      cmp = efp->cmp;
		      es = efp->es;
		      n = efp->n;
		      a = efp->a;
		      tmp6 = *((int *) efp->argp);
		    }
		    ifexp_result10 = tmp6 > 0 ? 1 : 0;
		  }
		else
		  {
		    ifexp_result10 = 0;
		  }
		if (ifexp_result10)
		  do
		    {
		      if (swaptype != 0)
			{
			  new_esp = esp;
			  tmp_arg = pl - es;
			  while (swapfunc (new_esp, pl, tmp_arg, es, swaptype),
				 (efp->tmp_esp = *((char **) esp)) != 0)
			    {
			      efp->ifexp_result12 = ifexp_result12;
			      efp->tmp7 = tmp7;
			      efp->ifexp_result10 = ifexp_result10;
			      efp->tmp6 = tmp6;
			      efp->s = s;
			      efp->t = t;
			      efp->swaptype = swaptype;
			      efp->r = r;
			      efp->pv = pv;
			      efp->pn = pn;
			      efp->pm = pm;
			      efp->pl = pl;
			      efp->pd = pd;
			      efp->pc = pc;
			      efp->pb = pb;
			      efp->pa = pa;
			      efp->cmp = cmp;
			      efp->es = es;
			      efp->n = n;
			      efp->a = a;
			      efp->call_id = 1;
			      return;
			    L_CALL7:
			      ;
			      ifexp_result12 = efp->ifexp_result12;
			      tmp7 = efp->tmp7;
			      ifexp_result10 = efp->ifexp_result10;
			      tmp6 = efp->tmp6;
			      s = efp->s;
			      t = efp->t;
			      swaptype = efp->swaptype;
			      r = efp->r;
			      pv = efp->pv;
			      pn = efp->pn;
			      pm = efp->pm;
			      pl = efp->pl;
			      pd = efp->pd;
			      pc = efp->pc;
			      pb = efp->pb;
			      pa = efp->pa;
			      cmp = efp->cmp;
			      es = efp->es;
			      n = efp->n;
			      a = efp->a;
			      tmp_arg = efp->tmp_arg;
			      new_esp = esp + 1;
			    }
			  0;
			}
		      else
			{
			  t = *((WORD *) pl);
			  *((WORD *) pl) = *((WORD *) (pl - es));
			  (void) (*((WORD *) (pl - es)) = t);
			}
		      0;
		      pl -= es;
		      if (pl > a)
			{
			  {
			    tmp_fp7 = cmp;
			    argp =
				(char *) ((Align_t *) esp +
					  (sizeof (char *) + sizeof (Align_t) +
					   -1) / sizeof (Align_t));
			    *((char **) argp) = pl, argp =
				(char *) ((Align_t *) argp +
					  (sizeof (char *) + sizeof (Align_t) +
					   -1) / sizeof (Align_t));
			    *((char **) argp) = pl - es, argp =
				(char *) ((Align_t *) argp +
					  (sizeof (char *) + sizeof (Align_t) +
					   -1) / sizeof (Align_t));
			    *((closure_t **) argp) = tmp_fp7;
			    efp->ifexp_result12 = ifexp_result12;
			    efp->tmp7 = tmp7;
			    efp->ifexp_result10 = ifexp_result10;
			    efp->tmp6 = tmp6;
			    efp->s = s;
			    efp->t = t;
			    efp->swaptype = swaptype;
			    efp->r = r;
			    efp->pv = pv;
			    efp->pn = pn;
			    efp->pm = pm;
			    efp->pl = pl;
			    efp->pd = pd;
			    efp->pc = pc;
			    efp->pb = pb;
			    efp->pa = pa;
			    efp->cmp = cmp;
			    efp->es = es;
			    efp->n = n;
			    efp->a = a;
			    efp->argp = argp;
			    efp->tmp_esp = argp;
			    efp->call_id = 2;
			    return;
			  L_CALL8:
			    ;
			    ifexp_result12 = efp->ifexp_result12;
			    tmp7 = efp->tmp7;
			    ifexp_result10 = efp->ifexp_result10;
			    tmp6 = efp->tmp6;
			    s = efp->s;
			    t = efp->t;
			    swaptype = efp->swaptype;
			    r = efp->r;
			    pv = efp->pv;
			    pn = efp->pn;
			    pm = efp->pm;
			    pl = efp->pl;
			    pd = efp->pd;
			    pc = efp->pc;
			    pb = efp->pb;
			    pa = efp->pa;
			    cmp = efp->cmp;
			    es = efp->es;
			    n = efp->n;
			    a = efp->a;
			    tmp7 = *((int *) efp->argp);
			  }
			  ifexp_result12 = tmp7 > 0 ? 1 : 0;
			}
		      else
			{
			  ifexp_result12 = 0;
			}
		    }
		  while (ifexp_result12);
	      }
	      pm += es;
	    }
	  while (pm < a + n * es);
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
	  new_esp = esp;
	  tmp_arg2 = pl + s;
	  tmp_arg3 = pl + 2 * s;
	  while ((pl =
		  med3 (new_esp, pl, tmp_arg2, tmp_arg3, cmp)) == (char *) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->ifexp_result12 = ifexp_result12;
	      efp->tmp7 = tmp7;
	      efp->ifexp_result10 = ifexp_result10;
	      efp->tmp6 = tmp6;
	      efp->s = s;
	      efp->t = t;
	      efp->swaptype = swaptype;
	      efp->r = r;
	      efp->pv = pv;
	      efp->pn = pn;
	      efp->pm = pm;
	      efp->pl = pl;
	      efp->pd = pd;
	      efp->pc = pc;
	      efp->pb = pb;
	      efp->pa = pa;
	      efp->cmp = cmp;
	      efp->es = es;
	      efp->n = n;
	      efp->a = a;
	      efp->call_id = 3;
	      return;
	    L_CALL9:
	      ;
	      ifexp_result12 = efp->ifexp_result12;
	      tmp7 = efp->tmp7;
	      ifexp_result10 = efp->ifexp_result10;
	      tmp6 = efp->tmp6;
	      s = efp->s;
	      t = efp->t;
	      swaptype = efp->swaptype;
	      r = efp->r;
	      pv = efp->pv;
	      pn = efp->pn;
	      pm = efp->pm;
	      pl = efp->pl;
	      pd = efp->pd;
	      pc = efp->pc;
	      pb = efp->pb;
	      pa = efp->pa;
	      cmp = efp->cmp;
	      es = efp->es;
	      n = efp->n;
	      a = efp->a;
	      tmp_arg2 = efp->tmp_arg2;
	      tmp_arg3 = efp->tmp_arg3;
	      new_esp = esp + 1;
	    }
	  new_esp = esp;
	  tmp_arg4 = pm - s;
	  tmp_arg5 = pm + s;
	  while ((pm =
		  med3 (new_esp, tmp_arg4, pm, tmp_arg5, cmp)) == (char *) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->ifexp_result12 = ifexp_result12;
	      efp->tmp7 = tmp7;
	      efp->ifexp_result10 = ifexp_result10;
	      efp->tmp6 = tmp6;
	      efp->s = s;
	      efp->t = t;
	      efp->swaptype = swaptype;
	      efp->r = r;
	      efp->pv = pv;
	      efp->pn = pn;
	      efp->pm = pm;
	      efp->pl = pl;
	      efp->pd = pd;
	      efp->pc = pc;
	      efp->pb = pb;
	      efp->pa = pa;
	      efp->cmp = cmp;
	      efp->es = es;
	      efp->n = n;
	      efp->a = a;
	      efp->call_id = 4;
	      return;
	    L_CALL10:
	      ;
	      ifexp_result12 = efp->ifexp_result12;
	      tmp7 = efp->tmp7;
	      ifexp_result10 = efp->ifexp_result10;
	      tmp6 = efp->tmp6;
	      s = efp->s;
	      t = efp->t;
	      swaptype = efp->swaptype;
	      r = efp->r;
	      pv = efp->pv;
	      pn = efp->pn;
	      pm = efp->pm;
	      pl = efp->pl;
	      pd = efp->pd;
	      pc = efp->pc;
	      pb = efp->pb;
	      pa = efp->pa;
	      cmp = efp->cmp;
	      es = efp->es;
	      n = efp->n;
	      a = efp->a;
	      tmp_arg4 = efp->tmp_arg4;
	      tmp_arg5 = efp->tmp_arg5;
	      new_esp = esp + 1;
	    }
	  new_esp = esp;
	  tmp_arg6 = pn - 2 * s;
	  tmp_arg7 = pn - s;
	  while ((pn =
		  med3 (new_esp, tmp_arg6, tmp_arg7, pn, cmp)) == (char *) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->ifexp_result12 = ifexp_result12;
	      efp->tmp7 = tmp7;
	      efp->ifexp_result10 = ifexp_result10;
	      efp->tmp6 = tmp6;
	      efp->s = s;
	      efp->t = t;
	      efp->swaptype = swaptype;
	      efp->r = r;
	      efp->pv = pv;
	      efp->pn = pn;
	      efp->pm = pm;
	      efp->pl = pl;
	      efp->pd = pd;
	      efp->pc = pc;
	      efp->pb = pb;
	      efp->pa = pa;
	      efp->cmp = cmp;
	      efp->es = es;
	      efp->n = n;
	      efp->a = a;
	      efp->call_id = 5;
	      return;
	    L_CALL11:
	      ;
	      ifexp_result12 = efp->ifexp_result12;
	      tmp7 = efp->tmp7;
	      ifexp_result10 = efp->ifexp_result10;
	      tmp6 = efp->tmp6;
	      s = efp->s;
	      t = efp->t;
	      swaptype = efp->swaptype;
	      r = efp->r;
	      pv = efp->pv;
	      pn = efp->pn;
	      pm = efp->pm;
	      pl = efp->pl;
	      pd = efp->pd;
	      pc = efp->pc;
	      pb = efp->pb;
	      pa = efp->pa;
	      cmp = efp->cmp;
	      es = efp->es;
	      n = efp->n;
	      a = efp->a;
	      tmp_arg6 = efp->tmp_arg6;
	      tmp_arg7 = efp->tmp_arg7;
	      new_esp = esp + 1;
	    }
	}
      new_esp = esp;
      while ((pm = med3 (new_esp, pl, pm, pn, cmp)) == (char *) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->ifexp_result12 = ifexp_result12;
	  efp->tmp7 = tmp7;
	  efp->ifexp_result10 = ifexp_result10;
	  efp->tmp6 = tmp6;
	  efp->s = s;
	  efp->t = t;
	  efp->swaptype = swaptype;
	  efp->r = r;
	  efp->pv = pv;
	  efp->pn = pn;
	  efp->pm = pm;
	  efp->pl = pl;
	  efp->pd = pd;
	  efp->pc = pc;
	  efp->pb = pb;
	  efp->pa = pa;
	  efp->cmp = cmp;
	  efp->es = es;
	  efp->n = n;
	  efp->a = a;
	  efp->call_id = 6;
	  return;
	L_CALL12:
	  ;
	  ifexp_result12 = efp->ifexp_result12;
	  tmp7 = efp->tmp7;
	  ifexp_result10 = efp->ifexp_result10;
	  tmp6 = efp->tmp6;
	  s = efp->s;
	  t = efp->t;
	  swaptype = efp->swaptype;
	  r = efp->r;
	  pv = efp->pv;
	  pn = efp->pn;
	  pm = efp->pm;
	  pl = efp->pl;
	  pd = efp->pd;
	  pc = efp->pc;
	  pb = efp->pb;
	  pa = efp->pa;
	  cmp = efp->cmp;
	  es = efp->es;
	  n = efp->n;
	  a = efp->a;
	  new_esp = esp + 1;
	}
    }
  if (swaptype != 0)
    {
      pv = a;
      if (swaptype != 0)
	{
	  new_esp = esp;
	  while (swapfunc (new_esp, pv, pm, es, swaptype),
		 (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->ifexp_result12 = ifexp_result12;
	      efp->tmp7 = tmp7;
	      efp->ifexp_result10 = ifexp_result10;
	      efp->tmp6 = tmp6;
	      efp->s = s;
	      efp->t = t;
	      efp->swaptype = swaptype;
	      efp->r = r;
	      efp->pv = pv;
	      efp->pn = pn;
	      efp->pm = pm;
	      efp->pl = pl;
	      efp->pd = pd;
	      efp->pc = pc;
	      efp->pb = pb;
	      efp->pa = pa;
	      efp->cmp = cmp;
	      efp->es = es;
	      efp->n = n;
	      efp->a = a;
	      efp->call_id = 7;
	      return;
	    L_CALL13:
	      ;
	      ifexp_result12 = efp->ifexp_result12;
	      tmp7 = efp->tmp7;
	      ifexp_result10 = efp->ifexp_result10;
	      tmp6 = efp->tmp6;
	      s = efp->s;
	      t = efp->t;
	      swaptype = efp->swaptype;
	      r = efp->r;
	      pv = efp->pv;
	      pn = efp->pn;
	      pm = efp->pm;
	      pl = efp->pl;
	      pd = efp->pd;
	      pc = efp->pc;
	      pb = efp->pb;
	      pa = efp->pa;
	      cmp = efp->cmp;
	      es = efp->es;
	      n = efp->n;
	      a = efp->a;
	      new_esp = esp + 1;
	    }
	  0;
	}
      else
	{
	  t = *((WORD *) pv);
	  *((WORD *) pv) = *((WORD *) pm);
	  (void) (*((WORD *) pm) = t);
	}
      0;
    }
  else
    {
      pv = (char *) &efp->v;
      efp->v = *((WORD *) pm);
    }
  pa = pb = a;
  pc = pd = a + (n - 1) * es;
  do
    {
      if (pb <= pc)
	{
	  {
	    tmp_fp8 = cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = pv, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = pb, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp8;
	    efp->ifexp_result18 = ifexp_result18;
	    efp->tmp13 = tmp13;
	    efp->ifexp_result14 = ifexp_result14;
	    efp->tmp10 = tmp10;
	    efp->ifexp_result12 = ifexp_result12;
	    efp->tmp7 = tmp7;
	    efp->ifexp_result10 = ifexp_result10;
	    efp->tmp6 = tmp6;
	    efp->s = s;
	    efp->t = t;
	    efp->swaptype = swaptype;
	    efp->r = r;
	    efp->pv = pv;
	    efp->pn = pn;
	    efp->pm = pm;
	    efp->pl = pl;
	    efp->pd = pd;
	    efp->pc = pc;
	    efp->pb = pb;
	    efp->pa = pa;
	    efp->cmp = cmp;
	    efp->es = es;
	    efp->n = n;
	    efp->a = a;
	    efp->argp = argp;
	    efp->tmp_esp = argp;
	    efp->call_id = 8;
	    return;
	  L_CALL14:
	    ;
	    ifexp_result18 = efp->ifexp_result18;
	    tmp13 = efp->tmp13;
	    ifexp_result14 = efp->ifexp_result14;
	    tmp10 = efp->tmp10;
	    ifexp_result12 = efp->ifexp_result12;
	    tmp7 = efp->tmp7;
	    ifexp_result10 = efp->ifexp_result10;
	    tmp6 = efp->tmp6;
	    s = efp->s;
	    t = efp->t;
	    swaptype = efp->swaptype;
	    r = efp->r;
	    pv = efp->pv;
	    pn = efp->pn;
	    pm = efp->pm;
	    pl = efp->pl;
	    pd = efp->pd;
	    pc = efp->pc;
	    pb = efp->pb;
	    pa = efp->pa;
	    cmp = efp->cmp;
	    es = efp->es;
	    n = efp->n;
	    a = efp->a;
	    tmp10 = *((int *) efp->argp);
	  }
	  ifexp_result14 = (r = tmp10) <= 0 ? 1 : 0;
	}
      else
	{
	  ifexp_result14 = 0;
	}
      if (ifexp_result14)
	do
	  {
	    if (r == 0)
	      {
		if (swaptype != 0)
		  {
		    new_esp = esp;
		    while (swapfunc (new_esp, pa, pb, es, swaptype),
			   (efp->tmp_esp = *((char **) esp)) != 0)
		      {
			efp->ifexp_result16 = ifexp_result16;
			efp->tmp11 = tmp11;
			efp->ifexp_result18 = ifexp_result18;
			efp->tmp13 = tmp13;
			efp->ifexp_result14 = ifexp_result14;
			efp->tmp10 = tmp10;
			efp->ifexp_result12 = ifexp_result12;
			efp->tmp7 = tmp7;
			efp->ifexp_result10 = ifexp_result10;
			efp->tmp6 = tmp6;
			efp->s = s;
			efp->t = t;
			efp->swaptype = swaptype;
			efp->r = r;
			efp->pv = pv;
			efp->pn = pn;
			efp->pm = pm;
			efp->pl = pl;
			efp->pd = pd;
			efp->pc = pc;
			efp->pb = pb;
			efp->pa = pa;
			efp->cmp = cmp;
			efp->es = es;
			efp->n = n;
			efp->a = a;
			efp->call_id = 9;
			return;
		      L_CALL15:
			;
			ifexp_result16 = efp->ifexp_result16;
			tmp11 = efp->tmp11;
			ifexp_result18 = efp->ifexp_result18;
			tmp13 = efp->tmp13;
			ifexp_result14 = efp->ifexp_result14;
			tmp10 = efp->tmp10;
			ifexp_result12 = efp->ifexp_result12;
			tmp7 = efp->tmp7;
			ifexp_result10 = efp->ifexp_result10;
			tmp6 = efp->tmp6;
			s = efp->s;
			t = efp->t;
			swaptype = efp->swaptype;
			r = efp->r;
			pv = efp->pv;
			pn = efp->pn;
			pm = efp->pm;
			pl = efp->pl;
			pd = efp->pd;
			pc = efp->pc;
			pb = efp->pb;
			pa = efp->pa;
			cmp = efp->cmp;
			es = efp->es;
			n = efp->n;
			a = efp->a;
			new_esp = esp + 1;
		      }
		    0;
		  }
		else
		  {
		    t = *((WORD *) pa);
		    *((WORD *) pa) = *((WORD *) pb);
		    (void) (*((WORD *) pb) = t);
		  }
		0;
		pa += es;
	      }
	    pb += es;
	    if (pb <= pc)
	      {
		{
		  tmp_fp9 = cmp;
		  argp =
		      (char *) ((Align_t *) esp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = pv, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = pb, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((closure_t **) argp) = tmp_fp9;
		  efp->ifexp_result16 = ifexp_result16;
		  efp->tmp11 = tmp11;
		  efp->ifexp_result18 = ifexp_result18;
		  efp->tmp13 = tmp13;
		  efp->ifexp_result14 = ifexp_result14;
		  efp->tmp10 = tmp10;
		  efp->ifexp_result12 = ifexp_result12;
		  efp->tmp7 = tmp7;
		  efp->ifexp_result10 = ifexp_result10;
		  efp->tmp6 = tmp6;
		  efp->s = s;
		  efp->t = t;
		  efp->swaptype = swaptype;
		  efp->r = r;
		  efp->pv = pv;
		  efp->pn = pn;
		  efp->pm = pm;
		  efp->pl = pl;
		  efp->pd = pd;
		  efp->pc = pc;
		  efp->pb = pb;
		  efp->pa = pa;
		  efp->cmp = cmp;
		  efp->es = es;
		  efp->n = n;
		  efp->a = a;
		  efp->argp = argp;
		  efp->tmp_esp = argp;
		  efp->call_id = 10;
		  return;
		L_CALL16:
		  ;
		  ifexp_result16 = efp->ifexp_result16;
		  tmp11 = efp->tmp11;
		  ifexp_result18 = efp->ifexp_result18;
		  tmp13 = efp->tmp13;
		  ifexp_result14 = efp->ifexp_result14;
		  tmp10 = efp->tmp10;
		  ifexp_result12 = efp->ifexp_result12;
		  tmp7 = efp->tmp7;
		  ifexp_result10 = efp->ifexp_result10;
		  tmp6 = efp->tmp6;
		  s = efp->s;
		  t = efp->t;
		  swaptype = efp->swaptype;
		  r = efp->r;
		  pv = efp->pv;
		  pn = efp->pn;
		  pm = efp->pm;
		  pl = efp->pl;
		  pd = efp->pd;
		  pc = efp->pc;
		  pb = efp->pb;
		  pa = efp->pa;
		  cmp = efp->cmp;
		  es = efp->es;
		  n = efp->n;
		  a = efp->a;
		  tmp11 = *((int *) efp->argp);
		}
		ifexp_result16 = (r = tmp11) <= 0 ? 1 : 0;
	      }
	    else
	      {
		ifexp_result16 = 0;
	      }
	  }
	while (ifexp_result16);
      if (pc >= pb)
	{
	  {
	    tmp_fp10 = cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = pv, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = pc, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp10;
	    efp->ifexp_result16 = ifexp_result16;
	    efp->tmp11 = tmp11;
	    efp->ifexp_result18 = ifexp_result18;
	    efp->tmp13 = tmp13;
	    efp->ifexp_result14 = ifexp_result14;
	    efp->tmp10 = tmp10;
	    efp->ifexp_result12 = ifexp_result12;
	    efp->tmp7 = tmp7;
	    efp->ifexp_result10 = ifexp_result10;
	    efp->tmp6 = tmp6;
	    efp->s = s;
	    efp->t = t;
	    efp->swaptype = swaptype;
	    efp->r = r;
	    efp->pv = pv;
	    efp->pn = pn;
	    efp->pm = pm;
	    efp->pl = pl;
	    efp->pd = pd;
	    efp->pc = pc;
	    efp->pb = pb;
	    efp->pa = pa;
	    efp->cmp = cmp;
	    efp->es = es;
	    efp->n = n;
	    efp->a = a;
	    efp->argp = argp;
	    efp->tmp_esp = argp;
	    efp->call_id = 11;
	    return;
	  L_CALL17:
	    ;
	    ifexp_result16 = efp->ifexp_result16;
	    tmp11 = efp->tmp11;
	    ifexp_result18 = efp->ifexp_result18;
	    tmp13 = efp->tmp13;
	    ifexp_result14 = efp->ifexp_result14;
	    tmp10 = efp->tmp10;
	    ifexp_result12 = efp->ifexp_result12;
	    tmp7 = efp->tmp7;
	    ifexp_result10 = efp->ifexp_result10;
	    tmp6 = efp->tmp6;
	    s = efp->s;
	    t = efp->t;
	    swaptype = efp->swaptype;
	    r = efp->r;
	    pv = efp->pv;
	    pn = efp->pn;
	    pm = efp->pm;
	    pl = efp->pl;
	    pd = efp->pd;
	    pc = efp->pc;
	    pb = efp->pb;
	    pa = efp->pa;
	    cmp = efp->cmp;
	    es = efp->es;
	    n = efp->n;
	    a = efp->a;
	    tmp13 = *((int *) efp->argp);
	  }
	  ifexp_result18 = (r = tmp13) >= 0 ? 1 : 0;
	}
      else
	{
	  ifexp_result18 = 0;
	}
      if (ifexp_result18)
	do
	  {
	    if (r == 0)
	      {
		if (swaptype != 0)
		  {
		    new_esp = esp;
		    while (swapfunc (new_esp, pc, pd, es, swaptype),
			   (efp->tmp_esp = *((char **) esp)) != 0)
		      {
			efp->ifexp_result20 = ifexp_result20;
			efp->tmp14 = tmp14;
			efp->ifexp_result16 = ifexp_result16;
			efp->tmp11 = tmp11;
			efp->ifexp_result18 = ifexp_result18;
			efp->tmp13 = tmp13;
			efp->ifexp_result14 = ifexp_result14;
			efp->tmp10 = tmp10;
			efp->ifexp_result12 = ifexp_result12;
			efp->tmp7 = tmp7;
			efp->ifexp_result10 = ifexp_result10;
			efp->tmp6 = tmp6;
			efp->s = s;
			efp->t = t;
			efp->swaptype = swaptype;
			efp->r = r;
			efp->pv = pv;
			efp->pn = pn;
			efp->pm = pm;
			efp->pl = pl;
			efp->pd = pd;
			efp->pc = pc;
			efp->pb = pb;
			efp->pa = pa;
			efp->cmp = cmp;
			efp->es = es;
			efp->n = n;
			efp->a = a;
			efp->call_id = 12;
			return;
		      L_CALL18:
			;
			ifexp_result20 = efp->ifexp_result20;
			tmp14 = efp->tmp14;
			ifexp_result16 = efp->ifexp_result16;
			tmp11 = efp->tmp11;
			ifexp_result18 = efp->ifexp_result18;
			tmp13 = efp->tmp13;
			ifexp_result14 = efp->ifexp_result14;
			tmp10 = efp->tmp10;
			ifexp_result12 = efp->ifexp_result12;
			tmp7 = efp->tmp7;
			ifexp_result10 = efp->ifexp_result10;
			tmp6 = efp->tmp6;
			s = efp->s;
			t = efp->t;
			swaptype = efp->swaptype;
			r = efp->r;
			pv = efp->pv;
			pn = efp->pn;
			pm = efp->pm;
			pl = efp->pl;
			pd = efp->pd;
			pc = efp->pc;
			pb = efp->pb;
			pa = efp->pa;
			cmp = efp->cmp;
			es = efp->es;
			n = efp->n;
			a = efp->a;
			new_esp = esp + 1;
		      }
		    0;
		  }
		else
		  {
		    t = *((WORD *) pc);
		    *((WORD *) pc) = *((WORD *) pd);
		    (void) (*((WORD *) pd) = t);
		  }
		0;
		pd -= es;
	      }
	    pc -= es;
	    if (pc >= pb)
	      {
		{
		  tmp_fp11 = cmp;
		  argp =
		      (char *) ((Align_t *) esp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = pv, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = pc, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((closure_t **) argp) = tmp_fp11;
		  efp->ifexp_result20 = ifexp_result20;
		  efp->tmp14 = tmp14;
		  efp->ifexp_result16 = ifexp_result16;
		  efp->tmp11 = tmp11;
		  efp->ifexp_result18 = ifexp_result18;
		  efp->tmp13 = tmp13;
		  efp->ifexp_result14 = ifexp_result14;
		  efp->tmp10 = tmp10;
		  efp->ifexp_result12 = ifexp_result12;
		  efp->tmp7 = tmp7;
		  efp->ifexp_result10 = ifexp_result10;
		  efp->tmp6 = tmp6;
		  efp->s = s;
		  efp->t = t;
		  efp->swaptype = swaptype;
		  efp->r = r;
		  efp->pv = pv;
		  efp->pn = pn;
		  efp->pm = pm;
		  efp->pl = pl;
		  efp->pd = pd;
		  efp->pc = pc;
		  efp->pb = pb;
		  efp->pa = pa;
		  efp->cmp = cmp;
		  efp->es = es;
		  efp->n = n;
		  efp->a = a;
		  efp->argp = argp;
		  efp->tmp_esp = argp;
		  efp->call_id = 13;
		  return;
		L_CALL19:
		  ;
		  ifexp_result20 = efp->ifexp_result20;
		  tmp14 = efp->tmp14;
		  ifexp_result16 = efp->ifexp_result16;
		  tmp11 = efp->tmp11;
		  ifexp_result18 = efp->ifexp_result18;
		  tmp13 = efp->tmp13;
		  ifexp_result14 = efp->ifexp_result14;
		  tmp10 = efp->tmp10;
		  ifexp_result12 = efp->ifexp_result12;
		  tmp7 = efp->tmp7;
		  ifexp_result10 = efp->ifexp_result10;
		  tmp6 = efp->tmp6;
		  s = efp->s;
		  t = efp->t;
		  swaptype = efp->swaptype;
		  r = efp->r;
		  pv = efp->pv;
		  pn = efp->pn;
		  pm = efp->pm;
		  pl = efp->pl;
		  pd = efp->pd;
		  pc = efp->pc;
		  pb = efp->pb;
		  pa = efp->pa;
		  cmp = efp->cmp;
		  es = efp->es;
		  n = efp->n;
		  a = efp->a;
		  tmp14 = *((int *) efp->argp);
		}
		ifexp_result20 = (r = tmp14) >= 0 ? 1 : 0;
	      }
	    else
	      {
		ifexp_result20 = 0;
	      }
	  }
	while (ifexp_result20);
      if (pb > pc)
	{
	  break;
	}
      if (swaptype != 0)
	{
	  new_esp = esp;
	  while (swapfunc (new_esp, pb, pc, es, swaptype),
		 (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->ifexp_result20 = ifexp_result20;
	      efp->tmp14 = tmp14;
	      efp->ifexp_result16 = ifexp_result16;
	      efp->tmp11 = tmp11;
	      efp->ifexp_result18 = ifexp_result18;
	      efp->tmp13 = tmp13;
	      efp->ifexp_result14 = ifexp_result14;
	      efp->tmp10 = tmp10;
	      efp->ifexp_result12 = ifexp_result12;
	      efp->tmp7 = tmp7;
	      efp->ifexp_result10 = ifexp_result10;
	      efp->tmp6 = tmp6;
	      efp->s = s;
	      efp->t = t;
	      efp->swaptype = swaptype;
	      efp->r = r;
	      efp->pv = pv;
	      efp->pn = pn;
	      efp->pm = pm;
	      efp->pl = pl;
	      efp->pd = pd;
	      efp->pc = pc;
	      efp->pb = pb;
	      efp->pa = pa;
	      efp->cmp = cmp;
	      efp->es = es;
	      efp->n = n;
	      efp->a = a;
	      efp->call_id = 14;
	      return;
	    L_CALL20:
	      ;
	      ifexp_result20 = efp->ifexp_result20;
	      tmp14 = efp->tmp14;
	      ifexp_result16 = efp->ifexp_result16;
	      tmp11 = efp->tmp11;
	      ifexp_result18 = efp->ifexp_result18;
	      tmp13 = efp->tmp13;
	      ifexp_result14 = efp->ifexp_result14;
	      tmp10 = efp->tmp10;
	      ifexp_result12 = efp->ifexp_result12;
	      tmp7 = efp->tmp7;
	      ifexp_result10 = efp->ifexp_result10;
	      tmp6 = efp->tmp6;
	      s = efp->s;
	      t = efp->t;
	      swaptype = efp->swaptype;
	      r = efp->r;
	      pv = efp->pv;
	      pn = efp->pn;
	      pm = efp->pm;
	      pl = efp->pl;
	      pd = efp->pd;
	      pc = efp->pc;
	      pb = efp->pb;
	      pa = efp->pa;
	      cmp = efp->cmp;
	      es = efp->es;
	      n = efp->n;
	      a = efp->a;
	      new_esp = esp + 1;
	    }
	  0;
	}
      else
	{
	  t = *((WORD *) pb);
	  *((WORD *) pb) = *((WORD *) pc);
	  (void) (*((WORD *) pc) = t);
	}
      0;
      pb += es;
      pc -= es;
    }
  while (1);
  pn = a + n * es;
  s = pa - a < pb - pa ? pa - a : pb - pa;
  if (s > 0)
    {
      new_esp = esp;
      tmp_arg8 = pb - s;
      while (swapfunc (new_esp, a, tmp_arg8, s, swaptype),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->ifexp_result20 = ifexp_result20;
	  efp->tmp14 = tmp14;
	  efp->ifexp_result16 = ifexp_result16;
	  efp->tmp11 = tmp11;
	  efp->ifexp_result18 = ifexp_result18;
	  efp->tmp13 = tmp13;
	  efp->ifexp_result14 = ifexp_result14;
	  efp->tmp10 = tmp10;
	  efp->ifexp_result12 = ifexp_result12;
	  efp->tmp7 = tmp7;
	  efp->ifexp_result10 = ifexp_result10;
	  efp->tmp6 = tmp6;
	  efp->s = s;
	  efp->t = t;
	  efp->swaptype = swaptype;
	  efp->r = r;
	  efp->pv = pv;
	  efp->pn = pn;
	  efp->pm = pm;
	  efp->pl = pl;
	  efp->pd = pd;
	  efp->pc = pc;
	  efp->pb = pb;
	  efp->pa = pa;
	  efp->cmp = cmp;
	  efp->es = es;
	  efp->n = n;
	  efp->a = a;
	  efp->call_id = 15;
	  return;
	L_CALL21:
	  ;
	  ifexp_result20 = efp->ifexp_result20;
	  tmp14 = efp->tmp14;
	  ifexp_result16 = efp->ifexp_result16;
	  tmp11 = efp->tmp11;
	  ifexp_result18 = efp->ifexp_result18;
	  tmp13 = efp->tmp13;
	  ifexp_result14 = efp->ifexp_result14;
	  tmp10 = efp->tmp10;
	  ifexp_result12 = efp->ifexp_result12;
	  tmp7 = efp->tmp7;
	  ifexp_result10 = efp->ifexp_result10;
	  tmp6 = efp->tmp6;
	  s = efp->s;
	  t = efp->t;
	  swaptype = efp->swaptype;
	  r = efp->r;
	  pv = efp->pv;
	  pn = efp->pn;
	  pm = efp->pm;
	  pl = efp->pl;
	  pd = efp->pd;
	  pc = efp->pc;
	  pb = efp->pb;
	  pa = efp->pa;
	  cmp = efp->cmp;
	  es = efp->es;
	  n = efp->n;
	  a = efp->a;
	  tmp_arg8 = efp->tmp_arg8;
	  new_esp = esp + 1;
	}
    }
  s = pd - pc < pn - pd - es ? pd - pc : pn - pd - es;
  if (s > 0)
    {
      new_esp = esp;
      tmp_arg9 = pn - s;
      while (swapfunc (new_esp, pb, tmp_arg9, s, swaptype),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->ifexp_result20 = ifexp_result20;
	  efp->tmp14 = tmp14;
	  efp->ifexp_result16 = ifexp_result16;
	  efp->tmp11 = tmp11;
	  efp->ifexp_result18 = ifexp_result18;
	  efp->tmp13 = tmp13;
	  efp->ifexp_result14 = ifexp_result14;
	  efp->tmp10 = tmp10;
	  efp->ifexp_result12 = ifexp_result12;
	  efp->tmp7 = tmp7;
	  efp->ifexp_result10 = ifexp_result10;
	  efp->tmp6 = tmp6;
	  efp->s = s;
	  efp->t = t;
	  efp->swaptype = swaptype;
	  efp->r = r;
	  efp->pv = pv;
	  efp->pn = pn;
	  efp->pm = pm;
	  efp->pl = pl;
	  efp->pd = pd;
	  efp->pc = pc;
	  efp->pb = pb;
	  efp->pa = pa;
	  efp->cmp = cmp;
	  efp->es = es;
	  efp->n = n;
	  efp->a = a;
	  efp->call_id = 16;
	  return;
	L_CALL22:
	  ;
	  ifexp_result20 = efp->ifexp_result20;
	  tmp14 = efp->tmp14;
	  ifexp_result16 = efp->ifexp_result16;
	  tmp11 = efp->tmp11;
	  ifexp_result18 = efp->ifexp_result18;
	  tmp13 = efp->tmp13;
	  ifexp_result14 = efp->ifexp_result14;
	  tmp10 = efp->tmp10;
	  ifexp_result12 = efp->ifexp_result12;
	  tmp7 = efp->tmp7;
	  ifexp_result10 = efp->ifexp_result10;
	  tmp6 = efp->tmp6;
	  s = efp->s;
	  t = efp->t;
	  swaptype = efp->swaptype;
	  r = efp->r;
	  pv = efp->pv;
	  pn = efp->pn;
	  pm = efp->pm;
	  pl = efp->pl;
	  pd = efp->pd;
	  pc = efp->pc;
	  pb = efp->pb;
	  pa = efp->pa;
	  cmp = efp->cmp;
	  es = efp->es;
	  n = efp->n;
	  a = efp->a;
	  tmp_arg9 = efp->tmp_arg9;
	  new_esp = esp + 1;
	}
    }
  if ((s = pb - pa) > es)
    {
      new_esp = esp;
      tmp_arg10 = s / es;
      while (quicksort (new_esp, a, tmp_arg10, es, cmp),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->ifexp_result20 = ifexp_result20;
	  efp->tmp14 = tmp14;
	  efp->ifexp_result16 = ifexp_result16;
	  efp->tmp11 = tmp11;
	  efp->ifexp_result18 = ifexp_result18;
	  efp->tmp13 = tmp13;
	  efp->ifexp_result14 = ifexp_result14;
	  efp->tmp10 = tmp10;
	  efp->ifexp_result12 = ifexp_result12;
	  efp->tmp7 = tmp7;
	  efp->ifexp_result10 = ifexp_result10;
	  efp->tmp6 = tmp6;
	  efp->s = s;
	  efp->t = t;
	  efp->swaptype = swaptype;
	  efp->r = r;
	  efp->pv = pv;
	  efp->pn = pn;
	  efp->pm = pm;
	  efp->pl = pl;
	  efp->pd = pd;
	  efp->pc = pc;
	  efp->pb = pb;
	  efp->pa = pa;
	  efp->cmp = cmp;
	  efp->es = es;
	  efp->n = n;
	  efp->a = a;
	  efp->call_id = 17;
	  return;
	L_CALL23:
	  ;
	  ifexp_result20 = efp->ifexp_result20;
	  tmp14 = efp->tmp14;
	  ifexp_result16 = efp->ifexp_result16;
	  tmp11 = efp->tmp11;
	  ifexp_result18 = efp->ifexp_result18;
	  tmp13 = efp->tmp13;
	  ifexp_result14 = efp->ifexp_result14;
	  tmp10 = efp->tmp10;
	  ifexp_result12 = efp->ifexp_result12;
	  tmp7 = efp->tmp7;
	  ifexp_result10 = efp->ifexp_result10;
	  tmp6 = efp->tmp6;
	  s = efp->s;
	  t = efp->t;
	  swaptype = efp->swaptype;
	  r = efp->r;
	  pv = efp->pv;
	  pn = efp->pn;
	  pm = efp->pm;
	  pl = efp->pl;
	  pd = efp->pd;
	  pc = efp->pc;
	  pb = efp->pb;
	  pa = efp->pa;
	  cmp = efp->cmp;
	  es = efp->es;
	  n = efp->n;
	  a = efp->a;
	  tmp_arg10 = efp->tmp_arg10;
	  new_esp = esp + 1;
	}
    }
  if ((s = pd - pc) > es)
    {
      new_esp = esp;
      tmp_arg11 = pn - s;
      tmp_arg12 = s / es;
      while (quicksort (new_esp, tmp_arg11, tmp_arg12, es, cmp),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->ifexp_result20 = ifexp_result20;
	  efp->tmp14 = tmp14;
	  efp->ifexp_result16 = ifexp_result16;
	  efp->tmp11 = tmp11;
	  efp->ifexp_result18 = ifexp_result18;
	  efp->tmp13 = tmp13;
	  efp->ifexp_result14 = ifexp_result14;
	  efp->tmp10 = tmp10;
	  efp->ifexp_result12 = ifexp_result12;
	  efp->tmp7 = tmp7;
	  efp->ifexp_result10 = ifexp_result10;
	  efp->tmp6 = tmp6;
	  efp->s = s;
	  efp->t = t;
	  efp->swaptype = swaptype;
	  efp->r = r;
	  efp->pv = pv;
	  efp->pn = pn;
	  efp->pm = pm;
	  efp->pl = pl;
	  efp->pd = pd;
	  efp->pc = pc;
	  efp->pb = pb;
	  efp->pa = pa;
	  efp->cmp = cmp;
	  efp->es = es;
	  efp->n = n;
	  efp->a = a;
	  efp->call_id = 18;
	  return;
	L_CALL24:
	  ;
	  ifexp_result20 = efp->ifexp_result20;
	  tmp14 = efp->tmp14;
	  ifexp_result16 = efp->ifexp_result16;
	  tmp11 = efp->tmp11;
	  ifexp_result18 = efp->ifexp_result18;
	  tmp13 = efp->tmp13;
	  ifexp_result14 = efp->ifexp_result14;
	  tmp10 = efp->tmp10;
	  ifexp_result12 = efp->ifexp_result12;
	  tmp7 = efp->tmp7;
	  ifexp_result10 = efp->ifexp_result10;
	  tmp6 = efp->tmp6;
	  s = efp->s;
	  t = efp->t;
	  swaptype = efp->swaptype;
	  r = efp->r;
	  pv = efp->pv;
	  pn = efp->pn;
	  pm = efp->pm;
	  pl = efp->pl;
	  pd = efp->pd;
	  pc = efp->pc;
	  pb = efp->pb;
	  pa = efp->pa;
	  cmp = efp->cmp;
	  es = efp->es;
	  n = efp->n;
	  a = efp->a;
	  tmp_arg11 = efp->tmp_arg11;
	  tmp_arg12 = efp->tmp_arg12;
	  new_esp = esp + 1;
	}
    }
}

struct modsort_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int m;
  int n;
  int *a;
  int tmp_arg13;
  closure_t comp_mod;
};

struct comp_mod_in_modsort_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int *pq;
  int *pp;
  struct modsort_frame *xfp;
};

char *
comp_mod_in_modsort (char *esp, void *xfp0)
{
  char *new_esp;
  struct comp_mod_in_modsort_frame *efp;
  struct modsort_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  int *pp = (parmp =
	     (char *) ((Align_t *) parmp -
		       (sizeof (int *) + sizeof (Align_t) +
			-1) / sizeof (Align_t)), *((int **) parmp));
  int *pq = (parmp =
	     (char *) ((Align_t *) parmp -
		       (sizeof (int *) + sizeof (Align_t) +
			-1) / sizeof (Align_t)), *((int **) parmp));
lgoto:
  ;
  efp = (struct comp_mod_in_modsort_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct comp_mod_in_modsort_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  *((int *) efp) =
      *pp % xfp->m < *pq % xfp->m ? 1 : (*pp % xfp->m == *pq % xfp->m ? 0 : -1);
  return 0;
  return 0;
}

void
modsort (char *esp, int *a, int n, int m)
{
  struct modsort_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int tmp_arg13;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct modsort_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct modsort_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL25;
	}
      goto L_CALL25;
    }
  efp = (struct modsort_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct modsort_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  tmp_arg13 = sizeof (int);
  while (quicksort (new_esp, a, n, tmp_arg13, &efp->comp_mod),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      efp->m = m;
      efp->n = n;
      efp->a = a;
      efp->comp_mod.fun = comp_mod_in_modsort;
      efp->comp_mod.fr = (void *) efp;
      efp->call_id = 0;
      return;
    L_CALL25:
      ;
      m = efp->m;
      n = efp->n;
      a = efp->a;
      tmp_arg13 = efp->tmp_arg13;
      new_esp = esp + 1;
    }
}
int *aa;
int nn;
int mm;

struct foo_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int n;
  int tmp_arg14;
};

void
foo (char *esp, int n)
{
  struct foo_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int tmp_arg14;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct foo_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct foo_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL26;
	case 1:
	  goto L_CALL27;
	}
      goto L_CALL26;
    }
  efp = (struct foo_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct foo_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (n)
    {
      new_esp = esp;
      tmp_arg14 = n - 1;
      while (foo (new_esp, tmp_arg14), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->n = n;
	  efp->call_id = 0;
	  return;
	L_CALL26:
	  ;
	  n = efp->n;
	  tmp_arg14 = efp->tmp_arg14;
	  new_esp = esp + 1;
	}
    }
  else
    {
      new_esp = esp;
      while (modsort (new_esp, aa, nn, mm),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->n = n;
	  efp->call_id = 1;
	  return;
	L_CALL27:
	  ;
	  n = efp->n;
	  new_esp = esp + 1;
	}
    }
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  double ttime;
  struct timeval tp2;
  struct timeval tp1;
  int *a;
  int nfoo;
  int i;
  int m;
  int n;
  char **argv;
  int argc;
};

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  int n;
  int m;
  int i;
  int nfoo;
  int *a;
  double ttime;

lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  n = 30;
  m = 10;
  nfoo = 0;
  if (argc > 1)
    {
      n = atoi (argv[1]);
    }
  if (argc > 2)
    {
      m = atoi (argv[2]);
    }
  if (argc > 3)
    {
      nfoo = atoi (argv[3]);
    }
  a = malloc (sizeof (int) * n);
  {
    i = 0;
    if (i < n)
      do
	{
	  a[i] = i;
	  i++;
	}
      while (i < n);
  }
  aa = a;
  nn = n;
  mm = m;
  if (argc > 4)
    {
      new_esp = esp;
      while (print_array (new_esp, a, n),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->ttime = ttime;
	  efp->a = a;
	  efp->nfoo = nfoo;
	  efp->i = i;
	  efp->m = m;
	  efp->n = n;
	  efp->argv = argv;
	  efp->argc = argc;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
    }
  gettimeofday (&efp->tp1, 0);
  if (nfoo)
    {
      new_esp = esp;
      while (foo (new_esp, nfoo), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->ttime = ttime;
	  efp->a = a;
	  efp->nfoo = nfoo;
	  efp->i = i;
	  efp->m = m;
	  efp->n = n;
	  efp->argv = argv;
	  efp->argc = argc;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
    }
  else
    {
      new_esp = esp;
      while (modsort (new_esp, a, n, m), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->ttime = ttime;
	  efp->a = a;
	  efp->nfoo = nfoo;
	  efp->i = i;
	  efp->m = m;
	  efp->n = n;
	  efp->argv = argv;
	  efp->argc = argc;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
    }
  gettimeofday (&efp->tp2, 0);
  if (argc > 4)
    {
      new_esp = esp;
      while (print_array (new_esp, a, n),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->ttime = ttime;
	  efp->a = a;
	  efp->nfoo = nfoo;
	  efp->i = i;
	  efp->m = m;
	  efp->n = n;
	  efp->argv = argv;
	  efp->argc = argc;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
    }
  ttime =
      (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec -
					       (efp->tp1).tv_usec) * 1.0e-6;
  printf ("%f\n", ttime);
  return 0;
}
