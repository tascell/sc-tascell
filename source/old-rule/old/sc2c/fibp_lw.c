#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


typedef char    (*(*nestfn_t) (char (*), void (*)));

typedef struct {
  nestfn_t        fun;
  void            (*fr);
}               closure_t;

typedef double  align_t;

int             lw_call(char (*esp)) {
  char            (*tmp_esp);

  closure_t(*clos) = *((closure_t(**)) esp);
  char            (*new_esp) = esp;

  while (tmp_esp = clos->fun(new_esp, clos->fr)) {
    if ((size_t) tmp_esp & 2)
      return (2);

    new_esp = esp + 1 + lw_call(tmp_esp);
  }
  return (0);
}

struct _thstelm;;

typedef enum {
  rsn_cont, rsn_retval
}               reason;

typedef         closure_t(*cont);

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

typedef         closure_t(*schdexit);

schdexit        cur_schd_exit = 0;

thst_ptr        cur_schd_thst_top = thst;

struct scheduling_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             ifexp_result4;
  int             ifexp_result3;
  void            (*tmp4);
  int             ifexp_result2;
  int             ifexp_result;
  cont            cc;
  thst_ptr        cp__2;
  thst_ptr        cp;
                  closure_t(*nonlocalexit);
  thst_ptr        mythst_top;
  thst_ptr        prev_thst_top;
  schdexit        prev_exit;
  closure_t       nonlocalexit0;
};

struct nonlocalexit_in_scheduling_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  struct scheduling_frame (*xfp);
};

char            (*nonlocalexit_in_scheduling(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct nonlocalexit_in_scheduling_frame (*efp);
  struct scheduling_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 7;
  char            (*parmp) = (char (*)) ((size_t) esp ^ esp_flag);

  efp = (struct nonlocalexit_in_scheduling_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct nonlocalexit_in_scheduling_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  xfp->call_id = -1;
  return ((char (*)) 2);
}

void            scheduling(char (*esp)) {
  char            (*argp);
  struct scheduling_frame (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 7;
  schdexit        prev_exit;
  thst_ptr        prev_thst_top;
  thst_ptr        mythst_top;

  closure_t(*nonlocalexit) = &((struct scheduling_frame(*)) esp)->nonlocalexit0;
  thst_ptr        cp;
  thst_ptr        cp__2;
  cont            cc;
  int             ifexp_result;
  int             ifexp_result2;
  void            (*tmp4);
  int             ifexp_result3;
  int             ifexp_result4;

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct scheduling_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct scheduling_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      ifexp_result4 = efp->ifexp_result4;
      ifexp_result3 = efp->ifexp_result3;
      tmp4 = efp->tmp4;
      ifexp_result2 = efp->ifexp_result2;
      ifexp_result = efp->ifexp_result;
      cc = efp->cc;
      cp__2 = efp->cp__2;
      cp = efp->cp;
      nonlocalexit = efp->nonlocalexit;
      mythst_top = efp->mythst_top;
      prev_thst_top = efp->prev_thst_top;
      prev_exit = efp->prev_exit;
      goto L_call;
    case 1:

      ifexp_result4 = efp->ifexp_result4;
      ifexp_result3 = efp->ifexp_result3;
      tmp4 = efp->tmp4;
      ifexp_result2 = efp->ifexp_result2;
      ifexp_result = efp->ifexp_result;
      cc = efp->cc;
      cp__2 = efp->cp__2;
      cp = efp->cp;
      nonlocalexit = efp->nonlocalexit;
      mythst_top = efp->mythst_top;
      prev_thst_top = efp->prev_thst_top;
      prev_exit = efp->prev_exit;
      goto L_call2;
    case -1:

      ifexp_result4 = efp->ifexp_result4;
      ifexp_result3 = efp->ifexp_result3;
      tmp4 = efp->tmp4;
      ifexp_result2 = efp->ifexp_result2;
      ifexp_result = efp->ifexp_result;
      cc = efp->cc;
      cp__2 = efp->cp__2;
      cp = efp->cp;
      nonlocalexit = efp->nonlocalexit;
      mythst_top = efp->mythst_top;
      prev_thst_top = efp->prev_thst_top;
      prev_exit = efp->prev_exit;
      goto L0;
    }
    goto L_call;
  }
  efp = (struct scheduling_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct scheduling_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  prev_exit = cur_schd_exit;
  prev_thst_top = cur_schd_thst_top;
  mythst_top = thst_top;
L0:
  ;
  cur_schd_exit = nonlocalexit;
  cur_schd_thst_top = thst_top;
  cur_schd_thst_top = mythst_top;
  do {{{
	cp = prev_thst_top;
	if (cp < mythst_top) {
	  do {
	    if ((*cp).stat != thr_scheduled) {
	      break;
	    }
	    cp++;
	  } while (cp < mythst_top);
	}
  }
  if (cp == mythst_top) {
    if (prev_exit) {{
	argp = (char (*)) ((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
	*((closure_t(**)) argp) = prev_exit;
	efp->nonlocalexit0.fun = nonlocalexit_in_scheduling;
	efp->nonlocalexit0.fr = (void (*)) efp;
	efp->ifexp_result4 = ifexp_result4;
	efp->ifexp_result3 = ifexp_result3;
	efp->tmp4 = tmp4;
	efp->ifexp_result2 = ifexp_result2;
	efp->ifexp_result = ifexp_result;
	efp->cc = cc;
	efp->cp__2 = cp__2;
	efp->cp = cp;
	efp->nonlocalexit = nonlocalexit;
	efp->mythst_top = mythst_top;
	efp->prev_thst_top = prev_thst_top;
	efp->prev_exit = prev_exit;
	efp->argp = argp;
	efp->tmp_esp = argp;
	efp->call_id = 0;
	return;
    L_call:
	if (esp_flag & 2) {
	  efp->tmp_esp = (char (*)) 2;
	  return;
	}
    }
    }
  }
  }
  {{
      cp__2 = thst_top - 1;
      if (cp__2 >= thst) {
	do {
	  if ((*cp__2).stat == thr_runnable) {{
	      ifexp_result2 = 1;
	  }
	  } else {{
	      if ((*cp__2).stat == thr_new_runnable) {{
		  ifexp_result = 1;
	      }
	      } else {{
		  ifexp_result = 0;
	      }
	      }
	      ifexp_result2 = ifexp_result;
	  }
	  }
	  if (ifexp_result2) {
	    break;
	  }
	  cp__2--;
	} while (cp__2 >= thst);
      }
  }
  if (cp__2 < thst) {{
  }
  }
    do {
      cc = (*cp__2).c;
      (*cp__2).c = 0;
      (*cp__2).stat = thr_scheduled;
      {
	argp = (char (*)) ((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
	*((reason(*)) argp) = rsn_cont, argp = (char (*)) ((align_t(*)) argp + (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t));
	*((struct _thstelm(**)) argp) = cp__2, argp = (char (*)) ((align_t(*)) argp + (sizeof(struct _thstelm(*)) + sizeof(align_t) + -1) / sizeof(align_t));
	*((closure_t(**)) argp) = cc;
	efp->nonlocalexit0.fun = nonlocalexit_in_scheduling;
	efp->nonlocalexit0.fr = (void (*)) efp;
	efp->ifexp_result4 = ifexp_result4;
	efp->ifexp_result3 = ifexp_result3;
	efp->tmp4 = tmp4;
	efp->ifexp_result2 = ifexp_result2;
	efp->ifexp_result = ifexp_result;
	efp->cc = cc;
	efp->cp__2 = cp__2;
	efp->cp = cp;
	efp->nonlocalexit = nonlocalexit;
	efp->mythst_top = mythst_top;
	efp->prev_thst_top = prev_thst_top;
	efp->prev_exit = prev_exit;
	efp->argp = argp;
	efp->tmp_esp = argp;
	efp->call_id = 1;
	return;
    L_call2:
	if (esp_flag & 2) {
	  efp->tmp_esp = (char (*)) 2;
	  return;
	}
	tmp4 = *((void (**)) efp->argp);
      }
      tmp4;
    } while ((*cp__2).stat == thr_runnable);
  }
  if (thst_top > thst) {{
      if ((*(thst_top - 1)).stat == thr_new_runnable) {{
	  ifexp_result3 = 1;
      }
      } else {{
	  ifexp_result3 = 0;
      }
      }
      ifexp_result4 = ifexp_result3;
  }
  } else {{
      ifexp_result4 = 0;
  }
  }
  if (ifexp_result4) {{
      thst_top--;
      break;
  }
  }
  } while (1);
  cur_schd_exit = prev_exit;
  cur_schd_thst_top = prev_thst_top;
}

struct thr_resume_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  thst_ptr        cp;
};

void            thr_resume(char (*esp), thst_ptr cp) {
  struct thr_resume_frame (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 7;

  efp = (struct thr_resume_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct thr_resume_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  if ((*cp).stat == thr_suspended) {{
      (*cp).stat = thr_runnable;
  }
  } else {{
      if ((*cp).stat == thr_new_suspended) {
	(*cp).stat = thr_new_runnable;
      }
  }
  }
}

struct pfib_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp12;
  int             tmp11;
                  closure_t(*nthr_c);
  int             ln__3;
  int             tmp;
                  closure_t(*pfib_c);
  int             t1;
  int             t2;
  thst_ptr        c0;
  int             tmp2;
  thst_ptr        c;
  int             nn;
  int             y;
  int             x;
  int             ln;
  int             n;
  cont            c_p;
  int             tmp_arg6;
  int             tmp_arg5;
  closure_t       nthr_c02;
  closure_t       pfib_c0;
};

struct pfib_c_in_pfib_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  void            (*tmp9);
  int             tmp8;
  int             tmp7;
  void            (*tmp6);
                  closure_t(*nthr_c);
  int             ln__2;
  int             tmp;
  reason          rsn;
  thst_ptr        cp;
  struct pfib_frame (*xfp);
  int             tmp_arg3;
  int             tmp_arg2;
  closure_t       nthr_c0;
};

struct nthr_c_in_pfib_c_in_pfib_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp5;
  reason          rsn__2;
  thst_ptr        cp__3;
  struct pfib_c_in_pfib_frame (*xfp);
  int             tmp_arg;
};

char            (*nthr_c_in_pfib_c_in_pfib(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct nthr_c_in_pfib_c_in_pfib_frame (*efp);
  struct pfib_c_in_pfib_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 7;
  char            (*parmp) = (char (*)) ((size_t) esp ^ esp_flag);
  thst_ptr        cp__3 = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(thst_ptr) + sizeof(align_t) + -1) / sizeof(align_t)), *((thst_ptr(*)) parmp));
  reason          rsn__2 = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t)), *((reason(*)) parmp));
  int             tmp5;
  int             tmp_arg;

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct nthr_c_in_pfib_c_in_pfib_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct nthr_c_in_pfib_c_in_pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp5 = efp->tmp5;
      rsn__2 = efp->rsn__2;
      cp__3 = efp->cp__3;
      xfp = efp->xfp;
      goto L_call3;
    case 1:

      tmp5 = efp->tmp5;
      rsn__2 = efp->rsn__2;
      cp__3 = efp->cp__3;
      xfp = efp->xfp;
      goto L_call4;
    }
    goto L_call3;
  }
  efp = (struct nthr_c_in_pfib_c_in_pfib_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct nthr_c_in_pfib_c_in_pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  switch (rsn__2) {
  case rsn_cont:

    switch (xfp->ln__2) {
    case 1:

      goto L1;
    }
    return;
  case rsn_retval:

    switch (xfp->ln__2) {
    case 1:

      *((void (**)) efp) = (void (*)) &xfp->xfp->t1;
      return (0);
    }
    return;
  }
  return;
  xfp->ln__2 = 1;
  tmp_arg = xfp->xfp->n - 1;
  while ((tmp5 = pfib(new_esp, xfp->nthr_c, tmp_arg)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
    if (!((size_t) efp->tmp_esp & 2)) {
      efp->tmp5 = tmp5;
      efp->rsn__2 = rsn__2;
      efp->cp__3 = cp__3;
      efp->xfp = xfp;
      efp->call_id = 0;
    }
    return (efp->tmp_esp);
L_call3:
    ;
    new_esp = esp + esp_flag;
    esp_flag = 0;
    tmp_arg = efp->tmp_arg;
  }
  xfp->xfp->t1 = tmp5;
L1:
  ;
  xfp->tmp = xfp->xfp->t1;
  xfp->xfp->x = xfp->tmp;
  {
    xfp->xfp->nn = xfp->xfp->nn + 1;
    if (xfp->xfp->nn <= 0) {
      while (thr_resume(new_esp, xfp->xfp->c), (efp->tmp_esp = *((char (**)) esp)) != 0) {
	if (!((size_t) efp->tmp_esp & 2)) {
	  efp->tmp5 = tmp5;
	  efp->rsn__2 = rsn__2;
	  efp->cp__3 = cp__3;
	  efp->xfp = xfp;
	  efp->call_id = 1;
	}
	return (efp->tmp_esp);
    L_call4:
	;
	new_esp = esp + esp_flag;
	esp_flag = 0;
      }
    }
  }
  return;
}

char            (*pfib_c_in_pfib(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct pfib_c_in_pfib_frame (*efp);
  struct pfib_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 7;
  char            (*parmp) = (char (*)) ((size_t) esp ^ esp_flag);
  thst_ptr        cp = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(thst_ptr) + sizeof(align_t) + -1) / sizeof(align_t)), *((thst_ptr(*)) parmp));
  reason          rsn = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t)), *((reason(*)) parmp));
  char            (*argp);
  int             tmp;
  int             ln__2;

  closure_t(*nthr_c) = &((struct pfib_c_in_pfib_frame(*)) esp)->nthr_c0;
  void            (*tmp6);
  int             tmp7;
  int             tmp8;
  void            (*tmp9);

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct pfib_c_in_pfib_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct pfib_c_in_pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp9 = efp->tmp9;
      tmp8 = efp->tmp8;
      tmp7 = efp->tmp7;
      tmp6 = efp->tmp6;
      nthr_c = efp->nthr_c;
      ln__2 = efp->ln__2;
      tmp = efp->tmp;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call5;
    case 1:

      tmp9 = efp->tmp9;
      tmp8 = efp->tmp8;
      tmp7 = efp->tmp7;
      tmp6 = efp->tmp6;
      nthr_c = efp->nthr_c;
      ln__2 = efp->ln__2;
      tmp = efp->tmp;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call6;
    case 2:

      tmp9 = efp->tmp9;
      tmp8 = efp->tmp8;
      tmp7 = efp->tmp7;
      tmp6 = efp->tmp6;
      nthr_c = efp->nthr_c;
      ln__2 = efp->ln__2;
      tmp = efp->tmp;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call7;
    case 3:

      tmp9 = efp->tmp9;
      tmp8 = efp->tmp8;
      tmp7 = efp->tmp7;
      tmp6 = efp->tmp6;
      nthr_c = efp->nthr_c;
      ln__2 = efp->ln__2;
      tmp = efp->tmp;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call8;
    case 4:

      tmp9 = efp->tmp9;
      tmp8 = efp->tmp8;
      tmp7 = efp->tmp7;
      tmp6 = efp->tmp6;
      nthr_c = efp->nthr_c;
      ln__2 = efp->ln__2;
      tmp = efp->tmp;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call9;
    case 5:

      tmp9 = efp->tmp9;
      tmp8 = efp->tmp8;
      tmp7 = efp->tmp7;
      tmp6 = efp->tmp6;
      nthr_c = efp->nthr_c;
      ln__2 = efp->ln__2;
      tmp = efp->tmp;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call10;
    }
    goto L_call5;
  }
  efp = (struct pfib_c_in_pfib_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct pfib_c_in_pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  switch (rsn) {
  case rsn_cont:

    switch (xfp->ln) {
    case 1:

      goto L1;
    case 2:

      goto L2;
    case 3:

      goto L3;
    }
    return;
  case rsn_retval:

    switch (xfp->ln) {
    case 2:

      *((void (**)) efp) = (void (*)) &xfp->t2;
      return (0);
    }
    return;
  }
  return;
  if (xfp->n <= 2) {{{
	closure_t(*tmp_fp);
	{
	  tmp_fp = xfp->c_p;
	  argp = (char (*)) ((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
	  *((reason(*)) argp) = rsn_retval, argp = (char (*)) ((align_t(*)) argp + (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t));
	  *((struct _thstelm(**)) argp) = cp, argp = (char (*)) ((align_t(*)) argp + (sizeof(struct _thstelm(*)) + sizeof(align_t) + -1) / sizeof(align_t));
	  *((closure_t(**)) argp) = tmp_fp;
	  efp->nthr_c0.fun = nthr_c_in_pfib_c_in_pfib;
	  efp->nthr_c0.fr = (void (*)) efp;
	  efp->tmp9 = tmp9;
	  efp->tmp8 = tmp8;
	  efp->tmp7 = tmp7;
	  efp->tmp6 = tmp6;
	  efp->nthr_c = nthr_c;
	  efp->ln__2 = ln__2;
	  efp->tmp = tmp;
	  efp->rsn = rsn;
	  efp->cp = cp;
	  efp->xfp = xfp;
	  efp->argp = argp;
	  efp->tmp_esp = argp;
	  efp->call_id = 0;
	  return (efp->tmp_esp);
      L_call5:
	  if (esp_flag & 2)
	    return ((char (*)) 2);

	  tmp6 = *((void (**)) efp->argp);
	}
	*((int (*)) tmp6) = 1;
  }
  }
  } else {{{{
	  int             tmp_arg3;

	  closure_t(*tmp_fp2);
	  xfp->nn = 0;
	  xfp->c = 0;
	  {
	    xfp->ln = 1;
	    (*thst_top).c = xfp->pfib_c;
	    (*thst_top).stat = thr_new_runnable;
	    thst_top++;
	  }
	  {
	    int             tmp_arg2;

	    ln__2 = 0;
	    ln__2 = 1;
	    tmp_arg2 = xfp->n - 1;
	    while ((tmp7 = pfib(new_esp, nthr_c, tmp_arg2)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
	      if (!((size_t) efp->tmp_esp & 2)) {
		efp->tmp9 = tmp9;
		efp->tmp8 = tmp8;
		efp->tmp7 = tmp7;
		efp->tmp6 = tmp6;
		efp->nthr_c = nthr_c;
		efp->ln__2 = ln__2;
		efp->tmp = tmp;
		efp->rsn = rsn;
		efp->cp = cp;
		efp->xfp = xfp;
		efp->nthr_c0.fun = nthr_c_in_pfib_c_in_pfib;
		efp->nthr_c0.fr = (void (*)) efp;
		efp->call_id = 1;
	      }
	      return (efp->tmp_esp);
	  L_call6:
	      ;
	      new_esp = esp + esp_flag;
	      esp_flag = 0;
	      tmp_arg2 = efp->tmp_arg2;
	    }
	    tmp = tmp7;
	    xfp->x = tmp;
	    {
	      xfp->nn = xfp->nn + 1;
	      if (xfp->nn <= 0) {
		while (thr_resume(new_esp, xfp->c), (efp->tmp_esp = *((char (**)) esp)) != 0) {
		  if (!((size_t) efp->tmp_esp & 2)) {
		    efp->tmp9 = tmp9;
		    efp->tmp8 = tmp8;
		    efp->tmp7 = tmp7;
		    efp->tmp6 = tmp6;
		    efp->nthr_c = nthr_c;
		    efp->ln__2 = ln__2;
		    efp->tmp = tmp;
		    efp->rsn = rsn;
		    efp->cp = cp;
		    efp->xfp = xfp;
		    efp->nthr_c0.fun = nthr_c_in_pfib_c_in_pfib;
		    efp->nthr_c0.fr = (void (*)) efp;
		    efp->call_id = 2;
		  }
		  return (efp->tmp_esp);
	      L_call7:
		  ;
		  new_esp = esp + esp_flag;
		  esp_flag = 0;
		}
	      }
	    }
	  }
	  if ((*(thst_top - 1)).stat != thr_new_runnable) {{
	      while (scheduling(new_esp), (efp->tmp_esp = *((char (**)) esp)) != 0) {
		if (!((size_t) efp->tmp_esp & 2)) {
		  efp->tmp9 = tmp9;
		  efp->tmp8 = tmp8;
		  efp->tmp7 = tmp7;
		  efp->tmp6 = tmp6;
		  efp->nthr_c = nthr_c;
		  efp->ln__2 = ln__2;
		  efp->tmp = tmp;
		  efp->rsn = rsn;
		  efp->cp = cp;
		  efp->xfp = xfp;
		  efp->nthr_c0.fun = nthr_c_in_pfib_c_in_pfib;
		  efp->nthr_c0.fr = (void (*)) efp;
		  efp->call_id = 3;
		}
		return (efp->tmp_esp);
	    L_call8:
		;
		new_esp = esp + esp_flag;
		esp_flag = 0;
	      }
	  }
	  } else {{
	      thst_top--;
	  }
	  }
      L1:
	  ;
	  xfp->ln = 2;
	  tmp_arg3 = xfp->n - 2;
	  while ((tmp8 = pfib(new_esp, xfp->pfib_c, tmp_arg3)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
	    if (!((size_t) efp->tmp_esp & 2)) {
	      efp->tmp9 = tmp9;
	      efp->tmp8 = tmp8;
	      efp->tmp7 = tmp7;
	      efp->tmp6 = tmp6;
	      efp->nthr_c = nthr_c;
	      efp->ln__2 = ln__2;
	      efp->tmp = tmp;
	      efp->rsn = rsn;
	      efp->cp = cp;
	      efp->xfp = xfp;
	      efp->nthr_c0.fun = nthr_c_in_pfib_c_in_pfib;
	      efp->nthr_c0.fr = (void (*)) efp;
	      efp->call_id = 4;
	    }
	    return (efp->tmp_esp);
	L_call9:
	    ;
	    new_esp = esp + esp_flag;
	    esp_flag = 0;
	    tmp_arg3 = efp->tmp_arg3;
	  }
	  xfp->t2 = tmp8;
      L2:
	  ;
	  xfp->tmp2 = xfp->t2;
	  xfp->y = xfp->tmp2;
	  {
	    xfp->nn = xfp->nn - 1;
	    if (xfp->nn < 0) {{
		xfp->c0 = cp;
		(*xfp->c0).c = xfp->pfib_c;
		(*xfp->c0).stat = thr_suspended;
		xfp->c = xfp->c0;
		xfp->ln = 3;
		return;
	    }
	    }
	  }
	  {
	    tmp_fp2 = xfp->c_p;
	    argp = (char (*)) ((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
	    *((reason(*)) argp) = rsn_retval, argp = (char (*)) ((align_t(*)) argp + (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t));
	    *((struct _thstelm(**)) argp) = cp, argp = (char (*)) ((align_t(*)) argp + (sizeof(struct _thstelm(*)) + sizeof(align_t) + -1) / sizeof(align_t));
	    *((closure_t(**)) argp) = tmp_fp2;
	    efp->nthr_c0.fun = nthr_c_in_pfib_c_in_pfib;
	    efp->nthr_c0.fr = (void (*)) efp;
	    efp->tmp9 = tmp9;
	    efp->tmp8 = tmp8;
	    efp->tmp7 = tmp7;
	    efp->tmp6 = tmp6;
	    efp->nthr_c = nthr_c;
	    efp->ln__2 = ln__2;
	    efp->tmp = tmp;
	    efp->rsn = rsn;
	    efp->cp = cp;
	    efp->xfp = xfp;
	    efp->argp = argp;
	    efp->tmp_esp = argp;
	    efp->call_id = 5;
	    return (efp->tmp_esp);
	L_call10:
	    if (esp_flag & 2)
	      return ((char (*)) 2);

	    tmp9 = *((void (**)) efp->argp);
	  }
	  *((int (*)) tmp9) = xfp->x + xfp->y;
  }
  }
  }
  }
L3:
  ;
  (*cp).c = xfp->c_p;
  (*cp).stat = thr_runnable;
  return;
}

struct nthr_c_in_pfib_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp10;
  reason          rsn;
  thst_ptr        cp;
  struct pfib_frame (*xfp);
  int             tmp_arg4;
};

char            (*nthr_c_in_pfib(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct nthr_c_in_pfib_frame (*efp);
  struct pfib_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 7;
  char            (*parmp) = (char (*)) ((size_t) esp ^ esp_flag);
  thst_ptr        cp = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(thst_ptr) + sizeof(align_t) + -1) / sizeof(align_t)), *((thst_ptr(*)) parmp));
  reason          rsn = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t)), *((reason(*)) parmp));
  int             tmp10;
  int             tmp_arg4;

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct nthr_c_in_pfib_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct nthr_c_in_pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp10 = efp->tmp10;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call11;
    case 1:

      tmp10 = efp->tmp10;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call12;
    }
    goto L_call11;
  }
  efp = (struct nthr_c_in_pfib_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct nthr_c_in_pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  switch (rsn) {
  case rsn_cont:

    switch (xfp->ln__3) {
    case 1:

      goto L1;
    }
    return;
  case rsn_retval:

    switch (xfp->ln__3) {
    case 1:

      *((void (**)) efp) = (void (*)) &xfp->t1;
      return (0);
    }
    return;
  }
  return;
  xfp->ln__3 = 1;
  tmp_arg4 = xfp->n - 1;
  while ((tmp10 = pfib(new_esp, xfp->nthr_c, tmp_arg4)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
    if (!((size_t) efp->tmp_esp & 2)) {
      efp->tmp10 = tmp10;
      efp->rsn = rsn;
      efp->cp = cp;
      efp->xfp = xfp;
      efp->call_id = 0;
    }
    return (efp->tmp_esp);
L_call11:
    ;
    new_esp = esp + esp_flag;
    esp_flag = 0;
    tmp_arg4 = efp->tmp_arg4;
  }
  xfp->t1 = tmp10;
L1:
  ;
  xfp->tmp = xfp->t1;
  xfp->x = xfp->tmp;
  {
    xfp->nn = xfp->nn + 1;
    if (xfp->nn <= 0) {
      while (thr_resume(new_esp, xfp->c), (efp->tmp_esp = *((char (**)) esp)) != 0) {
	if (!((size_t) efp->tmp_esp & 2)) {
	  efp->tmp10 = tmp10;
	  efp->rsn = rsn;
	  efp->cp = cp;
	  efp->xfp = xfp;
	  efp->call_id = 1;
	}
	return (efp->tmp_esp);
    L_call12:
	;
	new_esp = esp + esp_flag;
	esp_flag = 0;
      }
    }
  }
  return;
}

int             pfib(char (*esp), cont c_p, int n) {
  struct pfib_frame (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 7;
  int             ln;
  int             x;
  int             y;
  int             nn;
  thst_ptr        c;
  int             tmp2;
  thst_ptr        c0;
  int             t2;
  int             t1;

  closure_t(*pfib_c) = &((struct pfib_frame(*)) esp)->pfib_c0;
  int             tmp;
  int             ln__3;

  closure_t(*nthr_c) = &((struct pfib_frame(*)) esp)->nthr_c02;
  int             tmp11;
  int             tmp12;

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct pfib_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp12 = efp->tmp12;
      tmp11 = efp->tmp11;
      nthr_c = efp->nthr_c;
      ln__3 = efp->ln__3;
      tmp = efp->tmp;
      pfib_c = efp->pfib_c;
      t1 = efp->t1;
      t2 = efp->t2;
      c0 = efp->c0;
      tmp2 = efp->tmp2;
      c = efp->c;
      nn = efp->nn;
      y = efp->y;
      x = efp->x;
      ln = efp->ln;
      n = efp->n;
      c_p = efp->c_p;
      goto L_call13;
    case 1:

      tmp12 = efp->tmp12;
      tmp11 = efp->tmp11;
      nthr_c = efp->nthr_c;
      ln__3 = efp->ln__3;
      tmp = efp->tmp;
      pfib_c = efp->pfib_c;
      t1 = efp->t1;
      t2 = efp->t2;
      c0 = efp->c0;
      tmp2 = efp->tmp2;
      c = efp->c;
      nn = efp->nn;
      y = efp->y;
      x = efp->x;
      ln = efp->ln;
      n = efp->n;
      c_p = efp->c_p;
      goto L_call14;
    case 2:

      tmp12 = efp->tmp12;
      tmp11 = efp->tmp11;
      nthr_c = efp->nthr_c;
      ln__3 = efp->ln__3;
      tmp = efp->tmp;
      pfib_c = efp->pfib_c;
      t1 = efp->t1;
      t2 = efp->t2;
      c0 = efp->c0;
      tmp2 = efp->tmp2;
      c = efp->c;
      nn = efp->nn;
      y = efp->y;
      x = efp->x;
      ln = efp->ln;
      n = efp->n;
      c_p = efp->c_p;
      goto L_call15;
    case 3:

      tmp12 = efp->tmp12;
      tmp11 = efp->tmp11;
      nthr_c = efp->nthr_c;
      ln__3 = efp->ln__3;
      tmp = efp->tmp;
      pfib_c = efp->pfib_c;
      t1 = efp->t1;
      t2 = efp->t2;
      c0 = efp->c0;
      tmp2 = efp->tmp2;
      c = efp->c;
      nn = efp->nn;
      y = efp->y;
      x = efp->x;
      ln = efp->ln;
      n = efp->n;
      c_p = efp->c_p;
      goto L_call16;
    case 4:

      tmp12 = efp->tmp12;
      tmp11 = efp->tmp11;
      nthr_c = efp->nthr_c;
      ln__3 = efp->ln__3;
      tmp = efp->tmp;
      pfib_c = efp->pfib_c;
      t1 = efp->t1;
      t2 = efp->t2;
      c0 = efp->c0;
      tmp2 = efp->tmp2;
      c = efp->c;
      nn = efp->nn;
      y = efp->y;
      x = efp->x;
      ln = efp->ln;
      n = efp->n;
      c_p = efp->c_p;
      goto L_call17;
    }
    goto L_call13;
  }
  efp = (struct pfib_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct pfib_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  ln = 0;
  if (n <= 2) {{{
	return (1);
  }
  }
  } else {{{{
	  int             tmp_arg6;

	  nn = 0;
	  c = 0;
	  {
	    ln = 1;
	    (*thst_top).c = pfib_c;
	    (*thst_top).stat = thr_new_runnable;
	    thst_top++;
	  }
	  {
	    int             tmp_arg5;

	    ln__3 = 0;
	    ln__3 = 1;
	    tmp_arg5 = n - 1;
	    while ((tmp11 = pfib(new_esp, nthr_c, tmp_arg5)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
	      if (!((size_t) efp->tmp_esp & 2)) {
		efp->tmp12 = tmp12;
		efp->tmp11 = tmp11;
		efp->nthr_c = nthr_c;
		efp->ln__3 = ln__3;
		efp->tmp = tmp;
		efp->pfib_c = pfib_c;
		efp->t1 = t1;
		efp->t2 = t2;
		efp->c0 = c0;
		efp->tmp2 = tmp2;
		efp->c = c;
		efp->nn = nn;
		efp->y = y;
		efp->x = x;
		efp->ln = ln;
		efp->n = n;
		efp->c_p = c_p;
		efp->nthr_c02.fun = nthr_c_in_pfib;
		efp->nthr_c02.fr = (void (*)) efp;
		efp->pfib_c0.fun = pfib_c_in_pfib;
		efp->pfib_c0.fr = (void (*)) efp;
		efp->call_id = 0;
	      }
	      return ((int) 0 - 1);
	  L_call13:
	      ;
	      new_esp = esp + esp_flag;
	      esp_flag = 0;
	      tmp_arg5 = efp->tmp_arg5;
	    }
	    tmp = tmp11;
	    x = tmp;
	    {
	      nn = nn + 1;
	      if (nn <= 0) {
		while (thr_resume(new_esp, c), (efp->tmp_esp = *((char (**)) esp)) != 0) {
		  if (!((size_t) efp->tmp_esp & 2)) {
		    efp->tmp12 = tmp12;
		    efp->tmp11 = tmp11;
		    efp->nthr_c = nthr_c;
		    efp->ln__3 = ln__3;
		    efp->tmp = tmp;
		    efp->pfib_c = pfib_c;
		    efp->t1 = t1;
		    efp->t2 = t2;
		    efp->c0 = c0;
		    efp->tmp2 = tmp2;
		    efp->c = c;
		    efp->nn = nn;
		    efp->y = y;
		    efp->x = x;
		    efp->ln = ln;
		    efp->n = n;
		    efp->c_p = c_p;
		    efp->nthr_c02.fun = nthr_c_in_pfib;
		    efp->nthr_c02.fr = (void (*)) efp;
		    efp->pfib_c0.fun = pfib_c_in_pfib;
		    efp->pfib_c0.fr = (void (*)) efp;
		    efp->call_id = 1;
		  }
		  return ((int) 0 - 1);
	      L_call14:
		  ;
		  new_esp = esp + esp_flag;
		  esp_flag = 0;
		}
	      }
	    }
	  }
	  if ((*(thst_top - 1)).stat != thr_new_runnable) {{
	      while (scheduling(new_esp), (efp->tmp_esp = *((char (**)) esp)) != 0) {
		if (!((size_t) efp->tmp_esp & 2)) {
		  efp->tmp12 = tmp12;
		  efp->tmp11 = tmp11;
		  efp->nthr_c = nthr_c;
		  efp->ln__3 = ln__3;
		  efp->tmp = tmp;
		  efp->pfib_c = pfib_c;
		  efp->t1 = t1;
		  efp->t2 = t2;
		  efp->c0 = c0;
		  efp->tmp2 = tmp2;
		  efp->c = c;
		  efp->nn = nn;
		  efp->y = y;
		  efp->x = x;
		  efp->ln = ln;
		  efp->n = n;
		  efp->c_p = c_p;
		  efp->nthr_c02.fun = nthr_c_in_pfib;
		  efp->nthr_c02.fr = (void (*)) efp;
		  efp->pfib_c0.fun = pfib_c_in_pfib;
		  efp->pfib_c0.fr = (void (*)) efp;
		  efp->call_id = 2;
		}
		return ((int) 0 - 1);
	    L_call15:
		;
		new_esp = esp + esp_flag;
		esp_flag = 0;
	      }
	  }
	  } else {{
	      thst_top--;
	  }
	  }
	  ln = 2;
	  tmp_arg6 = n - 2;
	  while ((tmp12 = pfib(new_esp, pfib_c, tmp_arg6)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
	    if (!((size_t) efp->tmp_esp & 2)) {
	      efp->tmp12 = tmp12;
	      efp->tmp11 = tmp11;
	      efp->nthr_c = nthr_c;
	      efp->ln__3 = ln__3;
	      efp->tmp = tmp;
	      efp->pfib_c = pfib_c;
	      efp->t1 = t1;
	      efp->t2 = t2;
	      efp->c0 = c0;
	      efp->tmp2 = tmp2;
	      efp->c = c;
	      efp->nn = nn;
	      efp->y = y;
	      efp->x = x;
	      efp->ln = ln;
	      efp->n = n;
	      efp->c_p = c_p;
	      efp->nthr_c02.fun = nthr_c_in_pfib;
	      efp->nthr_c02.fr = (void (*)) efp;
	      efp->pfib_c0.fun = pfib_c_in_pfib;
	      efp->pfib_c0.fr = (void (*)) efp;
	      efp->call_id = 3;
	    }
	    return ((int) 0 - 1);
	L_call16:
	    ;
	    new_esp = esp + esp_flag;
	    esp_flag = 0;
	    tmp_arg6 = efp->tmp_arg6;
	  }
	  tmp2 = tmp12;
	  y = tmp2;
	  {
	    nn = nn - 1;
	    if (nn < 0) {{
		c0 = thst_top++;
		(*c0).c = pfib_c;
		(*c0).stat = thr_new_suspended;
		c = c0;
		ln = 3;
		while (scheduling(new_esp), (efp->tmp_esp = *((char (**)) esp)) != 0) {
		  if (!((size_t) efp->tmp_esp & 2)) {
		    efp->tmp12 = tmp12;
		    efp->tmp11 = tmp11;
		    efp->nthr_c = nthr_c;
		    efp->ln__3 = ln__3;
		    efp->tmp = tmp;
		    efp->pfib_c = pfib_c;
		    efp->t1 = t1;
		    efp->t2 = t2;
		    efp->c0 = c0;
		    efp->tmp2 = tmp2;
		    efp->c = c;
		    efp->nn = nn;
		    efp->y = y;
		    efp->x = x;
		    efp->ln = ln;
		    efp->n = n;
		    efp->c_p = c_p;
		    efp->nthr_c02.fun = nthr_c_in_pfib;
		    efp->nthr_c02.fr = (void (*)) efp;
		    efp->pfib_c0.fun = pfib_c_in_pfib;
		    efp->pfib_c0.fr = (void (*)) efp;
		    efp->call_id = 4;
		  }
		  return ((int) 0 - 1);
	      L_call17:
		  ;
		  new_esp = esp + esp_flag;
		  esp_flag = 0;
		}
	    }
	    }
	  }
	  return (x + y);
  }
  }
  }
  }
}

struct msc_main_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp15;
                  closure_t(*msc_main_c);
  int             t1;
  int             tmp3;
  int             n;
  int             ln;
  char            (**argv);
  int             argc;
  cont            c_p;
  closure_t       msc_main_c0;
};

struct msc_main_c_in_msc_main_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  void            (*tmp14);
  int             tmp13;
  reason          rsn;
  thst_ptr        cp;
  struct msc_main_frame (*xfp);
};

char            (*msc_main_c_in_msc_main(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct msc_main_c_in_msc_main_frame (*efp);
  struct msc_main_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 7;
  char            (*parmp) = (char (*)) ((size_t) esp ^ esp_flag);
  thst_ptr        cp = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(thst_ptr) + sizeof(align_t) + -1) / sizeof(align_t)), *((thst_ptr(*)) parmp));
  reason          rsn = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t)), *((reason(*)) parmp));
  char            (*argp);
  int             tmp13;
  void            (*tmp14);

  closure_t(*tmp_fp3);
  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct msc_main_c_in_msc_main_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct msc_main_c_in_msc_main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp14 = efp->tmp14;
      tmp13 = efp->tmp13;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call18;
    case 1:

      tmp14 = efp->tmp14;
      tmp13 = efp->tmp13;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call19;
    }
    goto L_call18;
  }
  efp = (struct msc_main_c_in_msc_main_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct msc_main_c_in_msc_main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  switch (rsn) {
  case rsn_cont:

    switch (xfp->ln) {
    case 1:

      goto L1;
    }
    return;
  case rsn_retval:

    switch (xfp->ln) {
    case 1:

      *((void (**)) efp) = (void (*)) &xfp->t1;
      return (0);
    }
    return;
  }
  return;
  xfp->n = atoi(xfp->argv[1]);
  xfp->ln = 1;
  while ((tmp13 = pfib(new_esp, xfp->msc_main_c, xfp->n)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
    if (!((size_t) efp->tmp_esp & 2)) {
      efp->tmp14 = tmp14;
      efp->tmp13 = tmp13;
      efp->rsn = rsn;
      efp->cp = cp;
      efp->xfp = xfp;
      efp->call_id = 0;
    }
    return (efp->tmp_esp);
L_call18:
    ;
    new_esp = esp + esp_flag;
    esp_flag = 0;
  }
  xfp->t1 = tmp13;
L1:
  ;
  xfp->tmp3 = xfp->t1;
  {
    tmp_fp3 = xfp->c_p;
    argp = (char (*)) ((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
    *((reason(*)) argp) = rsn_retval, argp = (char (*)) ((align_t(*)) argp + (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t));
    *((struct _thstelm(**)) argp) = cp, argp = (char (*)) ((align_t(*)) argp + (sizeof(struct _thstelm(*)) + sizeof(align_t) + -1) / sizeof(align_t));
    *((closure_t(**)) argp) = tmp_fp3;
    efp->tmp14 = tmp14;
    efp->tmp13 = tmp13;
    efp->rsn = rsn;
    efp->cp = cp;
    efp->xfp = xfp;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 1;
    return (efp->tmp_esp);
L_call19:
    if (esp_flag & 2)
      return ((char (*)) 2);

    tmp14 = *((void (**)) efp->argp);
  }
  *((int (*)) tmp14) = xfp->tmp3;
  (*cp).c = xfp->c_p;
  (*cp).stat = thr_runnable;
  return;
}

int             msc_main(char (*esp), cont c_p, int argc, char (**argv)) {
  struct msc_main_frame (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 7;
  int             ln;
  int             n;
  int             tmp3;
  int             t1;

  closure_t(*msc_main_c) = &((struct msc_main_frame(*)) esp)->msc_main_c0;
  int             tmp15;

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct msc_main_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct msc_main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp15 = efp->tmp15;
      msc_main_c = efp->msc_main_c;
      t1 = efp->t1;
      tmp3 = efp->tmp3;
      n = efp->n;
      ln = efp->ln;
      argv = efp->argv;
      argc = efp->argc;
      c_p = efp->c_p;
      goto L_call20;
    }
    goto L_call20;
  }
  efp = (struct msc_main_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct msc_main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  ln = 0;
  n = atoi( argv[1] );
  ln = 1;
  while ((tmp15 = pfib(new_esp, msc_main_c, n)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
    if (!((size_t) efp->tmp_esp & 2)) {
      efp->tmp15 = tmp15;
      efp->msc_main_c = msc_main_c;
      efp->t1 = t1;
      efp->tmp3 = tmp3;
      efp->n = n;
      efp->ln = ln;
      efp->argv = argv;
      efp->argc = argc;
      efp->c_p = c_p;
      efp->msc_main_c0.fun = msc_main_c_in_msc_main;
      efp->msc_main_c0.fr = (void (*)) efp;
      efp->call_id = 0;
    }
    return ((int) 0 - 1);
L_call20:
    ;
    new_esp = esp + esp_flag;
    esp_flag = 0;
  }
  tmp3 = tmp15;
  return (tmp3);
}

struct main_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
                  closure_t(*main_c);
  int             msc_main_ret;
  int             ln;
  char            (**argv);
  int             argc;
  closure_t       main_c0;
};

struct main_c_in_main_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp16;
  reason          rsn;
  thst_ptr        cp;
  struct main_frame (*xfp);
};

char            (*main_c_in_main(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct main_c_in_main_frame (*efp);
  struct main_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 7;
  char            (*parmp) = (char (*)) ((size_t) esp ^ esp_flag);
  thst_ptr        cp = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(thst_ptr) + sizeof(align_t) + -1) / sizeof(align_t)), *((thst_ptr(*)) parmp));
  reason          rsn = (parmp = (char (*)) ((align_t(*)) parmp - (sizeof(reason) + sizeof(align_t) + -1) / sizeof(align_t)), *((reason(*)) parmp));
  int             tmp16;

  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct main_c_in_main_frame(*)) esp;
    esp = (char (*)) ((align_t(*)) esp + (sizeof(struct main_c_in_main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**)) esp) = 0;
    switch ((*efp).call_id) {
    case 0:

      tmp16 = efp->tmp16;
      rsn = efp->rsn;
      cp = efp->cp;
      xfp = efp->xfp;
      goto L_call21;
    }
    goto L_call21;
  }
  efp = (struct main_c_in_main_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct main_c_in_main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  switch (rsn) {
  case rsn_cont:

    switch (xfp->ln) {
    case 0:

      goto L0;
    case 1:

      goto L1;
    }
    return;
  case rsn_retval:

    *((void (**)) efp) = (void (*)) &xfp->msc_main_ret;
    return (0);
  }
  return;
L0:
  ;
  xfp->ln = 1;
  while ((tmp16 = msc_main(new_esp, xfp->main_c, xfp->argc, xfp->argv)) == (int) 0 - 1 && (efp->tmp_esp = *((char (**)) esp)) != 0) {
    if (!((size_t) efp->tmp_esp & 2)) {
      efp->tmp16 = tmp16;
      efp->rsn = rsn;
      efp->cp = cp;
      efp->xfp = xfp;
      efp->call_id = 0;
    }
    return (efp->tmp_esp);
L_call21:
    ;
    new_esp = esp + esp_flag;
    esp_flag = 0;
  }
  xfp->msc_main_ret = tmp16;
L1:
  ;
  *((void (**)) efp) = xfp->msc_main_ret;
  /* 以下２行は変換後,つけたし */
  printf( "%d\n", xfp->msc_main_ret );
  exit( 0 );
  return (0);
}

int             main(int argc, char (**argv)) {
  struct main_frame (*efp);
  char            (*new_esp);
  char            (estack[65536]);
  char            (*esp) = estack;
  int             ln;
  int             msc_main_ret;

  closure_t(*main_c) = &((struct main_frame(*)) esp)->main_c0;
  efp = (struct main_frame(*)) esp;
  esp = (char (*)) ((align_t(*)) esp + (sizeof(struct main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**)) esp) = 0;
  new_esp = esp;
  ln = 0;
  (*thst_top).c = main_c;
  (*thst_top).stat = thr_runnable;
  thst_top++;
  do {
    while (scheduling(new_esp), (efp->tmp_esp = *((char (**)) esp)) != 0) {
      *((char (**)) esp) = 0;
      efp->main_c = main_c;
      efp->msc_main_ret = msc_main_ret;
      efp->ln = ln;
      efp->argv = argv;
      efp->argc = argc;
      efp->main_c0.fun = main_c_in_main;
      efp->main_c0.fr = (void (*)) efp;
      new_esp = esp + 1 + lw_call(efp->tmp_esp);
    }
  } while (1);
  return (0);
}
