/* Ako spraviť range, tak aby umožnovalo interovať data vo foo aj v
pripade konštantných dát (foo const) ? */

#include <vector>
#include <iostream>
using std::vector;
using std::cout;

struct foo
{
	typedef int & reference;
	typedef int const & const_reference;
	vector<int> data;
};


template <typename T>
struct const_selector
{
	typedef typename T::reference type;
};

template <typename T>
struct const_selector<T const>
{
	typedef typename T::const_reference type;
};

template <typename T>
class range_base
{
public:
	typedef typename const_selector<T>::type reference;

	range_base(T & f) : _f(f), _idx(0) {}
	reference operator*() {return _f.data[_idx];}
	void operator++() {++_idx;}
	operator bool() const {return _idx < _f.data.size();}

private:
	T & _f;
	int _idx;
};

typedef range_base<foo> foo_range;
typedef range_base<foo const> const_foo_range;


int main(int argc, char * argv[])
{
	foo f;
	for (int i = 0; i < 5; ++i)
		f.data.push_back(i);

	foo_range r(f);
	for (; r; ++r)
		cout << *r << ", ";
	cout << "\n";

	const_foo_range cr(f);
	for (; cr; ++cr)
		cout << *cr << ", ";
	cout << "\n";

	return 0;
}


