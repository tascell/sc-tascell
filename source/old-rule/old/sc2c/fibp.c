#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


struct _thstelm;;

typedef enum {
  rsn_cont, rsn_retval
}               reason;

typedef void    (*(*cont) (struct _thstelm(*), reason));

enum t_stat {
  thr_new_suspended, thr_new_runnable, thr_suspended, thr_runnable, thr_scheduled
};

struct _thstelm {
  cont            c;
  enum t_stat     stat;
};

typedef struct _thstelm thstelm;

typedef         thstelm(*thst_ptr);

thstelm(thst[65536]);

thst_ptr        thst_top = thst;

typedef void    (*schdexit) ();

schdexit        cur_schd_exit = 0;

thst_ptr        cur_schd_thst_top = thst;

void 
scheduling()
{
  __label__       L0;
  schdexit        prev_exit = cur_schd_exit;
  thst_ptr        prev_thst_top = cur_schd_thst_top;
  thst_ptr        mythst_top = thst_top;
  void            nonlocalexit() {
    goto L0;
  }
                  L0:
                 ;
  cur_schd_exit = nonlocalexit;
  cur_schd_thst_top = thst_top;
  cur_schd_thst_top = mythst_top;
  do {{
      thst_ptr        cp;
      {
	cp = prev_thst_top;
	if (cp < mythst_top)
	  do {
	    if ((*cp).stat != thr_scheduled)
	      break;
	  } while (cp++, cp < mythst_top);
      }
      if (cp == mythst_top)
	if (prev_exit)
	  prev_exit();

  }
  {
    thst_ptr        cp;
    cont            cc;
    {
      cp = thst_top - 1;
      if (cp >= thst)
	do {
	  if ((*cp).stat == thr_runnable || (*cp).stat == thr_new_runnable)
	    break;
	} while (cp--, cp >= thst);
    }
    if (cp < thst) {
      fprintf(stderr, "No active thread!\n");
      exit(1);
    }
    do {
      cc = (*cp).c;
      (*cp).c = 0;
      (*cp).stat = thr_scheduled;
      cc(cp, rsn_cont);
    } while ((*cp).stat == thr_runnable);
  }
  if (thst_top > thst && (*(thst_top - 1)).stat == thr_new_runnable) {
    thst_top--;
    break;
  }
  } while (1);
  cur_schd_exit = prev_exit;
  cur_schd_thst_top = prev_thst_top;
}

void 
thr_resume(thst_ptr cp)
{
  if ((*cp).stat == thr_suspended)
    (*cp).stat = thr_runnable;

  else if ((*cp).stat == thr_new_suspended)
    (*cp).stat = thr_new_runnable;
}

int 
pfib(cont c_p, int n)
{
  int             ln = 0;
  thst_ptr        tmp1;
  int             tmp2;
  int             tmp6;
  int             tmp7;
  int             tmp8;
  int             x;
  int             y;
  int             nn;
  thst_ptr        c;
  thst_ptr        c0;
  int             t2;
  int             t1;
  void            (*pfib_c(thst_ptr cp, reason rsn)) {
    switch (rsn) {
      case rsn_cont:

      switch (ln) {
	case 1:

	goto L1;
	case 2:

	goto L2;
	case 3:

	goto L3;
      }
      return;
      case rsn_retval:

      switch (ln) {
	case 2:

	return ((void (*)) &t2);
      }
      return;
    }
                    return;
    if (n <= 2) {
      *((int (*)) c_p(cp, rsn_retval)) = 1;
    } else {{
	tmp1 = (c = 0);
	tmp1;
	tmp2 = (nn = 0);
	tmp2;
	{
	  ln = 1;
	  thst_top->c = pfib_c;
	  thst_top->stat = thr_new_runnable;
	  thst_top++;
	}
	{
	  int             tmp3;
	  int             tmp4;
	  int             tmp5;
	  int             ln = 0;
	  void            (*nthr_c(thst_ptr cp, reason rsn)) {
	    switch (rsn) {
	      case rsn_cont:

	      switch (ln) {
		case 1:

		goto L1;
	      }
	      return;
	      case rsn_retval:

	      switch (ln) {
		case 1:

		return ((void (*)) &t1);
	      }
	      return;
	    }
	                    return;
	    ln = 1;
	    t1 = pfib(nthr_c, n - 1);
	L1:
	    ;
	    tmp3 = t1;
	    tmp4 = (x = tmp3);
	    tmp4;
	    {
	      tmp5 = (nn = nn + 1);
	      tmp5;
	      if (nn <= 0)
		thr_resume(c);
	    }
	    return;
	  }
	  ln = 1;
	  tmp3 = pfib(nthr_c, n - 1);
	  tmp4 = (x = tmp3);
	  tmp4;
	  {
	    tmp5 = (nn = nn + 1);
	    tmp5;
	    if (nn <= 0)
	      thr_resume(c);
	  }
	}
	if ((thst_top - 1)->stat != thr_new_runnable)
	  scheduling();

	else
	  thst_top--;
    L1:
	;
	ln = 2;
	t2 = pfib(pfib_c, n - 2);
    L2:
	;
	tmp6 = t2;
	tmp7 = (y = tmp6);
	tmp7;
	{
	  tmp8 = (nn = nn - 1);
	  tmp8;
	  if (nn < 0) {
	    c0 = cp;
	    c0->c = pfib_c;
	    c0->stat = thr_suspended;
	    c = c0;
	    ln = 3;
	    return;
	  }
	}
	*((int (*)) c_p(cp, rsn_retval)) = x + y;
    }
    }
L3:
    ;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  if (n <= 2) {
    return (1);
  } else {{
      tmp1 = (c = 0);
      tmp1;
      tmp2 = (nn = 0);
      tmp2;
      {
	ln = 1;
	thst_top->c = pfib_c;
	thst_top->stat = thr_new_runnable;
	thst_top++;
      }
      {
	int             tmp3;
	int             tmp4;
	int             tmp5;
	int             ln = 0;
	void            (*nthr_c(thst_ptr cp, reason rsn)) {
	  switch (rsn) {
	    case rsn_cont:

	    switch (ln) {
	      case 1:

	      goto L1;
	    }
	    return;
	    case rsn_retval:

	    switch (ln) {
	      case 1:

	      return ((void (*)) &t1);
	    }
	    return;
	  }
	                  return;
	  ln = 1;
	  t1 = pfib(nthr_c, n - 1);
      L1:
	  ;
	  tmp3 = t1;
	  tmp4 = (x = tmp3);
	  tmp4;
	  {
	    tmp5 = (nn = nn + 1);
	    tmp5;
	    if (nn <= 0)
	      thr_resume(c);
	  }
	  return;
	}
	ln = 1;
	tmp3 = pfib(nthr_c, n - 1);
	tmp4 = (x = tmp3);
	tmp4;
	{
	  tmp5 = (nn = nn + 1);
	  tmp5;
	  if (nn <= 0)
	    thr_resume(c);
	}
      }
      if ((thst_top - 1)->stat != thr_new_runnable)
	scheduling();

      else
	thst_top--;
      ln = 2;
      tmp6 = pfib(pfib_c, n - 2);
      tmp7 = (y = tmp6);
      tmp7;
      {
	tmp8 = (nn = nn - 1);
	tmp8;
	if (nn < 0) {
	  c0 = thst_top++;
	  c0->c = pfib_c;
	  c0->stat = thr_new_suspended;
	  c = c0;
	  ln = 3;
	  scheduling();
	}
      }
      return (x + y);
  }
  }
}

int 
msc_main(cont c_p, int argc, char (**argv))
{
  int             ln = 0;
  int             tmp1;
  int             n = (argc>1)?(atoi(argv[1])):(5);
  int t1;
  void            (*msc_main_c(thst_ptr cp, reason rsn)) {
    switch (rsn) {
      case rsn_cont:

      switch (ln) {
	case 1:

	goto L1;
      }
      return;
      case rsn_retval:

      switch (ln) {
	case 1:

	return ((void (*)) &t1);
      }
      return;
    }
                    return;
    ln = 1;
    t1 = pfib(msc_main_c, n);
L1:
    ;
    tmp1 = t1;
    *((int (*)) c_p(cp, rsn_retval)) = tmp1;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  ln = 1;
  tmp1 = pfib(msc_main_c, n);
  return (tmp1);
}

int 
main(int argc, char (**argv))
{
  int             ln = 0;
  int             msc_main_ret;
  void            (*main_c(thst_ptr cp, reason rsn)) {
    switch (rsn) {
      case rsn_cont:

      switch (ln) {
	case 0:

	goto L0;
	case 1:

	goto L1;
      }
      return;
      case rsn_retval:

      return ((void (*)) &msc_main_ret);
    }
    return;
    L0:
    ln = 1;
    msc_main_ret = msc_main(main_c, argc, argv);
    L1:
    printf("%d\n",msc_main_ret);
    exit(msc_main_ret);
  }
                  (*thst_top).c = main_c;
  (*thst_top).stat = thr_runnable;
  thst_top++;
  do {
    scheduling();
  } while (1);
  return (0);
}
