// filtered range implementation
#include <vector>
#include <iostream>

using std::vector;
using std::cout;

/* filter objekty nemeni */
template <typename Range, typename Pred>
class filter
{
public:
	filter(Range const & r, Pred const & cond = Pred{})
		: _r{r}, _cond{cond}, _it{_r.begin()}
	{
		satisfy_condition();
	}

	struct iterator
	{
		using value_type = typename Range::value_type;

		iterator(filter const * f) : _f{f} {}

		value_type const & operator*() const
		{
			return _f->current();
		}

		iterator & operator++()
		{
			if (!_f->next())
				_f = nullptr;
			return *this;
		}

		bool operator==(iterator rhs) const
		{
			return _f == rhs._f;
		}

		bool operator!=(iterator rhs) const {return !(*this == rhs);}

	private:
		filter const * _f;

		friend class filter;
	};

	using const_iterator = iterator;

	iterator begin() const {return iterator{this};}
	iterator end() const {return iterator{nullptr};}

private:
	bool next() const
	{
		++_it;
		satisfy_condition();
		return _it != _r.end();
	}

	void satisfy_condition() const
	{
		while (_it != _r.end() && !_cond(*_it))
			++_it;
	}

	typename Range::value_type const & current() const
	{
		return *_it;
	}

	Range const & _r;
	Pred const & _cond;
	mutable typename Range::const_iterator _it;
};

template <typename Range, typename Pred>
filter<Range, Pred> filtered(Range const & r, Pred const & p)
{
	return filter<Range, Pred>{r, p};
}


template <typename Range>
struct range_printer
{
	range_printer(Range const & r) : _r{r} {}

	friend std::ostream & operator<<(std::ostream & out, range_printer const & rhs)
	{
		out << "[";
		size_t i = 0;
		for (auto const & v : rhs._r)
		{
			if (i++ > 0)
				out << ", ";

			out << v;
		}
		out << "]";

		return out;
	}

private:
	Range const & _r;
};

template <typename Range>
range_printer<Range> printable(Range const & r)
{
	return range_printer<Range>{r};
}



class search_traversal_range  // full search traverse range (kedze nemeni obsah co tak pouzit search_traverse_view)
{
public:
	search_traversal_range(vector<int> & data, size_t from = 0)
		: _data{data}, _from{from}, _idx{from}
	{}
	
	struct iterator 
	{
		iterator(search_traversal_range const * r) : _r{r} {}
		
		int const & operator*() const
		{
			return _r->current();
		}
		
		iterator & operator++()
		{
			if (!_r->next())
				_r = nullptr;
			return *this;
		}
		
		bool operator==(iterator rhs) const
		{
			return _r == rhs._r;
		}
		
		bool operator!=(iterator rhs) const {return !(*this == rhs);}

	private:
		search_traversal_range const * _r;
		
		friend class search_traversal_range;
	};

	using const_iterator = iterator;
	using value_type = int;
	
	iterator begin() const {return iterator{this};}
	iterator end() const {return iterator{nullptr};}
	
private:
	bool next() const;
	int const & current() const;
	
	vector<int> & _data;
	size_t _from;
	mutable size_t _idx;
};


bool search_traversal_range::next() const
{
	_idx = (_idx+1) % _data.size(); 
	return _idx != _from;
}

int const & search_traversal_range::current() const
{
	return _data[_idx];
}

struct is_even_number
{
	bool operator()(int n) const {return (n % 2) == 0;}
};



int main(int argc, char * argv[])
{
	auto data = vector<int>{{1,2,3,4,5}};
	search_traversal_range r{data};
	cout << "r=" << printable(r) << "\n";
	
	search_traversal_range r2{data, 2};
	cout << "r2=" << printable(r2) << "\n";

	auto only_even = filter<search_traversal_range, is_even_number>{r};
	cout << "only_even=" << printable(only_even) << "\n";

	auto only_odd = filtered(r, [](int n){return (n%2) == 1;});
	cout << "only_odd=" << printable(only_odd) << "\n";

	cout << std::endl;
	return 0;
}
