/*
 * This code contains a rudimentary bucket sort, which is mainly meant
 * to be an example of using Cilk's locking calls. It is not an
 * optimized sort. It also contains a basic implementation of prefix
 * sum on an array. 
 */
static const char *ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/bucket.cilk $ $LastChangedBy: sukhaj $ $Rev: 517 $ $Date: 2003-10-27 10:05:37 -0500 (Mon, 27 Oct 2003) $";

/*
 * Copyright (c) 2003 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cilk-lib.cilkh>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <getoptions.h>

#define BASE 10000

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* prefix scan tree helper macros */

#define L_CHILD(x) (2*((x)+1)-1)
#define R_CHILD(x) (2*((x)+1))
#define TREE_LEAF_P(tree_num_internals, x) ((x) >= (tree_num_internals))
#define LEAF_TO_VERTEX(base, tree_num_internals, x) ((base) * ((x) - (tree_num_internals)))
#define INIT_TREE(n, tree, base, tree_num_internals) { \
  long tree_num_leaves; \
  tree_num_leaves = ((n)-1)/(base) + 1; \
  tree_num_internals = 1; \
  while(tree_num_internals < tree_num_leaves) \
    tree_num_internals *= 2; \
  tree_num_internals--; \
  tree = calloc(sizeof(int), tree_num_internals + tree_num_leaves); \
}

/* element in bucket */

typedef struct elem_struct {
     float num;
     struct elem_struct *next;
} elem;

/* bucket */

typedef struct bucket_struct {
     Cilk_lockvar lock;
     int num_elems;
     int num_before;
     elem *list;
} bucket;

/* buckets and elems */
bucket *buckets;
elem *elems;

/* prefix scan tree */
int *scan_tree;
int scan_tree_internals;

/* number of elems */
int n;

/* Procedure bucket_sort takes divides-and-conquers the array and
 * inserts the elements into the approapriate buckets. Each bucket
 * must be locked before elements are inserted. */

cilk void bucket_sort(float *array, int left, int right)
{
     if (right - left >= BASE) {
	  int mid = (left + right) / 2;
	  spawn bucket_sort(array, left, mid);
	  spawn bucket_sort(array, mid + 1, right);
     } else {
	  int i;
	  for (i = left; i < right + 1; i++) {
	       int idx = (int) floor(n * array[i]);
	       elem **current;
	       elem *new = &(elems[i]);
	       if (new == NULL)
		    Cilk_exit(1);

	       /* new element */
	       new->num = array[i];
	       new->next = NULL;

	       Cilk_lock(buckets[idx].lock);

	       /* find right place to insert */
	       current = &(buckets[idx].list);

	       while (*current != NULL && (*current)->num < array[i])
		    current = &((*current)->next);

	       /* insert */
	       if (*current == NULL)
		    *current = new;
	       else {
		    new->next = *current;
		    *current = new;
	       }
	       buckets[idx].num_elems++;

	       Cilk_unlock(buckets[idx].lock);
	  }
     }
}

/* Procedure place_elems divides-and-conquers the buckets,
 * placing the elements in the buckets into the right array
 * locations (in sorted_array) based on the previously 
 * calculated num_before field */

cilk void place_elems(float *sorted_array, int left, int right)
{
     if (right - left >= BASE) {
	  int mid = (left + right) / 2;
	  spawn place_elems(sorted_array, left, mid);
	  spawn place_elems(sorted_array, mid + 1, right);
     } else {
	  int i, c;
	  elem *current;
	  for (i = left; i < right + 1; i++) {
	       for (current = buckets[i].list, c = 0;
		    current != NULL;
		    current = current->next, c++) {
		    sorted_array[buckets[i].num_before + c] = current->num;
	       }
	  }
     }
}

/* Procedure scan_up simply computes the first part of the prefix sum
 * calculation by filling each node of scan_tree, which is a binary
 * tree with the buckets at its leaves, with the total number of
 * elements in the buckets at the leaves of the subtree rooted at
 * that node. This total is placed in the num_elems field. */

cilk int scan_up(int idx)
{
     if (TREE_LEAF_P(scan_tree_internals, idx)) {
	  int l = LEAF_TO_VERTEX(BASE, scan_tree_internals, idx);
	  int r = MIN(n, l + BASE);
	  int i;
	  for (i = l; i < r; i++)
	       scan_tree[idx] += buckets[i].num_elems;
     } else {
	  scan_tree[idx] += spawn scan_up(L_CHILD(idx));
	  scan_tree[idx] += spawn scan_up(R_CHILD(idx));
	  sync;
     }
     return scan_tree[idx];
}

/*
 * Procedure scan_down does the second part of the prefix sum
 * calculation by setting the num_before field of each bucket based on
 * the num_elem fields of the nodes in the scan tree.  In essence,
 * each right edge in the scan tree is being labeled with the num_elem
 * value of the left child of the edge's parent, and then each
 * bucket's num_before field is calculated to be the total of the
 * values on all the right edges on the path from the root of the scan
 * tree to the leaf of the scan tree corresponding to that bucket. 
 */

cilk void scan_down(int idx, int sum)
{
     if (TREE_LEAF_P(scan_tree_internals, idx)) {
	  int l = LEAF_TO_VERTEX(BASE, scan_tree_internals, idx);
	  int r = MIN(n, l + BASE);
	  int i;
	  if (l < n) {
	       buckets[l].num_before = sum;
	       for (i = l + 1; i < r; i++)
		    buckets[i].num_before = buckets[i - 1].num_before + buckets[i - 1].num_elems;
	  }
     } else {
	  spawn scan_down(L_CHILD(idx), sum);
	  spawn scan_down(R_CHILD(idx), scan_tree[L_CHILD(idx)] + sum);
	  sync;
     }
     return;
}

int usage(void)
{
     fprintf(stderr, "\nUsage: bucket [<cilk-options] [-n #] [-benchmark] [-h]\n\n");
     fprintf(stderr, "The program sorts a random array of n floating-point elements in the\n");
     fprintf(stderr, "range $[0, 1)$ using the linear time bucket sort algorithm.\n\n");
     return 1;
}

char *specifiers[] =
{"-n", "-benchmark", "-h", 0};
int opt_types[] =
{INTARG, BENCHMARK, BOOLARG, 0};

cilk int main(int argc, char *argv[])
{
     int i, benchmark, help;
     float *array;
     Cilk_time tm_begin, tm_elapsed;
     Cilk_time wk_begin, wk_elapsed;
     Cilk_time cp_begin, cp_elapsed;

     /* standard benchmark options */
     n = 64000;

     get_options(argc, argv, specifiers, opt_types, &n, &benchmark, &help);

     if (help)
	  return usage();

     if (benchmark) {
	  switch (benchmark) {
	      case 1:		/* short benchmark options -- a little work */
		   n = 80000;
		   break;
	      case 2:		/* standard benchmark options */
		   n = 8000000;
		   break;
	      case 3:		/* long benchmark options -- a lot of work */
		   n = 80000000;
		   break;
	  }
     }
     /* alloc memory */
     if ((array = malloc(n * sizeof(float))) == NULL)
	   return 3;
     if ((buckets = calloc(n, sizeof(bucket))) == NULL)
	  return 4;
     if ((elems = malloc(n * sizeof(elem))) == NULL)
	  return 5;
     INIT_TREE(n, scan_tree, BASE, scan_tree_internals);
     if (scan_tree == NULL)
	  return 6;

     /* init locks */
     for (i = 0; i < n; i++)
	  Cilk_lock_init(buckets[i].lock);

     /* fill array will numbers in range [0, 1) */
     for (i = 0; i < n; i++)
	  array[i] = (float) rand() / ((float) RAND_MAX + 1);

     /* Timing. "Start" timers */
     sync;
     cp_begin = Cilk_user_critical_path;
     wk_begin = Cilk_user_work;
     tm_begin = Cilk_get_wall_time();

     /* sort the array */
     spawn bucket_sort(array, 0, n - 1);
     sync;

     /* do prefix scan */
     i = spawn scan_up(0);
     sync;
     spawn scan_down(0, 0);
     sync;

     spawn place_elems(array, 0, n - 1);
     sync;

     /* Timing. "Stop" timers */
     tm_elapsed = Cilk_get_wall_time() - tm_begin;
     wk_elapsed = Cilk_user_work - wk_begin;
     cp_elapsed = Cilk_user_critical_path - cp_begin;

     /* verify sort */
     for (i = 1; i < n; i++)
	  if (array[i] < array[i - 1])
	       break;

     if (i != n)
	  printf("WRONG ANSWER!\n");
     else {
	  printf("\nCilk Example: bucket\n");
	  printf("	      running on %d processor%s\n\n", Cilk_active_size, Cilk_active_size > 1 ? "s" : "");
	  printf("Options: number of elements  n = %d\n\n", n);
	  printf("Running time  = %4f s\n", Cilk_wall_time_to_sec(tm_elapsed));
	  printf("Work          = %4f s\n", Cilk_time_to_sec(wk_elapsed));
	  printf("Critical path = %4f s\n\n", Cilk_time_to_sec(cp_elapsed));
     }

     free(buckets);
     free(elems);
     free(array);
     return 0;
}
