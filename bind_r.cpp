#include <iostream>
using std::cout;

/* clang++ -fsyntax-only <file.cpp> */
template <class R1, class R2>
struct bind_r
{
public:
	typedef typename R1::value_type value_type;

	bind_r(R1 r1, R2 r2) : _r1(r1), _r2(r2) {_first = true;}

	void operator++() 
	{
		if (_first)
		{
			++_r1;
			_first = _r1;
		}
		else
			++_r2;
	}

	value_type & operator*() {
		return get_range_elem();
	}

	value_type * operator->() {
		return &get_range_elem();
	}

	operator bool() {return _first ? _r1 : _r2;}

private:	
	value_type & get_range_elem() {return _first ? *_r1 : *_r2;}

private:
	R1 _r1;
	R2 _r2;
	bool _first;
};

class numeric_range
{
public:
	typedef int value_type;

	numeric_range(int from, int to)
		: _curr(from), _to(to)
	{}

	void operator++() {++_curr;}
	int & operator*() {return _curr;}
	operator bool() const {return _curr != _to;}

private:
	int _curr, _to;
};


int main(int argc, char * argv[])
{
	numeric_range r(1, 10), s(10, 20);
	bind_r<numeric_range, numeric_range> t(r, s);
	for (; t; ++t)
		cout << *t << "\n";

	return 0;
}

