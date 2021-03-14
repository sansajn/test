// hello sample
#include <iostream>
#include <boost/stacktrace.hpp>

using std::cout, std::endl;

struct foo
{
	void test() {
		cout << boost::stacktrace::stacktrace{} << endl;
	}
};

void goo()
{
	foo f;
	f.test();
}

int main(int argc, char * argv[])
{
	goo();
	return 0;
}
