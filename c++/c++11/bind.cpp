#include <functional>
#include <iostream>

using std::bind;
using std::cout;
using namespace std::placeholders;

struct A
{
	void foo()
	{
		cout << "A::foo()" << std::endl;
	}
	
	void with_args(int i)
    {
        cout << "A::with_args(int i=" << i << ")" << std::endl;
    }
};

int main(int argc, char * argv[])
{
	A a;
	auto f = bind(&A::foo, &a);
	f();
    
    auto g = bind(&A::with_args, &a, _1);
    g(42);
	
	return 0;
}
