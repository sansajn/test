// listing 4.1 comparing an argument to a predefined value
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

using std::partition;
using std::vector;
using std::ostream_iterator;
using std::cout;

class greater_than
{
public:
	greater_than(int value)
		: _value{value}
	{}

	bool operator()(int arg) const
	{
		return arg > _value;
	}

private:
	int _value;
};

int main(int argc, char * argv[])
{
	vector xs = {1, 2, 3, 4, 5, 6};
	partition(begin(xs), end(xs), greater_than{3});
	copy(begin(xs), end(xs), ostream_iterator<int>{cout, ","});
	cout << "\n";
	return 0;
}
