#include <iostream>

template <typename T>
struct iterator
{
	typedef T & reference;
};

template <typename T>
struct custom_iterator
	: iterator<T>
{
public:
	custom_iterator(T const & t) 
		: _t(t) 
	{}

	reference operator*() {return _t;}

private:
	T _t;
};

int main(int argc, char * argv[])
{
	custom_iterator<int> cit(11);
	std::cout << "*cit:" << *cit << "\n";
	return 0;
}

