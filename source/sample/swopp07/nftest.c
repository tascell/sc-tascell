
void foo ( void (*nf0)(void) ) {
  int i;
  for (i=0; i<10000000 ; i++)
    nf0();
}
#ifndef NF
int a=0;
void nf (void) { a++;}
#endif

int main(void)
{
  int a=0;
#ifdef NF
  void nf (void) { a++;}
#endif
  foo(nf);
  return(0);
}

