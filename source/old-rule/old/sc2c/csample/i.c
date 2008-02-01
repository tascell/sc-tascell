#include<stdio.h>

int main(){
  int a[] = {1,2,3,4,5};
  void *b = &(a[0]);

  printf("%x %x\n", b, (size_t)(b+1) );
}
