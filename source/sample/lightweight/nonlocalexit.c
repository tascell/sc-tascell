typedef char   *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t	  fun;
  void           *fr;
}		closure_t;
typedef unsigned int size_t;
typedef double	Align_t;

char           *lw_call(char *esp);

struct func_arg {
  void           *(*func) (char *, void *);
  void           *arg;
};

void           *thread_origin(void *farg);

int		printf     (char const *,...);

struct square_sum_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp4;
  int		  limit;
  int            *pflag;
  int		  n;
  closure_t	  square_sum_t;
};

struct square_sum_t_in_square_sum_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp3;
  int		  tmp2;
  int		  acc;
  int		  n;
  struct square_sum_frame *xfp;
  closure_t	  square_t;
};

struct square_t_in_square_sum_t_in_square_sum_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp;
  int		  acc;
  int		  n;
  struct square_sum_t_in_square_sum_frame *xfp;
};

char           *square_t_in_square_sum_t_in_square_sum(char *esp, void *xfp0)
{
  char           *new_esp;
  struct square_t_in_square_sum_t_in_square_sum_frame *efp;
  struct square_sum_t_in_square_sum_frame *xfp = xfp0;
  size_t	  esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  int		  n = (parmp = (char *)((Align_t *) parmp - (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t)), *((int *)parmp));
  int		  acc = (parmp = (char *)((Align_t *) parmp - (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t)), *((int *)parmp));
  char           *argp;
  int		  tmp;

LGOTO:
  ;
  efp = (struct square_t_in_square_sum_t_in_square_sum_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct square_t_in_square_sum_t_in_square_sum_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (n == 0) {
    *((int *)efp) = acc;
    return 0;
  } else if (acc > xfp->xfp->limit) {
    xfp->xfp->call_id = -1;
    return (char *)xfp->xfp;
  } else {
    {
      char           *goto_fr;

      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = acc + -1 + 2 * n, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = n - 1, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = &xfp->square_t;
      efp->tmp = tmp;
      efp->acc = acc;
      efp->n = n;
      efp->xfp = xfp;
      goto_fr = lw_call(argp);
      if (goto_fr)
	if ((char *)goto_fr < (char *)efp)
	  return goto_fr;
	else {
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
      tmp = efp->tmp;
      acc = efp->acc;
      n = efp->n;
      xfp = efp->xfp;
      tmp = *((int *)argp);
    }
    *((int *)efp) = tmp;
    return 0;
  }
  return 0;
}

char           *square_sum_t_in_square_sum(char *esp, void *xfp0)
{
  char           *new_esp;
  struct square_sum_t_in_square_sum_frame *efp;
  struct square_sum_frame *xfp = xfp0;
  size_t	  esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  int		  n = (parmp = (char *)((Align_t *) parmp - (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t)), *((int *)parmp));
  int		  acc = (parmp = (char *)((Align_t *) parmp - (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t)), *((int *)parmp));
  char           *argp;
  int		  tmp2;
  int		  tmp3;

LGOTO:
  ;
  efp = (struct square_sum_t_in_square_sum_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct square_sum_t_in_square_sum_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (n == 0) {
    *((int *)efp) = acc;
    return 0;
  } else if (acc > xfp->limit) {
    xfp->call_id = -1;
    return (char *)xfp;
  } else {
    {
      char           *goto_fr;

      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = 0, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = n, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = &efp->square_t;
      efp->square_t.fun = square_t_in_square_sum_t_in_square_sum;
      efp->square_t.fr = (void *)efp;
      efp->tmp3 = tmp3;
      efp->tmp2 = tmp2;
      efp->acc = acc;
      efp->n = n;
      efp->xfp = xfp;
      goto_fr = lw_call(argp);
      if (goto_fr)
	if ((char *)goto_fr < (char *)efp)
	  return goto_fr;
	else {
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
      tmp3 = efp->tmp3;
      tmp2 = efp->tmp2;
      acc = efp->acc;
      n = efp->n;
      xfp = efp->xfp;
      tmp2 = *((int *)argp);
    }
    {
      char           *goto_fr;

      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = acc + tmp2, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = n - 1, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = &xfp->square_sum_t;
      efp->square_t.fun = square_t_in_square_sum_t_in_square_sum;
      efp->square_t.fr = (void *)efp;
      efp->tmp3 = tmp3;
      efp->tmp2 = tmp2;
      efp->acc = acc;
      efp->n = n;
      efp->xfp = xfp;
      goto_fr = lw_call(argp);
      if (goto_fr)
	if ((char *)goto_fr < (char *)efp)
	  return goto_fr;
	else {
	  efp->tmp_esp = 0;
	  goto LGOTO;
	}
      tmp3 = efp->tmp3;
      tmp2 = efp->tmp2;
      acc = efp->acc;
      n = efp->n;
      xfp = efp->xfp;
      tmp3 = *((int *)argp);
    }
    *((int *)efp) = tmp3;
    return 0;
  }
  return 0;
}

int		square_sum (char *esp, int n, int *pflag)
{
  char           *argp;
  struct square_sum_frame *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  int		  limit;
  int		  tmp4;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct square_sum_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct square_sum_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL;
    case -1:
      tmp4 = efp->tmp4;
      limit = efp->limit;
      pflag = efp->pflag;
      n = efp->n;
      goto O_FLOW;
    }
    goto L_CALL;
  }
  efp = (struct square_sum_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct square_sum_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  limit = 8192;
  *pflag = 0;
  {
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((int *)argp) = 0, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((int *)argp) = n, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = &efp->square_sum_t;
    efp->square_sum_t.fun = square_sum_t_in_square_sum;
    efp->square_sum_t.fr = (void *)efp;
    efp->tmp4 = tmp4;
    efp->limit = limit;
    efp->pflag = pflag;
    efp->n = n;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return (int)0 - 1;
L_CALL:
    ;
    tmp4 = efp->tmp4;
    limit = efp->limit;
    pflag = efp->pflag;
    n = efp->n;
    tmp4 = *((int *)efp->argp);
  }
  return tmp4;
O_FLOW:
  ;
  {
    *pflag = 1;
    return -1;
  }
}

struct main_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp5;
  int		  flag;
};

int		main       ()
{
  struct main_frame *efp;
  char           *new_esp;
  char		  estack  [8192];
  char           *esp = estack;
  int		  tmp5;

LGOTO:
  ;
  efp = (struct main_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  new_esp = esp;
  while ((tmp5 = square_sum(new_esp, 100, &efp->flag)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
    char           *goto_fr;

    *((char **)esp) = 0;
    efp->tmp5 = tmp5;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto LGOTO;
    tmp5 = efp->tmp5;
    new_esp = esp + 1;
  }
  printf("1^2 + ... + 100^2 =%d\n", tmp5);
  return efp->flag;
}
