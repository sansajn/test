void foo(int i) {}

int main()
{
	for (int i = 0; i < 10; ++i)
		foo(i);

	for (int j = 0; j < 10; ++j)
		foo(i);

	return 0;		
}
