#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "xccmem.h"

#define MAX_V 8388929  /* 8M + 321 */
#define MAX_E 67109185 /* 64M + 321 */

#define V_LIST_ALLOC_SIZE 600
// #define V_LIST_ALLOC_SIZE 200

int v_length;

struct vertex {
  int degree;
  int first_e;
  int parent;
} vv[MAX_V];

int e_length;

struct edge {
  int next_v;
  int next_e;
} ee[MAX_E];

/* list of arrays of vertices */

int v_list_length;

struct v_list_body {
  struct v_list_body *next;
  int i;
  int v[V_LIST_ALLOC_SIZE];
} *v_list;

struct v_list_body *alloc_v_list(struct v_list_body *next){
  struct v_list_body *a = 
    (struct v_list_body *) malloc(sizeof(struct v_list_body));
  a->next = next;
  a->i = V_LIST_ALLOC_SIZE;
  return a;
}

struct v_list_body *dealloc_v_list(struct v_list_body *a){
  struct v_list_body *next = a->next;
  free(a);
  return next;
}

/* recursive DF search with a bound "b" spilling into v_list */



#ifdef PARALLEL_SEARCH

search1(int v, int b){
  int i;
  int d = vv[v].degree;
  int e = vv[v].first_e;
  for(i=0; i<d; i++, e = ee[e].next_e){
    int nv = ee[e].next_v;
    if(! cas_int(vv[nv].parent, 0, v+1)) {
      start_access_after_lock();
      // printf("mark %d\n", nv);
      if(b > 0){
	search1(nv, b-1);
      }else{
	if(v_list->i <= 0){
	  v_list = alloc_v_list(v_list);
	  v_list_length++;
	  // printf("v_list_length = %d\n", v_list_length);
	}
	// printf("push %d\n", nv);
	v_list->v[--v_list->i] = nv;
      }
    }
  }
}

#elif PARALLEL_SEARCH2

search1(int v, int b){
  int i;
  int d = vv[v].degree;
  int e = vv[v].first_e;
  for(i=0; i<d; i++, e = ee[e].next_e){
    int nv = ee[e].next_v;
    if(atomic_read_int_to_finish_write(vv[nv].parent) == 0){
      atomic_write_int(vv[nv].parent, v+1);
      // printf("mark %d\n", nv);
      if(b > 0){
	search1(nv, b-1);
      }else{
	if(v_list->i <= 0){
	  v_list = alloc_v_list(v_list);
	  v_list_length++;
	  // printf("v_list_length = %d\n", v_list_length);
	}
	// printf("push %d\n", nv);
	v_list->v[--v_list->i] = nv;
      }
    }
  }
}

#else

search1(int v, int b){
  int i;
  int d = vv[v].degree;
  int e = vv[v].first_e;
  for(i=0; i<d; i++, e = ee[e].next_e){
    int nv = ee[e].next_v;
    if(vv[nv].parent == 0){
      vv[nv].parent = v+1;
      // printf("mark %d\n", nv);
      if(b > 0){
	search1(nv, b-1);
      }else{
	if(v_list->i <= 0){
	  v_list = alloc_v_list(v_list);
	  v_list_length++;
	  // printf("v_list_length = %d\n", v_list_length);
	}
	// printf("push %d\n", nv);
	v_list->v[--v_list->i] = nv;
      }
    }
  }
}
#endif

#if 0
search_s(int v){
  int i;
  int d = vv[v].degree;
  int e = vv[v].first_e;
  int vi = v_list->i;
  for(i=0; i<d; i++, e = ee[e].next_e){
    int nv = ee[e].next_v;
    if(vv[nv].parent == 0){
      vv[nv].parent = v+1;
      {
	if(vi <= 0){
	  v_list->i = vi;
	  v_list = alloc_v_list(v_list);
	  v_list_length++;
	  vi = v_list->i;
	  // printf("v_list_length = %d\n", v_list_length);
	}
	// printf("push %d\n", nv);
	v_list->v[--vi] = nv;
      }
    }
  }
  v_list->i = vi;
}
#endif

/* serial DF search with v_list */

search_s2(int fv){
  struct v_list_body *my_v_list;
  int my_v_list_length;
  int vi;
  vv[fv].parent = fv+1;
  my_v_list = alloc_v_list(0);
  my_v_list_length = 1;
  vi = my_v_list->i;
  my_v_list->v[--vi] = fv;
  while (vi < V_LIST_ALLOC_SIZE) {
    int v = my_v_list->v[vi++];
    int i;
    int d = vv[v].degree;
    int e = vv[v].first_e;
    for(i=0; i<d; i++, e = ee[e].next_e){
      int nv = ee[e].next_v;
      if(vv[nv].parent == 0){
	vv[nv].parent = v+1;
	{
	  if(vi <= 0){
	    my_v_list->i = vi;
	    my_v_list = alloc_v_list(my_v_list);
	    my_v_list_length++;
	    vi = my_v_list->i;
	    // printf("my_v_list_length = %d\n", my_v_list_length);
	  }
	  // printf("s2 push %d\n", nv);
	  my_v_list->v[--vi] = nv;
	}
      }
    }
    while (vi == V_LIST_ALLOC_SIZE && my_v_list_length > 1){
      my_v_list = dealloc_v_list(my_v_list);
      my_v_list_length--;
      vi = my_v_list->i;
    }
  }
  my_v_list->i = vi;
}

/* 
   employing DF search1 for each vertex in v_list.
   repeat until the "next" v_list is empty.
*/

search(int v){
  struct v_list_body *my_v_list, *mlist, *next;
  int i, j, my_v_list_length, mlist_length;
  vv[v].parent = v+1;
  v_list = alloc_v_list(0);
  v_list_length = 1;
  search1(v, 30);
  do{
    mlist = my_v_list = v_list;
    mlist_length = my_v_list_length = v_list_length;
    // printf("my_v_list_length = %d\n", my_v_list_length);
    v_list = alloc_v_list(0);
    v_list_length = 1;
    for(i=0; i<my_v_list_length ; i++, my_v_list = my_v_list->next)
      for(j= my_v_list->i ; j < V_LIST_ALLOC_SIZE; j++)
	search1(my_v_list->v[j], 30);
    for(i=0; i<mlist_length ; i++)
      mlist = dealloc_v_list(mlist);
  }while(v_list_length > 1 || v_list->i < V_LIST_ALLOC_SIZE);
}

/* for graph construction */

int make_edge1(int v1, int v2, int e){
  // printf("%d-%d\n", v1, v2);
  ee[e].next_v = v2;
  ee[e].next_e = vv[v1].first_e;
  vv[v1].first_e = e++;
  vv[v1].degree++;
  return e;
}

int check_dup1(int v1, int v2){
  int i;
  int d = vv[v1].degree;
  int e = vv[v1].first_e;
  for(i=0; i<d; i++, e = ee[e].next_e){
    int nv = ee[e].next_v;
    if(nv == v2) return 0;
  }
  return 1;
}

int checked_make_edge1(int v1, int v2, int e){
  if(check_dup1(v1, v2))
      e = make_edge1(v1, v2, e);
  return e;
}

int make_edge(int v1, int v2, int e){
  // printf("%d-%d\n", v1, v2);
  e = make_edge1(v1, v2, e);
  e = make_edge1(v2, v1, e);
  return e;
}

int checked_make_edge(int v1, int v2, int e){
  e = checked_make_edge1(v1, v2, e);
  e = checked_make_edge1(v2, v1, e);
  return e;
}

/* 2D-torus */

make_2dt(int n){
  int ix, iy, e;
  unsigned long next = n;
  for(ix=0;ix<n;ix++)
    for(iy=0;iy<n;iy++){
      int k = ix*n+iy;
      vv[k].degree = 0;
      vv[k].first_e = 0;
      vv[k].parent = 0;
    }
  e = 0;
  for(ix=0;ix<n;ix++)
    for(iy=0;iy<n;iy++){
      int k = ix*n+iy;
      int px = ix - 1 ;
      int py = iy - 1 ;
      int sx = ix + 1 ;
      int sy = iy + 1 ;
      if (px < 0) px += n;
      if (py < 0) py += n;
      if (sx >= n) sx -= n;
      if (sy >= n) sy -= n;
#if 1
      // employing pseudo randomness
      {
	int v[4], p, q;
	v[0] = ix*n+py;
	v[1] = px*n+iy;
	v[2] = ix*n+sy;
	v[3] = sx*n+iy;
	next = next * 1103515245 + 12345;
	for(q=4; q>0; --q){
	  next = next * 1103515245 + 12345;
	  p = ((q * ((next / 65536) % 32768)) / 32768);
	  e = make_edge1(k, v[p], e);
	  v[p] = v[q-1];
	}
      }
#else
      // without pseudo randomness, some unfair preference exists.
      // for s2
      e = make_edge(ix*n+iy, ix*n+sy, e);
      e = make_edge(ix*n+iy, sx*n+iy, e);
      // for calldfs
      // e = make_edge(ix*n+iy, sx*n+iy, e);
      // e = make_edge(ix*n+iy, ix*n+sy, e);
#endif
    }
  v_length = n*n;
  e_length = e;
}

/* hyper-cube */

make_ncube(int n){
  int i, j, s = 1 << n, e = 0;
  unsigned long next = n;
  for(i = 0; i<s ; i++){
    vv[i].degree = 0;
    vv[i].first_e = 0;
    vv[i].parent = 0;
  }
  for(i = 0; i<s ; i++){
#if 1
    int p, q, *v = (int *)malloc(n*sizeof(int));
    for(j = 0; j<n; j++)
      v[j] = i ^ (1 << j);
    next = next * 1103515245 + 12345;
    for(q=n; q>0; --q){
      next = next * 1103515245 + 12345;
      p = ((q * ((next / 65536) % 32768)) / 32768);
      e = make_edge1(i, v[p], e);
      v[p] = v[q-1];
    }
    free(v);
#else
    // without pseudo randomness, some unfair preference exists.
    for(j = 0; j<n; j++)
      {
#if 1
	int b = 1 << j;
#else
	int b = 1 << (n-1-j);
#endif
	e = make_edge1(i, i^b, e);
      }
#endif
  }
  v_length = s;
  e_length = e;
}

/* random or Greiner93 */

make_random(int n, int m){
  int i, j, e = 0;
  unsigned long next = n + m;
  for(i = 0; i<n ; i++){
    vv[i].degree = 0;
    vv[i].first_e = 0;
    vv[i].parent = 0;
  }
  for(i = 0; i<n ; i++){
    next = next * 1103515245 + 12345;
    for(j = 0; j<m; j++){
      int v;
      next = next * 1103515245 + 12345;
      v = ((n * ((next / 65536) % 32768)) / 32768);
      e = checked_make_edge(i, v, e);
    }
  }
  v_length = n;
  e_length = e;
}

/* graph utils */

show_graph(){
  int v, e, i;
  for(v=0; v<v_length ; v++){
    printf("%9d[%d] (", v, vv[v].parent-1);
    {
      int d = vv[v].degree;
      int e = vv[v].first_e;
      if(d>0){
	  printf("%d", ee[e].next_v);	
	  e = ee[e].next_e;
	  for(i=1; i<d; i++, e = ee[e].next_e){
	    printf(",%d", ee[e].next_v);
	}
      }
    }
    printf(")\n");
  }
}

show_graph2(){
  int v, e, i;
  for(v=0; v<v_length ; v++){
    printf("%9d[%d] e: %d\n", v, vv[v].degree, vv[v].first_e);
  }
  for(e=0; e<e_length ; e++){
    printf("%9d => %d -> e: %d\n", e, ee[e].next_v, ee[e].next_e);
  }
}

check_graph(){
  int v;
  for(v=0; v<v_length ; v++){
    if(vv[v].parent-1 < 0 || vv[v].parent-1 >= v_length)
      printf("%9d[%d]\n", v, vv[v].parent-1);
  }
}

/* time */

double
elapsed_time(struct timeval tp[2])
{
  return tp[1].tv_sec-tp[0].tv_sec+1e-6*(tp[1].tv_usec-tp[0].tv_usec);
}

main(){
  struct timeval tp[2];
  gettimeofday(tp, 0);

  /* constructing a graph */
  // make_random(2000*2000, 2);
  // make_2dt(2000);
  // make_ncube(20);
  make_2dt(2000);

  gettimeofday(tp+1, 0);
  printf("%lf\n", elapsed_time(tp));

  printf("v: %d e: %d\n", v_length, e_length);

  //show_graph();
  //show_graph2();
  
  gettimeofday(tp, 0);

  /* making a spanning tree */
  // search(0);
  search_s2(0);

  gettimeofday(tp+1, 0);
  printf("%lf\n", elapsed_time(tp));
  // show_graph();
  check_graph();
}
