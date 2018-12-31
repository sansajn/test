// search range implementation
#include <vector>
#include <iostream>

using std::vector;
using std::cout;


class search_traversal_range  // full search traverse range
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



int main(int argc, char * argv[])
{
	auto data = vector<int>{{1,2,3,4,5}};
	search_traversal_range r{data};
	cout << "r={";
	for (int v : r)
		cout << v << ", ";
	cout << "}\n";
	
	search_traversal_range r2{data, 2};
	cout << "r2={";
	for (int v : r2)
		cout << v << ", ";
	cout << "}\n";

	cout << std::endl;
	return 0;
}
