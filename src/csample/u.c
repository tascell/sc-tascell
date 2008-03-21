int main( argc, argv )
char *argv[];
int argc;
{
  switch( argc ){
    case 'a':
      return( '\n' );
    case 'c':
      return( '\xc1' );
    case 'e':
      return( '\032' );
    default:
      return( 30L );
  }
}

