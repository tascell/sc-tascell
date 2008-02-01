#include<stdio.h>
typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double align_t;

char *
lw_call (char *esp)
{
  closure_t *clos = *((closure_t **) esp);

  return (clos->fun) (esp, clos->fr);
}

struct square_sum_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp4;
  closure_t *square_sum_t;
  int limit;
  int n;
  closure_t square_sum_t0;
};

struct square_sum_t_in_square_sum_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp3;
  int tmp2;
  closure_t *square_t;
  int acc;
  int n;
  struct square_sum_frame *xfp;
  closure_t square_t0;
};

struct square_t_in_square_sum_t_in_square_sum_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp;
  int acc;
  int n;
  struct square_sum_t_in_square_sum_frame *xfp;
};

char *
square_t_in_square_sum_t_in_square_sum (char *esp, void *xfp0)
{
  char *new_esp;
  struct square_t_in_square_sum_t_in_square_sum_frame *efp;
  struct square_sum_t_in_square_sum_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  int n = (parmp =
	   (char *) ((align_t *) parmp -
		     (sizeof (int) + sizeof (align_t) + -1) / sizeof (align_t)),
	   *((int *) parmp));
  int acc = (parmp =
	     (char *) ((align_t *) parmp -
		       (sizeof (int) + sizeof (align_t) +
			-1) / sizeof (align_t)), *((int *) parmp));
  char *argp;
  int tmp;

LGOTO:
  ;
  efp = (struct square_t_in_square_sum_t_in_square_sum_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct square_t_in_square_sum_t_in_square_sum_frame) +
		 sizeof (align_t) + -1) / sizeof (align_t));
  *((char **) esp) = 0;
  if (n == 0)
    {
      {
	*((int *) efp) = acc;
	return 0;
      }
    }
  else
    {
      {
	if (acc > xfp->xfp->limit)
	  {
	    {
	      xfp->xfp->call_id = -1;
	      return (char *) xfp->xfp;
	    }
	  }
	else
	  {
	    {
	      closure_t *tmp_fp;

	      {
		char *goto_fr;

		tmp_fp = xfp->square_t;
		argp =
		    (char *) ((align_t *) esp +
			      (sizeof (char *) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((int *) argp) = acc + -1 + 2 * n, argp =
		    (char *) ((align_t *) argp +
			      (sizeof (int) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((int *) argp) = n - 1, argp =
		    (char *) ((align_t *) argp +
			      (sizeof (int) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((closure_t **) argp) = tmp_fp;
		efp->tmp = tmp;
		efp->acc = acc;
		efp->n = n;
		efp->xfp = xfp;
		efp->argp = argp;
		efp->tmp_esp = argp;
		goto_fr = lw_call (efp->tmp_esp);
		if (goto_fr)
		  if ((char *) goto_fr < (char *) efp)
		    return goto_fr;
		  else if ((char *) goto_fr == (char *) efp)
		    goto LGOTO;
		tmp = efp->tmp;
		acc = efp->acc;
		n = efp->n;
		xfp = efp->xfp;
		tmp = *((int *) efp->argp);
	      }
	      *((int *) efp) = tmp;
	      return 0;
	    }
	  }
      }
    }
  return 0;
}

char *
square_sum_t_in_square_sum (char *esp, void *xfp0)
{
  char *new_esp;
  struct square_sum_t_in_square_sum_frame *efp;
  struct square_sum_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  int n = (parmp =
	   (char *) ((align_t *) parmp -
		     (sizeof (int) + sizeof (align_t) + -1) / sizeof (align_t)),
	   *((int *) parmp));
  int acc = (parmp =
	     (char *) ((align_t *) parmp -
		       (sizeof (int) + sizeof (align_t) +
			-1) / sizeof (align_t)), *((int *) parmp));
  char *argp;
  closure_t *square_t =
      &((struct square_sum_t_in_square_sum_frame *) esp)->square_t0;
  int tmp2;
  int tmp3;

LGOTO:
  ;
  efp = (struct square_sum_t_in_square_sum_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct square_sum_t_in_square_sum_frame) +
		 sizeof (align_t) + -1) / sizeof (align_t));
  *((char **) esp) = 0;
  if (n == 0)
    {
      {
	*((int *) efp) = acc;
	return 0;
      }
    }
  else
    {
      {
	if (acc > xfp->limit)
	  {
	    {
	      xfp->call_id = -1;
	      return (char *) xfp;
	    }
	  }
	else
	  {
	    {
	      closure_t *tmp_fp2;
	      closure_t *tmp_fp3;

	      {
		char *goto_fr;

		tmp_fp2 = square_t;
		argp =
		    (char *) ((align_t *) esp +
			      (sizeof (char *) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((int *) argp) = 0, argp =
		    (char *) ((align_t *) argp +
			      (sizeof (int) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((int *) argp) = n, argp =
		    (char *) ((align_t *) argp +
			      (sizeof (int) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((closure_t **) argp) = tmp_fp2;
		efp->square_t0.fun = square_t_in_square_sum_t_in_square_sum;
		efp->square_t0.fr = (void *) efp;
		efp->tmp3 = tmp3;
		efp->tmp2 = tmp2;
		efp->square_t = square_t;
		efp->acc = acc;
		efp->n = n;
		efp->xfp = xfp;
		efp->argp = argp;
		efp->tmp_esp = argp;
		goto_fr = lw_call (efp->tmp_esp);
		if (goto_fr)
		  if ((char *) goto_fr < (char *) efp)
		    return goto_fr;
		  else if ((char *) goto_fr == (char *) efp)
		    goto LGOTO;
		tmp3 = efp->tmp3;
		tmp2 = efp->tmp2;
		square_t = efp->square_t;
		acc = efp->acc;
		n = efp->n;
		xfp = efp->xfp;
		tmp2 = *((int *) efp->argp);
	      }
	      {
		char *goto_fr;

		tmp_fp3 = xfp->square_sum_t;
		argp =
		    (char *) ((align_t *) esp +
			      (sizeof (char *) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((int *) argp) = acc + tmp2, argp =
		    (char *) ((align_t *) argp +
			      (sizeof (int) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((int *) argp) = n - 1, argp =
		    (char *) ((align_t *) argp +
			      (sizeof (int) + sizeof (align_t) +
			       -1) / sizeof (align_t));
		*((closure_t **) argp) = tmp_fp3;
		efp->square_t0.fun = square_t_in_square_sum_t_in_square_sum;
		efp->square_t0.fr = (void *) efp;
		efp->tmp3 = tmp3;
		efp->tmp2 = tmp2;
		efp->square_t = square_t;
		efp->acc = acc;
		efp->n = n;
		efp->xfp = xfp;
		efp->argp = argp;
		efp->tmp_esp = argp;
		goto_fr = lw_call (efp->tmp_esp);
		if (goto_fr)
		  if ((char *) goto_fr < (char *) efp)
		    return goto_fr;
		  else if ((char *) goto_fr == (char *) efp)
		    goto LGOTO;
		tmp3 = efp->tmp3;
		tmp2 = efp->tmp2;
		square_t = efp->square_t;
		acc = efp->acc;
		n = efp->n;
		xfp = efp->xfp;
		tmp3 = *((int *) efp->argp);
	      }
	      *((int *) efp) = tmp3;
	      return 0;
	    }
	  }
      }
    }
  return 0;
}

int
square_sum (char *esp, int n)
{
  char *argp;
  struct square_sum_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  static int limit = 8192;
  closure_t *square_sum_t = &((struct square_sum_frame *) esp)->square_sum_t0;
  int tmp4;
  closure_t *tmp_fp4;

  if (esp_flag)
    {
      (size_t) esp ^= esp_flag;
      efp = (struct square_sum_frame *) esp;
      esp =
	  (char *) ((align_t *) esp +
		    (sizeof (struct square_sum_frame) + sizeof (align_t) +
		     -1) / sizeof (align_t));
      *((char **) esp) = 0;
    LGOTO:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_call;
	case -1:
	  tmp4 = efp->tmp4;
	  square_sum_t = efp->square_sum_t;
	  limit = efp->limit;
	  n = efp->n;
	  goto O_FLOW;
	}
      goto L_call;
    }
  efp = (struct square_sum_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct square_sum_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  {
    tmp_fp4 = square_sum_t;
    argp =
	(char *) ((align_t *) esp +
		  (sizeof (char *) + sizeof (align_t) + -1) / sizeof (align_t));
    *((int *) argp) = 0, argp =
	(char *) ((align_t *) argp +
		  (sizeof (int) + sizeof (align_t) + -1) / sizeof (align_t));
    *((int *) argp) = n, argp =
	(char *) ((align_t *) argp +
		  (sizeof (int) + sizeof (align_t) + -1) / sizeof (align_t));
    *((closure_t **) argp) = tmp_fp4;
    efp->square_sum_t0.fun = square_sum_t_in_square_sum;
    efp->square_sum_t0.fr = (void *) efp;
    efp->tmp4 = tmp4;
    efp->square_sum_t = square_sum_t;
    efp->limit = limit;
    efp->n = n;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return (int) 0 - 1;
  L_call:
    ;
    tmp4 = efp->tmp4;
    square_sum_t = efp->square_sum_t;
    limit = efp->limit;
    n = efp->n;
    tmp4 = *((int *) efp->argp);
  }
  return tmp4;
O_FLOW:
  ;
  return -1;
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp5;
};

int
main ()
{
  struct main_frame *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  int tmp5;

LGOTO:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((align_t *) esp +
		(sizeof (struct main_frame) + sizeof (align_t) +
		 -1) / sizeof (align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  while ((tmp5 = square_sum (new_esp, 100)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp5 = tmp5;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto LGOTO;
      new_esp = esp + 1;
    }
  return tmp5;
}
