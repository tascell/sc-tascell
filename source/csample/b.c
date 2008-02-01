int main()
{
  short s[3][4];

  printf("%d %d %d %d\n",
	 sizeof( *s ),
	 sizeof( **s ),
	 sizeof( s[0] ),
	 sizeof( s[0][0] ));
}
