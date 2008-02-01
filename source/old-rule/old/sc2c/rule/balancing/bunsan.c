#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* compare & swap を使えば 1つでよい */
static int n_req = 0;
static int n_done = 0;

void receive_req(int signo){ n_req++; }

void setsig(int signo, void (*sighndl)(int)){
  struct sigaction act;
  act.sa_handler = sighndl;
  act.sa_flags   = SA_RESTART; 
  sigemptyset(&act.sa_mask);
  if(sigaction(signo,&act,0) < 0){ perror("sigaction"); exit(1); }
}

void expand_buf(char **buf, size_t *n){
  *n *= 4; *buf = (char *) realloc(*buf, *n);
}

#define BUF_CHAR(buf, n, i, ch)                \
  do{                                          \
    if (*(i) == *(n)) expand_buf((buf), (n));  \
    (*(buf))[(*(i))++] = (ch);                 \
  }while(0)

int read_sexp(char **buf, size_t *n, size_t *i){
  int ch;
  while(isspace(ch = getchar()))
    ;
  if(ch == '('){
    BUF_CHAR(buf, n, i, ch);
    BUF_CHAR(buf, n, i, 0);
    while(read_sexp(buf, n, i))
      ;
    ch = getchar();
    if (ch == ')') {
      BUF_CHAR(buf, n, i, ch);
      BUF_CHAR(buf, n, i, 0);
      return 1;
    }
    return 0;
    /* error */
  } else if(ch == '"'){
    BUF_CHAR(buf, n, i, ch);
    while(1){
      ch = getchar();
      if(ch == '"'){ 
	BUF_CHAR(buf, n, i, ch); 
	BUF_CHAR(buf, n, i, 0); 
	return 1;
      }
      if(ch == '\\'){
	ch = getchar();
	BUF_CHAR(buf, n, i, ch);
      }
    }
    /* unreachable */
  } else if(ch != ')'){
    BUF_CHAR(buf, n, i, ch);
    do{
      ch = getchar();
      if(isspace(ch)) goto Le;
      switch(ch){
      case '"': case '(': case ')':
	goto Le;
      }
      BUF_CHAR(buf, n, i, ch);
    }while(1);
  Le:
    ungetc(ch, stdin);
    BUF_CHAR(buf, n, i, 0);
    return 1;
  }
  ungetc(ch, stdin);
  return 0;
}

void next_token(char *buf, size_t *j){
  size_t i = *j;
  while(buf[i]) i++;
  *j = i + 1;
}

int check_token(char *buf, size_t *j, char *str){
  size_t i = *j, k = 0;
  while(str[k] && buf[i+k] == str[k])
    k++;
  if(buf[i+k] == 0){
    *j = i + k + 1;
    return 1;
  }
  return 0;
}

void get_result(){
  int (*bk)(int, void *);
  size_t n = 1024, i = 0, j = 0;
  int ch;
  char *buf = (char *)malloc(n);
  if(read_sexp(&buf, &n, &i)){
    check_token(buf, &j, "(");
    check_token(buf, &j, "result");
    sscanf(buf+j, "%d", &bk); next_token(buf, &j);
    bk(1, buf);
  }
  free(buf);
}

double pi(int (*bk)(int, void *), int n1, int n2){
  if(n2 - n1 < 20){
    double s = 0.0;
    int i;
    for(i=n1;i<n2;i++)
      s += 1.0/(1.0e+8 + (1.0e-8 * i) * (i+1));
    return s * 4.0;
  }else{
    double s1, s2;
    int nm = (n1+n2)/2;
    int spawnable = 1;
    {
      int avail;
      int (*bk2)(int, void *) = bk;
      int bk(int rsn, void *rest){
	switch(rsn){
	case 0:
	  if(bk2(0, 0)) return 1;
	  if(spawnable){
	    spawnable = 0;
	    avail = 0;
	    printf("(task %d pi %d %d)\n", (void *)bk, nm, n2);
	    return 1;
	  }
	  return 0;
	case 1:
	  {
	    double s2a;
	    size_t j = 0;
	    char *buf = rest;
	    check_token(buf, &j, "(");
	    check_token(buf, &j, "result");
	    next_token(buf, &j);
	    check_token(buf, &j, "pi");
	    sscanf(buf+j, "%lf", &s2a); next_token(buf, &j);
	    check_token(buf, &j, ")");
	    s2 = s2a;
	    avail = 1;
	    return 0;
	  }
	}
      }
      if(n_req != n_done){ if(!bk(0, 0)) printf("(task 0 no)\n"); n_done++; }
      s1 = pi(bk, n1, nm);
      if(!spawnable){
	if(!avail) printf("(waiting %d)\n", (void *)bk);
	while(!avail) get_result(); 
      }
    }
    if(spawnable)
      s2 = pi(bk, nm, n2);
    return s1+s2;
  }
}

int read_exec_print(){
  size_t n = 1024, i = 0, j = 0;
  int ch;
  char *buf = (char *)malloc(n);
  int infun(){ return 0; }
  if(read_sexp(&buf, &n, &i)){
    int id;
    int k1, k2;
    /* 
       if(check_sexp(buf, &j, "(task %d pi %d %d)", &id, &k1, &k2)){
         ...
       } else if(check_sexp(buf, &j, "(task %d fib %d)", &id, &k1)) {
         ...
       }
       と書きたい....
     */
    check_token(buf, &j, "(");
    check_token(buf, &j, "task");
    sscanf(buf+j, "%d", &id);    
    next_token(buf, &j);
    check_token(buf, &j, "pi");
    sscanf(buf+j, "%d", &k1); next_token(buf, &j);
    sscanf(buf+j, "%d", &k2); next_token(buf, &j);
    check_token(buf, &j, ")");
    printf("(result %d pi %f)\n", id, pi(infun, k1, k2));
    free(buf);
    return 1;
  }
  free(buf);
  return 0;
}

main(int argc, char *argv[]){
  setsig(SIGINT, receive_req);
  while(read_exec_print());
  exit(0);
}


