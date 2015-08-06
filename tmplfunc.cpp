#include <vector>
#include <iterator>
#include <iostream>


template <class Container, typename T>
void push_back(Container & c, T const & x)
{
	c.push_back(x);
}

template <class Container>
typename Container::iterator begin(Container & c)
{
	return c.begin();
}

template <class Container>
typename Container::iterator end(Container & c)
{
	return c.end();
}

template <typename T>
class DummyArray
{
public:
	void add(T x) {_d.push_back(x);}
	int * start() {return &(*_d.begin());}
	int * stop() {return &(*_d.end());}
	void dump() {
		copy(_d.begin(), _d.end(), 
			std::ostream_iterator<T>(std::cout, " "));
	}

private:
	std::vector<T> _d;
};

template <typename T>
void push_back(DummyArray<T> & c, T const & x)
{
	c.add(x);
}

template <typename T>
T * begin(DummyArray<T> & c)
{
	return c.start();
}

template <typename T>
T * end(DummyArray<T> & c)
{
	return c.stop();
}


int main(int argc, char * argv[])
{
	DummyArray<int> a;
	push_back(a, 10);
	push_back(a, 15);
	push_back(a, 20);
//	a.dump();
	copy(begin(a), end(a), 
		std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\n";

	std::vector<int> v;
	push_back(v, 25);
	push_back(v, 35);
	push_back(v, 45);
	copy(begin(v), end(v), 
		std::ostream_iterator<int>(std::cout, " "));

	return 0;
}

