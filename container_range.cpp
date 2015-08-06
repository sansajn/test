/* clang++ <file> -std=c++0x -fsyntax-only */
#include <iostream>
using std::cout;

template <class Container>
struct container_traits 
{
	typedef typename Container::value_type value_type;
};

/* vyžaduje random-access kontainer */
template <class Container>
class container_range
{
public:
	typedef typename container_traits<Container>::value_type value_type;
	container_range(Container & c) : _idx(0), _c(c) {}
	container_range(Container const & c) : _idx(0), _c(c) {}
	container_range(Container && c) : _idx(0), _c(c) {}
	void operator++() {++_idx;}
	value_type & operator*() {return _c[_idx];}
	operator bool() {return _idx < _c.size();}

private:
	int _idx;
	Container _c;
};

template <typename T>
class DummyArray
{
public:
	DummyArray() {
		cout << "DummyArray()\n";
		_d = new T[10];
	}

	DummyArray(DummyArray const & rhs) {
		cout << "DummyArray(DummyArray const &)\n";
		_d = new T[10];
		std::copy(rhs._d, rhs._d + 10, _d);
	}

	DummyArray(DummyArray && rhs) {
		cout << "DummyArray(DummyArray &&)\n";
		_d = rhs._d;
		rhs._d = nullptr;
	}

	~DummyArray() {
		cout << "~DummyArray()\n";
		for (int i = 0; i < 10; ++i)
			_d[i] = -1;
		delete [] _d;
	}

	DummyArray & operator=(DummyArray const & rhs) {
		cout << "DummyArray::operator=()\n";
		std::copy(rhs._d, rhs._d + 10, _d);
		return *this;
	}
		
	T & operator[](int i) {return _d[i];}
	int size() {return 10;}

private:
	T * _d;		
};

template <class T>
struct container_traits<DummyArray<T>>
{
	typedef T value_type;
};


DummyArray<int> provider()
{
	DummyArray<int> d;
	for (int i = 0; i < 10; ++i)
		d[i] = i*i;
	return d;
}

int main(int argc, char * argv[])
{
/*
	DummyArray<int> a;
	for (int i = 0; i < 10; ++i)
		a[i] = i*i;
	
	container_range<DummyArray<int>> r(a);
*/

/*	
	DummyArray<int> a = provider();  // RVO
	container_range<DummyArray<int>> r(a);
*/

	container_range<DummyArray<int>> r(provider());
	for (r; r; ++r)
		cout << *r << " ";
	cout << "\n";	

	return 0;
}
