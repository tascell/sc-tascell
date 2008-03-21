#include<stdio.h>

int main(){
  char *p1 = "0123456789";
  char p2[] = "0123456789";

  printf("%d %d %d", sizeof p1, sizeof p2, sizeof "0123456789" );

  return( 0 );
}
