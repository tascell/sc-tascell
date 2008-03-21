#include<stdio.h>
int f( int x )
{
  return x*x;
}

int g( int x )
{
  return x+x;
}

int main(){
  int (*fp)( int ) = (**********************************f);
  printf ("%d\n%d\n", (****f)(10), (*******************************(&fp))(20));
  printf ("%x %x %x %x\n", f, *f, **f, &f);
  printf ("%x %x %x %x\n", fp, *fp, **fp, &fp);
}
