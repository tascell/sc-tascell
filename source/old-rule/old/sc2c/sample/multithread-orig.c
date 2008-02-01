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

struct sync
{
  int n;
  cont c;
};

void
f_notify (cont_f c_p, struct sync *s)
{
  int ln = 0;

  void *f_notify_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  }
	return;
      }
    return;
    ((*s).n)++;
    if ((*s).n <= 0)
      {
	thr_resume ((*s).c);
      }
    else
      {
      }
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  ((*s).n)++;
  if ((*s).n <= 0)
    {
      thr_resume ((*s).c);
    }
  else
    {
    }
}

void
f_wait (cont_f c_p, struct sync *s)
{
  thst_ptr c0;
  int ln = 0;

  void *f_wait_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 1:
	    goto L1;
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  }
	return;
      }
    return;
    ((*s).n)--;
    if ((*s).n < 0)
      {
	{
	  c0 = cp;
	  c0->c = f_wait_c;
	  c0->stat = thr_suspended;
	  (*s).c = c0;
	  ln = 1;
	  return;
	}
      L1:
	;
      }
    else
      {
      }
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  ((*s).n)--;
  if ((*s).n < 0)
    {
      {
	c0 = thst_top++;
	c0->c = f_wait_c;
	c0->stat = thr_new_suspended;
	(*s).c = c0;
	ln = 1;
	scheduling ();
      }
    }
  else
    {
    }
}

int
g (cont_f c_p, int x, int y)
{
  int ln = 0;

  void *g_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  }
	return;
      }
    return;
    *((int *) c_p (cp, rsn_retval)) = (x + y) / 2;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  return (x + y) / 2;
}

void
fw1 (cont_f c_p, int n1, int n2, int *tab1, int *tab2, struct sync *s1,
     struct sync *s2)
{
  int i = n1;
  int tmp;
  int i2 = n1;
  int tmp2;
  int ln = 0;

  void *fw1_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 1:
	    goto L12;
	  case 2:
	    goto L2;
	  case 3:
	    goto L3;
	  case 4:
	    goto L4;
	  case 5:
	    goto L5;
	  case 6:
	    goto L6;
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  case 1:
	    return (void *) &tmp;
	  case 4:
	    return (void *) &tmp2;
	  }
	return;
      }
    return;
    {
      if (i < n2)
	{
	  do
	    {
	      ln = 1;
	      tmp = g (fw1_c, tab1[i], tab1[i - 1]);
	    L12:
	      ;
	      tab2[i] = tmp;
	      i++;
	    }
	  while (i < n2);
	}
      else
	{
	}
    }
    ln = 2;
    f_notify (fw1_c, s1);
  L2:
    ;
    ln = 3;
    f_wait (fw1_c, s2);
  L3:
    ;
    {
      if (i2 < n2)
	{
	  do
	    {
	      ln = 4;
	      tmp2 = g (fw1_c, tab1[i2 + 1], tab2[i2]);
	    L4:
	      ;
	      tab1[i2] = tmp2;
	      i2++;
	    }
	  while (i2 < n2);
	}
      else
	{
	}
    }
    ln = 5;
    f_notify (fw1_c, s1);
  L5:
    ;
    ln = 6;
    f_wait (fw1_c, s2);
  L6:
    ;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  {
    if (i < n2)
      {
	do
	  {
	    ln = 1;
	    tmp = g (fw1_c, tab1[i], tab1[i - 1]);
	    tab2[i] = tmp;
	    i++;
	  }
	while (i < n2);
      }
    else
      {
      }
  }
  ln = 2;
  f_notify (fw1_c, s1);
  ln = 3;
  f_wait (fw1_c, s2);
  {
    if (i2 < n2)
      {
	do
	  {
	    ln = 4;
	    tmp2 = g (fw1_c, tab1[i2 + 1], tab2[i2]);
	    tab1[i2] = tmp2;
	    i2++;
	  }
	while (i2 < n2);
      }
    else
      {
      }
  }
  ln = 5;
  f_notify (fw1_c, s1);
  ln = 6;
  f_wait (fw1_c, s2);
}

void
divide_work (cont_f c_p, int n, int m)
{
  int tab1[1024];
  int tab2[1024];
  struct sync s1 = { 0, 0 };
  struct sync s2 = { 0, 0 };
  int i = 0;
  int ln = 0;

  void *divide_work_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 1:
	    goto L13;
	  case 2:
	    goto L22;
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  }
	return;
      }
    return;
    {
      if (i < m)
	{
	  do
	    {
	      {
		ln = 1;
		thst_top->c = divide_work_c;
		thst_top->stat = thr_new_runnable;
		thst_top++;
	      }
	      {
		int ln = 0;

		void *nthr_c (thst_ptr cp, reason rsn)
		{
		  switch (rsn)
		    {
		    case rsn_cont:
		      switch (ln)
			{
			case 1:
			  goto L14;
			}
		      return;
		    case rsn_retval:
		      switch (ln)
			{
			}
		      return;
		    }
		  return;
		  ln = 1;
		  fw1 (nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
		L14:
		  ;
		  cp->c = c_p;
		  cp->stat = thr_runnable;
		  return;
		}
		ln = 1;
		fw1 (nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
	      }
	      if ((thst_top - 1)->stat != thr_new_runnable)
		scheduling ();
	      else
		thst_top--;
	    L13:
	      ;
	      ln = 2;
	      fw1 (divide_work_c, n / 2, n - 2, tab1, tab2, &s2, &s1);
	    L22:
	      ;
	      i++;
	    }
	  while (i < m);
	}
      else
	{
	}
    }
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  {
    if (i < m)
      {
	do
	  {
	    {
	      ln = 1;
	      thst_top->c = divide_work_c;
	      thst_top->stat = thr_new_runnable;
	      thst_top++;
	    }
	    {
	      int ln = 0;

	      void *nthr_c (thst_ptr cp, reason rsn)
	      {
		switch (rsn)
		  {
		  case rsn_cont:
		    switch (ln)
		      {
		      case 1:
			goto L14;
		      }
		    return;
		  case rsn_retval:
		    switch (ln)
		      {
		      }
		    return;
		  }
		return;
		ln = 1;
		fw1 (nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
	      L14:
		;
		cp->c = c_p;
		cp->stat = thr_runnable;
		return;
	      }
	      ln = 1;
	      fw1 (nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
	    }
	    if ((thst_top - 1)->stat != thr_new_runnable)
	      scheduling ();
	    else
	      thst_top--;
	    ln = 2;
	    fw1 (divide_work_c, n / 2, n - 2, tab1, tab2, &s2, &s1);
	    i++;
	  }
	while (i < m);
      }
    else
      {
      }
  }
}

int
msc_main (cont_f c_p, int argc, char **argv)
{
  int n = 100;
  int m = 50;
  int ln = 0;

  void *msc_main_c (thst_ptr cp, reason rsn)
  {
    switch (rsn)
      {
      case rsn_cont:
	switch (ln)
	  {
	  case 1:
	    goto L15;
	  }
	return;
      case rsn_retval:
	switch (ln)
	  {
	  }
	return;
      }
    return;
    ln = 1;
    divide_work (msc_main_c, n, m);
  L15:
    ;
    *((int *) c_p (cp, rsn_retval)) = 0;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  ln = 1;
  divide_work (msc_main_c, n, m);
  return 0;
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
