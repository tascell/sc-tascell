int v_length;
struct vertex
{
  int degree;
  int first_e;
  int parent;
};
struct vertex vv[MAX_V];

int E_Length;
struct edge
{
  int next_v;
  int next_e;
};
struct edge ee[MAX_E];

struct v_list_body
{
  struct v_list_body *next;
  int i;
  int v[V_LIST_ALLOC_SIZE];
};

struct v_list_head
{
  struct v_list_body *first;
  struct v_list_body *last;
  int length;
};

/* worker local storage accessed by "WDATA" */
worker_data
{
  struct v_list_head v_list;
}

/* initializes worker_data */
worker_init
{
  WDATA.v_list.first = 0;
  WDATA.v_list.last = 0;
  WDATA.v_list.length = 0;
}

worker void search1_p (int v, int b, int k0, int k1);
worker void search_p_b (struct v_list_body *my_v_list, int i0, int i1);
worker void search_p_c (struct v_list_body *my_v_list, int j0, int j1);

task t_search1
{
 in: int v;
 in: int b;
 in: int k0;
 in: int k1;
 out: struct v_list_head v_list;
}

task_exec t_search1
{
  int stack_created_p = 0;
  if (!WDATA.v_list.first)
    {
      WDATA.v_list.first = alloc_v_list (0);
      WDATA.v_list.length = 1;
      WDATA.v_list.last = WDATA.v_list.first;
      stack_created_p = 1;
    }
  search1_p (this.v, this.b, this.k0, this.k1);
  if (stack_created_p)
    {
      if (WDATA.v_list.length > 1
          || (*WDATA.v_list.first).i < V_LIST_ALLOC_SIZE)
        {
          this.v_list.first = WDATA.v_list.first;
          this.v_list.last = WDATA.v_list.last;
          this.v_list.length = WDATA.v_list.length;
        }
      else
        {
          dealloc_v_list (WDATA.v_list.first);
          this.v_list.first = 0;
          this.v_list.last = 0;
          this.v_list.length = 0;
        }
      WDATA.v_list.first = 0;
      WDATA.v_list.last = 0;
      WDATA.v_list.length = 0;
    }
  else
    {
      this.v_list.first = 0;
      this.v_list.last = 0;
      this.v_list.length = 0;
    }
}


worker void
search1_p (int v, int b, int k0, int k1)
{
  int e = (vv[v]).first_e;
  int nv;
  for (int i0 = 0; i0 < k0; i++)
    e = (ee[e]).next_e;
  for (int i:k0, k1)
    {
      nv = (ee[e]).next_v;
      if (!cas_int ((vv[nv]).parent, 0, v + 1))
        {
          start_access_after_lock ();
          if (b > 0)
            search1_p (nv, b - 1, 0, (vv[nv]).degree);
          else
            {
              if (WDATA.v_list.first->i <= 0)
                {
                  WDATA.v_list.first = alloc_v_list (WDATA.v_list.first);
                  (WDATA.v_list.length)++;
                }
              (WDATA.v_list.first->v)[--WDATA.v_list.first->i] = nv;
            }
        }
      e = (ee[e]).next_e;
    }
  handles t_search1 (int k0_2, int k1_2)
  {                             /* k0_2--k1_2 is a subrange of k0--k1 */
    /* executed to initialize :in fields of a spawned task */
    {
      this.v = v;
      this.b = b;
      this.k0 = k0_2;
      this.k1 = k1_2;
    }
    /* executed to get the result by referring :out fields */
    {
      WDATA.v_list.first =
        nconc_v_list (this.v_list.first, this.v_list.last,
                      WDATA.v_list.first);
      WDATA.v_list.length += this.v_list.length;
    }
  }
}

worker void
search_p (int v)
{
  struct v_list_head my_v_list, mlist;
  int i;
  (vv[v]).parent = v + 1;
  WDATA.v_list.first = alloc_v_list (0);
  WDATA.v_list.length = 1;
  WDATA.v_list.last = WDATA.v_list.first;
  search1_p (v, 30, 0, (vv[v]).degree);
  while (WDATA.v_list.length > 1 || WDATA.v_list.first->i < V_LIST_ALLOC_SIZE)
    {
      my_v_list = WDATA.v_list;
      mlist = my_v_list;
      WDATA.v_list.first = alloc_v_list (0);
      WDATA.v_list.length = 1;
      WDATA.v_list.last = WDATA.v_list.first;
      search_p_b (my_v_list.first, 0, my_v_list.length);
      for (i = 0; i < mlist.length; i++)
        mlist.first = dealloc_v_list (mlist.first);
    }
}

task t_search_b
{
 in: struct v_list_body *my_v_list_first;
 in: int i0;
 in: int i1;
 out: struct v_list_head v_list;
}

task_exec t_search_b
{
  int stack_created_p = 0;
  if (!WDATA.v_list.first)
    {
      WDATA.v_list.first = alloc_v_list (0);
      WDATA.v_list.length = 1;
      WDATA.v_list.last = WDATA.v_list.first;
      stack_created_p = 1;
    }
  search_p_b (this.my_v_list_first, this.i0, this.i1);
  if (stack_created_p)
    {
      if (WDATA.v_list.length > 1
          || WDATA.v_list.first->i < V_LIST_ALLOC_SIZE)
        {
          this.v_list.first = WDATA.v_list.first;
          this.v_list.last = WDATA.v_list.last;
          this.v_list.length = WDATA.v_list.length;
        }
      else
        {
          dealloc_v_list (WDATA.v_list.first);
          this.v_list.first = 0;
          this.v_list.last = 0;
          this.v_list.length = 0;
        }
      WDATA.v_list.first = 0;
      WDATA.v_list.last = 0;
      WDATA.v_list.length = 0;
    }
  else
    {
      this.v_list.first = 0;
      this.v_list.last = 0;
      this.v_list.length = 0;
    }
}

worker void
search_p_b (struct v_list_body *my_v_list_first, int i0, int i1)
{
  struct v_list_body *my_v_list_first0;
  for (int a = 0; a < i0; a++)
    my_v_list_first = (*my_v_list_first).next;
  i1 -= i0;
  i0 = 0;
  my_v_list_first0 = my_v_list_first;
for (int i:i0, i1)
    {
      search_p_c (my_v_list_first->i, V_LIST_ALLOC_SIZE);
      my_v_list_first = (*my_v_list_first).next;
    } handles t_search_b (int i0_2, int i1_2)
  {                             /* i0_2--i1_2 is a subrange of i0--i1 */
    /* executed to initialize :in fields of a spawned task */
    {
      this.my_v_list_first = my_v_list_first0;
      this.i0 = i0_2;
      this.i1 = i1_2;
    }
    /* executed to get the result by referring :out fields */
    {
      WDATA.v_list.first =
        nconc_v_list (this.v_list.first, this.v_list.last,
                      WDATA.v_list.first);
      WDATA.v_list.length += this.v_list.length;
    }
  }
}

task t_search_c
{
 in: struct v_list_body *my_v_list_cur;
 in: int j0;
 in: int j1;
 out: v_list_head v_list;
}

task_exec t_search_c
{
  int stack_created_p = 0;
  if (!WDATA.v_list.first)
    {
      WDATA.v_list.first = alloc_v_list (0);
      WDATA.v_list.length = 1;
      WDATA.v_list.last = WDATA.v_list.first;
      stack_created_p = 1;
    }
  search_p_c (this.my_v_list_cur, this.j0, this.j1);
  if (stack_created_p)
    {
      if (WDATA.v_list.length > 1
          || WDATA.v_list.first->i < V_LIST_ALLOC_SIZE)
        {
          this.v_list.first = WDATA.v_list.first;
          this.v_list.last = WDATA.v_list.last;
          this.v_list.length = WDATA.v_list.length;
        }
      else
        {
          dealloc_v_list (WDATA.v_list.first);
          this.v_list.first = 0;
          this.v_list.last = 0;
          this.v_list.length = 0;
        }
      WDATA.v_list.first = 0;
      WDATA.v_list.last = 0;
      WDATA.v_list.length = 0;
    }
  else
    {
      this.v_list.first = 0;
      this.v_list.last = 0;
      this.v_list.length = 0;
    }
}

worker void
search_p_c (struct v_list_body *my_v_list_cur, int j0, int j1)
{
for (int j:j0, j1)
    {
      search1_p ((my_v_list_cur->v)[j], 30, 0,
                 (vv[(my_v_list_cur->v)[j]]).degree);
    } handles t_search_c (int j0_2, int j1_2)
  {                             /* j0_2--j1_2 is a subrange of j0--j1 */
    /* executed to initialize :in fields of a spawned task */
    {
      this.my_v_list_cur = my_v_list_cur;
      this.j0 = j0_2;
      this.j1 = j1_2;
    }
    /* executed to get the result by referring :out fields */
    {
      WDATA.v_list.first =
        nconc_v_list (this.v_list.first, this.v_list.last,
                      WDATA.v_list.first);
      WDATA.v_list.length += this.v_list.length;
    }
  }
}

task_exec spanning_start
{
  make_graph ();
  search_p (0);
}
