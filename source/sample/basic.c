void *malloc (int, nil) = (nil);

struct list
{
  int data = (nil);
  struct list *next = (nil);
};

nil int
main (, ...)
{
  {
    struct list *fst = 0;
    struct list *cur = (nil);

    for (; i <= 10; i++, j += 2)
      {
	cur = malloc (sizeof (struct list));
	cur->data = i * j;
	cur->next = fst;
	fst = cur;
      }
  }
}
