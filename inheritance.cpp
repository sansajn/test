#include <iostream>
using std::cout;

class foo 
{
public:
	virtual void f() {cout << "foo::f()\n";}
};

class goo
	: public foo
{
public:
	void f() {cout << "goo::f()\n";}
};

void func(foo & f) {f.f();}


int main()
{
	foo * o = new goo();

	foo & r = *o;
	r.f();  // goo::f()

	func(r);  // goo::f()

	return 0;
}
