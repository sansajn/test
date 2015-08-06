#include <iostream>
using std::cout;

typedef void * POSITION;

template <typename K, typename V>
class CMap
{
private:
	struct Assoc
	{
		K key;
		V value;
		int hash;
	};

public:
	CMap() : _elems(0) {
		for (int i = 0; i < 10; ++i)
			_table[i] = NULL;
	}

	~CMap() {
		for (int i = 0; i < 10; ++i)
			delete _table[i];
	}

	POSITION GetStartPosition() const {
		for (int i = 0; i < 10; ++i)
			if (_table[i])
				return POSITION(_table[i]);
		return NULL;
	}

	void GetNextAssoc(POSITION & nextPos, K & key, V & value) {
		Assoc * a = static_cast<Assoc *>(nextPos);
		for (int i = a->hash+1; i < 10; ++i)
		{
			if (_table[i])
			{
				nextPos = POSITION(_table[i]);
				key = _table[i]->key;
				value = _table[i]->value;
				return;
			}
		}
		nextPos = NULL;
	}

	K const & GetAssocKey(POSITION pos) {
		return static_cast<Assoc *>(pos)->key;
	}

	V & GetAssocValue(POSITION pos) {
		return static_cast<Assoc *>(pos)->value;
	}

	V & operator[](K const & k) {
		int hash = k%10;
		if (!_table[hash])
		{
			_table[hash] = new Assoc;
			_table[hash]->hash = hash;
			_table[hash]->key = k;
			_elems += 1;
		}
		return _table[hash]->value;
	}

	int Size() const {return _elems;}
	bool Empty() const {return Size() == 0;}

private:
	int _elems;
	Assoc * _table[10];
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


template <typename K, typename V>
struct MapPair
{
	K first;
	V second;
	MapPair(K key, V value) : first(key), second(value) {}
};

// forward iterator
template <typename K, typename V>
class MapIterator
{
public:
	typedef MapPair<K const &, V &> ValueType;

	MapIterator() : _c(NULL), _pos(NULL) {}
//	MapIterator(CMap<K, V> & c, POSITION pos) : _c(&c), _pos(pos) {}

	MapIterator(CMap<K, V> const & c, POSITION pos) 
		: _c(&const_cast< CMap<K, V> >(c)), _pos(pos)
	{}

	ValueType operator*() {
		return ValueType(_c->GetAssocKey(_pos), _c->GetAssocValue(_pos));
	}

	MapIterator & operator++() {
		K key;
		V value;
		_c->GetNextAssoc(_pos, key, value);
		return *this;
	}

	bool operator==(MapIterator const & rhs) const {
		return _pos == rhs._pos;
	}

	bool operator!=(MapIterator const & rhs) const {
		return !(*this == rhs);
	}

private:
	POSITION _pos;
	CMap<K, V> * _c;
};

template <typename K, typename V>
struct CMapTraitsImpl
{
	typedef MapIterator<K, V> Iterator;
	typedef MapIterator<K, V const> ConstIterator;

	static Iterator Begin(CMap<K, V> & c)
	{
		return Iterator(c, c.GetStartPosition());
	}

	static ConstIterator Begin(CMap<K, V> const & c)
	{
		return ConstIterator(c, c.GetStartPosition());
	}

	static Iterator End(CMap<K, V> & c)
	{
		return Iterator();
	}

	static ConstIterator End(CMap<K, V> const & c)
	{
		return ConstIterator();
	}
};

template <typename K, typename V>
struct ContainerTraits< CMap<K, V> > : CMapTraitsImpl<K, V>
{};

template <typename K, typename V>
struct ContainerTraits<CMap<K, V> const> : CMapTraitsImpl<K, V>
{};

void dump(CMap<int, int> const & c)
{
//	ContainerTraits< CMap<int, int> >::ConstIterator cit;

	MapIterator<int, int const> cit(c, c.GetStartPosition());

//	cit = Begin(c);
/*	
	for (cit = Begin(c); cit != End(c); ++cit)
		cout << "key:" << (*cit).first << ", value:" << (*cit).second << "\n";
*/		
}

int main(int argc, char * argv[])
{
	CMap<int, int> mapped;
	for (int i = 10; i > 0; --i)
		mapped[i] = 10+1-i;

	cout << "size:" << mapped.Size() << "\n";

/*	
	ContainerTraits< CMap<int, int> >::Iterator it;
	for (it = Begin(mapped); it != End(mapped); ++it)
		cout << "key:" << (*it).first << ", value:" << (*it).second << "\n";
*/
	cout << "\n";

	dump(mapped);


	return 0;
}

