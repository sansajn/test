#include <sstream>
#include <iostream>

using namespace std;

void swap_test();

int main(int argc, char * argv[])
{
	ostringstream o;
	o << "Hello I'm Patric.";
	auto buf = o.rdbuf();
	return 0;
}


void swap_test()
{
	ostringstream o;
	o << "c++";

	ostringstream p;
	p << "python";

//	o.swap(p);  // not implemented in stdlibc++

	cout << "p:" << p.str() << "\n";
}

