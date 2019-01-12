// hello sample
#include <iostream>
#include <boost/stacktrace.hpp>

using std::cout;

void foo()
{
	cout << boost::stacktrace::stacktrace{} << "\n";
}

void goo()
{
	foo();
}

int main(int argc, char * argv[])
{
	goo();
	return 0;
}
