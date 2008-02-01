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

struct f_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  closure_t      *pg;
};

int		f          (char *esp, closure_t * pg)
{
  char           *argp;
  struct f_frame *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct f_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct f_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL;
    }
    goto L_CALL;
  }
  efp = (struct f_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct f_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  {
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((int *)argp) = 0, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = pg;
    efp->pg = pg;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return (int)0 - 1;
L_CALL:
    ;
    pg = efp->pg;
  }
  return 0;
}

int		g          (char *esp, closure_t * pg);

struct g_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp;
  closure_t      *pg;
  closure_t	  g1;
};

struct g1_in_g_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  m;
  struct g_frame *xfp;
};

char           *g1_in_g(char *esp, void *xfp0)
{
  char           *new_esp;
  struct g1_in_g_frame *efp;
  struct g_frame *xfp = xfp0;
  size_t	  esp_flag = (size_t) esp & 3;
  char           *parmp = (char *)((size_t) esp ^ esp_flag);
  int		  m = (parmp = (char *)((Align_t *) parmp - (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t)), *((int *)parmp));

LGOTO:
  ;
  efp = (struct g1_in_g_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct g1_in_g_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (m > 0) {
    xfp->call_id = -1;
    return (char *)xfp;
  } else {
    new_esp = esp;
    while (g(new_esp, &xfp->g1), (efp->tmp_esp = *((char **)esp)) != 0) {
      char           *goto_fr;

      *((char **)esp) = 0;
      efp->m = m;
      efp->xfp = xfp;
      goto_fr = lw_call(efp->tmp_esp);
      if (goto_fr && (char *)goto_fr < (char *)efp)
	return goto_fr;
      if ((char *)goto_fr == (char *)efp)
	goto LGOTO;
      m = efp->m;
      xfp = efp->xfp;
      new_esp = esp + 1;
    }
  }
  return 0;
}

int		g          (char *esp, closure_t * pg)
{
  char           *argp;
  struct g_frame *efp;
  char           *new_esp;
  size_t	  esp_flag = (size_t) esp & 3;
  int		  tmp;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct g_frame *)esp;
    esp = (char *)((Align_t *) esp + (sizeof(struct g_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:
      goto L_CALL2;
    case 1:
      goto L_CALL3;
    case 2:
      goto L_CALL4;
    case -1:
      tmp = efp->tmp;
      pg = efp->pg;
      goto L1;
    }
    goto L_CALL2;
  }
  efp = (struct g_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct g_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  if (pg == 0) {
    new_esp = esp;
    while ((tmp = f(new_esp, &efp->g1)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->tmp = tmp;
      efp->pg = pg;
      efp->g1.fun = g1_in_g;
      efp->g1.fr = (void *)efp;
      efp->call_id = 0;
      return (int)0 - 1;
  L_CALL2:
      ;
      tmp = efp->tmp;
      pg = efp->pg;
      new_esp = esp + 1;
    }
    return tmp;
  } else {
    {
      argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((int *)argp) = 1, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
      *((closure_t **) argp) = &efp->g1;
      efp->g1.fun = g1_in_g;
      efp->g1.fr = (void *)efp;
      efp->tmp = tmp;
      efp->pg = pg;
      efp->argp = argp;
      efp->tmp_esp = argp;
      efp->call_id = 1;
      return (int)0 - 1;
  L_CALL3:
      ;
      tmp = efp->tmp;
      pg = efp->pg;
    }
    return 0;
  }
L1:
  ;
  if (pg == 0)
    return 0;
  else {
    argp = (char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((int *)argp) = 1, argp = (char *)((Align_t *) argp + (sizeof(int) + sizeof(Align_t) + -1) / sizeof(Align_t));
    *((closure_t **) argp) = pg;
    efp->g1.fun = g1_in_g;
    efp->g1.fr = (void *)efp;
    efp->tmp = tmp;
    efp->pg = pg;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 2;
    return (int)0 - 1;
L_CALL4:
    ;
    tmp = efp->tmp;
    pg = efp->pg;
  }
}

struct main_frame {
  char           *tmp_esp;
  char           *argp;
  int		  call_id;
  int		  tmp2;
};

int		main       ()
{
  struct main_frame *efp;
  char           *new_esp;
  char		  estack  [8192];
  char           *esp = estack;
  int		  tmp2;

LGOTO:
  ;
  efp = (struct main_frame *)esp;
  esp = (char *)((Align_t *) esp + (sizeof(struct main_frame) + sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  new_esp = esp;
  while ((tmp2 = g(new_esp, 0)) == (int)0 - 1 && (efp->tmp_esp = *((char **)esp)) != 0) {
    char           *goto_fr;

    *((char **)esp) = 0;
    efp->tmp2 = tmp2;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char *)goto_fr == (char *)efp)
      goto LGOTO;
    tmp2 = efp->tmp2;
    new_esp = esp + 1;
  }
  return tmp2;
}
