// g++ limits.cpp -o limits
#include <limits>
#include <iostream>

using std::numeric_limits;
using std::cout;

template <typename T>
void print_info(char const * name)
{
	cout << name
		<< ": max=" << numeric_limits<T>::max() 
		<< ", min=" << numeric_limits<T>::min() 
		<< ", lowest=" << numeric_limits<T>::lowest() 
		<< ", epsilon=" << numeric_limits<T>::epsilon()
		<< ", size=" << sizeof(T) << "\n";
}

int main(int argc, char * argv[])
{
	print_info<bool>("bool");
	print_info<char>("char");
	print_info<short>("short");
	print_info<int>("int");
	print_info<long>("long");
	print_info<float>("float");
	print_info<double>("double");
	print_info<long double>("long double");
	return 0;
}
