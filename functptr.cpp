// ukazateľ na funkciu

using func_ptr = int * (*)(double);

int * foo(double d)
{
	return new int(d);
}

int main(int argc,char * argv[])
{
	func_ptr f = foo;
	int * i = foo(10.10);
	return 0;
}
