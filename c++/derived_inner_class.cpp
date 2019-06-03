#include <iostream>

using std::cout;

struct A
{
	struct B
	{
		B() {cout << "A::B::B()\n";}
	};

	A() {cout << "A::A()\n";}
};

struct C : public A
{
	C() {cout << "C::C()\n";}

	struct B : public A::B
	{
		B() {cout << "C::B::B()\n";}
	};
};


int main(int argc, char * argv[])
{
	C c;
//	C::B cb;
	c.foo();

	return 0;
}

