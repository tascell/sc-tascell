#include<stdio.h>
#include<stdlib.h>
struct _thstelm;
typedef enum
{ rsn_cont, rsn_retval } reason;
typedef void *(*cont_f) (struct _thstelm *, reason);
enum _stat
{ thr_new_suspended, thr_new_runnable, thr_suspended, thr_runnable,
      thr_scheduled };

struct _thstelm
{
  cont_f c;
  enum _stat stat;
};
typedef struct _thstelm thstelm;
typedef struct _thstelm *thst_ptr;
typedef thst_ptr cont;
thstelm thst[65536];
thst_ptr thst_top = thst;
typedef void (*schdexit) ();
schdexit cur_schd_exit = 0;
thst_ptr cur_schd_thst_top = thst;

void
scheduling ()
{
  __label__ L0;
  schdexit prev_exit = cur_schd_exit;
  thst_ptr prev_thst_top = cur_schd_thst_top;
  thst_ptr mythst_top = thst_top;

  void nonlocalexit ()
  {
    goto L0;
  }
L0:
  ;
  cur_schd_exit = nonlocalexit;
  cur_schd_thst_top = thst_top = mythst_top;
  while (1)
    {
      {
	thst_ptr cp;

	for (cp = prev_thst_top; cp < mythst_top; cp++)
	  {
	    if (cp->stat != thr_scheduled)
	      break;
	    if (cp == mythst_top)
	      if (prev_exit)
		prev_exit ();
	  }
      }
      {
	thst_ptr cp;
	cont_f cc;

	for (cp = thst_top - 1; cp >= thst; cp--)
	  {
	    if (cp->stat == thr_runnable || cp->stat == thr_new_runnable)
	      break;
	  }
	if (cp < thst)
	  {
	    fprintf (stderr, "No Active thread!\n");
	    exit (1);
	  }
	do
	  {
	    cc = cp->c;
	    cp->c = 0;
	    cp->stat = thr_scheduled;
	    cc (cp, rsn_cont);
	  }
	while (cp->stat == thr_runnable);
      }
      if (thst_top > thst && (thst_top - 1)->stat == thr_new_runnable)
	{
	  thst_top--;
	  break;
	}
    }
  cur_schd_exit = prev_exit;
  cur_schd_thst_top = prev_thst_top;
}

void
thr_resume (thst_ptr cp)
{
  if (cp->stat == thr_suspended)
    cp->stat = thr_runnable;
  else if (cp->stat == thr_new_suspended)
    cp->stat = thr_new_runnable;
}

#include<stdio.h>
#include<stdlib.h>

int
pfib (cont_f c_p, int n)
{
  int x;
  int y;
  int nn = 0;
  cont c = 0;
  int tmp2;
  thst_ptr c0;
  int ln = 0;

  void *pfib_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 1:
	    goto L1;
	  case 2:
	    goto L2;
	  case 3:
	    goto L3;
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  case 2:
	    return (void *) &tmp2;
	  }
	return;
      }
    return;
    if (n <= 2)
      {
	{
	  *((int *) c_p (cp, rsn_retval)) = 1;
	  cp->c = c_p;
	  cp->stat = thr_runnable;
	  return;
	}
      }
    else
      {
	{
	  {
	    {
	      ln = 1;
	      thst_top->c = pfib_c;
	      thst_top->stat = thr_new_runnable;
	      thst_top++;
	    }
	    {
	      int tmp;
	      int ln = 0;

	      void *nthr_c (thst_ptr cp, reason rsn)
	      {
		switch (rsn)
		  {
		  case rsn_cont:
		    switch (ln)
		      {
		      case 1:
			goto L12;
		      }
		    return;
		  case rsn_retval:
		    switch (ln)
		      {
		      case 1:
			return (void *) &tmp;
		      }
		    return;
		  }
		return;
		ln = 1;
		tmp = pfib (nthr_c, n - 1);
	      L12:
		;
		x = tmp;
		nn++;
		if (nn == 0)
		  {
		    thr_resume (c);
		  }
		else
		  {
		  }
		cp->c = c_p;
		cp->stat = thr_runnable;
		return;
	      }
	      ln = 1;
	      tmp = pfib (nthr_c, n - 1);
	      x = tmp;
	      nn++;
	      if (nn == 0)
		{
		  thr_resume (c);
		}
	      else
		{
		}
	    }
	    if ((thst_top - 1)->stat != thr_new_runnable)
	      scheduling ();
	    else
	      thst_top--;
	  L1:
	    ;
	    ln = 2;
	    tmp2 = pfib (pfib_c, n - 2);
	  L2:
	    ;
	    y = tmp2;
	    nn = nn - 1;
	    if (nn < 0)
	      {
		{
		  c0 = cp;
		  c0->c = pfib_c;
		  c0->stat = thr_suspended;
		  c = c0;
		  ln = 3;
		  return;
		}
	      L3:
		;
	      }
	    else
	      {
	      }
	    *((int *) c_p (cp, rsn_retval)) = x + y;
	    cp->c = c_p;
	    cp->stat = thr_runnable;
	    return;
	  }
	}
      }
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  if (n <= 2)
    {
      {
	return 1;
      }
    }
  else
    {
      {
	{
	  {
	    ln = 1;
	    thst_top->c = pfib_c;
	    thst_top->stat = thr_new_runnable;
	    thst_top++;
	  }
	  {
	    int tmp;
	    int ln = 0;

	    void *nthr_c (thst_ptr cp, reason rsn)
	    {
	      switch (rsn)
		{
		case rsn_cont:
		  switch (ln)
		    {
		    case 1:
		      goto L12;
		    }
		  return;
		case rsn_retval:
		  switch (ln)
		    {
		    case 1:
		      return (void *) &tmp;
		    }
		  return;
		}
	      return;
	      ln = 1;
	      tmp = pfib (nthr_c, n - 1);
	    L12:
	      ;
	      x = tmp;
	      nn++;
	      if (nn == 0)
		{
		  thr_resume (c);
		}
	      else
		{
		}
	      cp->c = c_p;
	      cp->stat = thr_runnable;
	      return;
	    }
	    ln = 1;
	    tmp = pfib (nthr_c, n - 1);
	    x = tmp;
	    nn++;
	    if (nn == 0)
	      {
		thr_resume (c);
	      }
	    else
	      {
	      }
	  }
	  if ((thst_top - 1)->stat != thr_new_runnable)
	    scheduling ();
	  else
	    thst_top--;
	  ln = 2;
	  tmp2 = pfib (pfib_c, n - 2);
	  y = tmp2;
	  nn = nn - 1;
	  if (nn < 0)
	    {
	      {
		c0 = thst_top++;
		c0->c = pfib_c;
		c0->stat = thr_new_suspended;
		c = c0;
		ln = 3;
		scheduling ();
	      }
	    }
	  else
	    {
	    }
	  return x + y;
	}
      }
    }
}

int
msc_main (cont_f c_p, int argc, char **argv)
{
  int n = 4;
  int tmp3;
  int ln = 0;

  void *msc_main_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 1:
	    goto L13;
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  case 1:
	    return (void *) &tmp3;
	  }
	return;
      }
    return;
    if (argc > 1)
      {
	n = (atoi ((argv[1])));
      }
    else
      {
      }
    ln = 1;
    tmp3 = pfib (msc_main_c, n);
  L13:
    ;
    printf ("fib(%d)=%d\n", n, tmp3);;
    *((int *) c_p (cp, rsn_retval)) = n;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  if (argc > 1)
    {
      n = (atoi ((argv[1])));
    }
  else
    {
    }
  ln = 1;
  tmp3 = pfib (msc_main_c, n);
  printf ("fib(%d)=%d\n", n, tmp3);;
  return n;
}

int
main (int argc, char **argv)
{
  __label__ MAIN_RET;
  int ln = 0;
  int msc_main_ret;

  void *main_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 0:
	    goto L0;
	  case 1:
	    goto L1;
	  }
	return;
      case rsn_retval:
	return (void *) &msc_main_ret;
      }
    return;
  L0:
    ln = 1;
    msc_main_ret = msc_main (main_c, argc, argv);
  L1:
    goto MAIN_RET;
  }
  (*thst_top).c = main_c;
  (*thst_top).stat = thr_runnable;
  thst_top++;
  do
    {
      scheduling ();
    }
  while (1);
MAIN_RET:
  return msc_main_ret;
}
