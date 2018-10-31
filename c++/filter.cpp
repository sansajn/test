// range like filter filter implementation
#include <vector>
#include <iostream>

using std::vector;
using std::cout;

template <typename Range, typename Pred>
class filter
{
public:
	filter(Range const & r, Pred const & p = Pred{})
		: _r{r}, _p{p}
	{}

	class const_iterator
	{
	public:
		const_iterator(typename Range::const_iterator beg, filter const & f)
			: _it{beg}, _f{f}
		{
			satisfy_predicate();
		}

		const_iterator & operator++()
		{
			++_it;
			satisfy_predicate();
			return *this;
		}

		typename Range::value_type const & operator*() {return *_it;}

		bool operator==(const_iterator const & rhs) const {return _it == rhs._it;}
		bool operator!=(const_iterator const & rhs) const {return _it != rhs._it;}

	private:
		void satisfy_predicate()
		{
			while (_it != _f._r.end() && !_f._p(*_it))
				++_it;
		}

		typename Range::const_iterator _it;
		filter const & _f;
	};

	const_iterator begin() const {return const_iterator{_r.begin(), *this};}
	const_iterator end() const {return const_iterator{_r.end(), *this};}

private:
	Range const & _r;
	Pred const & _p;
};

template <typename Range, typename Pred>
filter<Range, Pred> filtered(Range const & r, Pred const & p)
{
	return filter<Range, Pred>{r, p};
}

struct is_positive_number
{
	bool operator()(int n) const {return n > 0;}
};

int main(int argc, char * argv[])
{
	vector<int> v{1, 2, -3, 0, 4, -5};

	auto f = filter<vector<int>, is_positive_number>(v);
	for (int n : f)
		cout << n << " ";
	cout << "\n";

	// with lambda
	for (int n : filtered(v, [](int n){return n > 0;}))
		cout << n << " ";

	cout << "\ndone!\n";
	return 0;
}
