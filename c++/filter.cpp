// range like filter filter implementation
#include <vector>
#include <iostream>

using std::vector;
using std::cout;

template <typename Range, typename Pred>
class filter
{
public:
	filter(Range const & r, Pred const & p)
		: _r{r}, _p{p}
	{}

	class const_iterator
	{
	public:
		const_iterator(typename Range::const_iterator beg, typename Range::const_iterator end, Pred const & pred)
			: _it{beg}, _end{end}, _p{pred}
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
			while (_it != _end && !_p(*_it))
				++_it;
		}

		typename Range::const_iterator _it, _end;
		Pred const & _p;
	};

	const_iterator begin() const {return const_iterator{_r.begin(), _r.end(), _p};}
	const_iterator end() const {return const_iterator{_r.end(), _r.end(), _p};}

private:
	Range const & _r;
	Pred const & _p;
};

struct is_positive_number
{
	bool operator()(int n) const {return n > 0;}
};

int main(int argc, char * argv[])
{
	vector<int> v{1, 2, -3, 0, 4, -5};

	auto f = filter<vector<int>, is_positive_number>(v, is_positive_number{});
	for (int e : f)
		cout << e << " ";

	cout << "\ndone!";
	return 0;
}
