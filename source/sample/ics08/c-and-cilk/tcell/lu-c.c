#include <pthread.h>
#include <stdio.h>
enum node
{ OUTSIDE, INSIDE, ANY };

struct cmd
{
  int c;
  enum node node;
  char *v[5];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  struct cmd_list *next;
};

void read_to_eol (void);

void recv_rslt (struct cmd, void *);

void recv_task (struct cmd, void *);

void recv_treq (struct cmd);

void recv_rack (struct cmd);

void recv_none (struct cmd);
struct task;
struct thread_data;
void (*task_doers[256]) (struct thread_data *, void *);
void (*task_senders[256]) (struct thread_data *, void *);
void *(*task_receivers[256]) (struct thread_data *);
void (*rslt_senders[256]) (struct thread_data *, void *);
void (*rslt_receivers[256]) (struct thread_data *, void *);
enum task_stat
{ TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE,
    TASK_SUSPENDED };
enum task_home_stat
{ TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE };

struct task
{
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  enum node rslt_to;
  char rslt_head[256];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  char task_head[256];
};

struct thread_data
{
  struct task_home *volatile req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char ndiv_buf[32];
  char tno_buf[8];
  char id_str[32];
  char buf[1024];
};
extern int divisibility_flag;

void send_divisible (void);

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (struct thread_data *thr);

void send_int (int n);

int recv_int (void);

void handle_req (int (*)(void), struct thread_data *);
#include<sys/time.h>

double
elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
						     (tp[0]).tv_usec);
}

void decomp_lu (int n, double *a, int nx, int anx);

void decomp_r (int n1, int n2, double *b, double *a, int nx, int an);

void decomp_d (int n1, int n2, double *b, double *a, int nx, int an);

void decomp_rd (int n1, int n2, int n3, double *b1, double *b2, double *a,
		int nx, int anx);

void zero_mat (double *a, int n1, int n2, int nx);
enum task_tp2
{ TASK_CMD, TASK_LU, TASK_R, TASK_D, TASK_RD };

struct task_lu
{
  enum task_tp2 tt;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  int anx;
  int tmpnx;
};

void
put_task (struct task_lu *st, enum task_tp2 tt, int n1, int n2, int n3,
	  double *b1, double *b2, double *a, int nx, int anx)
{
  (*st).tt = tt;
  (*st).n1 = n1;
  (*st).n2 = n2;
  (*st).n3 = n3;
  (*st).b1 = b1;
  (*st).b2 = b2;
  (*st).a = a;
  (*st).nx = nx;
  (*st).anx = anx;
  (*st).tmpnx = 0;
  if (a == 0)
    {
      (*st).tmpnx = n3;
      (*st).a = (double *) malloc (sizeof (double) * n2 * n3);
      zero_mat ((*st).a, n2, n3, n3);
    }
}

void
merge_task (struct task_lu *x, double *a, int anx)
{
  int n2 = (*x).n2;
  int n3 = (*x).n3;
  double *tmp = (*x).a;
  int i;
  int j;
  if (a == tmp)
    return;
  {
    i = 0;
    for (; i < n2; i++)
      {
	j = 0;
	for (; j < n3; j++)
	  {
	    a[i * anx + j] += tmp[i * n3 + j];
	  }
      }
  }
  free (tmp);
}

void
decomp_lu_0 (int n, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    double w = 1.0 / a[0];
    {
      j = 1;
      for (; j < n; j++)
	{
	  a[j] *= w;
	}
    }
  }
  {
    i = 1;
    for (; i < n; i++)
      {
	{
	  k = 0;
	  for (; k < i - 1; k++)
	    {
	      double aik = a[i * anx + k];
	      {
		j = k + 1;
		for (; j < n; j++)
		  {
		    a[i * anx + j] -= aik * a[k * anx + j];
		  }
	      }
	    }
	}
	{
	  double aik = a[i * anx + i - 1];
	  double w;
	  a[i * anx + i] -= aik * a[(i - 1) * anx + i];
	  w = 1.0 / a[i * anx + i];
	  {
	    j = i + 1;
	    for (; j < n; j++)
	      {
		a[i * anx + j] =
		  w * (a[i * anx + j] - aik * a[(i - 1) * anx + j]);
	      }
	  }
	}
      }
  }
}

void
decomp_lu (int n, double *a, int nx, int anx)
{
  struct timeval tp[2];
  if (n <= 4)
    {
      decomp_lu_0 (n, a, nx, anx);
      return;
    }
  {
    int n1 = n > 16 ? n / 4 : n / 2;
    int n2 = n - n1;
    {
      {
	decomp_r (n1, n2, a, a + n1, nx, anx);
      }
      {
	decomp_d (n1, n2, a, a + n1 * nx, nx, anx);
      }
    }
    decomp_rd (n1, n2, n2, a + n1 * nx, a + n1, a + (n1 * nx + n1), nx, anx);
    decomp_lu (n2, a + (n1 * nx + n1), nx, anx);
  }
}

void
decomp_r_0 (int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    double w = 1.0 / b[0];
    {
      j = 0;
      for (; j < n2; j++)
	{
	  a[j] *= w;
	}
    }
  }
  {
    i = 1;
    for (; i < n1; i++)
      {
	{
	  k = 0;
	  for (; k < i - 1; k++)
	    {
	      double aik = b[i * nx + k];
	      {
		j = 0;
		for (; j < n2; j++)
		  {
		    a[i * anx + j] -= aik * a[k * anx + j];
		  }
	      }
	    }
	}
	{
	  double aik = b[i * nx + i - 1];
	  double w = 1.0 / b[i * nx + i];
	  {
	    j = 0;
	    for (; j < n2; j++)
	      {
		a[i * anx + j] =
		  w * (a[i * anx + j] - aik * a[(i - 1) * anx + j]);
	      }
	  }
	}
      }
  }
}

void
decomp_r (int n1, int n2, double *b, double *a, int nx, int anx)
{
  if (n1 <= 4)
    {
      decomp_r_0 (n1, n2, b, a, nx, anx);
      return;
    }
  {
    int n1_1 = n1 / 2;
    int n1_2 = n1 - n1_1;
    decomp_r (n1_1, n2, b, a, nx, anx);
    decomp_rd (n1_1, n1_2, n2, b + n1_1 * nx, a, a + n1_1 * nx, nx, anx);
    decomp_r (n1_2, n2, b + (n1_1 * nx + n1_1), a + n1_1 * nx, nx, anx);
  }
}

void
decomp_d_0 (int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    i = 0;
    for (; i < n2; i++)
      {
	k = 0;
	for (; k < n1 - 1; k++)
	  {
	    double aik = a[i * anx + k];
	    {
	      j = k + 1;
	      for (; j < n1; j++)
		{
		  a[i * anx + j] -= aik * b[k * nx + j];
		}
	    }
	  }
      }
  }
}

void
decomp_d (int n1, int n2, double *b, double *a, int nx, int anx)
{
  if (n2 <= 4)
    {
      decomp_d_0 (n1, n2, b, a, nx, anx);
      return;
    }
  {
    int n2_1 = n2 / 2;
    int n2_2 = n2 - n2_1;
    {
      {
	decomp_d (n1, n2_1, b, a, nx, anx);
      }
      {
	decomp_d (n1, n2_2, b, a + n2_1 * nx, nx, anx);
      }
    }
  }
}

void
decomp_rd_0 (int n1, int n2, int n3, double *b1, double *b2, double *a,
	     int nx, int anx)
{
  int i;
  int j;
  int k;
  {
    i = 0;
    for (; i < n2; i++)
      {
	k = 0;
	for (; k < n1; k++)
	  {
	    double aik = b1[i * nx + k];
	    {
	      j = 0;
	      for (; j < n3; j++)
		{
		  a[i * anx + j] -= aik * b2[k * nx + j];
		}
	    }
	  }
      }
  }
}

void
decomp_rd (int n1, int n2, int n3, double *b1, double *b2, double *a, int nx,
	   int anx)
{
  if (n1 <= 4 && n2 <= 4)
    {
      decomp_rd_0 (n1, n2, n3, b1, b2, a, nx, anx);
      return;
    }
  if (n1 > n2)
    {
      int n1_1 = n1 / 2;
      int n1_2 = n1 - n1_1;
      {
	{
	  decomp_rd (n1_1, n2, n3, b1, b2, a, nx, anx);
	}
	{
	  decomp_rd (n1_2, n2, n3, b1 + n1_1, b2 + n1_1 * nx, a, nx, anx);
	}
      }
    }
  else
    {
      int n2_1 = n2 / 2;
      int n2_2 = n2 - n2_1;
      {
	{
	  decomp_rd (n1, n2_1, n3, b1, b2, a, nx, anx);
	}
	{
	  decomp_rd (n1, n2_2, n3, b1 + n2_1 * nx, b2, a + n2_1 * anx, nx,
		     anx);
	}
      }
    }
}

void
decomp_lu_1 (int n, double *a, int nx, int anx)
{
  int i;
  {
    i = 0;
    for (; i < n; i += 16)
      {
	int n1 = 16;
	if (n1 > n - i)
	  {
	    n1 = n - i;
	  }
	decomp_d_0 (i, n1, a, a + i * nx, nx, anx);
	decomp_rd (i, n1, n - i, a + i * nx, a + i, a + (i * nx + i), nx,
		   anx);
	decomp_lu_0 (n1, a + (i * nx + i), nx, anx);
	decomp_r_0 (n1, n - i - n1, a + (i * nx + i), a + (i * nx + i + n1),
		    nx, anx);
      }
  }
}

void
genmat (int n, double *a, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for (; i < n; i++)
      {
	j = 0;
	for (; j < n; j++)
	  {
	    a[i * nx + j] = n - abs (i - j);
	  }
      }
  }
}

void
printmat (int n, double *a, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for (; i < n; i++)
      {
	j = 0;
	for (; j < n; j++)
	  {
	    fprintf (stderr, "%6.3lf%c", a[i * nx + j],
		     j == n - 1 ? '\n' : ' ');
	  }
      }
  }
  putc ('\n', stderr);
}

void
copy_l (int n, double *a, double *l, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for (; i < n; i++)
      {
	j = 0;
	for (; j < n; j++)
	  {
	    l[i * nx + j] = i < j ? 0.0 : a[i * nx + j];
	  }
      }
  }
}

void
copy_u (int n, double *a, double *u, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for (; i < n; i++)
      {
	j = 0;
	for (; j < n; j++)
	  {
	    u[i * nx + j] = i == j ? 1.0 : (i > j ? 0.0 : a[i * nx + j]);
	  }
      }
  }
}

void
transpose (int n, double *a, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for (; i < n; i++)
      {
	j = 0;
	for (; j < i; j++)
	  {
	    double t1 = a[i * nx + j];
	    double t2 = a[j * nx + i];
	    a[i * nx + j] = t2, a[j * nx + i] = t1;
	  }
      }
  }
}

void
matmul (int n, double *a, double *b, double *c, int nx)
{
  int bs = 12;
  int i0;
  int j0;
  int i;
  int j;
  int k;
  transpose (n, b, nx);
  {
    i0 = 0;
    for (; i0 < n; i0 += bs)
      {
	j0 = 0;
	for (; j0 < n; j0 += bs)
	  {
	    i = i0;
	    for (; i < i0 + bs && i < n; i++)
	      {
		j = j0;
		for (; j < j0 + bs && j < n; j++)
		  {
		    double s = 0.0;
		    {
		      k = 0;
		      for (; k < n; k++)
			{
			  s += a[i * nx + k] * b[j * nx + k];
			}
		    }
		    c[i * nx + j] = s;
		  }
	      }
	  }
      }
  }
}

void
diffmat (int n, double *a, double *b, int nx)
{
  double s = 0.0;
  int i;
  int j;
  {
    i = 0;
    for (; i < n; i++)
      {
	j = 0;
	for (; j < n; j++)
	  {
	    s +=
	      (a[i * nx + j] - b[i * nx + j]) * (a[i * nx + j] -
						 b[i * nx + j]);
	  }
      }
  }
  fprintf (stderr, "diff: %lf\n", s);
}

void
swap_doubles (double *a, int n)
{
  int i;
  {
    i = 0;
    for (; i < n; i++)
      {
	unsigned long *xp = (unsigned long *) (a + i);
	unsigned long x0 = xp[0];
	unsigned long x1 = xp[1];
	unsigned long y0 = x0;
	unsigned long y1 = x1;
	y0 = y0 << 8 | (x0 >>= 8) & 255;
	y0 = y0 << 8 | (x0 >>= 8) & 255;
	y0 = y0 << 8 | x0 >> 8;
	y1 = y1 << 8 | (x1 >>= 8) & 255;
	y1 = y1 << 8 | (x1 >>= 8) & 255;
	y1 = y1 << 8 | x1 >> 8;
	xp[1] = y0;
	xp[0] = y1;
      }
  }
}

union int_4
{
  int i;
  unsigned char c[4];
};

void
input_mat (double *a, int n1, int n2, int nx)
{
  int i;
  int e;
  union int_4 x;
  x.i = 1;
  scanf (" (");
  read_to_eol ();
  scanf (" %*d %*d %d", &e);
  read_to_eol ();
  {
    i = 0;
    for (; i < n1; i++)
      {
	size_t sz = 0;
	while (sz < n2)
	  {
	    sz += fread (a + (i * nx + sz), sizeof (double), n2 - sz, stdin);
	  }
      }
  }
  scanf (" )");
  if ((int) (x.c)[0] != e)
    {
      i = 0;
      for (; i < n1; i++)
	{
	  swap_doubles (a + i * nx, n2);
	}
    }
}

void
output_mat (double *a, int n1, int n2, int nx)
{
  int i;
  union int_4 x;
  x.i = 1;
  printf (" (\n %d %d %d\n", sizeof (double) * n1 * n2, sizeof (double),
	  (int) (x.c)[0]);
  {
    i = 0;
    for (; i < n1; i++)
      {
	size_t sz = 0;
	while (sz < n2)
	  {
	    sz +=
	      fwrite (a + (i * nx + sz), sizeof (double), n2 - sz, stdout);
	  }
      }
  }
  printf (" )");
}

void
input_mat_l (double *a, int n1, int nx)
{
  int i;
  int e;
  union int_4 x;
  x.i = 1;
  scanf (" (");
  read_to_eol ();
  scanf (" %*d %*d %d", &e);
  read_to_eol ();
  {
    i = 0;
    for (; i < n1; i++)
      {
	size_t sz = 0;
	while (sz <= i)
	  {
	    sz +=
	      fread (a + (i * nx + sz), sizeof (double), i + 1 - sz, stdin);
	  }
      }
  }
  scanf (" )");
  if ((int) (x.c)[0] != e)
    {
      i = 0;
      for (; i < n1; i++)
	{
	  swap_doubles (a + i * nx, i + 1);
	}
    }
}

void
output_mat_l (double *a, int n1, int nx)
{
  int i;
  union int_4 x;
  x.i = 1;
  printf (" (\n %d %d %d\n", sizeof (double) * n1 * (n1 + 1) / 2,
	  sizeof (double), (int) (x.c)[0]);
  {
    i = 0;
    for (; i < n1; i++)
      {
	size_t sz = 0;
	while (sz <= i)
	  {
	    sz +=
	      fwrite (a + (i * nx + sz), sizeof (double), i + 1 - sz, stdout);
	  }
      }
  }
  printf (" )");
}

void
input_mat_u (double *a, int n1, int nx)
{
  int i;
  int e;
  union int_4 x;
  x.i = 1;
  scanf (" (");
  read_to_eol ();
  scanf (" %*d %*d %d", &e);
  read_to_eol ();
  {
    i = 0;
    for (; i < n1; i++)
      {
	size_t sz = 0;
	while (sz < n1 - i - 1)
	  {
	    sz +=
	      fread (a + (i * nx + i + 1 + sz), sizeof (double),
		     n1 - i - 1 - sz, stdin);
	  }
      }
  }
  scanf (" )");
  if ((int) (x.c)[0] != e)
    {
      i = 0;
      for (; i < n1; i++)
	{
	  swap_doubles (a + (i * nx + i + 1), n1 - i - 1);
	}
    }
}

void
output_mat_u (double *a, int n1, int nx)
{
  int i;
  union int_4 x;
  x.i = 1;
  printf (" (\n %d %d %d\n", sizeof (double) * n1 * (n1 - 1) / 2,
	  sizeof (double), (int) (x.c)[0]);
  {
    i = 0;
    for (; i < n1; i++)
      {
	size_t sz = 0;
	while (sz < n1 - i - 1)
	  {
	    sz +=
	      fwrite (a + (i * nx + i + 1 + sz), sizeof (double),
		      n1 - i - 1 - sz, stdout);
	  }
      }
  }
  printf (" )");
}

void
zero_mat (double *a, int n1, int n2, int nx)
{
  int i;
  int j;
  {
    i = 0;
    for (; i < n1; i++)
      {
	j = 0;
	for (; j < n2; j++)
	  {
	    a[i * nx + j] = 0.0;
	  }
      }
  }
}

void
proc_rslt (double *a)
{
  printf ("%lf", *a);
}

void
proc_cmd (int n1, int n2, int n3, double *ansp)
{
  struct timeval tp[2];
  int n = n1;
  int al = n2;
  int d = n3;
  int nx;
  double *a;
  double *l;
  double *u;
  double *c;
  nx = n;
  a = (double *) malloc (sizeof (double) * n * n);
  genmat (n, a, nx);
  if (d > 1)
    {
      printmat (n, a, nx);
    }
  gettimeofday (tp, 0);
  switch (al)
    {
    case 1:
      decomp_lu_0 (n, a, nx, nx);
      break;
    case 2:
      decomp_lu (n, a, nx, nx);
      break;
    case 3:
      decomp_lu_1 (n, a, nx, nx);
      break;
    }
  gettimeofday (tp + 1, 0);
  *ansp = elapsed_time (tp);
  fprintf (stderr, "%lf\n", *ansp);
  if (d > 0)
    {
      l = (double *) malloc (n * n * sizeof (double));
      u = (double *) malloc (n * n * sizeof (double));
      c = (double *) malloc (n * n * sizeof (double));
      if (d > 1)
	{
	  printmat (n, a, nx);
	}
      copy_l (n, a, l, nx);
      copy_u (n, a, u, nx);
      if (d > 1)
	{
	  printmat (n, l, nx);
	}
      if (d > 1)
	{
	  printmat (n, u, nx);
	}
      matmul (n, l, u, c, nx);
      if (d > 1)
	{
	  printmat (n, c, nx);
	}
      genmat (n, a, nx);
      diffmat (n, a, c, nx);
      free (l);
      free (u);
      free (c);
    }
  free (a);
}

void
do_task_lu_task (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int nx = (*pthis).nx;
  int anx = (*pthis).tmpnx ? (*pthis).tmpnx : (*pthis).anx;
  fprintf (stderr, "start %d\n", (*pthis).tt);
  switch ((*pthis).tt)
    {
    case TASK_CMD:
      proc_cmd (n1, n2, n3, a);
      break;
    case TASK_LU:
      decomp_lu (n1, a, nx, anx);
      break;
    case TASK_R:
      decomp_r (n1, n2, b1, a, nx, anx);
      break;
    case TASK_D:
      decomp_d (n1, n2, b1, a, nx, anx);
      break;
    case TASK_RD:
      decomp_rd (n1, n2, n3, b1, b2, a, nx, anx);
      break;
    }
}

void
send_task_lu_task (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int nx = (*pthis).nx;
  int anx = (*pthis).anx;
  send_int ((*pthis).tt);
  switch ((*pthis).tt)
    {
    case TASK_LU:
      send_int (n1);
      output_mat (a, n1, n1, anx);
      break;
    case TASK_R:
      send_int (n1);
      send_int (n2);
      output_mat_l (b1, n1, nx);
      output_mat (a, n1, n2, anx);
      break;
    case TASK_D:
      send_int (n1);
      send_int (n2);
      output_mat_u (b1, n1, nx);
      output_mat (a, n2, n1, anx);
      break;
    case TASK_RD:
      send_int (n1);
      send_int (n2);
      send_int (n3);
      output_mat (b1, n2, n1, nx);
      output_mat (b2, n1, n3, nx);
      break;
    }
}

struct task_lu *
recv_task_lu_task (struct thread_data *_thr)
{
  struct task_lu *pthis = malloc (sizeof (struct task_lu));
  enum task_tp2 tt;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  static double ans;
  tt = recv_int ();
  switch (tt)
    {
    case TASK_CMD:
      n1 = recv_int ();
      n2 = recv_int ();
      n3 = recv_int ();
      a = &ans;
      break;
    case TASK_LU:
      n1 = recv_int ();
      nx = n1;
      a = (double *) malloc (sizeof (double) * n1 * n1);
      input_mat (a, n1, n1, nx);
      break;
    case TASK_R:
      n1 = recv_int ();
      n2 = recv_int ();
      nx = n1 + n2;
      b1 = (double *) malloc (sizeof (double) * n1 * nx);
      input_mat_l (b1, n1, nx);
      a = b1 + n1;
      input_mat (a, n1, n2, nx);
      break;
    case TASK_D:
      n1 = recv_int ();
      n2 = recv_int ();
      nx = n1;
      b1 = (double *) malloc (sizeof (double) * (n1 + n2) * nx);
      input_mat_u (b1, n1, nx);
      a = b1 + n1 * nx;
      input_mat (a, n2, n1, nx);
      break;
    case TASK_RD:
      n1 = recv_int ();
      n2 = recv_int ();
      n3 = recv_int ();
      nx = n1 + n3;
      a = (double *) malloc (sizeof (double) * (n1 + n2) * nx);
      b1 = a + n1 * nx;
      b2 = a + n1;
      a += n1 * nx + n1;
      input_mat (b1, n2, n1, nx);
      input_mat (b2, n1, n3, nx);
      zero_mat (a, n2, n3, nx);
      break;
    }
  (*pthis).tt = tt;
  (*pthis).n1 = n1;
  (*pthis).n2 = n2;
  (*pthis).n3 = n3;
  (*pthis).b1 = b1;
  (*pthis).b2 = b2;
  (*pthis).a = a;
  (*pthis).nx = nx;
  (*pthis).anx = nx;
  (*pthis).tmpnx = 0;
  return pthis;
}

void
send_task_lu_rslt (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int anx = (*pthis).anx;
  switch ((*pthis).tt)
    {
    case TASK_CMD:
      proc_rslt (a);
      break;
    case TASK_LU:
      output_mat (a, n1, n1, anx);
      free (a);
      break;
    case TASK_R:
      output_mat (a, n1, n2, anx);
      free (b1);
      break;
    case TASK_D:
      output_mat (a, n2, n1, anx);
      free (b1);
      break;
    case TASK_RD:
      output_mat (a, n2, n3, anx);
      a -= n1 * anx + n1;
      free (a);
      break;
    }
  free (pthis);
}

void
recv_task_lu_rslt (struct thread_data *_thr, struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int tmpnx = (*pthis).tmpnx;
  int anx = (*pthis).anx;
  fprintf (stderr, "recv-rslt-body\n");
  switch ((*pthis).tt)
    {
    case TASK_LU:
      input_mat (a, n1, n1, anx);
      break;
    case TASK_R:
      input_mat (a, n1, n2, anx);
      break;
    case TASK_D:
      input_mat (a, n2, n1, anx);
      break;
    case TASK_RD:
      if (tmpnx == 0)
	{
	  tmpnx = n3;
	  (*pthis).a = (double *) malloc (sizeof (double) * n2 * n3);
	}
      input_mat ((*pthis).a, n2, n3, tmpnx);
      break;
    }
  fprintf (stderr, "recv-rslt-body fin\n");
}

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_task_lu_task};
void (*task_senders[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) send_task_lu_task};
void *(*task_receivers[256]) (struct thread_data *) =
{
(void *(*)(struct thread_data *)) recv_task_lu_task};
void (*rslt_senders[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) send_task_lu_rslt};
void (*rslt_receivers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) recv_task_lu_rslt};
