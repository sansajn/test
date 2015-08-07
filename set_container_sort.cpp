// custom triedenie pomocou set kontajneru
#include <set>
#include <iostream>

using std::set;
using std::cout;

struct foo 
{
	unsigned deadline;
	foo(unsigned d) : deadline{d} {}
};

struct foo_sort 
{
	bool operator()(foo * const & a, foo * const & b) const 
	{
		if (a->deadline != b->deadline)
			return a->deadline < b->deadline;
		else
			return a < b;
	}
};

int main(int argc, char * argv[])
{
	set<foo *, foo_sort> s;
	s.insert(new foo{3});
	s.insert(new foo{1});
	s.insert(new foo{0});
	s.insert(new foo{5});
	s.insert(new foo{0});
	s.insert(new foo{1});

	for (foo * f : s)
		cout << f->deadline << ", ";

	cout << "\ndone!\n";

	return 0;
}
