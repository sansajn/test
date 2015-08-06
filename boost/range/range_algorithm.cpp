// range algoritmi
#include <map>
#include <iostream>
#include <boost/range.hpp> 
#include <boost/range/algorithm.hpp> 
#include <boost/range/adaptor/map.hpp> 

struct foo
{
	typedef typename std::map<int, int>::iterator unit_iterator;
	boost::iterator_range<unit_iterator> units() {return boost::make_iterator_range(_units);}
	std::map<int, int> _units;
};

int main(int argc, char * argv[])
{
	foo f;
	for (int i = 0; i < 10; ++i)
		f._units.insert(std::make_pair(i, i*i));

	// find
	auto r = boost::find_if(f.units(), 
		[](foo::unit_iterator::value_type const & v){return v.second == 2*2;});
	if (!r.empty())
		std::cout << r->first << ":" << r->second << "\n";

	return 0;
}
