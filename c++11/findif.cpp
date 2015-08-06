// std riešenie
#include <map>
#include <iostream>
#include <algorithm>

struct foo
{
	typedef std::map<int, int> unit_map;
	unit_map const & units() const {return _units;}
	unit_map _units;
};

int main(int argc, char * argv[])
{
	foo f;
	for (int i = 0; i < 10; ++i)
		f._units.insert(std::make_pair(i, i*i));

	auto it = std::find_if(f.units().begin(), f.units().end(),
		[](foo::unit_map::value_type const & v){return v.second == 2*2;});

	if (it != f.units().end())
		std::cout << it->first << ":" << it->second << "\n";

	// by key
	foo::unit_map const & units = f.units();
	it = units.find(2);
	if (it != units.end())
		std::cout << it->first << ":" << it->second << "\n";

	return 0;
}
