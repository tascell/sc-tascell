#include<stdio.h>
#include<math.h>

int main()
{
  int k;
  
  printf ("%f", pow(2,3));

  for( k=1 ; k<=100 ; k++ ){
    printf("1- %f\t= ", pow( (1-(1/(double)k)), k));
    printf("%f\n", 1- pow( (1-(1/(double)k)), k));
  }
}
