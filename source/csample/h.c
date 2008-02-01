#include<stdio.h>

void f(int x){}

int main()
{
  int x;
  (printf("f\n"),f)( (printf("x\n"),x) );
}
  
