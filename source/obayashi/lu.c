#include "worker.h"
void outp_int(int n){ printf(" %d", n); }
int  inp_int(){ int t; scanf(" %d", &t); return t; }

/* app */

// #define A_ALLOC_MSG

double
elapsed_time(struct timeval tp[2])
{
  return tp[1].tv_sec-tp[0].tv_sec+1e-6*(tp[1].tv_usec-tp[0].tv_usec);
}


/*
  (0,0)     ...   (0, nx-1)
  (1,0)     ...   (1, nx-1)
   ...             ...
  (ny-1,0)  ...   (ny-1, nx-1)
*/

/* eventually, may use different nx(s) */

void decomp_lu(FF, int n, double *a, int nx, int anx);
void decomp_r(FF, int n1, int n2, double *b, double *a, int nx, int anx);
void decomp_d(FF, int n1, int n2, double *b, double *a, int nx, int anx);
void decomp_rd(FF, int n1, int n2, int n3, double *b1, double *b2, 
	       double *a, int nx, int anx);

void zero_mat(double *a, int n1, int n2, int nx);

typedef enum { TASK_CMD, TASK_LU, TASK_R, TASK_D, TASK_RD } task_tp;

struct task_lu {
  task_tp tt;
  int n1, n2, n3;
  double *b1, *b2, *a;
  int nx;
  int anx;
  int tmpnx;
};

void output_task(task_tp tt, int n1, int n2, int n3,
		 double *b1, double *b2, double *a, int nx);
void input_rslt(task_tp tt, int n1, int n2, int n3,
		double *b1, double *b2, double *a, int nx);

void put_task(struct task_lu *st, task_tp tt, int n1, int n2, int n3,
	      double *b1, double *b2, double *a, int nx, int anx){
  st->tt = tt; st->n1 = n1; st->n2 = n2;  st->n3 = n3; 
  st->b1 = b1; st->b2 = b2; st->a = a; st->nx = nx; st->anx = anx;
  st->tmpnx=0;
  if (a == 0){
    st->tmpnx = n3;
    st->a = (double *)malloc(sizeof(double)*n2*n3);
#ifdef A_ALLOC_MSG
    fprintf(stderr, "alloc st->a=%x\n", st->a);
#endif
    zero_mat(st->a, n2, n3, n3);
  }
}

void merge_task(struct task_lu *x, double *a, int anx){
  int n2=x->n2, n3=x->n3;
  double *tmp=x->a;
  int i, j;
  if (a == tmp) return;
  for(i=0;i<n2;i++)
    for(j=0;j<n3;j++)
      a[i*anx+j] += tmp[i*n3+j];
#ifdef A_ALLOC_MSG
  fprintf(stderr, "free  st->a=%x\n", tmp);
#endif
  free(tmp);
}

void
decomp_lu_0(int n, double *a, int nx, int anx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"lu_0: %d %d\n", n, a);
#endif
  {
    double w = 1.0/a[0];
    for(j=1;j<n;j++) a[j] *= w;
  }
  for(i=1;i<n;i++)
    {
      for(k=0;k<i-1;k++)
	{
	  double aik = a[i*anx+k];
	  for(j=k+1;j<n;j++) a[i*anx+j] -= aik * a[k*anx+j];
	}
      {
	double aik = a[i*anx+i-1];
	double w;
	a[i*anx+i] -= aik * a[(i-1)*anx+i];
	w = 1.0/a[i*anx+i];
	for(j=i+1;j<n;j++) a[i*anx+j] = w * (a[i*anx+j] - aik * a[(i-1)*anx+j]);
      }
    }
}

void
decomp_lu(FF, int n, double *a, int nx, int anx)
{
  struct timeval tp[2];
  
  if(n <= 4)
    {
      decomp_lu_0(n, a, nx, anx);
      return;
    }
  {
    /* int n1 = (n/4), n2 = n-n1; */
    /* int n1 = (n>8)?(n/4):(n/2), n2 = n-n1; */
    int n1 = (n>16)?(n/4):(n/2), n2 = n-n1;
    decomp_lu(FA, n1, a, nx, anx);
    // seq
    DO_TWO(struct task_lu, st, 
	   decomp_r(FA, n1, n2, a, a+n1, nx, anx),
	   decomp_d(FA, n1, n2, a, a+(n1*nx), nx, anx),
	   put_task(st, TASK_D, n1, n2, 0, a, 0, a+(n1*nx), nx, anx),
	   0);
    // output_task(TASK_D, n1, n2, 0, a, 0, a+(n1*nx), nx)
    // input_rslt(TASK_D, n1, n2, 0, a, 0, a+(n1*nx), nx)

    // seq
    decomp_rd(FA, n1, n2, n2, a+(n1*nx), a+n1, a+((n1*nx)+n1), nx, anx);
    // seq
    decomp_lu(FA, n2, a+((n1*nx)+n1), nx, anx);
  }
}


void
decomp_r_0(int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"r_0: %d %d %d %d\n", n1, n2, b, a);
#endif
  {
    double w = 1.0/b[0];
    for(j=0;j<n2;j++) a[j] *= w;
  }
  for(i=1;i<n1;i++)
    {
      for(k=0;k<i-1;k++)
	{
	  double aik = b[i*nx+k];
	  for(j=0;j<n2;j++) a[i*anx+j] -= aik * a[k*anx+j];
	}
      {
	double aik = b[i*nx+i-1];
	double w = 1.0/b[i*nx+i];
	for(j=0;j<n2;j++) a[i*anx+j] = w * (a[i*anx+j] - aik * a[(i-1)*anx+j]);
      }
    }


}

void
decomp_r(FF, int n1, int n2, double *b, double *a, int nx, int anx)
{
  if(n1 <= 4)
    {
      decomp_r_0(n1, n2, b, a, nx, anx);
      return;
    }
  {
    int n1_1 = n1/2, n1_2 = n1-n1_1;
    decomp_r(FA, n1_1, n2, b, a, nx, anx);
    // seq
    decomp_rd(FA, n1_1, n1_2, n2, b+(n1_1*nx), a, a+(n1_1*nx), nx, anx);
    // seq
    decomp_r(FA, n1_2, n2, b+(n1_1*nx+n1_1), a+(n1_1*nx), nx, anx);
  }
}

void
decomp_d_0(int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"d_0: %d %d %d %d\n", n1, n2, b, a);
#endif
  for(i=0;i<n2;i++)
    {
      for(k=0;k<n1-1;k++)
	{
	  double aik = a[i*anx+k];
	  for(j=k+1;j<n1;j++) a[i*anx+j] -= aik * b[k*nx+j];
	}

    }
}

void
decomp_d(FF, int n1, int n2, double *b, double *a, int nx, int anx)
{
  if(n2 <= 4)
    {
      decomp_d_0(n1, n2, b, a, nx, anx);
      return;
    }
  {
    int n2_1 = n2/2, n2_2 = n2-n2_1;
    DO_TWO(struct task_lu, st, decomp_d(FA, n1, n2_1, b, a, nx, anx),
	   decomp_d(FA, n1, n2_2, b, a+(n2_1*nx), nx, anx),
	   put_task(st, TASK_D, n1, n2_2, 0, b, 0, a+(n2_1*nx), nx, anx),
	   0);
    // output_task(TASK_D, n1, n2_2, 0, b, 0, a+(n2_1*nx), nx),
    // input_rslt(TASK_D, n1, n2_2, 0, b, 0, a+(n2_1*nx), nx)
  }
}

void
decomp_rd_0(int n1, int n2, int n3, 
	    double *b1, double *b2, double *a, int nx, int anx)
{
  int i,j,k;
#if 0
  fprintf(stderr,"rd_0: %d %d %d %d %d %d\n", n1, n2, n3, b1, b2, a);
#endif
  for(i=0;i<n2;i++)
    for(k=0;k<n1;k++)
      {
	double aik = b1[i*nx+k];
	for(j=0;j<n3;j++)
	  a[i*anx+j] -= aik * b2[k*nx+j];
      }
}

void
decomp_rd(FF, int n1, int n2, int n3, 
	  double *b1, double *b2, double *a, int nx, int anx)
{
  if(n1 <= 4 && n2 <= 4)
    {
      decomp_rd_0(n1, n2, n3, b1, b2, a, nx, anx);
      return;
    }
  if(n1 > n2)
    {
      int n1_1 = n1/2, n1_2 = n1 - n1_1;
      DO_TWO(struct task_lu, st,
	     decomp_rd(FA, n1_1, n2, n3, b1, b2, a, nx, anx),
	     decomp_rd(FA, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx, anx),
	     put_task(st, TASK_RD, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), 0, nx, anx),
	     merge_task(st, a, anx));
      // output_task(TASK_RD, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx),
      // input_rslt(TASK_RD, n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx)
    }
  else
    {
      int n2_1 = n2/2, n2_2 = n2 - n2_1;
      DO_TWO(struct task_lu, st,
	     decomp_rd(FA, n1, n2_1, n3, b1, b2, a, nx, anx),
	     decomp_rd(FA, n1, n2_2, n3, b1+(n2_1*nx), b2, a+(n2_1*anx), nx, anx),
	     put_task(st, TASK_RD, n1, n2_2, n3, 
		      b1+(n2_1*nx), b2, a+(n2_1*anx), nx, anx),
	     merge_task(st, a+(n2_1*anx), anx));
      // output_task(TASK_RD, n1, n2_2, n3, 
      //    b1+(n2_1*nx), b2, a+(n2_1*nx), nx),
      // input_rslt(TASK_RD, n1, n2_2, n3, 
      //    b1+(n2_1*nx), b2, a+(n2_1*nx), nx)
    }
}


void
decomp_lu_1(FF, int n, double *a, int nx, int anx)
{
  int i;
  for(i=0;i<n;i+=16){
    int n1 = 16;
    if (n1 > n-i) n1 = n-i;
    decomp_d_0(i, n1, a, a+(i*nx), nx, anx);
    decomp_rd(FA, i, n1, n-i, a+(i*nx), a+i, a+(i*nx+i), nx, anx);
    decomp_lu_0(n1, a+(i*nx+i), nx, anx);
    decomp_r_0(n1, n-i-n1, a+(i*nx+i), a+(i*nx+i+n1), nx, anx);
  }
}

void
genmat(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      a[i*nx+j] = n - abs(i-j);
}

void
printmat(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      fprintf(stderr,"%6.3lf%c", a[i*nx+j], (j==n-1) ? '\n' : ' ');
  putc('\n', stderr);
}

void
copy_l(int n, double *a, double *l, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      l[i*nx+j] = (i<j) ? 0.0 : a[i*nx+j];
}

void
copy_u(int n, double *a, double *u, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      u[i*nx+j] = (i==j) ? 1.0 : (i>j) ? 0.0 : a[i*nx+j];
}

void
transpose(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++) 
    for(j=0;j<i;j++)
      {
	double t1 = a[i*nx+j], t2 = a[j*nx+i];
	a[i*nx+j] = t2, a[j*nx+i] = t1;
      }
}

void
matmul(int n, double *a, double *b, double *c, int nx)
{
  int bs=12;
  int i0,j0,i,j,k;
  transpose(n, b, nx);
  for(i0=0;i0<n;i0+=bs)
    for(j0=0;j0<n;j0+=bs)
      for(i=i0;i<i0+bs && i<n;i++)
	for(j=j0;j<j0+bs && j<n;j++)
	  {
	    double s = 0.0;
	    for(k=0;k<n;k++) s += a[i*nx+k] * b[j*nx+k];
	    c[i*nx+j] = s;
	  }
}

void
diffmat(int n, double *a, double *b, int nx)
{
  double s=0.0;
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      s += (a[i*nx+j]-b[i*nx+j])*(a[i*nx+j]-b[i*nx+j]);
  fprintf(stderr,"diff: %lf\n", s);
}

/* */


void swap_doubles(double *a, int n){
  /* assuming that long is 32-bit, and double is 64-bit */
  int i;
  /* fprintf(stderr, "swapping...\n"); */
  for(i=0;i<n;i++){
    unsigned long *xp = (unsigned long *)(a+i);
    unsigned long x0 = xp[0], x1 = xp[1];
    unsigned long y0 = x0, y1 = x1;
    y0 = (y0 << 8) | ((x0 >>= 8) & 0xff);
    y0 = (y0 << 8) | ((x0 >>= 8) & 0xff);
    y0 = (y0 << 8) | ((x0 >> 8));
    y1 = (y1 << 8) | ((x1 >>= 8) & 0xff);
    y1 = (y1 << 8) | ((x1 >>= 8) & 0xff);
    y1 = (y1 << 8) | ((x1 >> 8));
    xp[1] = y0; xp[0] = y1;
  }
  /* fprintf(stderr, "swapping...done\n"); */
}

void input_mat(double *a, int n1, int n2, int nx){
  int i, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();
  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz < n2)
      sz += fread(a+(i*nx+sz), sizeof(double), n2-sz, stdin);
  }
  scanf(" )");
  if((int)x.c[0] != e)
    for(i=0;i<n1;i++)
      swap_doubles(a+(i*nx), n2);
}


void output_mat(double *a, int n1, int n2, int nx){
  int i;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  printf(" (\n %d %d %d\n", 
	 sizeof(double)*n1*n2, sizeof(double), (int)x.c[0]);
  for(i=0;i<n1;i++)
    {
      size_t sz = 0;
      while(sz < n2)
	sz += fwrite(a+(i*nx+sz), sizeof(double), n2-sz, stdout);
    }
  printf(" )");
}

void input_mat_l(double *a, int n1, int nx){
  int i, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();
  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz <= i)
      sz += fread(a+(i*nx+sz), sizeof(double), (i+1)-sz, stdin);
  }
  scanf(" )");
  if((int)x.c[0] != e)
    for(i=0;i<n1;i++)
      swap_doubles(a+(i*nx), i+1);
}

void output_mat_l(double *a, int n1, int nx){
  int i;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  printf(" (\n %d %d %d\n", 
	 sizeof(double)*n1*(n1+1)/2, sizeof(double), (int)x.c[0]);
  for(i=0;i<n1;i++)
    {
      size_t sz = 0;
      while(sz <= i)
	sz += fwrite(a+(i*nx+sz), sizeof(double), (i+1)-sz, stdout);
    }
  printf(" )");
}

void input_mat_u(double *a, int n1, int nx){
  int i, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;

  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();

  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz < n1-i-1)
      sz += fread(a+(i*nx+i+1+sz), sizeof(double), (n1-i-1)-sz, stdin);
  }
  scanf(" )");
  if((int)x.c[0] != e)
    for(i=0;i<n1;i++)
      swap_doubles(a+(i*nx+i+1), n1-i-1);
}

void output_mat_u(double *a, int n1, int nx){
  int i;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  printf(" (\n %d %d %d\n", 
	 sizeof(double)*n1*(n1-1)/2, sizeof(double), (int)x.c[0]);
  for(i=0;i<n1;i++)
    {
      size_t sz = 0;
      while(sz < n1-i-1)
	sz += fwrite(a+(i*nx+i+1+sz), sizeof(double), (n1-i-1)-sz, stdout);
    }
  printf(" )");
}

#if 0 
void input_merge_mat(double *a, int n1, int n2, int nx){
  int i, j, e;
  union { int i; unsigned char c[4]; } x;
  x.i = 1;
  scanf(" ("); read_to_eol(); 
  scanf(" %*d %*d %d", &e); read_to_eol();
  double *tmp = (double *)malloc(sizeof(double)*n2);
  for(i=0;i<n1;i++){
    size_t sz = 0;
    while(sz < n2)
      sz += fread(tmp+sz, sizeof(double), n2-sz, stdin);
    if((int)x.c[0] != e)
      swap_doubles(tmp, n2);
    for(j=0;j<n2;j++)
      a[i*nx+j] += tmp[j];
  }
  scanf(" )");
  free(tmp);
}
#endif

void zero_mat(double *a, int n1, int n2, int nx){
  int i, j;
  for(i=0;i<n1;i++)
    for(j=0;j<n2;j++)
      a[i*nx+j] = 0.0;
}

void send_task_body(struct thread_data *thr, void *x0){
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int nx=x->nx;
  int anx=x->anx;
  outp_int(x->tt); 
  switch(x->tt){
  case TASK_LU: 
    outp_int(n1);
    output_mat(a, n1, n1, anx);
    break;
  case TASK_R:
    outp_int(n1);
    outp_int(n2);
    output_mat_l(b1, n1, nx);
    output_mat(a, n1, n2, anx);
    break;
  case TASK_D:
    outp_int(n1);
    outp_int(n2);
    output_mat_u(b1, n1, nx);
    output_mat(a, n2, n1, anx);
    break;
  case TASK_RD:
    outp_int(n1);
    outp_int(n2);
    outp_int(n3);
    output_mat(b1, n2, n1, nx);
    output_mat(b2, n1, n3, nx);
    break;
  }
}

void *recv_task_body(struct thread_data *thr){
  struct task_lu *x = (struct task_lu *) malloc(sizeof(struct task_lu));
  task_tp tt; int n1; int n2; int n3;
  double *b1; double *b2; double *a; int nx;
  static double ans;
  tt = inp_int(); 
  switch(tt){
  case TASK_CMD:
    n1 = inp_int();
    n2 = inp_int();
    n3 = inp_int();
    a = &ans;
    break;
  case TASK_LU: 
    n1 = inp_int();
    nx = n1;
    a = (double *)malloc(sizeof(double)*n1*n1);
    input_mat(a, n1, n1, nx);
    break;
  case TASK_R:
    n1 = inp_int();
    n2 = inp_int();
    nx = n1 + n2;
    b1 = (double *)malloc(sizeof(double)*n1*nx);
    input_mat_l(b1, n1, nx);
    a = b1 + n1;
    input_mat(a, n1, n2, nx);
    break;
  case TASK_D:
    n1 = inp_int();
    n2 = inp_int();
    nx = n1;
    b1 = (double *)malloc(sizeof(double)*(n1+n2)*nx);
    input_mat_u(b1, n1, nx);
    a = b1 + (n1*nx);
    input_mat(a, n2, n1, nx);
    break;
  case TASK_RD:
    n1 = inp_int();
    n2 = inp_int();
    n3 = inp_int();
    nx = n1 + n3;
    a = (double *)malloc(sizeof(double)*(n1+n2)*nx);
    b1 = a + (n1*nx);
    b2 = a + n1;
    a += ((n1*nx)+n1);
    input_mat(b1, n2, n1, nx);
    input_mat(b2, n1, n3, nx);
    zero_mat(a, n2, n3, nx);
    break;
  }
  x->tt = tt; x->n1 = n1; x->n2 = n2; x->n3 = n3;
  x->b1 = b1; x->b2 = b2; x->a = a;
  x->nx = nx; x->anx = nx;
  x->tmpnx = 0;
  return x;
}

void proc_rslt(double *);

void send_rslt_body(struct thread_data *thr, void *x0){
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int anx=x->anx;
  switch(x->tt){
  case TASK_CMD:
    proc_rslt(a);
    break;
  case TASK_LU:
    output_mat(a, n1, n1, anx);
    free(a);
    break;
  case TASK_R:
    output_mat(a, n1, n2, anx);
    free(b1);
    break;
  case TASK_D:
    output_mat(a, n2, n1, anx);
    free(b1);
    break;
  case TASK_RD:
    output_mat(a, n2, n3, anx);
    a -= ((n1*anx)+n1);
    free(a);
    break;
  }
  free(x);
}

void recv_rslt_body(struct thread_data *thr, void *x0){
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int tmpnx=x->tmpnx;
  int anx=x->anx;
  
  fprintf(stderr, "recv_rslt_body\n");
  
  switch(x->tt){
  case TASK_LU:
    input_mat(a, n1, n1, anx);
    break;
  case TASK_R:
    input_mat(a, n1, n2, anx);
    break;
  case TASK_D:
    input_mat(a, n2, n1, anx);
    break;
  case TASK_RD:
    if (tmpnx == 0){
      tmpnx = n3;
      x->a = (double *)malloc(sizeof(double)*n2*n3);
#ifdef A_ALLOC_MSG
      fprintf(stderr, "alloc  x->a=%x\n", x->a);
#endif
    }
    input_mat(x->a, n2, n3, tmpnx);
    break;
  }
  fprintf(stderr, "recv_rlst_body fin\n");
}

void proc_cmd(FF, int, int, int, double *);

void do_task_body(struct thread_data *_thr, void *x0){
  int _bk(){ return 0; }
  struct task_lu *x = x0;
  int n1=x->n1, n2=x->n2, n3=x->n3;
  double *b1=x->b1, *b2=x->b2, *a=x->a;
  int nx=x->nx;
  int anx=(x->tmpnx)?(x->tmpnx):x->anx;
    
  fprintf(stderr, "start %d\n", x->tt);

  switch(x->tt){
  case TASK_CMD: proc_cmd(FA, n1, n2, n3, a); break;
  case TASK_LU:  decomp_lu(FA, n1, a, nx, anx); break;
  case TASK_R:   decomp_r(FA, n1, n2, b1, a, nx, anx); break;
  case TASK_D:   decomp_d(FA, n1, n2, b1, a, nx, anx); break;
  case TASK_RD:  decomp_rd(FA, n1, n2, n3, b1, b2, a, nx, anx); break;
  }
}

void proc_cmd(FF, int n1, int n2, int n3, double *ansp){
  struct timeval tp[2];
  int n = n1, al = n2, d = n3, nx;
  double *a, *l, *u, *c;
  nx = n;
  a = (double *)malloc(sizeof(double)*n*n);
#ifdef TEST_MSG_ALLOC
  fprintf(stderr, "id=%d alloc a=%x", _thr->id, a);
#endif
  genmat(n, a, nx);
  if(d>1) printmat(n, a, nx);

  gettimeofday(tp, 0);
  switch(al){
  case 1: decomp_lu_0(n, a, nx, nx); break;
  case 2: decomp_lu(FA, n, a, nx, nx); break;
  case 3: decomp_lu_1(FA, n, a, nx, nx); break;
  }
  gettimeofday(tp+1, 0);
  *ansp = elapsed_time(tp);
  fprintf(stderr, "%lf\n", *ansp);
  if(d>0){
    l = (double *)malloc(sizeof(double)*n*n);
    u = (double *)malloc(sizeof(double)*n*n);
    c = (double *)malloc(sizeof(double)*n*n);
    if(d>1)printmat(n, a, nx);
    copy_l(n, a, l, nx);
    copy_u(n, a, u, nx);
    if(d>1)printmat(n, l, nx);
    if(d>1)printmat(n, u, nx);
    matmul(n, l, u, c, nx);
    if(d>1)printmat(n, c, nx);
    genmat(n, a, nx);
    diffmat(n, a, c, nx);
    free(l); free(u); free(c);
  }
#ifdef TEST_MSG_ALLOC
  fprintf(stderr, "id=%d free a=%x", _thr->id, a);
#endif
  free(a);
}

void proc_rslt(double *a){ printf("%lf", *a); }
