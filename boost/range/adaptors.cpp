/*! \file adaptors.cpp
basic adaptors (map_values, reversed, filtered, ...) sample */

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

using namespace std;

template <typename V>
void print(V const & v)
{
	for (auto const & e : v)
		cout << e << ", ";
	cout << endl;
}

int main(int argc, char * argv[])
{
	map<string, int> m;
	m["peter"] = 15;
	m["alena"] = 20;
	m["janka"] = 22;
	m["adolf"] = 25;

	vector<int> v;
	boost::copy(m|boost::adaptors::map_values, std::back_inserter(v));
	print(v);  // 25, 20, 22, 15, 

	v.clear();
	vector<int> v1{1, 2, 3, 4, 5};
	boost::copy(v1|boost::adaptors::reversed, std::back_inserter(v));
	print(v);  // 5, 4, 3, 2, 1, 

	v.clear();
	boost::copy(m|boost::adaptors::filtered(
			[](map<string, int>::value_type & v){return v.first[0] == 'a';})
		|boost::adaptors::map_values, std::back_inserter(v));
	print(v);  // 25, 20,

	// len pre porovnanie
	v.clear();
	for (auto & e : m)
		if (e.first[0] == 'a')
			v.push_back(e.second);
	print(v);  // 25, 20,

	return 0;
}

