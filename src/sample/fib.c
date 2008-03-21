
void
send_fib_task2 (struct thread_data *_thr, struct _task_fib0 *pthis)
{
  struct _task_fib0 this = *pthis;

  send_int (this.n);
}

void
recv_fib_task3 (struct thread_data *_thr)
{
  struct _task_fib0 *pthis = malloc (sizeof (struct _task_fib0));
  struct _task_fib0 this = *pthis;

  recv_int (&this.n);
}

void
send_fib_rslt4 (struct thread_data *_thr, struct _task_fib0 *pthis)
{
  struct _task_fib0 this = *pthis;

  send_int (this.r);
  free (pthis);
}

void
recv_fib_rslt5 (struct thread_data *_thr, struct _task_fib0 *pthis)
{
  struct _task_fib0 this = *pthis;

  recv_int (&this.r);
}

int fib (int (*bk) (), struct thread_data *_thr, int n);

struct _task_fib0
{
  int n;
  int r;
};

void
do_fib_task1 (struct thread_data *_thr, struct _task_fib0 *this)
{
  struct _task_fib0 this = *pthis;

  int _bk ()
  {
    return 0;
  }
  (this.r = fib (bk, _thr, this.n)) ();
}

int
fib (int (*bk) (), struct thread_data *_thr, int n)
{
  if (n <= 2)
    {
      return 1;
    }
  else
    {
      int s1;
      int s2;

      {
	struct _task_fib0 this;
	int spawned6 = 0;

	if (divisibility_flag == 1)
	  send_divisible ();
	{
	  int (*_bk2) ();

	  int _bk ()
	  {
	    if (spawned6)
	      return 0;
	    _bk2 ();
	    if ((*_thr).treq_top)
	      {
		this.n = n - 2;
		spawned6 = 1;
		make_and_send_task (_thr, this);
		return 1;
	      }
	    return 0;
	  }
	  if ((*_thr).req)
	    handre_req (bk, _thr);
	  (s1 = fib (bk, _thr, n - 1)) ();
	  if (spawned6)
	    {
	      wait_rslt (_thr);
	      s2 = this.r;
	    }
	  else
	    {
	      (s2 = fib (bk, _thr, n - 2)) ();
	    }
	}
      }
      return s1 + s2;
    }
}
void (*task_bodies[1]) (struct thread_data *, void *) =
{
do_fib_task1};
void (*task_senders[1]) (struct thread_data *, void *) =
{
send_fib_task2};
void (*task_receivers[1]) (struct thread_data *) =
{
recv_fib_task3};
void (*rslt_senders[1]) (struct thread_data *, void *) =
{
send_fib_rslt4};
void (*rslt_receivers[1]) (struct thread_data *, void *) =
{
recv_fib_rslt5};
