#include <iostream>
using std::cout;

template <typename T, typename void (*FUNC)(T)>
struct foo
{
	void exec() {FUNC(_d);}
	T _d;
};

void goo(int n)
{
	cout << "goo(int:" << n << ")\n";
}

int main(int argc, char * argv[])
{
	foo<int, goo> f;
	f._d = 15;
	f.exec();
	return 0;
}
