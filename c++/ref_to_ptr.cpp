#include <iostream>
#include <cstddef>

using std::cout;

struct base {};

struct derived : public base
{};

void foo(int *& n)
{
	*n = 0;
}

void foo(base *& b)
{
	b = nullptr;
}

void foo(base ** b)
{
	*b = nullptr;
}

template <typename T>
void foo(T *& obj)
{
	obj = nullptr;
}

int main(int argc, char * argv[])
{
	int * n = new int{16};
	foo(n);
	cout << "value of n is " << *n << "\n";
	delete n;

	derived * d = new derived;
	base * b = (base *)d;
	foo(b);
	cout << "address of b is " << (ptrdiff_t)(b) << "\n";
	delete d;

	derived * d2 = new derived;
	foo((base **)&d2);
	cout << "address of d2 is " << (ptrdiff_t)(d2) << "\n";

	derived * d3 = new derived;
	foo(d3);
	cout << "address of d3 is " << (ptrdiff_t)(d3) << "\n";

	return 0;
}
