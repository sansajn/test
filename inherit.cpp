#include <iostream>
using std::cout;


class foo 
{
public:
	void f1() {
		cout << "foo::f1()\n";
	}
};

class goo
	: public foo
{
public:
	void f1() {
		cout << "goo::f1()\n";
	}
};

int main(int argc, char * argv[])
{
	goo g;
	g.f1();
	return 0;
}

