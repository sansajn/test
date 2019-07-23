/* volanie pretazenej virtualnej clenskej funkcie z konstruktora zakladnej triedy 
see https://isocpp.org/wiki/faq/strange-inheritance#calling-virtuals-from-ctors */
#include <iostream>

using std::cout;

class foo
{
public:
	foo()
	{
		a();  // will always call foo::a()
	}

	virtual void a() {cout << "foo::a()\n";}
};

class goo : public foo
{
public:

	void a() override {cout << "goo::a()\n";}
};

int main(int argc, char * argv[])
{
	goo g;  // foo::a(), dynamic binding is disabled in a base constructor
	g.a();  // goo::a()
	foo * f = &g;
	f->a();  // goo::a()

	return 0;
}
