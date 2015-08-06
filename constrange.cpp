#include <utility>
using std::pair;
using std::make_pair;
#include <iostream>
using std::cout;

class array
{
public:
	int * begin() {cout << "begin()\n"; return i;}
	int const * begin() const {cout << "begin() const\n"; return i;}	

private:
	int i[20];
};

class foo
{
public:
	typedef int * iterator;
	typedef int const * const_iterator;
	typedef pair<iterator, iterator> range;
	typedef pair<const_iterator, const_iterator> const_range;

	range important_range() {return empty_range();}
	const_range important_range() const {return empty_range();}

private:
	range empty_range() {
		cout << "empty_range()\n";
		return make_pair(arr.begin(), arr.begin());
	}

	const_range empty_range() const {
		cout << "empty_range() const\n";
		return make_pair(arr.begin(), arr.begin());
	}

private:
	array arr;
};

int main(int argc, char * argv[])
{
	foo f;
	foo::range r = f.important_range();

	foo const cf = f;
	foo::const_range cr = cf.important_range();

	return 0;
}


