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

lgoto:
  ;
  efp = (struct swapfunc_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct swapfunc_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->a = a;
  efp->b = b;
  efp->n = n;
  efp->swaptype = swaptype;
  if (efp->swaptype <= 1)
    {
      0;
      if (efp->n > 0)
	do
	  {
	    efp->t = *((WORD *) efp->a);
	    *((WORD *) efp->a) = *((WORD *) efp->b);
	    *((WORD *) efp->b) = efp->t;
	    efp->a += sizeof (WORD);
	    efp->b += sizeof (WORD);
	    efp->n -= sizeof (WORD);
	  }
	while (efp->n > 0);
    }
  else if (efp->n > 0)
    do
      {
	efp->t2 = *efp->a;
	*efp->a = *efp->b;
	*efp->b = efp->t2;
	efp->a += 1;
	efp->b += 1;
	efp->n -= 1;
      }
    while (efp->n > 0);
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
  closure_t *tmp_fp;
  closure_t *tmp_fp2;
  closure_t *tmp_fp3;
  closure_t *tmp_fp4;
  closure_t *tmp_fp5;

lgoto:
  ;
  efp = (struct med3_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct med3_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->a = a;
  efp->b = b;
  efp->c = c;
  efp->cmp = cmp;
  {
    char *goto_fr;

    tmp_fp = efp->cmp;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((char **) argp) = efp->b, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((char **) argp) = efp->a, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp;
    goto_fr = lw_call (argp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	{
	  efp->tmp_esp = goto_fr;
	  return (char *) 0 - 1;
	}
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    efp->tmp = *((int *) argp);
  }
  if (efp->tmp < 0)
    {
      {
	char *goto_fr;

	tmp_fp2 = efp->cmp;
	argp =
	    (char *) ((Align_t *) esp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = efp->c, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = efp->b, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((closure_t **) argp) = tmp_fp2;
	goto_fr = lw_call (argp);
	if (goto_fr)
	  if ((char *) goto_fr < (char *) efp)
	    {
	      efp->tmp_esp = goto_fr;
	      return (char *) 0 - 1;
	    }
	  else
	    {
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	efp->tmp2 = *((int *) argp);
      }
      if (efp->tmp2 < 0)
	{
	  efp->ifexp_result2 = efp->b;
	}
      else
	{
	  {
	    char *goto_fr;

	    tmp_fp3 = efp->cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->c, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->a, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp3;
	    goto_fr = lw_call (argp);
	    if (goto_fr)
	      if ((char *) goto_fr < (char *) efp)
		{
		  efp->tmp_esp = goto_fr;
		  return (char *) 0 - 1;
		}
	      else
		{
		  efp->tmp_esp = 0;
		  goto lgoto;
		}
	    efp->tmp3 = *((int *) argp);
	  }
	  efp->ifexp_result2 = efp->tmp3 < 0 ? efp->c : efp->a;
	}
      efp->ifexp_result5 = efp->ifexp_result2;
    }
  else
    {
      {
	char *goto_fr;

	tmp_fp4 = efp->cmp;
	argp =
	    (char *) ((Align_t *) esp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = efp->c, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) argp) = efp->b, argp =
	    (char *) ((Align_t *) argp +
		      (sizeof (char *) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((closure_t **) argp) = tmp_fp4;
	goto_fr = lw_call (argp);
	if (goto_fr)
	  if ((char *) goto_fr < (char *) efp)
	    {
	      efp->tmp_esp = goto_fr;
	      return (char *) 0 - 1;
	    }
	  else
	    {
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	efp->tmp4 = *((int *) argp);
      }
      if (efp->tmp4 > 0)
	{
	  efp->ifexp_result4 = efp->b;
	}
      else
	{
	  {
	    char *goto_fr;

	    tmp_fp5 = efp->cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->c, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->a, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp5;
	    goto_fr = lw_call (argp);
	    if (goto_fr)
	      if ((char *) goto_fr < (char *) efp)
		{
		  efp->tmp_esp = goto_fr;
		  return (char *) 0 - 1;
		}
	      else
		{
		  efp->tmp_esp = 0;
		  goto lgoto;
		}
	    efp->tmp5 = *((int *) argp);
	  }
	  efp->ifexp_result4 = efp->tmp5 > 0 ? efp->c : efp->a;
	}
      efp->ifexp_result5 = efp->ifexp_result4;
    }
  return efp->ifexp_result5;
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

lgoto:
  ;
  efp = (struct print_array_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct print_array_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->a = a;
  efp->n = n;
  {
    efp->i = 0;
    if (efp->i < efp->n)
      do
	{
	  printf ("%d%c", (efp->a)[efp->i], efp->i + 1 < efp->n ? ' ' : '\n');
	  (efp->i)++;
	}
      while (efp->i < efp->n);
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
  closure_t *tmp_fp6;
  char *tmp_arg;
  closure_t *tmp_fp7;
  char *tmp_arg2;
  char *tmp_arg3;
  char *tmp_arg4;
  char *tmp_arg5;
  char *tmp_arg6;
  char *tmp_arg7;
  closure_t *tmp_fp8;
  closure_t *tmp_fp9;
  closure_t *tmp_fp10;
  closure_t *tmp_fp11;
  char *tmp_arg8;
  char *tmp_arg9;
  long tmp_arg10;
  char *tmp_arg11;
  long tmp_arg12;

lgoto:
  ;
  efp = (struct quicksort_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct quicksort_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->a = a;
  efp->n = n;
  efp->es = es;
  efp->cmp = cmp;
  efp->swaptype =
      (efp->a - (char *) 0 | efp->es) % sizeof (WORD) ? 2 : (efp->es >
							     sizeof (WORD) ? 1 :
							     0);
  if (efp->n < 7)
    {
      efp->pl = efp->a;
      {
	efp->pm = efp->a + efp->es;
	if (efp->pm < efp->a + efp->n * efp->es)
	  do
	    {
	      {
		efp->pl = efp->pm;
		if (efp->pl > efp->a)
		  {
		    {
		      char *goto_fr;

		      tmp_fp6 = efp->cmp;
		      argp =
			  (char *) ((Align_t *) esp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((char **) argp) = efp->pl, argp =
			  (char *) ((Align_t *) argp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((char **) argp) = efp->pl - efp->es, argp =
			  (char *) ((Align_t *) argp +
				    (sizeof (char *) + sizeof (Align_t) +
				     -1) / sizeof (Align_t));
		      *((closure_t **) argp) = tmp_fp6;
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
		      efp->tmp6 = *((int *) argp);
		    }
		    efp->ifexp_result10 = efp->tmp6 > 0 ? 1 : 0;
		  }
		else
		  {
		    efp->ifexp_result10 = 0;
		  }
		if (efp->ifexp_result10)
		  do
		    {
		      if (efp->swaptype != 0)
			{
			  tmp_arg = efp->pl - efp->es;
			  while (swapfunc
				 (esp, efp->pl, tmp_arg, efp->es,
				  efp->swaptype), (efp->tmp_esp =
						   *((char **) esp)) != 0)
			    {
			      if ((char *) efp->tmp_esp < (char *) efp)
				return;
			      efp->tmp_esp = 0;
			      goto lgoto;
			    }
			  0;
			}
		      else
			{
			  efp->t = *((WORD *) efp->pl);
			  *((WORD *) efp->pl) = *((WORD *) (efp->pl - efp->es));
			  (void) (*((WORD *) (efp->pl - efp->es)) = efp->t);
			}
		      0;
		      efp->pl -= efp->es;
		      if (efp->pl > efp->a)
			{
			  {
			    char *goto_fr;

			    tmp_fp7 = efp->cmp;
			    argp =
				(char *) ((Align_t *) esp +
					  (sizeof (char *) + sizeof (Align_t) +
					   -1) / sizeof (Align_t));
			    *((char **) argp) = efp->pl, argp =
				(char *) ((Align_t *) argp +
					  (sizeof (char *) + sizeof (Align_t) +
					   -1) / sizeof (Align_t));
			    *((char **) argp) = efp->pl - efp->es, argp =
				(char *) ((Align_t *) argp +
					  (sizeof (char *) + sizeof (Align_t) +
					   -1) / sizeof (Align_t));
			    *((closure_t **) argp) = tmp_fp7;
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
			    efp->tmp7 = *((int *) argp);
			  }
			  efp->ifexp_result12 = efp->tmp7 > 0 ? 1 : 0;
			}
		      else
			{
			  efp->ifexp_result12 = 0;
			}
		    }
		  while (efp->ifexp_result12);
	      }
	      efp->pm += efp->es;
	    }
	  while (efp->pm < efp->a + efp->n * efp->es);
      }
      return;
    }
  efp->pm = efp->a + efp->n / 2 * efp->es;
  if (efp->n > 7)
    {
      efp->pl = efp->a;
      efp->pn = efp->a + (efp->n - 1) * efp->es;
      if (efp->n > 40)
	{
	  efp->s = efp->n / 8 * efp->es;
	  tmp_arg2 = efp->pl + efp->s;
	  tmp_arg3 = efp->pl + 2 * efp->s;
	  while ((efp->pl =
		  med3 (esp, efp->pl, tmp_arg2, tmp_arg3,
			efp->cmp)) == (char *) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	  tmp_arg4 = efp->pm - efp->s;
	  tmp_arg5 = efp->pm + efp->s;
	  while ((efp->pm =
		  med3 (esp, tmp_arg4, efp->pm, tmp_arg5,
			efp->cmp)) == (char *) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	  tmp_arg6 = efp->pn - 2 * efp->s;
	  tmp_arg7 = efp->pn - efp->s;
	  while ((efp->pn =
		  med3 (esp, tmp_arg6, tmp_arg7, efp->pn,
			efp->cmp)) == (char *) 0 - 1
		 && (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	}
      while ((efp->pm =
	      med3 (esp, efp->pl, efp->pm, efp->pn, efp->cmp)) == (char *) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    }
  if (efp->swaptype != 0)
    {
      efp->pv = efp->a;
      if (efp->swaptype != 0)
	{
	  while (swapfunc (esp, efp->pv, efp->pm, efp->es, efp->swaptype),
		 (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	  0;
	}
      else
	{
	  efp->t = *((WORD *) efp->pv);
	  *((WORD *) efp->pv) = *((WORD *) efp->pm);
	  (void) (*((WORD *) efp->pm) = efp->t);
	}
      0;
    }
  else
    {
      efp->pv = (char *) &efp->v;
      efp->v = *((WORD *) efp->pm);
    }
  efp->pa = efp->pb = efp->a;
  efp->pc = efp->pd = efp->a + (efp->n - 1) * efp->es;
  do
    {
      if (efp->pb <= efp->pc)
	{
	  {
	    char *goto_fr;

	    tmp_fp8 = efp->cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->pv, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->pb, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp8;
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
	    efp->tmp10 = *((int *) argp);
	  }
	  efp->ifexp_result14 = (efp->r = efp->tmp10) <= 0 ? 1 : 0;
	}
      else
	{
	  efp->ifexp_result14 = 0;
	}
      if (efp->ifexp_result14)
	do
	  {
	    if (efp->r == 0)
	      {
		if (efp->swaptype != 0)
		  {
		    while (swapfunc
			   (esp, efp->pa, efp->pb, efp->es, efp->swaptype),
			   (efp->tmp_esp = *((char **) esp)) != 0)
		      {
			if ((char *) efp->tmp_esp < (char *) efp)
			  return;
			efp->tmp_esp = 0;
			goto lgoto;
		      }
		    0;
		  }
		else
		  {
		    efp->t = *((WORD *) efp->pa);
		    *((WORD *) efp->pa) = *((WORD *) efp->pb);
		    (void) (*((WORD *) efp->pb) = efp->t);
		  }
		0;
		efp->pa += efp->es;
	      }
	    efp->pb += efp->es;
	    if (efp->pb <= efp->pc)
	      {
		{
		  char *goto_fr;

		  tmp_fp9 = efp->cmp;
		  argp =
		      (char *) ((Align_t *) esp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = efp->pv, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = efp->pb, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((closure_t **) argp) = tmp_fp9;
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
		  efp->tmp11 = *((int *) argp);
		}
		efp->ifexp_result16 = (efp->r = efp->tmp11) <= 0 ? 1 : 0;
	      }
	    else
	      {
		efp->ifexp_result16 = 0;
	      }
	  }
	while (efp->ifexp_result16);
      if (efp->pc >= efp->pb)
	{
	  {
	    char *goto_fr;

	    tmp_fp10 = efp->cmp;
	    argp =
		(char *) ((Align_t *) esp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->pv, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((char **) argp) = efp->pc, argp =
		(char *) ((Align_t *) argp +
			  (sizeof (char *) + sizeof (Align_t) +
			   -1) / sizeof (Align_t));
	    *((closure_t **) argp) = tmp_fp10;
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
	    efp->tmp13 = *((int *) argp);
	  }
	  efp->ifexp_result18 = (efp->r = efp->tmp13) >= 0 ? 1 : 0;
	}
      else
	{
	  efp->ifexp_result18 = 0;
	}
      if (efp->ifexp_result18)
	do
	  {
	    if (efp->r == 0)
	      {
		if (efp->swaptype != 0)
		  {
		    while (swapfunc
			   (esp, efp->pc, efp->pd, efp->es, efp->swaptype),
			   (efp->tmp_esp = *((char **) esp)) != 0)
		      {
			if ((char *) efp->tmp_esp < (char *) efp)
			  return;
			efp->tmp_esp = 0;
			goto lgoto;
		      }
		    0;
		  }
		else
		  {
		    efp->t = *((WORD *) efp->pc);
		    *((WORD *) efp->pc) = *((WORD *) efp->pd);
		    (void) (*((WORD *) efp->pd) = efp->t);
		  }
		0;
		efp->pd -= efp->es;
	      }
	    efp->pc -= efp->es;
	    if (efp->pc >= efp->pb)
	      {
		{
		  char *goto_fr;

		  tmp_fp11 = efp->cmp;
		  argp =
		      (char *) ((Align_t *) esp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = efp->pv, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((char **) argp) = efp->pc, argp =
		      (char *) ((Align_t *) argp +
				(sizeof (char *) + sizeof (Align_t) +
				 -1) / sizeof (Align_t));
		  *((closure_t **) argp) = tmp_fp11;
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
		  efp->tmp14 = *((int *) argp);
		}
		efp->ifexp_result20 = (efp->r = efp->tmp14) >= 0 ? 1 : 0;
	      }
	    else
	      {
		efp->ifexp_result20 = 0;
	      }
	  }
	while (efp->ifexp_result20);
      if (efp->pb > efp->pc)
	{
	  break;
	}
      if (efp->swaptype != 0)
	{
	  while (swapfunc (esp, efp->pb, efp->pc, efp->es, efp->swaptype),
		 (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      if ((char *) efp->tmp_esp < (char *) efp)
		return;
	      efp->tmp_esp = 0;
	      goto lgoto;
	    }
	  0;
	}
      else
	{
	  efp->t = *((WORD *) efp->pb);
	  *((WORD *) efp->pb) = *((WORD *) efp->pc);
	  (void) (*((WORD *) efp->pc) = efp->t);
	}
      0;
      efp->pb += efp->es;
      efp->pc -= efp->es;
    }
  while (1);
  efp->pn = efp->a + efp->n * efp->es;
  efp->s =
      efp->pa - efp->a <
      efp->pb - efp->pa ? efp->pa - efp->a : efp->pb - efp->pa;
  if (efp->s > 0)
    {
      tmp_arg8 = efp->pb - efp->s;
      while (swapfunc (esp, efp->a, tmp_arg8, efp->s, efp->swaptype),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    }
  efp->s =
      efp->pd - efp->pc <
      efp->pn - efp->pd - efp->es ? efp->pd - efp->pc : efp->pn - efp->pd -
      efp->es;
  if (efp->s > 0)
    {
      tmp_arg9 = efp->pn - efp->s;
      while (swapfunc (esp, efp->pb, tmp_arg9, efp->s, efp->swaptype),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    }
  if ((efp->s = efp->pb - efp->pa) > efp->es)
    {
      tmp_arg10 = efp->s / efp->es;
      while (quicksort (esp, efp->a, tmp_arg10, efp->es, efp->cmp),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    }
  if ((efp->s = efp->pd - efp->pc) > efp->es)
    {
      tmp_arg11 = efp->pn - efp->s;
      tmp_arg12 = efp->s / efp->es;
      while (quicksort (esp, tmp_arg11, tmp_arg12, efp->es, efp->cmp),
	     (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
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
  struct comp_mod_in_modsort_frame *efp;
  struct modsort_frame *xfp = xfp0;
  char *parmp = (char *) esp;

lgoto:
  ;
  efp = (struct comp_mod_in_modsort_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct comp_mod_in_modsort_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  efp->pp = (parmp =
	     (char *) ((Align_t *) parmp -
		       (sizeof (int *) + sizeof (Align_t) +
			-1) / sizeof (Align_t)), *((int **) parmp));
  efp->pq = (parmp =
	     (char *) ((Align_t *) parmp -
		       (sizeof (int *) + sizeof (Align_t) +
			-1) / sizeof (Align_t)), *((int **) parmp));
  *((int *) efp) =
      *efp->pp % xfp->m < *efp->pq % xfp->m ? 1 : (*efp->pp % xfp->m ==
						   *efp->pq % xfp->m ? 0 : -1);
  return 0;
  return 0;
}

void
modsort (char *esp, int *a, int n, int m)
{
  struct modsort_frame *efp;
  int tmp_arg13;

lgoto:
  ;
  efp = (struct modsort_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct modsort_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->a = a;
  efp->n = n;
  efp->m = m;
  efp->comp_mod.fun = comp_mod_in_modsort;
  efp->comp_mod.fr = (void *) efp;
  tmp_arg13 = sizeof (int);
  while (quicksort (esp, efp->a, efp->n, tmp_arg13, &efp->comp_mod),
	 (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return;
      efp->tmp_esp = 0;
      goto lgoto;
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
  int tmp_arg14;

lgoto:
  ;
  efp = (struct foo_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct foo_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->n = n;
  if (efp->n)
    {
      tmp_arg14 = efp->n - 1;
      while (foo (esp, tmp_arg14), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
    }
  else
    while (modsort (esp, aa, nn, mm), (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return;
	efp->tmp_esp = 0;
	goto lgoto;
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
  char estack[65536];
  char *esp = estack;

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
  efp->n = 30;
  efp->m = 10;
  efp->nfoo = 0;
  if (efp->argc > 1)
    {
      efp->n = atoi ((efp->argv)[1]);
    }
  if (efp->argc > 2)
    {
      efp->m = atoi ((efp->argv)[2]);
    }
  if (efp->argc > 3)
    {
      efp->nfoo = atoi ((efp->argv)[3]);
    }
  efp->a = malloc (sizeof (int) * efp->n);
  {
    efp->i = 0;
    if (efp->i < efp->n)
      do
	{
	  (efp->a)[efp->i] = efp->i;
	  (efp->i)++;
	}
      while (efp->i < efp->n);
  }
  aa = efp->a;
  nn = efp->n;
  mm = efp->m;
  if (efp->argc > 4)
    while (print_array (esp, efp->a, efp->n),
	   (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return (int) 0 - 1;
	efp->tmp_esp = 0;
	goto lgoto;
      }
  gettimeofday (&efp->tp1, 0);
  if (efp->nfoo)
    while (foo (esp, efp->nfoo), (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return (int) 0 - 1;
	efp->tmp_esp = 0;
	goto lgoto;
      }
  else
    while (modsort (esp, efp->a, efp->n, efp->m),
	   (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return (int) 0 - 1;
	efp->tmp_esp = 0;
	goto lgoto;
      }
  gettimeofday (&efp->tp2, 0);
  if (efp->argc > 4)
    while (print_array (esp, efp->a, efp->n),
	   (efp->tmp_esp = *((char **) esp)) != 0)
      {
	if ((char *) efp->tmp_esp < (char *) efp)
	  return (int) 0 - 1;
	efp->tmp_esp = 0;
	goto lgoto;
      }
  efp->ttime =
      (efp->tp2).
     tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec -
					       (efp->tp1).tv_usec) * 1.0e-6;
  printf ("time: %f\n", efp->ttime);
  return 0;
}
