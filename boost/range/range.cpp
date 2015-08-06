#include <set>
#include <string>
#include <iostream>
#include <boost/range.hpp>

using namespace std;

class foo
{
public:
	foo()
	{
		_flags.insert("object");
		_flags.insert("camera");
		_flags.insert("overlay");
	}

	boost::iterator_range<set<string>::const_iterator> flags() const
	{
		return boost::make_iterator_range(_flags);
	}

private:
	set<string> _flags;
};

/* #1 vytvorí rozsah s iterátoru, #2 vytvorí rozsah z kontajneru */
void from_container()
{
	vector<int> v{1, 3, 5, 7, 9};
	auto r = boost::make_iterator_range(begin(v), end(v));  // #1
	auto s = boost::make_iterator_range(v);  // #2

	set<string> flags;
	flags.insert("object");
	flags.insert("camera");
	flags.insert("overlay");
	auto t = boost::make_iterator_range(flags);  // #3

	for (string const & e : t)
		cout << e << ", ";
	cout << "\n";
}

int main(int argc, char * argv[])
{
	from_container();

	foo f;
	
//	for_each(f.flags(), [](cout << flag << ", ";));

	for (string const & flag : f.flags())
		cout << flag << ", ";
	cout << "\n";


	return 0;
}
