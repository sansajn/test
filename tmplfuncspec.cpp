/* function specialisation */
// stl
#include <algorithm>
#include <vector>
#include <iostream>
using std::cout;


template <typename T>
class foo 
{
public:
	typedef typename std::vector<int>::const_iterator const_iterator;
	const_iterator weird_find(const T & what) const {return std::find(_vec.begin(), _vec.end(), what);}
	const_iterator end() const {return _vec.end();}

private:
	std::vector<T> _vec;
};

template <typename K, typename V>
class foo_map {};

template <class Container, typename T>  // #1
bool contains(const Container & where, const T & what)
{
	cout << "contains(const Container &, const T &)\n";
	return std::find(where.begin(), where.end(), what) != where.end();
}

template <typename T>  // #2
bool contains(const foo<T> & where, const T & what)
{
	cout << "contains(const foo<T> &, const T&)\n";
	return where.weird_find(what) != where.end();
}

/*template <typename K, typename V>
bool contains(const foo_map<K, V> & where, const V & what)
{
	cout << "contains(const foo_map<K, V> &, const V &)\n";
	return where.weird_find(what) != where.end();
}*/

template <typename T>
struct foo_a {};
	

/* Čiastočná špecializácia funkcie nie je povolená, preto funkcia musí
byť špecilizovaná úplne. */

template <>  // #3
bool contains<foo_a<int>, int>(foo_a<int> const & where, int const & what)
{
	cout << "contains<foo_a<int>, int>(foo_a<int> const & where, int const & what)\n";
	return true;
}

/* takže, toto nebude fungovať lebo to je čiastočná špecializácia
povolená iba u tries.

template <typename T>
bool contains< foo_a<int> >(foo_a<int> const & where, T const & what) {return true;} */

int main()
{
	std::vector<int> v;  
	contains(v, 10);  // zavolá #1  

	foo<int> f;
	contains(f, 15);  // zavolá #2

	foo_a<int> fa;
	contains(fa, 11);  // zavolá #3

	return true;
}

