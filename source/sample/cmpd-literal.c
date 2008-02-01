
int printf (char const *, ...);

void
f1 (void)
{
  int *p = (int[5]) { 50, 39 };
  *p = 30;
  p = (int[2])
  {
  *p};
  printf ("%d\n", p[0]);
}

struct s
{
  int i;
};

int
f2 (void)
{
  struct s *p = 0;
  struct s *q;
  int j = 0;

again:
  ;
  q = p;
  p = &((struct s)
	{
	j++});
  if (j < 2)
    goto again;
  return p == q && (*q).i == 1;
}

struct point
{
  int x;
  int y;
};

void
drawline (struct point from, struct point to)
{
  printf ("draw:(%d, %d) <--> (%d, %d)\n", from.x, from.y, to.x, to.y);
  return;
}

void
p_drawline (struct point *pfrom, struct point *pto)
{
  printf ("DRAW:(%d, %d) <--> (%d, %d)\n", (*pfrom).x, (*pfrom).y, (*pto).x,
	  (*pto).y);
  return;
}

struct int_list
{
  int car;
  struct int_list *cdr;
};

int
main (void)
{
  struct int_list endless_zeros = (struct int_list) { 0, &endless_zeros };
  drawline ((struct point)
	    {
	    .x = 1,.y = 1}, (struct point)
	    {
	    .y = 4,.x = 3});
  p_drawline (&((struct point)
		{
		.x = 1,.y = 1}), &((struct point)
				   {
				   .y = 4,.x = 3}));
  (float const[6])
  {
  1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  printf ("%s\n", (char[20])
	  {
	  "abcdefghij"});
  f1 ();
  printf ("(f2) = %d\n", f2 ());
  return 0;
}
