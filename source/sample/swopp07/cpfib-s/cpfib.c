int cpfib(void (*save0) lightweight (), int n)
{
  int pc = 0;         /* pseudo program counter */
  int s = 0;
  void save1 lightweight (){ /* nested function */
    save0();          /* saving caller's state */
    save_pc(pc);      /* saving pc state */
    save_int(n);      /* saving variable state */
    save_int(s);      /* saving variable state */
  }
  if (n <= 2) return 1;
  pc = 1;   /* inc program counter before call */
  s += cpfib(save1, n-1);
  pc = 2;   /* inc program counter before call */
  s += cpfib(save1, n-2);
  return s;
}
