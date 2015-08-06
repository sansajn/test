/* Implementácia iterátora */
#include <iostream>
#include <algorithm>
#include <iterator>


template <typename T>
class array
{
public:
	typedef T value_type;
	typedef value_type & reference;	
	typedef value_type const & const_reference;
	typedef value_type * iterator;

	iterator begin() {return _d;}
	iterator end() {return _d+10;}

	reference operator[](int i) {return _d[i];}
	const_reference operator[](int i) const {return _d[i];}

private:
	T _d[10];
};

class graph
{
public:
	typedef int vertex_descriptor;

	class iterator 
		: public std::iterator<std::input_iterator_tag, int>
	{
	public:
		iterator() {}
		iterator(int ** elem) : _elem(elem) {}
		int & operator*() {return **_elem;}
		iterator & operator++() {++_elem; return *this;}
		bool operator==(iterator const & rhs) const {return _elem == rhs._elem;}
		bool operator!=(iterator const & rhs) const {return !(*this == rhs);}

	private:
		int ** _elem;		
	};

	graph() {
		for (int i = 0; i < 10; ++i)
			_verts[i] = i;  // vertex cost
	}

	typedef iterator vertex_iterator;
	typedef std::pair<vertex_iterator, vertex_iterator> adjacency_range;

	adjacency_range adjacent_vertices(vertex_descriptor const v) {
		int idx = 0;
		for (int i = -2; i < 3; ++i)
		{
			if (i != 0)
				_adjsbuf[idx++] = &_verts[v+i];
		}

		return std::make_pair(iterator(_adjsbuf.begin()),
			iterator(&_adjsbuf[0]+4));
	}

private:
	array<int> _verts;
	array<int *> _adjsbuf;
};


int main(int argc, char * argv[])
{	
	graph g;
	graph::adjacency_range r = g.adjacent_vertices(4);

	std::copy(r.first, r.second, std::ostream_iterator<int>(std::cout, " "));

	cout << "\n";
	
	graph::vertex_iterator it;
	for (it = r.first; it != r.second; ++it)
		std::cout << *it << " ";

	return 0;
}

