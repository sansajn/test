/* Test insert iterátora na std::set.
$ g++ -std=c++0x -o inserter inserter.cpp */
#include <set>
using std::set;
#include <iterator>
using std::inserter;
using std::insert_iterator;
#include <iostream>
using std::cout;

template <class Set>
void print_set(const Set & s);


int main(int argc, char * argv[])
{
	set<int> nums;
	auto i = inserter(nums, nums.end());
	*i = 30;
	*i = 10;
	*i = 05;
	*i = 50;
	*i = 20;
	cout << "nums:" << nums.size() << " -> ";
	print_set(nums);
	return 0;
}


template <class Set>
void print_set(const Set & s)
{
	for (auto & e : s)
		cout << e << " ";
	cout << "\n";
}
