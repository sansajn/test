/*! clang++ listtrait.cpp -o listtrait */
#include <iostream>
using std::cout;

#include "Iterator.h"

typedef void * POSITION;

namespace Library {

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

};  // Library


template <typename T>
struct CListTraitsImpl
{
	template <class Container>
	class IteratorImpl
	{
	public:
		typedef typename RcL::IteratorTraits<Container>::ValueType ValueType;
		typedef ValueType & Reference;
		typedef ValueType const & ConstReference;

		IteratorImpl(Container & c, POSITION it)
			: _it(it), _c(c)
		{}

		Reference operator*() {return _c.GetAt(_it);}
		ConstReference operator*() const {return _c.GetAt(_it);}
		Reference & operator->() {return _c.GetAt(_it);}
		ConstReference operator->() const {return _c.GetAt(_it);}
		IteratorImpl & operator++() {_c.GetNext(_it); return *this;}
		IteratorImpl & operator--() {_c.GetPrev(_it); return *this;}
		bool operator==(IteratorImpl const & b) const {return _it == b._it;}
		bool operator!=(IteratorImpl const & b) const {!(*this == b);}

		IteratorImpl operator++(int) 
		{
			IteratorImpl tmp(*this);
			_c.GetNext(_it);
			return tmp;
		}

		IteratorImpl operator--(int) 
		{
			IteratorImpl tmp(*this);
			_c.GetPev(_it);
			return tmp;
		}

	private:
		POSITION _it;
		Container & _c;
	};

	typedef IteratorImpl< Library::CList<T> > Iterator;

/*	
	template <typename U>
	class Iterator
		: public RcL::Iterator<RcL::BidirectionalIteratorTag, U>
	{
	public:
		Iterator( Library::CList<U> & lst, POSITION pos)
			: _pos(pos), _lst(lst)
		{}

		U & operator*() {return _lst.GetAt(_pos);}
		U const & operator*() const {return _lst.GetAt(_pos);}
		U & operator->() {return _lst.GetAt(_pos);}
		U const & operator->() const {return _lst.GetAt(_pos);}
		Iterator & operator++() {_lst.GetNext(_pos); return *this;}
		Iterator & operator--() {_lst.GetPrev(_pos); return *this;}
		bool operator==(Iterator const & b) const {return _pos == b._pos;}
		bool operator!=(Iterator const & b) const {!(*this == b);}

		Iterator operator++(int) 
		{
			Iterator tmp(*this);
			_lst.GetNext(_pos);
			return tmp;
		}

		Iterator operator--(int) 
		{
			Iterator tmp(*this);
			_lst.GetPev(_pos);
			return tmp;
		}
		
	private:
		POSITION _pos;
		Library::CList<U> & _lst;
	};
*/	

	static Iterator Begin(Library::CList<T> & lst)
	{
		return Iterator(lst, lst.GetHeadPosition());
	}
};

template <typename T>
struct ContainerTraits< Library::CList<T> >
	: CListTraitsImpl<T>
{};

template <typename T>
struct ContainerTraits<Library::CList<T> const>
	: CListTraitsImpl<T>
{};

void dump(Library::CList<int> const & lst)
{
/*	
	CListTraitsImpl<int>::Iterator<int> it(lst, lst.GetHeadPosition());
	for (int i = 0; i < lst.Size(); ++i, ++it)
		cout << *it << " ";
*/		
}


int main(int argc, char * argv[])
{
	Library::CList<int> lst;
	for (int i = 10; i > 0; --i)
		lst.PushBack(i);

	cout << "size:" << lst.Size() << "\n";

	CListTraitsImpl<int>::Iterator it(lst, lst.GetHeadPosition());

	int counter = 10;
	do
	{
		cout << "*it:" << *it << "\n";
		++it;
	} 
	while (--counter);

	cout << "or\n";

	dump(lst);

	return 0;
}

