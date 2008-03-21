typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);
typedef long size_t;

struct h_frame3
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int tmp1;
  int tmp0;
  closure_t *g;
  int i;
};

int
h (char *esp, int i, closure_t * g)
{
  char *argp;
  struct h_frame3 *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int tmp0;
  int tmp1;
  closure_t *tmp_fp4;
  closure_t *tmp_fp6;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct h_frame3 *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct h_frame3) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL5;
	case 1:
	  goto L_CALL7;
	}
      goto L_CALL5;
    }
  efp = (struct h_frame3 *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct h_frame3) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  {
    tmp_fp4 = g;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((int *) argp) = i, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (int) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp4;
    efp->tmp1 = tmp1;
    efp->tmp0 = tmp0;
    efp->g = g;
    efp->i = i;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return (int) 0 - 1;
  L_CALL5:
    ;
    tmp1 = efp->tmp1;
    tmp0 = efp->tmp0;
    g = efp->g;
    i = efp->i;
    tmp0 = *((int *) efp->argp);
  }
  {
    tmp_fp6 = g;
    argp =
	(char *) ((Align_t *) esp +
		  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((int *) argp) = tmp0, argp =
	(char *) ((Align_t *) argp +
		  (sizeof (int) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = tmp_fp6;
    efp->tmp1 = tmp1;
    efp->tmp0 = tmp0;
    efp->g = g;
    efp->i = i;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 1;
    return (int) 0 - 1;
  L_CALL7:
    ;
    tmp1 = efp->tmp1;
    tmp0 = efp->tmp0;
    g = efp->g;
    i = efp->i;
    tmp1 = *((int *) efp->argp);
  }
  return tmp1;
}

struct foo_frame8
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int (*tmp_fp11) ();
  int y;
  int x;
  int a;
  closure_t *tmp_arg12;
  closure_t g1;
};

struct g1_in_foo9_frame10
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int b;
  struct foo_frame8 *xfp;
};

char *
g1_in_foo9 (char *esp, void *xfp0)
{
  char *new_esp;
  struct g1_in_foo9_frame10 *efp;
  struct foo_frame8 *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
  int b = (parmp =
	   (char *) ((Align_t *) parmp -
		     (sizeof (int) + sizeof (Align_t) + -1) / sizeof (Align_t)),
	   *((int *) parmp));
LGOTO:
  ;
  efp = (struct g1_in_foo9_frame10 *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct g1_in_foo9_frame10) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  (xfp->x)++;
  *((int *) efp) = xfp->a + b;
  return 0;
  return 0;
}

int
foo (char *esp, int a)
{
  struct foo_frame8 *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int x;
  int y;
  int (*tmp_fp11) ();
  closure_t *tmp_arg12;

  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct foo_frame8 *) esp;
      esp =
	  (char *) ((Align_t *) esp +
		    (sizeof (struct foo_frame8) + sizeof (Align_t) +
		     -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:
      switch ((*efp).call_id)
	{
	case 0:
	  goto L_CALL13;
	}
      goto L_CALL13;
    }
  efp = (struct foo_frame8 *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct foo_frame8) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  x = 0;
  y = 0;
  new_esp = esp;
  tmp_fp11 = h;
  tmp_arg12 = &efp->g1;
  while ((y = tmp_fp11 (new_esp, 10, tmp_arg12)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      efp->tmp_fp11 = tmp_fp11;
      efp->y = y;
      efp->x = x;
      efp->a = a;
      efp->g1.fun = g1_in_foo9;
      efp->g1.fr = (void *) efp;
      efp->call_id = 0;
      return (int) 0 - 1;
    L_CALL13:
      ;
      tmp_fp11 = efp->tmp_fp11;
      y = efp->y;
      x = efp->x;
      a = efp->a;
      new_esp = esp + 1;
    }
  return x + y;
}

struct main_frame14
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int (*tmp_fp15) ();
  int tmp2;
};

int
main ()
{
  struct main_frame14 *efp;
  char *new_esp;
  char estack[65536];
  char *esp = estack;
  int tmp2;
  int (*tmp_fp15) ();

LGOTO:
  ;
  efp = (struct main_frame14 *) esp;
  esp =
      (char *) ((Align_t *) esp +
		(sizeof (struct main_frame14) + sizeof (Align_t) +
		 -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  new_esp = esp;
  tmp_fp15 = foo;
  while ((tmp2 = tmp_fp15 (new_esp, 1)) == (int) 0 - 1
	 && (efp->tmp_esp = *((char **) esp)) != 0)
    {
      char *goto_fr;

      *((char **) esp) = 0;
      efp->tmp_fp15 = tmp_fp15;
      efp->tmp2 = tmp2;
      goto_fr = lw_call (efp->tmp_esp);
      if ((char *) goto_fr == (char *) efp)
	goto LGOTO;
      tmp_fp15 = efp->tmp_fp15;
      tmp2 = efp->tmp2;
      new_esp = esp + 1;
    }
  return tmp2;
}
