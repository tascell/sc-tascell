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

char            (*lw_call(char (*esp))) {
  closure_t(*clos) = *((closure_t(**)) esp);
  return (clos->fun(esp, clos->fr));
}

struct h_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp2;
  int             tmp;
                  closure_t(*g);
  int             i;
};

int             h(char (*esp), int i, closure_t(*g)){
  char            (*argp);
  struct h_frame  (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 3;
  int             tmp;
  int             tmp2;
  closure_t(*tmp_fp);
  closure_t(*tmp_fp2);
  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct h_frame(*)) esp;
    esp = (char (*))((align_t(*)) esp + (sizeof(struct h_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**))esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:

      goto L_call;
    case 1:

      goto L_call2;
    }
    goto L_call;
  }
  efp = (struct h_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct h_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  {
    tmp_fp = g;
    argp = (char (*))((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
    *((int (*))argp) = i, argp = (char (*))((align_t(*)) argp + (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t));
    *((closure_t(**)) argp) = tmp_fp;
    efp->tmp2 = tmp2;
    efp->tmp = tmp;
    efp->g = g;
    efp->i = i;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return ((int)0 - 1);
L_call:
    ;
    tmp2 = efp->tmp2;
    tmp = efp->tmp;
    g = efp->g;
    i = efp->i;
    tmp = *((int (*))efp->argp);
  }
  {
    tmp_fp2 = g;
    argp = (char (*))((align_t(*)) esp + (sizeof(char (*)) + sizeof(align_t) + -1) / sizeof(align_t));
    *((int (*))argp) = tmp, argp = (char (*))((align_t(*)) argp + (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t));
    *((closure_t(**)) argp) = tmp_fp2;
    efp->tmp2 = tmp2;
    efp->tmp = tmp;
    efp->g = g;
    efp->i = i;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 1;
    return ((int)0 - 1);
L_call2:
    ;
    tmp2 = efp->tmp2;
    tmp = efp->tmp;
    g = efp->g;
    i = efp->i;
    tmp2 = *((int (*))efp->argp);
  }
  return (tmp2);
}

struct foo_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp3;
                  closure_t(*g1);
  int             x;
  int             a;
  int             tmp_arg;
  closure_t       g10;
};

struct g1_in_foo_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             b;
  struct foo_frame (*xfp);
};

char            (*g1_in_foo(char (*esp), void (*xfp0))) {
  char            (*new_esp);
  struct g1_in_foo_frame (*efp);
  struct foo_frame (*xfp) = xfp0;
  size_t          esp_flag = (size_t) esp & 3;
  char            (*parmp) = (char (*))((size_t) esp ^ esp_flag);
  int             b = (parmp = (char (*))((align_t(*)) parmp - (sizeof(int) + sizeof(align_t) + -1) / sizeof(align_t)), *((int (*))parmp));
LGOTO:
  ;
  efp = (struct g1_in_foo_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct g1_in_foo_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  (xfp->x)++;
  *((int (*))efp) = xfp->a + b;
  return (0);
  return (0);
}

int             foo(char (*esp), int a){
  struct foo_frame (*efp);
  char            (*new_esp);
  size_t          esp_flag = (size_t) esp & 3;
  int             x = 0;
  closure_t(*g1) = &((struct foo_frame(*)) esp)->g10;
  int             tmp3;
  int             tmp_arg;
  if (esp_flag) {
    (size_t) esp ^= esp_flag;
    efp = (struct foo_frame(*)) esp;
    esp = (char (*))((align_t(*)) esp + (sizeof(struct foo_frame) + sizeof(align_t) + -1) / sizeof(align_t));
    *((char (**))esp) = 0;
LGOTO:
    switch ((*efp).call_id) {
    case 0:

      goto L_call3;
    }
    goto L_call3;
  }
  efp = (struct foo_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct foo_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  new_esp = esp;
  tmp_arg = a + 1;
  while ((tmp3 = h(new_esp, tmp_arg, g1)) == (int)0 - 1 && (efp->tmp_esp = *((char (**))esp)) != 0) {
    efp->tmp3 = tmp3;
    efp->g1 = g1;
    efp->x = x;
    efp->a = a;
    efp->g10.fun = g1_in_foo;
    efp->g10.fr = (void (*))efp;
    efp->call_id = 0;
    return ((int)0 - 1);
L_call3:
    ;
    tmp3 = efp->tmp3;
    g1 = efp->g1;
    x = efp->x;
    a = efp->a;
    tmp_arg = efp->tmp_arg;
    new_esp = esp + 1;
  }
  return (tmp3 + x);
}

struct main_frame {
  char            (*tmp_esp);
  char            (*argp);
  int             call_id;
  int             tmp4;
};

int             main() {
  struct main_frame (*efp);
  char            (*new_esp);
  char            (estack[65536]);
  char            (*esp) = estack;
  int             tmp4;
LGOTO:
  ;
  efp = (struct main_frame(*)) esp;
  esp = (char (*))((align_t(*)) esp + (sizeof(struct main_frame) + sizeof(align_t) + -1) / sizeof(align_t));
  *((char (**))esp) = 0;
  new_esp = esp;
  while ((tmp4 = foo(new_esp, 1)) == (int)0 - 1 && (efp->tmp_esp = *((char (**))esp)) != 0) {
    char            (*goto_fr);
    *((char (**))esp) = 0;
    efp->tmp4 = tmp4;
    goto_fr = lw_call(efp->tmp_esp);
    if ((char (*))goto_fr == (char (*))efp)
      goto LGOTO;

    new_esp = esp + 1;
  }
  return (tmp4);
}
