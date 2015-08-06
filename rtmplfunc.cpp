#include <iostream>
using std::cout;

template <typename R> R foo(int);
template <> int foo(int) {return 10;}
template <> float foo(int) {return 2.34;}

int main(int argc, char * argv[])
{
	cout << foo<float>(19) << "\n";
	cout << foo<int>(32) << "\n";
	return 0;
}
