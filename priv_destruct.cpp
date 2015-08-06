#include <iostream>
using std::cout;

class foo
{
private:
	foo() {cout << "foo::foo()\n";}
	virtual ~foo() {cout << "foo::~foo()\n";}

public:
	static foo * instance() {return new foo;}
	static void destroy(foo * p) {delete p;}
};

class goo : public foo
{
private:
	goo() {cout << "goo::goo()\n";}
	~goo() {cout << "goo::~goo()\n";}

public:
	static goo * instance() {return new goo;}
	static void destroy(goo * p) {delete p;}
};

int main(int argc, char * argv[])
{
	goo * p = goo::instance();
	goo::destroy(p);
	return 0;
}
