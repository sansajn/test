#include <iostream>
using std::cout;

struct foo
{
	void a() {b();}
	virtual void b() {cout << "foo::b()\n";}
};

struct goo : public foo
{
	void b() override {cout << "goo::b()\n";}
};

int main(int argc, char * argv[])
{
	goo g;
	g.a();
	return 0;
}
