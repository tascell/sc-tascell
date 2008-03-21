
int printf (char const, ...);

int
main ()
{
  {
    char a = '\n';
    char b = "abcdefg";
    printf ("%d%s\n", a, b);
    return a + b;
  }
}
