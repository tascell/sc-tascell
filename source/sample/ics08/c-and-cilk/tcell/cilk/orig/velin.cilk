#include <stdio.h>

int actives[2];

cilk int refine(void) {
	return 999;
}

cilk int main(void) {

	actives[2] = spawn refine();
	if( actives[2] == 999 ) {
		printf("velin OK\n");
        }
	else {
		printf("velin BAD\n");
	}

	return 0;
}
