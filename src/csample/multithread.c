#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


struct _thstelm;;

typedef enum {
  rsn_cont, rsn_retval
}               reason;

typedef void    (*(*cont) (struct _thstelm(*), reason));

struct _thstelm {
  cont            c;
  enum _stat {
    thr_new_suspended, thr_new_runnable, thr_suspended, thr_runnable, thr_scheduled
  };
  enum _stat      stat;
};

typedef struct _thstelm thstelm;

typedef struct _thstelm (*thst_ptr);

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
  cur_schd_thst_top = (thst_top = mythst_top);
  while (1) {{
      thst_ptr        cp;
      for (cp = prev_thst_top; cp < mythst_top; cp++) {
	if (cp->stat != thr_scheduled)
	  break;

	if (cp == mythst_top)
	  if (prev_exit)
	    prev_exit();

      }
  }
  {
    thst_ptr        cp;
    cont            cc;
    for (cp = thst_top - 1; cp >= thst; cp--) {
      if (cp->stat == thr_runnable || cp->stat == thr_new_runnable)
	break;
    }
    if (cp < thst) {
      fprintf(stderr, "No Active thread!\n");
      exit(1);
    }
    do {
      cc = cp->c;
      cp->c = 0;
      cp->stat = thr_scheduled;
      cc(cp, rsn_cont);
    } while (cp->stat == thr_runnable);
  }
  if (thst_top > thst && (thst_top - 1)->stat == thr_new_runnable) {
    thst_top--;
    break;
  }
  }
  cur_schd_exit = prev_exit;
  cur_schd_thst_top = prev_thst_top;
}

void 
thr_resume(thst_ptr cp)
{
  if (cp->stat == thr_suspended)
    cp->stat = thr_runnable;

  else if (cp->stat == thr_new_suspended)
    cp->stat = thr_new_runnable;
}

struct sync {
  int             n;
  thst_ptr        c;
};

void 
f_notify(cont c_p, struct sync(*s))
{
  (s->n)++;
  if (s->n <= 0)
    thr_resume(s->c);
}

void 
f_wait(cont c_p, struct sync(*s))
{
  int             ln = 0;
  void            (*f_wait_c(thst_ptr cp, reason rsn)) {
    switch (rsn) {
      case rsn_cont:

      switch (ln) {
	case 1:

	goto L1;
      }
      return;
      case rsn_retval:

      switch (ln) {
      }
      return;
    }
                    return;
    {
      (s->n)--;
      if (s->n < 0) {
	thst_ptr        c0 = cp;
	c0->c = f_wait_c;
	c0->stat = thr_suspended;
	s->c = c0;
	ln = 1;
	return;
      }
    }
L1:
    ;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  {
    (s->n)--;
    if (s->n < 0) {
      thst_ptr        c0 = thst_top++;
      c0->c = f_wait_c;
      c0->stat = thr_new_suspended;
      s->c = c0;
      ln = 1;
      scheduling();
    }
  }
}

int             g(cont c_p, int x, int y) {
  return ((x + y) / 2);
}

void            fw1(cont c_p, int n1, int n2, int (*tab1), int (*tab2), struct sync(*s1), struct 
		    sync(*s2))
{
  int             ln = 0;
  int             i = n1;
  int             i2;
  int             t1;
  int             t4;
  void            fw1_c(thst_ptr cp, reason rsn) {
    switch (rsn) {
      case rsn_cont:

      switch (ln) {
	case 1:

	goto L1;
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

      switch (ln) {
	case 1:

	return ((void (*)) &t1);
	case 4:

	return ((void (*)) &t4);
      }
      return;
    }
                    return;
    while (i < n2) {
      ln = 1;
      t1 = g(fw1_c, tab1[i], tab1[i - 1]);
  L1:
      ;
      tab2[i] = t1;
      i++;
    }
    ln = 2;
    f_notify(fw1_c, s1);
L2:
    ;
    ln = 3;
    f_wait(fw1_c, s2);
L3:
    ;
    i2 = n1;
    while (i2 < n2) {
      ln = 4;
      t4 = g(fw1_c, tab2[i + 1], tab2[i]);
  L4:
      ;
      tab1[i] = t4;
      i2++;
    }
    ln = 5;
    f_notify(fw1_c, s1);
L5:
    ;
    ln = 6;
    f_wait(fw1_c, s2);
L6:
    ;
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  while (i < n2) {
    ln = 1;
    tab2[i] = g(fw1_c, tab1[i], tab1[i - 1]);
    i++;
  }
  ln = 2;
  f_notify(fw1_c, s1);
  ln = 3;
  f_wait(fw1_c, s2);
  i2 = n1;
  while (i2 < n2) {
    ln = 4;
    tab1[i] = g(fw1_c, tab2[i + 1], tab2[i]);
    i2++;
  }
  ln = 5;
  f_notify(fw1_c, s1);
  ln = 6;
  f_wait(fw1_c, s2);
  return;
}

void 
divide_work(cont c_p, int n, int m)
{
  int             ln;
  int             (tab1[n]);
  int             (tab2[n]);
  struct sync     s1 = {0, 0};
  struct sync     s2 = {0, 0};
  int             i = 0;
  void            (*divide_work_c(thst_ptr cp, reason rsn)) {
    switch (rsn) {
      case rsn_cont:

      switch (ln) {
	case 1:

	goto L1;
	case 2:

	goto L2;
      }
      return;
      case rsn_retval:

      switch (ln) {
      }
      return;
    }
                    return;
    while (i < m) {{
	ln = 1;
	thst_top->c = divide_work_c;
	thst_top->stat = thr_new_runnable;
	thst_top++;
	{
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
	      }
	      return;
	    }
	                    return;
	    fw1(nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
	L1:
	    ;
	    return;
	  }
	  ln = 1;
	  fw1(nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
	}
	if ((thst_top - 1)->stat != thr_new_runnable)
	  scheduling();

	else
	  thst_top--;
    }
L1:
    ;
    ln = 2;
    fw1(divide_work_c, n / 2, n - 2, tab1, tab2, &s2, &s1);
L2:
    i++;
    }
    cp->c = c_p;
    cp->stat = thr_runnable;
    return;
  }
  while (i < m) {{
      ln = 1;
      thst_top->c = divide_work_c;
      thst_top->stat = thr_new_runnable;
      thst_top++;
      {
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
	    }
	    return;
	  }
	                  return;
	  fw1(nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
      L1:
	  return;
	}
	ln = 1;
	fw1(nthr_c, 1, n / 2 - 1, tab1, tab2, &s1, &s2);
      }
      if ((thst_top - 1)->stat != thr_new_runnable)
	scheduling();

      else
	thst_top--;
  }
  ln = 2;
  fw1(divide_work_c, n / 2, n - 2, tab1, tab2, &s2, &s1);
  i++;
  }
}
