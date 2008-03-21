int g( int x )
{
  return x*x;
}

int main(){
  int y=g(4);
  int z=g(y);
  int ar[3] = {1,2,g(4)};

  return z;
}
