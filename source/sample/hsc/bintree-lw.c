typedef char *(*nestfn_t) (char *, void *);
typedef struct {
  nestfn_t fun;
  void *fr;
} closure_t;
typedef unsigned int size_t;
typedef double Align_t;

char *lw_call(char *esp);

struct func_arg {
  void *(*func) (char *, void *);
  void *arg;
};

void *thread_origin(void *farg);

struct desc_struct {
  unsigned int size;
  unsigned int asize;
  int fli_len;
  unsigned int *fli;
};

struct array_object {
  struct desc_struct *d;
  int length;
  unsigned int el_size;
  unsigned int size;
  unsigned int asize;
  char body[0];
};

extern void *getmem(char *esp, closure_t * scan, unsigned int size);

extern void *move(void *vp);

struct _Bintree {
  int key;
  int val;
  struct hsc__Bintree *left;
  struct hsc__Bintree *right;
};

struct hsc__Bintree {
  struct desc_struct *d;
  struct _Bintree data;
};
unsigned int _Bintree_map[] =
    { (unsigned int)&(*((struct hsc__Bintree *)0)).data +
      (unsigned int)&(*((struct _Bintree *)0)).right,
      (unsigned int)&(*((struct hsc__Bintree *)0)).data +
      (unsigned int)&(*((struct _Bintree *)0)).left };
struct desc_struct _Bintree_desc =
    { sizeof(struct hsc__Bintree),
      (sizeof(struct hsc__Bintree) + sizeof(double) +
      -1) / sizeof(double) * sizeof(double),
      sizeof(_Bintree_map) / sizeof(unsigned int), _Bintree_map };
static int maxins;
static int maxsearch;

struct newBintree_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct hsc__Bintree *hsc_tmp_str;
  struct hsc__Bintree *p;
  void *tmp;
  int v;
  int k;
  closure_t *scan0;
  closure_t scan1;
};

struct scan1_in_newBintree_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct newBintree_frame *xfp;
};

char *scan1_in_newBintree(char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_newBintree_frame *efp;
  struct newBintree_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *)((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct scan1_in_newBintree_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct scan1_in_newBintree_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->p = move(xfp->p);
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->scan0;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    xfp = efp->xfp;
  }
  return 0;
  return 0;
}

struct hsc__Bintree *newBintree(char *esp, closure_t * scan0, int k, int v)
{
  struct newBintree_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  void *tmp;
  struct hsc__Bintree *p;
  struct hsc__Bintree *hsc_tmp_str;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct newBintree_frame *)esp;
    esp =
	(char *)((Align_t *) esp + (sizeof(struct newBintree_frame) +
	    sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
  LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL;
    }
    goto L_CALL;
  }
  efp = (struct newBintree_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct newBintree_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  p = 0;
  hsc_tmp_str = 0;
  {
    new_esp = esp;
    while ((tmp =
	    getmem(new_esp, &efp->scan1,
		(sizeof(struct hsc__Bintree) + sizeof(double) +
		    -1) / sizeof(double) * sizeof(double))) == (void *)0 - 1
	&& (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->hsc_tmp_str = hsc_tmp_str;
      efp->p = p;
      efp->tmp = tmp;
      efp->v = v;
      efp->k = k;
      efp->scan0 = scan0;
      efp->scan1.fun = scan1_in_newBintree;
      efp->scan1.fr = (void *)efp;
      efp->call_id = 0;
      return (struct hsc__Bintree *)0 - 1;
    L_CALL:;
      hsc_tmp_str = efp->hsc_tmp_str;
      p = efp->p;
      tmp = efp->tmp;
      v = efp->v;
      k = efp->k;
      scan0 = efp->scan0;
      new_esp = esp + 1;
    }
    p = (hsc_tmp_str = (struct hsc__Bintree *)tmp, *hsc_tmp_str =
	(struct hsc__Bintree) {
	  (struct desc_struct *)(0 | (unsigned int)&_Bintree_desc), {
	k, v, 0, 0}}, hsc_tmp_str);
  }
  return p;
}

struct insert_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct hsc__Bintree *tmp3;
  struct hsc__Bintree *tmp2;
  struct hsc__Bintree *y;
  int v;
  int k;
  struct hsc__Bintree *x;
  closure_t *scan0;
  closure_t scan1;
};

struct scan1_in_insert_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct insert_frame *xfp;
};

char *scan1_in_insert(char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_insert_frame *efp;
  struct insert_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *)((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct scan1_in_insert_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct scan1_in_insert_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->y = move(xfp->y);
  xfp->x = move(xfp->x);
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->scan0;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    xfp = efp->xfp;
  }
  return 0;
  return 0;
}

void insert(char *esp, closure_t * scan0, struct hsc__Bintree *x, int k, int v)
{
  struct insert_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  struct hsc__Bintree *y;
  struct hsc__Bintree *tmp2;
  struct hsc__Bintree *tmp3;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct insert_frame *)esp;
    esp =
	(char *)((Align_t *) esp + (sizeof(struct insert_frame) +
	    sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
  LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL2;
    case 1:
      goto L_CALL3;
    }
    goto L_CALL2;
  }
  efp = (struct insert_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct insert_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  y = 0;
  while (1) {
    if ((*x).data.key == k) {
      (*x).data.val = v;
      return;
    } else if (k < (*x).data.key) {
      y = (*x).data.left;
      if (!y) {
	{
	  new_esp = esp;
	  while ((tmp2 =
		  newBintree(new_esp, &efp->scan1, k,
		      v)) == (struct hsc__Bintree *)0 - 1
	      && (efp->tmp_esp = *((char **)esp)) != 0) {
	    efp->tmp2 = tmp2;
	    efp->y = y;
	    efp->v = v;
	    efp->k = k;
	    efp->x = x;
	    efp->scan0 = scan0;
	    efp->scan1.fun = scan1_in_insert;
	    efp->scan1.fr = (void *)efp;
	    efp->call_id = 0;
	    return;
	  L_CALL2:;
	    tmp2 = efp->tmp2;
	    y = efp->y;
	    v = efp->v;
	    k = efp->k;
	    x = efp->x;
	    scan0 = efp->scan0;
	    new_esp = esp + 1;
	  }
	  (*x).data.left = tmp2;
	}
	return;
      }
    } else {
      y = (*x).data.right;
      if (!y) {
	{
	  new_esp = esp;
	  while ((tmp3 =
		  newBintree(new_esp, &efp->scan1, k,
		      v)) == (struct hsc__Bintree *)0 - 1
	      && (efp->tmp_esp = *((char **)esp)) != 0) {
	    efp->tmp3 = tmp3;
	    efp->tmp2 = tmp2;
	    efp->y = y;
	    efp->v = v;
	    efp->k = k;
	    efp->x = x;
	    efp->scan0 = scan0;
	    efp->scan1.fun = scan1_in_insert;
	    efp->scan1.fr = (void *)efp;
	    efp->call_id = 1;
	    return;
	  L_CALL3:;
	    tmp3 = efp->tmp3;
	    tmp2 = efp->tmp2;
	    y = efp->y;
	    v = efp->v;
	    k = efp->k;
	    x = efp->x;
	    scan0 = efp->scan0;
	    new_esp = esp + 1;
	  }
	  (*x).data.right = tmp3;
	}
	return;
      }
    }
    x = y;
  }
}

struct search_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  int v0;
  int k;
  struct hsc__Bintree *x;
  closure_t *scan0;
  closure_t scan1;
};

struct scan1_in_search_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct search_frame *xfp;
};

char *scan1_in_search(char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_search_frame *efp;
  struct search_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *)((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct scan1_in_search_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct scan1_in_search_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->x = move(xfp->x);
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->scan0;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    xfp = efp->xfp;
  }
  return 0;
  return 0;
}

int search(char *esp, closure_t * scan0, struct hsc__Bintree *x, int k, int v0)
{
  struct search_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;

LGOTO:;
  efp = (struct search_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct search_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  while (x) {
    if ((*x).data.key == k)
      return (*x).data.val;
    else if (k < (*x).data.key) {
      x = (*x).data.left;
    } else {
      x = (*x).data.right;
    }
  }
  return v0;
}

struct randinsert_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct array_object *hsc_tmp_arr;
  int hsc_tmp_length;
  unsigned int hsc_tmp_size;
  unsigned int hsc_tmp_asize;
  unsigned int *wp;
  struct array_object *seed;
  int k;
  int i;
  void *tmp4;
  int n;
  struct hsc__Bintree *this;
  closure_t *scan0;
  closure_t scan1;
};

struct scan1_in_randinsert_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct randinsert_frame *xfp;
};

char *scan1_in_randinsert(char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_randinsert_frame *efp;
  struct randinsert_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *)((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct scan1_in_randinsert_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct scan1_in_randinsert_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->seed = move(xfp->seed);
  xfp->this = move(xfp->this);
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->scan0;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    xfp = efp->xfp;
  }
  return 0;
  return 0;
}

void randinsert(char *esp, closure_t * scan0, struct hsc__Bintree *this, int n)
{
  struct randinsert_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  void *tmp4;
  int i;
  int k;
  struct array_object *seed;
  unsigned int *wp;
  unsigned int hsc_tmp_asize;
  unsigned int hsc_tmp_size;
  int hsc_tmp_length;
  struct array_object *hsc_tmp_arr;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct randinsert_frame *)esp;
    esp =
	(char *)((Align_t *) esp + (sizeof(struct randinsert_frame) +
	    sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
  LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL4;
    case 1:
      goto L_CALL5;
    }
    goto L_CALL4;
  }
  efp = (struct randinsert_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct randinsert_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  seed = 0;
  wp = 0;
  hsc_tmp_asize = 0;
  hsc_tmp_size = 0;
  hsc_tmp_length = 0;
  hsc_tmp_arr = 0;
  {
    new_esp = esp;
    while ((tmp4 = getmem(new_esp, &efp->scan1, hsc_tmp_asize)) == (void *)0 - 1
	&& (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->hsc_tmp_arr = hsc_tmp_arr;
      efp->hsc_tmp_length = hsc_tmp_length;
      efp->hsc_tmp_size = hsc_tmp_size;
      efp->hsc_tmp_asize = hsc_tmp_asize;
      efp->wp = wp;
      efp->seed = seed;
      efp->k = k;
      efp->i = i;
      efp->tmp4 = tmp4;
      efp->n = n;
      efp->this = this;
      efp->scan0 = scan0;
      efp->scan1.fun = scan1_in_randinsert;
      efp->scan1.fr = (void *)efp;
      efp->call_id = 0;
      return;
    L_CALL4:;
      hsc_tmp_arr = efp->hsc_tmp_arr;
      hsc_tmp_length = efp->hsc_tmp_length;
      hsc_tmp_size = efp->hsc_tmp_size;
      hsc_tmp_asize = efp->hsc_tmp_asize;
      wp = efp->wp;
      seed = efp->seed;
      k = efp->k;
      i = efp->i;
      tmp4 = efp->tmp4;
      n = efp->n;
      this = efp->this;
      scan0 = efp->scan0;
      new_esp = esp + 1;
    }
    seed = (hsc_tmp_length = 3, hsc_tmp_size =
	sizeof(struct array_object) + hsc_tmp_length * sizeof(unsigned int),
	hsc_tmp_asize =
	(hsc_tmp_size + sizeof(double) + -1) / sizeof(double) * sizeof(double),
	hsc_tmp_arr = (struct array_object *)tmp4, (*hsc_tmp_arr).d =
	(struct desc_struct *)3, (*hsc_tmp_arr).length =
	hsc_tmp_length, (*hsc_tmp_arr).el_size =
	sizeof(unsigned int), (*hsc_tmp_arr).size =
	hsc_tmp_size, (*hsc_tmp_arr).asize = hsc_tmp_asize, wp =
	(unsigned int *)(*hsc_tmp_arr).body, wp[0] = 3, wp[1] = 4, wp[2] =
	5, hsc_tmp_arr);
  }
  {
    i = 0;
    for (; i < n; i++) {
      k = rand_r((unsigned int *)(*seed).body);
      new_esp = esp;
      while (insert(new_esp, &efp->scan1, this, k, k), (efp->tmp_esp =
	      *((char **)esp)) != 0) {
	efp->hsc_tmp_arr = hsc_tmp_arr;
	efp->hsc_tmp_length = hsc_tmp_length;
	efp->hsc_tmp_size = hsc_tmp_size;
	efp->hsc_tmp_asize = hsc_tmp_asize;
	efp->wp = wp;
	efp->seed = seed;
	efp->k = k;
	efp->i = i;
	efp->tmp4 = tmp4;
	efp->n = n;
	efp->this = this;
	efp->scan0 = scan0;
	efp->scan1.fun = scan1_in_randinsert;
	efp->scan1.fr = (void *)efp;
	efp->call_id = 1;
	return;
      L_CALL5:;
	hsc_tmp_arr = efp->hsc_tmp_arr;
	hsc_tmp_length = efp->hsc_tmp_length;
	hsc_tmp_size = efp->hsc_tmp_size;
	hsc_tmp_asize = efp->hsc_tmp_asize;
	wp = efp->wp;
	seed = efp->seed;
	k = efp->k;
	i = efp->i;
	tmp4 = efp->tmp4;
	n = efp->n;
	this = efp->this;
	scan0 = efp->scan0;
	new_esp = esp + 1;
      }
    }
  }
}

struct randsearch_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct array_object *hsc_tmp_arr2;
  int hsc_tmp_length2;
  unsigned int hsc_tmp_size2;
  unsigned int hsc_tmp_asize2;
  unsigned int *wp2;
  struct array_object *seed;
  int k;
  int i;
  void *tmp5;
  int n;
  struct hsc__Bintree *this;
  closure_t *scan0;
  closure_t scan1;
};

struct scan1_in_randsearch_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct randsearch_frame *xfp;
};

char *scan1_in_randsearch(char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_randsearch_frame *efp;
  struct randsearch_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *)((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct scan1_in_randsearch_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct scan1_in_randsearch_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->seed = move(xfp->seed);
  xfp->this = move(xfp->this);
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->scan0;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    xfp = efp->xfp;
  }
  return 0;
  return 0;
}

void randsearch(char *esp, closure_t * scan0, struct hsc__Bintree *this, int n)
{
  struct randsearch_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  void *tmp5;
  int i;
  int k;
  struct array_object *seed;
  unsigned int *wp2;
  unsigned int hsc_tmp_asize2;
  unsigned int hsc_tmp_size2;
  int hsc_tmp_length2;
  struct array_object *hsc_tmp_arr2;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct randsearch_frame *)esp;
    esp =
	(char *)((Align_t *) esp + (sizeof(struct randsearch_frame) +
	    sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
  LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL6;
    case 1:
      goto L_CALL7;
    }
    goto L_CALL6;
  }
  efp = (struct randsearch_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct randsearch_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  seed = 0;
  wp2 = 0;
  hsc_tmp_asize2 = 0;
  hsc_tmp_size2 = 0;
  hsc_tmp_length2 = 0;
  hsc_tmp_arr2 = 0;
  {
    new_esp = esp;
    while ((tmp5 =
	    getmem(new_esp, &efp->scan1, hsc_tmp_asize2)) == (void *)0 - 1
	&& (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->hsc_tmp_arr2 = hsc_tmp_arr2;
      efp->hsc_tmp_length2 = hsc_tmp_length2;
      efp->hsc_tmp_size2 = hsc_tmp_size2;
      efp->hsc_tmp_asize2 = hsc_tmp_asize2;
      efp->wp2 = wp2;
      efp->seed = seed;
      efp->k = k;
      efp->i = i;
      efp->tmp5 = tmp5;
      efp->n = n;
      efp->this = this;
      efp->scan0 = scan0;
      efp->scan1.fun = scan1_in_randsearch;
      efp->scan1.fr = (void *)efp;
      efp->call_id = 0;
      return;
    L_CALL6:;
      hsc_tmp_arr2 = efp->hsc_tmp_arr2;
      hsc_tmp_length2 = efp->hsc_tmp_length2;
      hsc_tmp_size2 = efp->hsc_tmp_size2;
      hsc_tmp_asize2 = efp->hsc_tmp_asize2;
      wp2 = efp->wp2;
      seed = efp->seed;
      k = efp->k;
      i = efp->i;
      tmp5 = efp->tmp5;
      n = efp->n;
      this = efp->this;
      scan0 = efp->scan0;
      new_esp = esp + 1;
    }
    seed = (hsc_tmp_length2 = 3, hsc_tmp_size2 =
	sizeof(struct array_object) + hsc_tmp_length2 * sizeof(unsigned int),
	hsc_tmp_asize2 =
	(hsc_tmp_size2 + sizeof(double) + -1) / sizeof(double) * sizeof(double),
	hsc_tmp_arr2 = (struct array_object *)tmp5, (*hsc_tmp_arr2).d =
	(struct desc_struct *)3, (*hsc_tmp_arr2).length =
	hsc_tmp_length2, (*hsc_tmp_arr2).el_size =
	sizeof(unsigned int), (*hsc_tmp_arr2).size =
	hsc_tmp_size2, (*hsc_tmp_arr2).asize = hsc_tmp_asize2, wp2 =
	(unsigned int *)(*hsc_tmp_arr2).body, wp2[0] = 8, wp2[1] = 9, wp2[2] =
	10, hsc_tmp_arr2);
  }
  ((unsigned int *)(*seed).body)[2] = 100;
  {
    i = 0;
    for (; i < n; i++) {
      k = rand_r((unsigned int *)(*seed).body);
      new_esp = esp;
      while (search(new_esp, &efp->scan1, this, k, 0), (efp->tmp_esp =
	      *((char **)esp)) != 0) {
	efp->hsc_tmp_arr2 = hsc_tmp_arr2;
	efp->hsc_tmp_length2 = hsc_tmp_length2;
	efp->hsc_tmp_size2 = hsc_tmp_size2;
	efp->hsc_tmp_asize2 = hsc_tmp_asize2;
	efp->wp2 = wp2;
	efp->seed = seed;
	efp->k = k;
	efp->i = i;
	efp->tmp5 = tmp5;
	efp->n = n;
	efp->this = this;
	efp->scan0 = scan0;
	efp->scan1.fun = scan1_in_randsearch;
	efp->scan1.fr = (void *)efp;
	efp->call_id = 1;
	return;
      L_CALL7:;
	hsc_tmp_arr2 = efp->hsc_tmp_arr2;
	hsc_tmp_length2 = efp->hsc_tmp_length2;
	hsc_tmp_size2 = efp->hsc_tmp_size2;
	hsc_tmp_asize2 = efp->hsc_tmp_asize2;
	wp2 = efp->wp2;
	seed = efp->seed;
	k = efp->k;
	i = efp->i;
	tmp5 = efp->tmp5;
	n = efp->n;
	this = efp->this;
	scan0 = efp->scan0;
	new_esp = esp + 1;
      }
    }
  }
}

struct hsc_main_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct hsc__Bintree *hsc_tmp_str2;
  struct hsc__Bintree *root;
  int index;
  int searchskip;
  void *tmp6;
  int ifexp_result4;
  int ifexp_result3;
  int ifexp_result;
  int ifexp_result2;
  struct array_object *argv;
  int argc;
  closure_t *scan0;
  closure_t scan1;
};

struct scan1_in_hsc_main_frame {
  char *tmp_esp;
  char *argp;
  int call_id;
  struct hsc_main_frame *xfp;
};

char *scan1_in_hsc_main(char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_hsc_main_frame *efp;
  struct hsc_main_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *)((size_t) esp ^ esp_flag);
  char *argp;

LGOTO:;
  efp = (struct scan1_in_hsc_main_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct scan1_in_hsc_main_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  xfp->root = move(xfp->root);
  xfp->argv = move(xfp->argv);
  {
    char *goto_fr;
    argp =
	(char *)((Align_t *) esp + (sizeof(char *) + sizeof(Align_t) +
	    -1) / sizeof(Align_t));
    *((closure_t **) argp) = xfp->scan0;
    efp->xfp = xfp;
    goto_fr = lw_call(argp);
    if (goto_fr)
      if ((char *)goto_fr < (char *)efp)
	return goto_fr;
      else {
	efp->tmp_esp = 0;
	goto LGOTO;
      }
    xfp = efp->xfp;
  }
  return 0;
  return 0;
}

int hsc_main(char *esp, closure_t * scan0, int argc, struct array_object *argv)
{
  struct hsc_main_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int ifexp_result2;
  int ifexp_result;
  int ifexp_result3;
  int ifexp_result4;
  void *tmp6;
  int searchskip;
  int index;
  struct hsc__Bintree *root;
  struct hsc__Bintree *hsc_tmp_str2;

  if (esp_flag) {
    esp = (char *)((size_t) esp ^ esp_flag);
    efp = (struct hsc_main_frame *)esp;
    esp =
	(char *)((Align_t *) esp + (sizeof(struct hsc_main_frame) +
	    sizeof(Align_t) + -1) / sizeof(Align_t));
    *((char **)esp) = 0;
  LGOTO:switch ((*efp).call_id) {
    case 0:
      goto L_CALL8;
    case 1:
      goto L_CALL9;
    case 2:
      goto L_CALL10;
    }
    goto L_CALL8;
  }
  efp = (struct hsc_main_frame *)esp;
  esp =
      (char *)((Align_t *) esp + (sizeof(struct hsc_main_frame) +
	  sizeof(Align_t) + -1) / sizeof(Align_t));
  *((char **)esp) = 0;
  searchskip = 0;
  index = 1;
  root = 0;
  hsc_tmp_str2 = 0;
  {
    if (argc > 1) {
      if (strcmp((char *)(*((struct array_object **)(*argv).body)[index]).body,
	      "-s") == 0) {
	ifexp_result2 = 1;
      } else {
	ifexp_result2 = 0;
      }
      ifexp_result = ifexp_result2;
    } else {
      ifexp_result = 0;
    }
    if (ifexp_result) {
      --argc;
      ++index;
      searchskip = 1;
    }
  }
  {
    if (argc > 1) {
      ifexp_result3 =
	  atoi((char *)(*((struct array_object **)(*argv).body)[index]).body);
    } else {
      ifexp_result3 = 100000;
    }
    maxins = ifexp_result3;
  }
  {
    if (argc > 2) {
      ifexp_result4 =
	  atoi((char *)(*((struct array_object **)(*argv).body)[1 +
		  index]).body);
    } else {
      ifexp_result4 = 300000;
    }
    maxsearch = ifexp_result4;
  }
  printf("Bintree=%d, masins=%d, maxsearch=%d\n", sizeof(struct hsc__Bintree),
      maxins, maxsearch);
  {
    new_esp = esp;
    while ((tmp6 =
	    getmem(new_esp, &efp->scan1,
		(sizeof(struct hsc__Bintree) + sizeof(double) +
		    -1) / sizeof(double) * sizeof(double))) == (void *)0 - 1
	&& (efp->tmp_esp = *((char **)esp)) != 0) {
      efp->hsc_tmp_str2 = hsc_tmp_str2;
      efp->root = root;
      efp->index = index;
      efp->searchskip = searchskip;
      efp->tmp6 = tmp6;
      efp->ifexp_result4 = ifexp_result4;
      efp->ifexp_result3 = ifexp_result3;
      efp->ifexp_result = ifexp_result;
      efp->ifexp_result2 = ifexp_result2;
      efp->argv = argv;
      efp->argc = argc;
      efp->scan0 = scan0;
      efp->scan1.fun = scan1_in_hsc_main;
      efp->scan1.fr = (void *)efp;
      efp->call_id = 0;
      return (int)0 - 1;
    L_CALL8:;
      hsc_tmp_str2 = efp->hsc_tmp_str2;
      root = efp->root;
      index = efp->index;
      searchskip = efp->searchskip;
      tmp6 = efp->tmp6;
      ifexp_result4 = efp->ifexp_result4;
      ifexp_result3 = efp->ifexp_result3;
      ifexp_result = efp->ifexp_result;
      ifexp_result2 = efp->ifexp_result2;
      argv = efp->argv;
      argc = efp->argc;
      scan0 = efp->scan0;
      new_esp = esp + 1;
    }
    root = (hsc_tmp_str2 = (struct hsc__Bintree *)tmp6, *hsc_tmp_str2 =
	(struct hsc__Bintree) {
	  (struct desc_struct *)(0 | (unsigned int)&_Bintree_desc), {
	0, 0, 0, 0}}, hsc_tmp_str2);
  }
  new_esp = esp;
  while (randinsert(new_esp, &efp->scan1, root, maxins), (efp->tmp_esp =
	  *((char **)esp)) != 0) {
    efp->hsc_tmp_str2 = hsc_tmp_str2;
    efp->root = root;
    efp->index = index;
    efp->searchskip = searchskip;
    efp->tmp6 = tmp6;
    efp->ifexp_result4 = ifexp_result4;
    efp->ifexp_result3 = ifexp_result3;
    efp->ifexp_result = ifexp_result;
    efp->ifexp_result2 = ifexp_result2;
    efp->argv = argv;
    efp->argc = argc;
    efp->scan0 = scan0;
    efp->scan1.fun = scan1_in_hsc_main;
    efp->scan1.fr = (void *)efp;
    efp->call_id = 1;
    return (int)0 - 1;
  L_CALL9:;
    hsc_tmp_str2 = efp->hsc_tmp_str2;
    root = efp->root;
    index = efp->index;
    searchskip = efp->searchskip;
    tmp6 = efp->tmp6;
    ifexp_result4 = efp->ifexp_result4;
    ifexp_result3 = efp->ifexp_result3;
    ifexp_result = efp->ifexp_result;
    ifexp_result2 = efp->ifexp_result2;
    argv = efp->argv;
    argc = efp->argc;
    scan0 = efp->scan0;
    new_esp = esp + 1;
  }
  if (!searchskip) {
    new_esp = esp;
    while (randsearch(new_esp, &efp->scan1, root, maxsearch), (efp->tmp_esp =
	    *((char **)esp)) != 0) {
      efp->hsc_tmp_str2 = hsc_tmp_str2;
      efp->root = root;
      efp->index = index;
      efp->searchskip = searchskip;
      efp->tmp6 = tmp6;
      efp->ifexp_result4 = ifexp_result4;
      efp->ifexp_result3 = ifexp_result3;
      efp->ifexp_result = ifexp_result;
      efp->ifexp_result2 = ifexp_result2;
      efp->argv = argv;
      efp->argc = argc;
      efp->scan0 = scan0;
      efp->scan1.fun = scan1_in_hsc_main;
      efp->scan1.fr = (void *)efp;
      efp->call_id = 2;
      return (int)0 - 1;
    L_CALL10:;
      hsc_tmp_str2 = efp->hsc_tmp_str2;
      root = efp->root;
      index = efp->index;
      searchskip = efp->searchskip;
      tmp6 = efp->tmp6;
      ifexp_result4 = efp->ifexp_result4;
      ifexp_result3 = efp->ifexp_result3;
      ifexp_result = efp->ifexp_result;
      ifexp_result2 = efp->ifexp_result2;
      argv = efp->argv;
      argc = efp->argc;
      scan0 = efp->scan0;
      new_esp = esp + 1;
    }
  }
  return 0;
}
