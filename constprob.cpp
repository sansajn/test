#include <utility>
#include <iterator>
#include <algorithm>
#include <iostream>


class array
{
public:
	typedef int * iterator;
	typedef int const * const_iterator;

	iterator begin() {return _d;}
	const_iterator begin() const {return _d;}
	iterator end() {return _d+10;}
	const_iterator end() const {return _d+10;}
	int & operator[](int i) {return _d[i];}
	int const & operator[](int i) const {return _d[i];}

private:
	int _d[10];
};


class foo
{
public:
	typedef int * Iterator;
	typedef std::pair<Iterator, Iterator> Range;

	foo() {
		for (int i = 0; i < 10; ++i)
			_d[i] = i*i;		
	}

	Range elements() {return std::make_pair(_d.begin(), _d.end());}

private:
	 array _d;
};


int main(int argc, char * argv[])
{
	foo f;
	foo::Range r = f.elements();
	copy(r.first, r.second, std::ostream_iterator<int>(std::cout, " "));
	return 0;
}
