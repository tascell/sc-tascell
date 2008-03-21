/*
 * This program checks that a function compiles correctly when it doesn't
 * have any incoming arguments or return value.  
 */

#include <stdio.h>
int i = 0;

cilk void func(void) {
     i++;
}

cilk void main(void) {
     spawn func();
     spawn func();
     spawn func();
     sync;

     if( i==3 ) {
	  printf("test_func_no_arg_no_ret OK\n");
     } else {
	  printf("test_func_no_arg_no_ret BAD\n");
     }
}

