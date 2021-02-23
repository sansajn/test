#include <iostream>
#include <cstddef>
using std::cout;


int main(int argc, char * argv[])
{
	cout << "bool:" << sizeof(bool) << "\n"
		<< "char:" << sizeof(char) << "\n"
		<< "wchar_t:" << sizeof(wchar_t) << "\n"
		<< "short:" << sizeof(short) << "\n"
		<< "int:" << sizeof(int) << "\n"
		<< "unsigned:" << sizeof(unsigned) << "\n"
		<< "long:" << sizeof(long) << "\n"
		<< "long long:" << sizeof(long long) << "\n"
		<< "float:" << sizeof(float) << "\n"
		<< "double:" << sizeof(double) << "\n"
		<< "long double:" << sizeof(long double) << "\n"
//		<< "__float128: " << sizeof(__float128) << "\n"
		<< "int *:" << sizeof(int *) << "\n"
		<< "double *:" << sizeof(double *) << "\n"
		<< "size_t:" << sizeof(size_t) << "\n"
		<< "ptrdiff_t:" << sizeof(ptrdiff_t) << "\n";
	return 0;
}
