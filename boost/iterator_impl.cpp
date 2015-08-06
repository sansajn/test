#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

class custom_iterator
	: public std::iterator<std::forward_iterator_tag, custom_iterator>
{
public:
	typedef custom_iterator self;

	custom_iterator() {}
	explicit custom_iterator(std::vector<int> & v) 
		: _it(v.begin()), _end(v.end()) 
	{}

	int & operator*() {return *_it;}
	int * operator->() {return &(*_it);}
	
	self & operator++() {
		++_it;
		if (_it == _end)
			_it = std::vector<int>::iterator();
		return *this;
	}

	self operator++(int) {
		self tmp = *this; 
		++(*this); 
		return tmp;
	}

	bool operator==(self const & rhs) const {return _it == rhs._it;}
	bool operator!=(self const & rhs) const {return !(*this == rhs);}

private:
	typename std::vector<int>::iterator _it, _end;
};  // custom_iterator

class facade_custom_iterator
	: public boost::iterator_facade<facade_custom_iterator, int, std::forward_iterator_tag>
{
public:
	facade_custom_iterator() {}
	explicit facade_custom_iterator(std::vector<int> & v) 
		: _it(v.begin()), _end(v.end())
	{}

private:
	int & dereference() const {return *_it;}
	bool equal(facade_custom_iterator const & rhs) const {return _it == rhs._it;}

	void increment() {
		++_it;
		if (_it == _end)
			_it = std::vector<int>::iterator();
	}

	std::vector<int>::iterator _it, _end;

	friend class boost::iterator_core_access;
};  // facade_custom_iterator


class adaptor_custom_iterator
	: public boost::iterator_adaptor<adaptor_custom_iterator, std::vector<int>::iterator, int, std::forward_iterator_tag>
{
public:
	typedef boost::iterator_adaptor<adaptor_custom_iterator, std::vector<int>::iterator, int, std::forward_iterator_tag> super_t;

	adaptor_custom_iterator() {}
	explicit adaptor_custom_iterator(std::vector<int> & v) 
		: super_t(v.begin()), _end(v.end()) 
	{}

private:
	void increment() {
		auto & it = base_reference();
		++it;
		if (it == _end)
			it = std::vector<int>::iterator();
	}

	std::vector<int>::iterator _end;

	friend class boost::iterator_core_access;
};  // adaptor_custom_iterator


template <typename Container>
void print(Container const & c, char const * name)
{
	std::cout << name << ": ";
	for (auto e : c)
		std::cout << e << ", ";
}

int main(int argc, char * argv[])
{
	std::vector<int> v{1, 5, 3, 3, 2};
	
	std::replace(custom_iterator(v), custom_iterator(), 3, 0);
	print(v, "custom_iterator");
	std::cout << "\n";

	std::replace(facade_custom_iterator(v), facade_custom_iterator(), 0, 3);
	print(v, "facade_custom_iterator");
	std::cout << "\n";

	std::replace(adaptor_custom_iterator(v), adaptor_custom_iterator(), 3, 0);
	print(v, "adaptor_custom_iterator");
	std::cout << "\n";

	return 0;
}
