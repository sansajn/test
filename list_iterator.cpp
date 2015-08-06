#include <iostream>
using std::cout;

typedef void * POSITION;

template <typename T>
class CList
{
protected:
	struct Node
	{
		Node * prev;
		Node * next;
		T data;
	};

public:
	CList() 
		: _head(NULL), _tail(NULL), _size(0) 
	{}

	POSITION GetHeadPosition() const {return _head;}
	T & GetAt(POSITION pos) {return ((Node *)(pos))->data;}

	T & GetNext(POSITION & pos) {
		pos = ((Node *)(pos))->next; 
		return ((Node *)(pos))->data;
	}

	int Size() const {return _size;}

	void PushBack(T const & x) 
	{
		_size += 1;
		Node * n = new Node;
		n->data = x;
		n->next = NULL;

		if (_head)
		{
			n->prev = _tail;
			_tail->next = n;
			_tail = n;
		}
		else
		{
			_head = n;
			_tail = n;
		}
	}

private:
	Node * _head;
	Node * _tail;
	int _size;
};


//! Musí mať rovnakú štruktúru ako Library::CList::CNode.
template <typename T>
struct ListNode
{
	ListNode * pPrev;
	ListNode * pNext;
	T data;
};

template <typename T>
struct ListIterator
{
	typedef T ValueType;
	typedef T * Pointer;
	typedef T & Reference;
	typedef ListNode<T> Node;
	typedef ListIterator<T> Iterator;
	typedef ListIterator<T const> ConstIterator;

	ListIterator() : _node(NULL) {}
	ListIterator(POSITION it) : _node((Node *)(it)) {}

	Reference operator*() const {return _node->data;}
	Pointer operator->() const {return _node->data;}

	ListIterator & operator++() {
		_node = _node->pNext;
		return *this;
	}

	ListIterator & operator++(int) {
		ListIterator tmp;
		_node = _node->pNext;
		return tmp;
	}

	ListIterator & operator--() {
		_node = _node->pPrev;
		return *this;
	}

	ListIterator & operator--(int) {
		ListIterator tmp;
		_node = _node->pPrev;
		return tmp;
	}

	bool operator==(ListIterator const & b) const {return _node == b._node;}
	bool operator!=(ListIterator const & b) const {return !(*this == b);}

private:
	Node * _node;
};

template <class Container>
struct ContainerTraits
{
	typedef typename Container::Iterator Iterator;
	typedef typename Container::ConstIterator ConstIterator;

	template <class C>
	static Iterator Begin(C & c) {return c.Begin();}

	template <class C>
	static Iterator Begin(C const & c) {return c.Begin();}

	template <class C>
	static Iterator End(C & c)	{return c.End();}

	template <class C>
	static ConstIterator End(C const & c) {return c.End();}
};

template <typename Container>
typename ContainerTraits<Container>::Iterator Begin(Container & c)
{
	return ContainerTraits<Container>::Begin(c);
}

template <typename Container>
typename ContainerTraits<Container>::ConstIterator Begin(Container const & c)
{
	return ContainerTraits<Container>::Begin(c);
}

template <typename Container>
typename ContainerTraits<Container>::Iterator End(Container & c)
{
	return ContainerTraits<Container>::End(c);
}

template <typename Container>
typename ContainerTraits<Container>::ConstIterator End(Container const & c)
{
	return ContainerTraits<Container>::End(c);
}

template <typename T>
struct CListTraitsImpl
{
	typedef ListIterator<T> Iterator;
	typedef ListIterator<T const> ConstIterator;

	static Iterator Begin(CList<T> & lst)
	{
		return Iterator(lst.GetHeadPosition());
	}

	static ConstIterator Begin(CList<T> const & lst)
	{
		return ConstIterator(lst.GetHeadPosition());
	}

	static Iterator End(CList<T> & lst)
	{
		return Iterator();
	}

	static ConstIterator End(CList<T> const & lst)
	{
		return ConstIterator();
	}
};

template <typename T>
struct ContainerTraits< CList<T> > : CListTraitsImpl<T>
{};

template <typename T>
struct ContainerTraits<CList<T> const> : CListTraitsImpl<T>
{};

void dump(CList<int> const & lst)
{
	ContainerTraits<CList<int> const>::ConstIterator cit;
	for (cit = Begin(lst); cit != End(lst); ++cit)
		cout << *cit << " ";
}


int main(int argc, char * argv[])
{
	CList<int> lst;
	for (int i = 0; i < 10; ++i)
		lst.PushBack(i);

	cout << "size:" << lst.Size() << "\n";


	ContainerTraits< CList<int> >::Iterator it;
	for (it = Begin(lst); it != End(lst); ++it)
		cout << "*it:" << *it << "\n";

	cout << "or\n";

	dump(lst);

	cout << "\n";

	return 0;
}

