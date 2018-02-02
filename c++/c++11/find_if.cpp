// std riešenie
#include <map>
#include <iostream>
#include <algorithm>
#include <functional>

using std::cout;

struct foo
{
	using unit_map = std::map<int, int>;
	unit_map const & units() const {return _units;}
	unit_map _units;
};

bool equal(foo::unit_map::value_type const & val, int expected)
{
	return val.second == expected;
}

int main(int argc, char * argv[])
{
	foo f;
	for (int i = 0; i < 10; ++i)
		f._units.insert(std::make_pair(i, i*i));

	// via lambda
	auto it = std::find_if(f.units().begin(), f.units().end(),
		[](foo::unit_map::value_type const & v){return v.second == 2*2;});

	if (it != f.units().end())
		std::cout << it->first << ":" << it->second << "\n";
	else
		cout << "not found\n";
	
	// via bind
	auto it_bind = std::find_if(f.units().begin(), f.units().end(),
		std::bind(equal, std::placeholders::_1, 2*2));
		
	if (it_bind != f.units().end())
		std::cout << it_bind->first << ":" << it_bind->second << "\n";
	else
		cout << "not found\n";
	

	// by key
	foo::unit_map const & units = f.units();
	it = units.find(2);
	if (it != units.end())
		std::cout << it->first << ":" << it->second << "\n";

	return 0;
}
