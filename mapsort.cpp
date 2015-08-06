/* oderered map - preto lebo dáta su v štruktúre zotriedené (v takej
hash-mape nie a preto sa aj nazýva unordered-map) */
#include <map>
#include <set>
#include <iostream>

using std::map;
using std::set;
using std::cout;
using std::begin;
using std::end;

int main(int argc, char * argv[])
{
	map<int, int> m;
	m[3] = 1;
	m[1] = 2;
	m[4] = 3;
	m[5] = 4;
	m[2] = 5;
	for (auto & e : m)
		cout << e.first << ":" << e.second << ", ";
	cout << "\n";

	set<int> s;
	s.insert(3);
	s.insert(1);
	s.insert(4);
	s.insert(5);
	s.insert(2);
	for (auto & e : s)
		cout << e << ", ";
	cout << "\n";

	return 0;
}

