#include <functional>
#include <iostream>

using std::bind;
using std::cout;

struct A
{
	void foo()
	{
		cout << "A::foo()" << std::endl;
	}
};

int main(int argc, char * argv[])
{
	A a;
	auto f = bind(&A::foo, a);
	f();
	
	return 0;
}
