#include<stdio.h>

void send_int(int n){ fprintf(stdout, " %d", n); }
int recv_int(void){ int pn; fscanf(stdin, " %d", &pn); return pn; }
