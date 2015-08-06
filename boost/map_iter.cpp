#include <map>
#include <iostream>
#include <boost/iterator/iterator_facade.hpp>

//! Implementuje MultiPassInputIterator pre std::map.
template <typename I>
class key_iterator
	: public boost::iterator_facade<key_iterator<I>, typename I::value_type::first_type, std::input_iterator_tag>
{
public:
	using key_type = typename I::value_type::first_type;

	key_iterator() {}
	explicit key_iterator(I iter) : _it(iter) {}

private:
	key_type const & dereference() const {return _it->first;}
	bool equal(key_iterator<I> const & rhs) const {return _it == rhs._it;}
	void increment() {++_it;}

	I _it;

	friend class boost::iterator_core_access;
};  // key_iterator

//! Implementuje MultiPassInputIterator.
template <typename K, typename V>
class key_range
	: public boost::iterator_facade<key_range<K, V>, K const, std::input_iterator_tag>
{
public:
	key_range() {}
	explicit key_range(std::map<K, V> const & m) : _it(m.cbegin()), _end(m.cend()) 
	{
		if (_it == _end)
			_it = typename std::map<K, V>::const_iterator();
	}

private:
	K const & dereference() const {return _it->first;}
	bool equal(key_range<K, V> const & rhs) const {return _it == rhs._it;}

	void increment() {
		++_it;
		if (_it == _end)
			_it = typename std::map<K, V>::const_iterator();
	}

	typename std::map<K, V>::const_iterator _it, _end;

	friend class boost::iterator_core_access;
};  // key_range

template <typename K, typename V>
key_range<K, V> make_key_range(std::map<K, V> & m) {return key_range<K, V>(m);}

int main(int argc, char * argv[])
{
	std::map<int, int> g;
	g[3] = 1;
	g[1] = 4;
	g[7] = 9;
	key_range<int, int> it, end;
	for (it = make_key_range(g); it != end; ++it)
		std::cout << *it << ", ";
	std::cout << "\n";

	typedef key_iterator<std::map<int, int>::iterator> key_iter;
	key_iter it2, end2 = key_iter(g.end());
	for (it2 = key_iter(g.begin()); it2 != end2; ++it2)
		std::cout << *it2 << ", ";
	std::cout << "\n";

	return 0;
}

