/* skúška friend konštrukcie */
#include <iostream>
using std::cout;
#include <utility>
using std::pair;
using std::make_pair;
#include "tie.hpp"
using loe::tie;


template <typename T, size_t N>
class static_vector 
{
public:
	typedef T * iterator;
	static_vector() : _elems(0) {}
	iterator begin() {return _d;}
	iterator end() {return _d+_elems;}
	T & operator[](int i) {return _d[i];}
	int size() const {return _elems;}
	void push_back(T const & x) {_d[_elems++] = x;}

private:
	T _d[N];
	int _elems;	
};

struct graphelement {
	int id;
	graphelement(int element_id) : id(element_id) {}
};


class navi_logic
{
public:
	struct adjacent_edge_context {
		int id;
	};

private:
	struct adjacent_edge {
		graphelement const * edge;
		adjacent_edge_context context;
	};

public:
	struct adjacency_iterator 
	{
	public:
		typedef adjacency_iterator self;
		adjacency_iterator() : _e(NULL) {}
		adjacency_iterator(adjacent_edge * e) : _e(e) {}
		adjacent_edge const & operator*() {return *_e;}
		adjacent_edge_context const & context() {return _e->context;}
		void operator++() {++_e;}
		bool operator==(self const & rhs) {return _e == rhs._e;}
		bool operator!=(self const & rhs) {return !(*this == rhs);}

	private:
		adjacent_edge * _e;
	};

	typedef pair<adjacency_iterator, adjacency_iterator> range;

	range adjacent_elements() {
		return make_pair(_adjedges.begin(), _adjedges.end());
	}

	navi_logic() {
		for (int i = 0; i < 10; ++i) {
			adjacent_edge e;
			e.edge = new graphelement(i);
			e.context.id = i*i;
			_adjedges.push_back(e);
		}
	}

	~navi_logic() {
		static_vector<adjacent_edge, 20>::iterator it = _adjedges.begin(), 
			e_it = _adjedges.end();
		for (; it != e_it; ++it)
			delete (*it).edge;
	}

private:
	static_vector<adjacent_edge, 20> _adjedges;
};


int main()
{
	navi_logic logic;
	int counter = 0;
	navi_logic::adjacency_iterator it, e_it;
	for (tie(it, e_it) = logic.adjacent_elements(); it != e_it; ++it, counter++)
	{
		cout << "#" << counter << ". edge.id:" << (*it).id 
			<< ", edge.context:" << it.context().id << "\n";
	}
	return 0;
}
