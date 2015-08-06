#include <iostream>
using std::cout;

template<typename ... Args>
struct foo
{
	static const unsigned int size = sizeof ... (Args);
};


int main(int argc, char * argv[])
{
	typedef foo<int, double, int, int> foo_type;
  	foo_type f;
	cout << "number of arguments for f is " << foo_type::size << "\n";
	return 0;
}

