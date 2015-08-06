#include <iostream>
using std::cout;

struct foo {};
struct goo : foo
{
	int i;
};

int main()
{
	goo g;
	g.i = 101;

	foo & f = g;
	
	foo f2;
	f = f2;  // je vobec nieco taketo validne ??

	cout << "g.i:" << g.i << "\n";	

	return 0;
}

