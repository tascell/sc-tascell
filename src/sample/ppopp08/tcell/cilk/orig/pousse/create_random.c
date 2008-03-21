#include <stdio.h>

typedef unsigned long long u64;

u64 r;

int  main()
{
     int  x;
     int  y;
     int  t;
     int  i;
     char s[32];
     char buf[32];

     srand48( 293 );
     lrand48 ();
     
     for (t=0; t<3; t++)
     {
	  printf("\n");
	  for (x=0; x<32; x++)
	  {
	       printf("\n");
	       for (y=0; y<32; y++)
	       {
		    if ((y%4)==0) printf("\n");

		    r = 0ULL; 
		    for (i=0; i<64; i+=8)
			 r |= ((((u64) lrand48()) >> 7) & 0xff ) << i;
		    
		    sprintf(buf, "%llx", r );
		    sprintf(s, "0x%s", buf );
		    sprintf(buf,"%s,                  ", s );
		    buf[20] = 0;
		    printf("%s", buf );
		    
	       }
	  }
     }

     printf("\n\n");

}
