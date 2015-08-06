/* Program sa pokúša objasniť nasledujúcu hlášku: 
 
	error C2783: 'bool Rca::StillSearching(SearchAlgo::SearchStateE,SearchAlgo::SearchStateE)' : could not deduce template argument for 'SearchAlgo'

Verdikt: normal nemôžem volať ako 'normal(f.s)', lebo potom kompilátor
nemôže odvodiť typ šablónového parametra. Riešenim je volať funkciu
ako normal<foo_type>(f.s). */
#include <iostream>
using std::cout;

template <typename T>
struct foo
{
	enum state {NORMAL, PARANORMAL};
	state s;
	T interesting_value;
};

template <class Foo>
bool normal(typename Foo::state s)
{
	return s ==  Foo::NORMAL;
}


int main()
{
	typedef foo<int> foo_type;
	foo_type f;
	f.s = foo_type::NORMAL;
	if (normal<foo_type>(f.s))
		cout << "Hello I'm normal!\n";
	else
		cout << "I'm paranormal!\n";

	return 0;
}
