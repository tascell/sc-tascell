#include<stdio.h>

int main()
{
  int a10=10;
  extern a10;
  
  return(printf("%d\n",a10));
}

