typedef void (*sht) (void);
typedef double align_t;
typedef unsigned int size_t;

struct desc_struct {
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
};
typedef struct desc_struct descriptor;
typedef descriptor *desc_t;

struct array_object {
  desc_t d;
  int length;
  size_t el_size;
  size_t size;
  size_t asize;
  char body[0];
};

extern void *getmem(sht scan, size_t size);

extern void *move(void *vp);

struct _Bintree {
  int key;
  int val;
  struct hsc__Bintree *left;
  struct hsc__Bintree *right;
};

struct hsc__Bintree {
  desc_t d;
  struct _Bintree data;
};
size_t _Bintree_map[] =
    { (size_t) & (*((struct hsc__Bintree *)0)).data +
      (size_t) & (*((struct _Bintree *)0)).right,
      (size_t) & (*((struct hsc__Bintree *)0)).data +
      (size_t) & (*((struct _Bintree *)0)).left };
descriptor _Bintree_desc =
    { sizeof(struct hsc__Bintree),
      (sizeof(struct hsc__Bintree) + sizeof(align_t) +
      -1) / sizeof(align_t) * sizeof(align_t),
      sizeof(_Bintree_map) / sizeof(size_t), _Bintree_map };
static int maxins;
static int maxsearch;

struct hsc__Bintree *newBintree(sht scan0, int k, int v)
{
  struct hsc__Bintree *p = 0;
  struct hsc__Bintree *hsc_tmp_str = 0;

  void scan1(void) {
    p = move(p);
    scan0();
    return;
  }
  p = (hsc_tmp_str =
      (struct hsc__Bintree *)getmem(scan1,
	  (sizeof(struct hsc__Bintree) + sizeof(align_t) +
	      -1) / sizeof(align_t) * sizeof(align_t)), *hsc_tmp_str =
      (struct hsc__Bintree) {
	(desc_t) (0 | (size_t) & _Bintree_desc), {
      k, v, 0, 0}}, hsc_tmp_str);
  return p;
}

void insert(sht scan0, struct hsc__Bintree *x, int k, int v)
{
  struct hsc__Bintree *y = 0;

  void scan1(void) {
    y = move(y);
    x = move(x);
    scan0();
    return;
  }
  while (1) {
    if ((*x).data.key == k) {
      (*x).data.val = v;
      return;
    } else if (k < (*x).data.key) {
      y = (*x).data.left;
      if (!y) {
	(*x).data.left = newBintree(scan1, k, v);
	return;
      }
    } else {
      y = (*x).data.right;
      if (!y) {
	(*x).data.right = newBintree(scan1, k, v);
	return;
      }
    }
    x = y;
  }
}

int search(sht scan0, struct hsc__Bintree *x, int k, int v0)
{

  void scan1(void) {
    x = move(x);
    scan0();
    return;
  }
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

void randinsert(sht scan0, struct hsc__Bintree *this, int n)
{
  int i;
  int k;
  struct array_object *seed = 0;
  unsigned int *wp = 0;
  size_t hsc_tmp_asize = 0;
  size_t hsc_tmp_size = 0;
  int hsc_tmp_length = 0;
  struct array_object *hsc_tmp_arr = 0;

  void scan1(void) {
    seed = move(seed);
    this = move(this);
    scan0();
    return;
  }
  seed = (hsc_tmp_length = 3, hsc_tmp_size =
      sizeof(struct array_object) + hsc_tmp_length * sizeof(unsigned int),
      hsc_tmp_asize =
      (hsc_tmp_size + sizeof(align_t) + -1) / sizeof(align_t) * sizeof(align_t),
      hsc_tmp_arr =
      (struct array_object *)getmem(scan1, hsc_tmp_asize), (*hsc_tmp_arr).d =
      (desc_t) 3, (*hsc_tmp_arr).length =
      hsc_tmp_length, (*hsc_tmp_arr).el_size =
      sizeof(unsigned int), (*hsc_tmp_arr).size =
      hsc_tmp_size, (*hsc_tmp_arr).asize = hsc_tmp_asize, wp =
      (unsigned int *)(*hsc_tmp_arr).body, wp[0] = 3, wp[1] = 4, wp[2] =
      5, hsc_tmp_arr);
  {
    i = 0;
    for (; i < n; i++) {
      k = rand_r((unsigned int *)(*seed).body);
      insert(scan1, this, k, k);
    }
  }
}

void randsearch(sht scan0, struct hsc__Bintree *this, int n)
{
  int i;
  int k;
  struct array_object *seed = 0;
  unsigned int *wp2 = 0;
  size_t hsc_tmp_asize2 = 0;
  size_t hsc_tmp_size2 = 0;
  int hsc_tmp_length2 = 0;
  struct array_object *hsc_tmp_arr2 = 0;

  void scan1(void) {
    seed = move(seed);
    this = move(this);
    scan0();
    return;
  }
  seed = (hsc_tmp_length2 = 3, hsc_tmp_size2 =
      sizeof(struct array_object) + hsc_tmp_length2 * sizeof(unsigned int),
      hsc_tmp_asize2 =
      (hsc_tmp_size2 + sizeof(align_t) +
	  -1) / sizeof(align_t) * sizeof(align_t), hsc_tmp_arr2 =
      (struct array_object *)getmem(scan1, hsc_tmp_asize2), (*hsc_tmp_arr2).d =
      (desc_t) 3, (*hsc_tmp_arr2).length =
      hsc_tmp_length2, (*hsc_tmp_arr2).el_size =
      sizeof(unsigned int), (*hsc_tmp_arr2).size =
      hsc_tmp_size2, (*hsc_tmp_arr2).asize = hsc_tmp_asize2, wp2 =
      (unsigned int *)(*hsc_tmp_arr2).body, wp2[0] = 8, wp2[1] = 9, wp2[2] =
      10, hsc_tmp_arr2);
  ((unsigned int *)(*seed).body)[2] = 100;
  {
    i = 0;
    for (; i < n; i++) {
      k = rand_r((unsigned int *)(*seed).body);
      search(scan1, this, k, 0);
    }
  }
}

int hsc_main(sht scan0, int argc, struct array_object *argv)
{
  int searchskip = 0;
  int index = 1;
  struct hsc__Bintree *root = 0;
  struct hsc__Bintree *hsc_tmp_str2 = 0;

  void scan1(void) {
    root = move(root);
    argv = move(argv);
    scan0();
    return;
  }
  if (argc > 1
      && strcmp((char *)(*((struct array_object **)(*argv).body)[index]).body,
	  "-s") == 0) {
    --argc;
    ++index;
    searchskip = 1;
  }
  maxins =
      argc >
      1 ? atoi((char *)(*((struct array_object **)(*argv).body)[index]).
      body) : 100000;
  maxsearch =
      argc >
      2 ? atoi((char *)(*((struct array_object **)(*argv).body)[1 +
	      index]).body) : 300000;
  printf("Bintree=%d, masins=%d, maxsearch=%d\n", sizeof(struct _Bintree),
      maxins, maxsearch);
  root = (hsc_tmp_str2 =
      (struct hsc__Bintree *)getmem(scan1,
	  (sizeof(struct hsc__Bintree) + sizeof(align_t) +
	      -1) / sizeof(align_t) * sizeof(align_t)), *hsc_tmp_str2 =
      (struct hsc__Bintree) {
	(desc_t) (0 | (size_t) & _Bintree_desc), {
      0, 0, 0, 0}}, hsc_tmp_str2);
  randinsert(scan1, root, maxins);
  if (!searchskip)
    randsearch(scan1, root, maxsearch);
  return 0;
}
