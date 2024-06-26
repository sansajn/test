// method1: member functions and nested types
#include <iostream>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

using namespace std;

struct foo
{
	typedef int * iterator;
	typedef int const * const_iterator;
	foo() : arr{1, 2, 3, 2 ,5} {}
	iterator begin() {return arr;}
	const_iterator begin() const {return nullptr;}
	iterator end() {return arr+5;}
	const_iterator end() const {return nullptr;}
	int arr[5];
};

int main(int argc, char * argv[])
{
	foo f;
	cout << boost::count(f, 2) << "\n";

	for (auto e : f)
		cout << e << ", ";
	cout << "\n";

	auto r = f|boost::adaptors::filtered([](int i){return i > 2;});
	for (auto e : r)
		cout << e << ", ";
	cout << "\n";

	for (auto e : f|boost::adaptors::filtered([](int i){return i > 2;}))
		cout << e << ", ";
	cout << "\n";

	return 0;
}
