enum eabc {
a, b, c = 4};

int		f          (int x, enum eabc y)
{
  enum edef {
    d, e, f
  };
  enum edef	  z = d;

  return a + b + c + d + e + f + z;
}
