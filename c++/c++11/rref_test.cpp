// argument v trave r-ref a jeho nasledne move-nutie
#include <utility>
#include <iostream>

using std::move;
using std::cout;

struct foo
{
	foo() {cout << "foo()\n";}
	foo(foo && other) {cout << "foo(foo &&)\n";}
	foo & operator=(foo && other) {cout << "foo::operator=(foo &&)\n"; return *this;}
	foo(foo const &) = delete;
	void operator=(foo const &) = delete;
};

struct goo
{
	void assign(foo && f)
	{
		_f = move(f);  // ekvivalent swap(_f, f)
	}

	foo _f;
};

int main(int argc, char * argv[])
{
	goo g;
	g.assign(foo{});
	return 0;
}
