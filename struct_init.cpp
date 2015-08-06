#include <iostream>
using std::cout;

struct foo {
	int a;
	int b;
	long c;
	double d;
};

foo f()
{
	foo tmp = {1, 2, 3, 3.03};
	return tmp;
}

foo g()
{
	foo tmp = {0};
	return tmp;
}

foo h()
{
	foo tmp = {1};
	return tmp;
}

int main(int argc, char * argv[])
{
	foo prof1 = f();
	foo prof2 = g();
	foo prof3 = h();
	cout << prof3.a << " " << prof3.b << " " << prof3.c << " " 
		<< prof3.d << "\n";
	return 0;
}

