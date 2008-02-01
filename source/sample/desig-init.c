
int printf (char const *, ...);
typedef struct
{
  int quot;
  int rem;
} div_t;

union uni
{
  int any_member;
  float flt_member;
};

struct sab
{
  int a[3];
  int b;
};

int
main (void)
{
  char s1[] = "abc";
  char t1[3] = "abc";
  char *p = "abc";
  char const *nm[2] = {[0] = "member zero",[1] = "member one" };
  div_t answer = {.quot = 2,.rem = -1 };
  struct sab w[3] = {[0].a = {1},[1].a[0] = 2 };
  int a[10] = { 1, 3, 5, 7, 9,[10 - 5] = 8, 6, 4, 2, 0 };
  union uni u = {.any_member = 42 };
  return 0;
}
