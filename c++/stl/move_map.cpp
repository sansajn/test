// test move-nutia celej mapy
#include <map>
#include <utility>
#include <iostream>
using std::map;
using std::pair;
using std::cout;

struct foo
{
	foo(map<int, int> && m) : _m{move(m)}
	{}
	
	map<int, int> _m;
};

int main(int argc, char * argv[])
{
	map<int, int> m;
	m[2] = 5;
	map<int, int> n{move(m)};
	for (pair<int const, int> kv : n)
		cout << kv.first << " -> " << kv.second << "\n";
	
	foo f{move(n)};
	for (pair<int const, int> kv : f._m)
		cout << kv.first << " -> " << kv.second << "\n";
	
	return 0;
}
