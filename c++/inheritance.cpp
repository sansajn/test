// calling constructors/destructors in class hierarchy sample

#include <iostream>
using std::cout;

struct foo {
	foo() {cout << "foo::foo()\n";}
	~foo() {cout << "foo::~foo()\n";}
};

struct goo : public foo {
	goo() {cout << "goo::goo()\n";}
	~goo() {cout << "goo::~goo()\n";}
};

int main(int argc, char * argv[]) {
	{
		goo g;
	}
	cout << "done\n!";
	return 0;
}
