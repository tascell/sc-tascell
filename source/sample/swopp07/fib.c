typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);
int count = 0;
int scan_inc = 3;
int scan_start = 10;

int printf (char const *, ...);

int atoi (char const *);

struct fib_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp4;
  int tmp2;
  void (*scan1) (char *, void);
  int n;
  void (*scan0) (void);
  int tmp_arg2;
  int tmp_arg;
};

struct scan1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
};

int
fib (char *esp, void (*scan0) (void), int n)
{
  struct fib_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;

  void scan1 (char *esp, void)
  {
    struct scan1_frame *efp;
    char *new_esp;
    size_t esp_flag = (size_t) esp & 3;

    if (esp_flag)
      {
	esp = (char *) ((size_t) esp ^ esp_flag);
	efp = (struct scan1_frame *) esp;
	esp =
	    (char *) ((Align_t *) esp +
		      (sizeof (struct scan1_frame) + sizeof (Align_t) +
		       -1) / sizeof (Align_t));
	*((char **) esp) = 0;
      lgoto:
	switch ((*efp).call_id)
	  {
	  case 0:
	    goto L_CALL;
	  }
	goto L_CALL;
      }
    efp = (struct scan1_frame *) esp;
    esp =
	(char *) ((Align_t *) esp +
		  (sizeof (struct scan1_frame) + sizeof (Align_t) +
		   -1) / sizeof (Align_t));
    *((char **) esp) = 0;
    printf ("<fib ");
    printf ("n=0x%x ", (int) n);
    printf (">\n");
    new_esp = esp;
    while (scan0 (new_esp), (efp->tmp_esp = *((char **) esp)) != 0)
      {
	efp->call_id = 0;
	return;
      L_CALL:
	;
	new_esp = esp + 1;
      }
  }
  int tmp2;
  int tmp4;
  int tmp_arg;
  int tmp_arg2;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct fib_frame *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct fib_frame) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    lgoto:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL2;
	case 1:
	  goto L_CALL3;
	case 2:
	  goto L_CALL4;
	case 3:
	  goto L_CALL5;
	}
      goto L_CALL2;
    }
  efp = (struct fib_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct fib_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (n <= 2)
    {
      return 1;
    }
  else
    {
      if ((count += scan_inc) > scan_start)
	{
	  count -= scan_start;
	  new_esp = esp;
	  while (scan1 (new_esp), (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->tmp4 = tmp4;
	      efp->tmp2 = tmp2;
	      efp->scan1 = scan1;
	      efp->n = n;
	      efp->scan0 = scan0;
	      efp->call_id = 0;
	      return (int) 0 - 1;
	    L_CALL2:
	      ;
	      tmp4 = efp->tmp4;
	      tmp2 = efp->tmp2;
	      scan1 = efp->scan1;
	      n = efp->n;
	      scan0 = efp->scan0;
	      new_esp = esp + 1;
	    }
	  0;
	}
      else
	{
	  0;
	}
      0;
      new_esp = esp;
      tmp_arg = n - 2;
      while ((tmp2 = fib (new_esp, scan1, tmp_arg)) == (int) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->tmp4 = tmp4;
	  efp->tmp2 = tmp2;
	  efp->scan1 = scan1;
	  efp->n = n;
	  efp->scan0 = scan0;
	  efp->call_id = 1;
	  return (int) 0 - 1;
	L_CALL3:
	  ;
	  tmp4 = efp->tmp4;
	  tmp2 = efp->tmp2;
	  scan1 = efp->scan1;
	  n = efp->n;
	  scan0 = efp->scan0;
	  tmp_arg = efp->tmp_arg;
	  new_esp = esp + 1;
	}
      if ((count += scan_inc) > scan_start)
	{
	  count -= scan_start;
	  new_esp = esp;
	  while (scan1 (new_esp), (efp->tmp_esp = *((char **) esp)) != 0)
	    {
	      efp->tmp4 = tmp4;
	      efp->tmp2 = tmp2;
	      efp->scan1 = scan1;
	      efp->n = n;
	      efp->scan0 = scan0;
	      efp->call_id = 2;
	      return (int) 0 - 1;
	    L_CALL4:
	      ;
	      tmp4 = efp->tmp4;
	      tmp2 = efp->tmp2;
	      scan1 = efp->scan1;
	      n = efp->n;
	      scan0 = efp->scan0;
	      new_esp = esp + 1;
	    }
	  0;
	}
      else
	{
	  0;
	}
      0;
      new_esp = esp;
      tmp_arg2 = n - 1;
      while ((tmp4 = fib (new_esp, scan1, tmp_arg2)) == (int) 0 - 1
	     && (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  efp->tmp4 = tmp4;
	  efp->tmp2 = tmp2;
	  efp->scan1 = scan1;
	  efp->n = n;
	  efp->scan0 = scan0;
	  efp->call_id = 3;
	  return (int) 0 - 1;
	L_CALL5:
	  ;
	  tmp4 = efp->tmp4;
	  tmp2 = efp->tmp2;
	  scan1 = efp->scan1;
	  n = efp->n;
	  scan0 = efp->scan0;
	  tmp_arg2 = efp->tmp_arg2;
	  new_esp = esp + 1;
	}
      return tmp2 + tmp4;
    }
}

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp6;
  void (*scan1) (char *, void);
  int a;
  char **argv;
  int argc;
};

struct scan1_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
};

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  int a;

  void scan1 (char *esp, void)
  {
    struct scan1_frame *efp;
    char *new_esp;
    size_t esp_flag = (size_t) esp & 3;

  lgoto:
    ;
    efp = (struct scan1_frame *) esp;
    esp =
	(char *) ((Align_t *) esp +
		  (sizeof (struct scan1_frame) + sizeof (Align_t) +
		   -1) / sizeof (Align_t));
    *((char **) esp) = 0;
    printf ("<main ");
    printf ("argv=0x%x ", (int) argv);
    printf ("argc=0x%x ", (int) argc);
    printf (">\n");
    printf ("a=0x%x\n", (int) a);
    {
      printf ("\n");
      return;
    }
  }
  int tmp6;

lgoto:
  ;
  efp = (struct main_frame *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  a = 22;
  if (argc > 1)
    {
      a = atoi (argv[1]);
    }
  if ((count += scan_inc) > scan_start)
    {
      count -= scan_start;
      new_esp = esp;
      while (scan1 (new_esp), (efp->tmp_esp = *((char **) esp)) != 0)
	{
	  char *goto_fr;

	  *((char **) esp) = 0;
	  efp->tmp6 = tmp6;
	  efp->scan1 = scan1;
	  efp->a = a;
	  efp->argv = argv;
	  efp->argc = argc;
	  goto_fr = lw_call (efp->tmp_esp);
	  if ((char *) goto_fr == (char *) efp)
	    goto lgoto;
	  new_esp = esp + 1;
	}
      0;
    }
  else
    {
      0;
    }
  0;
  new_esp = esp;
  while ((tmp6 = fib (new_esp, scan1, a)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp6 = tmp6;
      efp->scan1 = scan1;
      efp->a = a;
      efp->argv = argv;
      efp->argc = argc;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto lgoto;
      new_esp = esp + 1;
    }
  printf ("fib(%d) = %d\n", a, tmp6);
  return 0;
}
