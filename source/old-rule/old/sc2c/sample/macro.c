
 int printf( int  const , ... );

 int main(  )
{
{
 int i = 0;
if( i < 3 )
 do 
{
 char str[3][10] = { "abcdefg\n", "hijklmn\n", "opqrstu\n" };
printf( "\n" );
{
 int i = 0;
if( i < 3 )
 do 
{
printf( str[i] );
}while( i++,i < 3 );
 }
}while( i++,i < 3 );
 }
}

 void swap( int *a, int *b )
{
 int buffer;
buffer = &a;
&a = &b;
&b = buffer;
return ;
}

 void swap_w( char **a, char **b )
{
 char *buffer;
buffer = &a;
&a = &b;
&b = buffer;
return ;
}

 void *(*gg( void *(*ff)( int , int  ) ))( int , int  );
typedef void *gg_t( void *(*)( int , int  ) );
