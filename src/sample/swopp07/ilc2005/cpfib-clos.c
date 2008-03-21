typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *
lw_call (char *esp)
{
  closure_t *clos = *((closure_t **) esp);

  return (clos->fun) (esp, clos->fr);
}
typedef unsigned int size_t;

int printf (char const *format, ...);

int atoi (char const *nptr);

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

void
save_int (int i)
{
  printf ("int: %d\n", i);
}

void
save_pc (int pc)
{
  printf ("pc: %d\n", pc);
}

int n_in;

int
fib (int n)
{
  int s;

  ;
  if (n <= 2)
    {
      return 1;
    }
  else
    {
      int tmp;
      int tmp2;

      s = 0;
      tmp = fib (n - 1);
      s += tmp;
      tmp2 = fib (n - 2);
      s += tmp2;
      return s;
    }
}
int done = 0;

struct cpfib_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp4;
  int tmp3;
  int s;
  int pc;
  int n;
  closure_t *scan0;
  int tmp_arg2;
  int tmp_arg;
  closure_t save1;
};

struct save1_in_cpfib_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct cpfib_frame *xfp;
};

char *
save1_in_cpfib (char *esp, void *xfp0)
{
  struct save1_in_cpfib_frame *efp;
  struct cpfib_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;
  closure_t *tmp_fp;

lgoto:
  ;
  efp = (struct save1_in_cpfib_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct save1_in_cpfib_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  {
    char *goto_fr;

    tmp_fp = xfp->scan0;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp;
    efp->argp = argp;
    efp->tmp_esp = argp;
    goto_fr = lw_call (efp->tmp_esp);
    if (goto_fr)
      if ((char *) goto_fr < (char *) efp)
	return goto_fr;
      else
	{
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
  }
  save_pc (xfp->pc);
  save_int (xfp->s);
  save_int (xfp->n);
  return 0;
}

int
cpfib (char *esp, closure_t * scan0, int n)
{
  struct cpfib_frame *efp;
  int tmp_arg;
  int tmp_arg2;

lgoto:
  ;
  efp = (struct cpfib_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct cpfib_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->scan0 = scan0;
  efp->n = n;
  efp->save1.fun = save1_in_cpfib;
  efp->save1.fr = (void *) efp;
  efp->pc = 0;
  efp->s = 0;
  if (efp->n <= 2)
    {
      return 1;
    }
  else
    {
      efp->pc = 1;
      tmp_arg = efp->n - 1;
      while ((efp->tmp3 = cpfib (esp, &efp->save1, tmp_arg)) == (int) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return (int) 0 - 1;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
      efp->s += efp->tmp3;
      efp->pc = 2;
      tmp_arg2 = efp->n - 2;
      while ((efp->tmp4 = cpfib (esp, &efp->save1, tmp_arg2)) == (int) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  if ((char *) efp->tmp_esp < (char *) efp)
	    return (int) 0 - 1;
	  efp->tmp_esp = 0;
	  goto lgoto;
	}
      efp->s += efp->tmp4;
      return efp->s;
    }
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int ifexp_result;
  int tmp5;
  int i;
  struct timeval tp2;
  struct timeval tp1;
  char **argv;
  int argc;
  closure_t save1;
};

struct save1_in_main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct main_frame *xfp;
};

char *
save1_in_main (char *esp, void *xfp0)
{
  struct save1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  char *parmp = (char *) esp;

lgoto:
  ;
  efp = (struct save1_in_main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct save1_in_main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  return 0;
}

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
  efp->save1.fun = save1_in_main;
  efp->save1.fr = (void *) efp;
  if (efp->argc > 1)
    {
      efp->tmp5 = atoi ((efp->argv)[1]);
      efp->ifexp_result = efp->tmp5;
    }
  else
    {
      efp->ifexp_result = 30;
    }
  n_in = efp->ifexp_result;
  printf ("cpfig(%d)\n", n_in);
  gettimeofday (&efp->tp1, 0);
  while ((efp->i = cpfib (esp, &efp->save1, n_in)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      if ((char *) efp->tmp_esp < (char *) efp)
	return (int) 0 - 1;
      efp->tmp_esp = 0;
      goto lgoto;
    }
  gettimeofday (&efp->tp2, 0);
  printf ("fib(%d)=%d\n", n_in, efp->i);
  printf ("%f\n",
	  (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec -
						   (efp->tp1).tv_usec) *
	  1.0e-6);
  return 0;
}
