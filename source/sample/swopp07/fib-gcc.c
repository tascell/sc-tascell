int count = 0;
int scan_inc = 3;
int scan_start = 10;

int printf (char const *, ...);

int atoi (char const *);

int
fib (void (*scan0) (void), int n)
{

  void scan1 (void)
  {
    printf ("<fib ");
    printf ("n=0x%x ", (int) n);
    printf (">\n");
    scan0 ();
  }
  if (n <= 2)
    return 1;
  else
    return ((count += scan_inc) > scan_start ? (count -=
						scan_start, scan1 ()) : 0,
	    fib (scan1, n - 2)) + ((count += scan_inc) > scan_start ? (count -=
								       scan_start,
								       scan1 ())
				   : 0, fib (scan1, n - 1));
}

int
main (int argc, char **argv)
{
  int a = 22;

  void scan1 (void)
  {
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
  if (argc > 1)
    a = atoi (argv[1]);
  printf ("fib(%d) = %d\n", a,
	  ((count += scan_inc) > scan_start ? (count -=
					       scan_start, scan1 ()) : 0,
	   fib (scan1, a)));
  return 0;
}
