#include <iostream>
#include <typeinfo>
using std::cout;

template <typename T, T val>
struct integral_constant
{
	typedef T type;
	static T const value = val;
};


int main(int argc, char * argv[])
{
	typedef integral_constant<int, 101> lucky_number;

	cout << "value:" << lucky_number::value << "\n"
		<< "type:" << typeid(lucky_number::value).name() << "\n";

	return 0;
}

