void *malloc (int);

struct list
{
  int data;
  struct list *next;
};

int
main ()
{
  {
    struct list *fst = 0;
    struct list *cur;

    {
      int i = 0;
      int j = 0;
      for (; i <= 10; i++, j += 2)
        {
          cur = malloc (sizeof (struct list));
          cur->data = i * j;
          cur->next = fst;
          fst = cur;
        }
    }
  }
}
