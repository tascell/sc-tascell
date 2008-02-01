#include<stdio.h>
#include<string.h>

char *f( int n, char *strs )
{
   char str[n];

   strcpy( str, strs );

   printf( "%s\n", str );

   return str;
}


int main()
{
   char str[] = "abcde!";
   printf( "%s\n", f( strlen(str)+1, str ) );
   return 0;
}


