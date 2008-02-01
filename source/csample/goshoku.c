#include <stdio.h>

int main()
{
  /* a^b * cdef = abcdef  */
  {
    long a,b;
    long a_b, cdef;
    for( a=2 ; a<=9 ; a++ )
      for( b=2, a_b=a*a ; b<=9 && a_b<10000 ; b++, a_b *= a )
	{
	  long aboooo = a*100000 + b*10000;
	  for( cdef=1000 ; cdef<=9999 ; cdef++ )
	    if( a_b * cdef == aboooo + cdef )
	      printf("%d^%d*%d=%d\n", a,b,cdef,aboooo+cdef);
	}
  }

  /* ab^c * def = abcdef */
  {
    long ab,c;
    long ab_c, def;
    for( ab=10 ; ab<=99 ; ab++ )
      for( c=1, ab_c=ab ; c<=9 && ab_c<10000 ; c++, ab_c*=ab )
	{
	  long abcooo = ab*10000 + c*1000;
	  for( def=100 ; def<=999 ; def++ )
	    if( ab_c * def == abcooo + def )
	      printf("%d^%d*%d=%d\n",ab,c,def,abcooo+def);
	}
  }

  /* abc^d * ef = abcdef */
  {
    long abc,d;
    long abc_d, ef;
    for( abc=100 ; abc<=999 ; abc++ )
      for( d=1, abc_d=abc ; d<=9 && abc_d<100000 ; d++, abc_d*=abc )
	{
	  long abcdoo = abc*1000 + d*100;
	  for( ef=10 ; ef<=99 ; ef++ )
	    if( abc_d * ef == abcdoo + ef )
	      printf("%d^%d*%d=%d\n",abc,d,ef,abcdoo+ef);
	}
  }

  /* 2^bc * def = abcdef */
  {
    long bc,def;
    long z_bc;
    for( bc=10, z_bc=1024 ; bc<=99 && z_bc<10000 ; bc++, z_bc*=2 )
      {
	long abcooo = 200000 + bc*1000;
	for( def = 100 ; def<=999 ; def+=2 )
	  if( z_bc * def == abcooo + def )
	    printf("%d^%d*%d=%d\n",2,bc,def,abcooo+def);
      }
  }

  return 0;
}
