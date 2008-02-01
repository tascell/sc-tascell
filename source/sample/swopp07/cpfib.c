typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);
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
    return 1;
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
  char *new_esp;
  struct save1_in_cpfib_frame *efp;
  struct cpfib_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
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
  {
    char *goto_fr;

    tmp_fp = xfp->scan0;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp;
    efp->xfp = xfp;
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
    xfp = efp->xfp;
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
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int pc;
  int s;
  int tmp3;
  int tmp4;
  int tmp_arg;
  int tmp_arg2;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct cpfib_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct cpfib_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL;
	case 1:
	  goto L_CALL2;
	}
      goto L_CALL;
    }
  efp = (struct cpfib_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct cpfib_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  pc = 0;
  s = 0;
  if (n <= 2)
    {
      return 1;
    }
  else
    {
      pc = 1;
      new_esp = esp;
      tmp_arg = n - 1;
      while ((tmp3 = cpfib (new_esp, &efp->save1, tmp_arg)) == (int) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->tmp4 = tmp4;
	  efp->tmp3 = tmp3;
	  efp->s = s;
	  efp->pc = pc;
	  efp->n = n;
	  efp->scan0 = scan0;
	  efp->save1.fun = save1_in_cpfib;
	  efp->save1.fr = (void *) efp;
	  efp->call_id = 0;
	  return (int) 0 - 1;
	L_CALL:
	  ;
	  tmp4 = efp->tmp4;
	  tmp3 = efp->tmp3;
	  s = efp->s;
	  pc = efp->pc;
	  n = efp->n;
	  scan0 = efp->scan0;
	  tmp_arg = efp->tmp_arg;
	  new_esp = esp + 1;
	}
      s += tmp3;
      pc = 2;
      new_esp = esp;
      tmp_arg2 = n - 2;
      while ((tmp4 = cpfib (new_esp, &efp->save1, tmp_arg2)) == (int) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->tmp4 = tmp4;
	  efp->tmp3 = tmp3;
	  efp->s = s;
	  efp->pc = pc;
	  efp->n = n;
	  efp->scan0 = scan0;
	  efp->save1.fun = save1_in_cpfib;
	  efp->save1.fr = (void *) efp;
	  efp->call_id = 1;
	  return (int) 0 - 1;
	L_CALL2:
	  ;
	  tmp4 = efp->tmp4;
	  tmp3 = efp->tmp3;
	  s = efp->s;
	  pc = efp->pc;
	  n = efp->n;
	  scan0 = efp->scan0;
	  tmp_arg2 = efp->tmp_arg2;
	  new_esp = esp + 1;
	}
      s += tmp4;
      return s;
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
  char *new_esp;
  struct save1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);

lgoto:
  ;
  efp = (struct save1_in_main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct save1_in_main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  return 0;
}

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  int i;
  int tmp5;
  int ifexp_result;

lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (argc > 1)
    {
      tmp5 = atoi (argv[1]);
      ifexp_result = tmp5;
    }
  else
    {
      ifexp_result = 30;
    }
  n_in = ifexp_result;
  printf ("cpfib(%d)\n", n_in);
  gettimeofday (&efp->tp1, 0);
  new_esp = esp;
  while ((i = cpfib (new_esp, &efp->save1, n_in)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->ifexp_result = ifexp_result;
      efp->tmp5 = tmp5;
      efp->i = i;
      efp->argv = argv;
      efp->argc = argc;
      efp->save1.fun = save1_in_main;
      efp->save1.fr = (void *) efp;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  gettimeofday (&efp->tp2, 0);
  printf ("fib(%d)=%d\n", n_in, i);
  printf ("time: %f\n",
	  (efp->tp2).tv_sec - (efp->tp1).tv_sec + ((efp->tp2).tv_usec -
						   (efp->tp1).tv_usec) *
	  1.0e-6);
  return 0;
}
