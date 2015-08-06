// -std=c++0x
#include <iostream>
using std::cout;
#include <iomanip>
using std::hex;


template <size_t N>
void foo(uint32_t const & val) 
{
	uint8_t * p = (uint8_t *)&val;
	cout << hex << int(p[N-1]) << " ";
	foo<N-1>(val);
}

template <>
void foo<0>(uint32_t const & val) 
{
	cout << "\n";
}


void printer(uint32_t const & val)
{
	foo<sizeof(uint32_t)>(val);
}

void printer_() {}

template <typename T, typename ... Args>
void printer_(T const & value, Args const & ... args)
{
	foo<sizeof(T)>(value);
	printer_(args ...);
}



int main(int argc, char * argv[])
{
	int i = 12345;
	printer(i);

	int j = 345, k = 1, m = 9876;
	printer_(i, j, k, m);

	return 0;
}
