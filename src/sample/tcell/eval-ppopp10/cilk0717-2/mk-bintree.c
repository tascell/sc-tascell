void make_bintree (int depth)
{
  int n = (1 << depth) - 1;
  unsigned long next = depth;
  int i;
  int e;

  fprintf (stderr, "Depth %d binary tree\n", depth);
  for (i = 0; i < n; i++)
    {
      (vv[i]).degree = 0;
      (vv[i]).first_e = 0;
      (vv[i]).parent = 0;
    }
  e = 0;
  for (i = 0; i < n; i++)
    {
      if (1) // random
	{
	  int v[3];
	  int p;
	  int q;
	  int n_e;

	  if (i == 0)
	    {
	      n_e = 2;
	      v[0] = i * 2 + 1;
	      v[1] = i * 2 + 2;
	    }
	  else if (i < (n - 1) / 2)
	    {
	      n_e = 3;
	      v[0] = (i - 1) / 2;
	      v[1] = i * 2 + 1;
	      v[2] = i * 2 + 2;
	    }
	  else
	    {
	      n_e = 1;
	      v[0] = (i - 1) / 2;
	    }
	  next = next * 1103515245 + 12345;
	  for (q = n_e; q > 0; q--)
	    {
	      next = next * 1103515245 + 12345;
	      p = q * (next / 65536 % 32768) / 32768;
	      e = check_make_edge1 (i, v[p], e);
	      v[p] = v[q - 1];
	    }
	}
      else // no random
        if (i > 0)
          e = make_edge (i, (i - 1) / 2, e);
        else;
    }
  v_length = n;
  e_length = e;
}
