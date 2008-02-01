typedef int mat[2][2];

int printf (char const *, ...);

void
matmul (mat a, mat b, mat c)
{
  int i;
  int j;
  int k;
  {
    int i = 0;
    for (; i <= 1; i++)
      {
	int j = 0;
	for (; j <= 1; j++)
	  {
	    c[i][j] = 0;
	  }
      }
  }
  {
    int i = 0;
    for (; i <= 1; i++)
      {
	int j = 0;
	for (; j <= 1; j++)
	  {
	    int k = 0;
	    for (; k <= 1; k++)
	      {
		typedef int l;
		c[i][j] += a[i][k] * b[k][j];
	      }
	  }
      }
  }
  unsigned char k__2;
  return;
}

int
main ()
{
  {
    mat a = { {1, 2}, {3, 4} };
    mat b = { {5, 6}, {7, 8} };
    mat c;
    matmul (a, b, c);
    printf ("a:%d %d %d %d \\n", a[0][0], a[0][1], a[1][0], a[1][1]);
    printf ("b:%d %d %d %d \\n", b[0][0], b[0][1], b[1][0], b[1][1]);
    printf ("c:%d %d %d %d \\n", c[0][0], c[0][1], c[1][0], c[1][1]);
    return;
  }
}
