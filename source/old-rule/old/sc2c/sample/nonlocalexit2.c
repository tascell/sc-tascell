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

char            (*
		 lw_call(char (*esp)))
{
  closure_t(*clos) = *((closure_t(**)) esp);
  return (clos->fun(esp, clos->fr));
}

struct f_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
                  closure_t(*pg);
};

int             f(char (*esp), 
		  closure_t(*pg))
{
  char            (*argp);
  struct f_frame  (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 3;
  closure_t(*tmp_fp);
  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct f_frame(*)) esp;
    esp = (char (*))((align_t(*)) esp + (sizeof(struct f_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**))esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:

      goto L_call;
    }
    goto L_call;
  }
  efp = (struct f_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct f_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  {
    tmp_fp = pg;
    argp = (char (*))((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
    *((int (*))argp) = 0, argp = (char (*))((align_t(*)) argp + (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t));
    *((closure_t(**)) argp) = tmp_fp;
    efp->pg = pg;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return ((int)0 - 1);
L_call:
    ;
    pg = efp->pg;
  }
  return (0);
}

struct g_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp2;
                  closure_t(*g1);
                  closure_t(*pg);
  closure_t       g10;
};

struct g1_in_g_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp;
  int             m;
  struct g_frame  (*xfp);
};

char            (*g1_in_g(char (*esp), 
			  void (*xfp0)))
{
  char            (*new_esp);
  struct g1_in_g_frame (*efp);
  struct g_frame  (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char            (*parmp) = (char (*))((size_t) esp ^ esp_flag);
  int             m = (parmp = (char (*))((align_t(*)) parmp - (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t)), *((int (*))parmp));
  int             tmp;
LGOTO:
  ;
  efp = (struct g1_in_g_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct g1_in_g_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  if (m > 0) {
    xfp->call_id = -1;
    return ((char (*))xfp);
  } else {
    new_esp = esp;
    while ((tmp = g(new_esp, xfp->g1)) == (int)0 - 1 && (efp->tmp_esp = *((char (**))esp)) != 0) {
      char            (*goto_fr);
      *((char (**))esp) = 0;
      efp->tmp = tmp;
      efp->m = m;
      efp->xfp = xfp;
      goto_fr = lw_call(efp->tmp_esp);
      if (goto_fr && (char (*))goto_fr < (char (*))efp)
	return (goto_fr);

      if ((char (*))goto_fr == (char (*))efp)
	goto LGOTO;

      new_esp = esp + 1;
    }
    tmp;
  }
  return (0);
}

int             g(char (*esp), 
		  closure_t(*pg))
{
  char            (*argp);
  struct g_frame  (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 3;
  closure_t(*g1) = &((struct g_frame(*)) esp)->g10;
  int             tmp2;
  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct g_frame(*)) esp;
    esp = (char (*))((align_t(*)) esp + (sizeof(struct g_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**))esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:

      goto L_call2;
    case 1:

      goto L_call3;
    case 2:

      goto L_call4;
    case -1:

      tmp2 = efp->tmp2;
      g1 = efp->g1;
      pg = efp->pg;
      goto L1;
    }
    goto L_call2;
  }
  efp = (struct g_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct g_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  if (pg == 0) {
    new_esp = esp;
    while ((tmp2 = f(new_esp, g1)) == (int)0 - 1 && (efp->tmp_esp = *((char (**))esp)) != 0) {
      efp->tmp2 = tmp2;
      efp->g1 = g1;
      efp->pg = pg;
      efp->g10.fun = g1_in_g;
      efp->g10.fr = (void (*))efp;
      efp->call_id = 0;
      return ((int)0 - 1);
  L_call2:
      ;
      tmp2 = efp->tmp2;
      g1 = efp->g1;
      pg = efp->pg;
      new_esp = esp + 1;
    }
    return (tmp2);
  } else {
    closure_t(*tmp_fp2);
    {
      tmp_fp2 = g1;
      argp = (char (*))((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
      *((int (*))argp) = 1, argp = (char (*))((align_t(*)) argp + (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t));
      *((closure_t(**)) argp) = tmp_fp2;
      efp->g10.fun = g1_in_g;
      efp->g10.fr = (void (*))efp;
      efp->tmp2 = tmp2;
      efp->g1 = g1;
      efp->pg = pg;
      efp->argp = argp;
      efp->tmp_esp = argp;
      efp->call_id = 1;
      return ((int)0 - 1);
  L_call3:
      ;
      tmp2 = efp->tmp2;
      g1 = efp->g1;
      pg = efp->pg;
    }
    return (0);
  }
L1:
  ;
  if (pg == 0) {
    return (0);
  } else {
    closure_t(*tmp_fp3);
    {
      tmp_fp3 = pg;
      argp = (char (*))((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
      *((int (*))argp) = 1, argp = (char (*))((align_t(*)) argp + (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t));
      *((closure_t(**)) argp) = tmp_fp3;
      efp->g10.fun = g1_in_g;
      efp->g10.fr = (void (*))efp;
      efp->tmp2 = tmp2;
      efp->g1 = g1;
      efp->pg = pg;
      efp->argp = argp;
      efp->tmp_esp = argp;
      efp->call_id = 2;
      return ((int)0 - 1);
  L_call4:
      ;
      tmp2 = efp->tmp2;
      g1 = efp->g1;
      pg = efp->pg;
    }
  }
}

struct main_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp3;
};

int 
main()
{
  struct main_frame (*efp);
  char            (*new_esp);
  char            (estack[65536]);
  char            (*esp) = estack;
  int             tmp3;
LGOTO:
  ;
  efp = (struct main_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  new_esp = esp;
  while ((tmp3 = g(new_esp, 0)) == (int)0 - 1 && (efp->tmp_esp = *((char (**))esp)) != 0) {
    char            (*goto_fr);
    *((char (**))esp) = 0;
    efp->tmp3 = tmp3;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char (*))goto_fr == (char (*))efp)
      goto LGOTO;

    new_esp = esp + 1;
  }
  return (tmp3);
}
