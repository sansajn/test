// implementuje jednosmerny zoznam
#include <iostream>
#include <cassert>

using std::cout;

template <typename T>
class forward_list
{
private:
	struct node {
		T data;
		node * next;
	};

public:
	forward_list() : _root{nullptr}, _count{0} {}
	
	bool empty() const {return _count == 0;}
	size_t size() const {return _count;}

	void push_front(T const & v) {
		node * n = new node;
		n->data = v;
		n->next = _root;
		_root = n;
		++_count;
	}

	void pop_front() {
		assert(!empty() && "snazim sa zmazat prvok prazneho zoznamu");
		node * n = _root;
		_root = n->next;
		delete n;
		--_count;
	}

	T & front() {
		return _root->data;
	}

	void clear() {
		node * n  = _root;
		while (n) 
		{
			node * p = n;
			n = n->next;
			delete p;
		}
		_count = 0;
	}

	struct iterator {  // forward-iterator
		iterator() : _p{nullptr} {}
		iterator(node * n) : _p{n} {}
		T & operator*() {return _p->data;}
		iterator operator++() {_p = _p->next; return *this;}  // pre-increment
		bool operator==(iterator rhs) const {return _p == rhs._p;}
		bool operator!=(iterator rhs) const {return !(*this == rhs);}
		node * _p;
	};

	iterator begin() {return iterator{_root};}
	iterator end() {return iterator{};}

	void insert_after(iterator pos, T const & v) {
		node * n = new node;
		n->data = v;
		n->next = pos._p->next;
		pos._p->next = n;
		++_count;
	}

	void erase_after(iterator pos) {
		node * n = pos._p->next;
		if (!n)
			return;
		pos._p->next = n->next;
		delete n;
		--_count;
	}

private:
	node * _root;
	size_t _count;
};

template <typename T>
void print(forward_list<T> & lst)
{
	for (T & i : lst) 
		cout << i << ", "; 
	cout << std::endl;
}

int main(int argc, char * argv[])
{
	forward_list<int> lst;
	assert(lst.empty());
	assert(lst.size() == 0);

	lst.push_front(1);
	lst.push_front(2);
	lst.push_front(3);
	assert(!lst.empty());
	assert(lst.size() == 3);  // 3, 2, 1
	
	lst.pop_front();
	lst.pop_front();
	assert(lst.size() == 1);  // 1

	lst.push_front(4);
	lst.push_front(5);
	lst.push_front(6);  // 6, 5, 4, 1

	auto it = lst.begin();
	++it;
	++it;
	lst.insert_after(it, 7);  // 6, 5, 4, 7, 1
	assert(lst.size() == 5);

	it = lst.begin();
	++it;
	lst.erase_after(it);
	lst.erase_after(it);   // 6, 5, 1
	assert(lst.size() == 3);

	for (int & i : lst)
		cout << i << ", ";   // 6, 5, 1
	cout << std::endl;

	return 0;
}
