// range like filter filter implementation
#include <vector>
#include <iostream>

using std::vector;
using std::cout;

template <typename Range, typename Pred>
class static_filter
{
public:
	static_filter(Range const * r = nullptr)
		: _r{r}
	{}

	class const_iterator  // TODO: use STL compatible iterator
	{
	public:
		const_iterator(typename Range::const_iterator beg, static_filter const & f)
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
			while (_it != _f._r->end() && !_f._p(*_it))
				++_it;
		}

		typename Range::const_iterator _it;
		static_filter const & _f;
	};

	const_iterator begin() const {return const_iterator{_r->begin(), *this};}
	const_iterator end() const {return const_iterator{_r->end(), *this};}

private:
	Pred _p;
	Range const * _r;
};


struct is_positive_number
{
	bool operator()(int n) const {return n > 0;}
};

int main(int argc, char * argv[])
{
	vector<int> v{1, 2, -3, 0, 4, -5};

	auto f = static_filter<vector<int>, is_positive_number>(&v);
	for (int n : f)
		cout << n << " ";
	cout << "\n";

	using positive_numbers_view = static_filter<vector<int>, is_positive_number>;
	positive_numbers_view positive_numbers;
	positive_numbers = positive_numbers_view{&v};
	for (int n : positive_numbers)
		cout << n << " ";
	cout << "\n";


	cout << "\ndone!\n";
	return 0;
}
