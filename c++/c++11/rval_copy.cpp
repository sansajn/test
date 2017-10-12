/* test r-value copy operátora/konštruktora */
#include <iostream>
#include <stdexcept>
#include <algorithm>
using std::cout;


class vector
{
public:
	vector()
		: _elems(new float[100]), _size(0)
	{
		cout << "vector()\n";
	}
	
	vector(int s)
		: _elems(new float[100]), _size(s)
	{
		cout << "vector(int)\n";
	}

	vector(vector && v) 
	{
		cout << "vector(vector &&)\n";
		*this = v;
	}
	
	vector(vector const & v) 
	{
		cout << "vector(vector const &)\n";		
		*this = v;
	}
	
	vector(std::initializer_list<float> lst)
		: _elems(new float[100]), _size(lst.size())
	{
		cout << "vector(initializer_list<float> lst)\n";
		std::copy(lst.begin(), lst.end(), _elems);
	}

	~vector() 
	{
		cout << "~vector()\n";
		delete [] _elems;
	}

	vector & operator=(vector && v)
	{
		cout << "vector::operator(vector &&)\n";
		_elems = v._elems;
		_size = v._size;
		v._elems = nullptr;
		v._size = 0;
		return *this;
	}

	vector & operator=(vector const & v)
	{
		cout << "vector::operator=(vector const &)\n";
		if (_elems == v._elems)
			return *this;
		delete [] _elems;
		_size = v._size;
		_elems = new float[100];
		for (int i = 0; i < _size; ++i)
			_elems[i] = v._elems[i];
		return *this;
	}

	float & operator[](int i) {return _elems[i];}
	float const & operator[](int i) const {return _elems[i];}

	int size() const {return _size;}

private:
	int _size;
	float * _elems;
};

vector operator+(vector const & a, vector const & b)
{
	cout << "operator+(vector const &, vector const &)\n";

	if (a.size() != b.size())
		throw std::length_error("sizes not match");

	vector tmp(a.size());
	for (int i = 0; i < a.size(); ++i)
		tmp[i] = a[i] + b[i];

	return tmp;
}

void print(vector const & v);

int main(int argc, char * argv[])
{
	vector v{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
	vector u{5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
	vector w;
	w = v + u;	

	print(v);
	print(u);
	print(w);
	cout << "w.size():" << w.size() << "\n";

	return 0;
}

void print(vector const & v)
{
	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << " ";
	cout << "\n";
}

