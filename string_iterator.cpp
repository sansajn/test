#include <string>
#include <iostream>
using std::cout;

typedef int INT;
typedef char TCHAR;
typedef int BOOL32;

namespace Library {

class CString
{
public:
	CString() {}
	CString(char const * s) : _s(s) {}
	INT GetLength() const {return _s.size();}
	TCHAR const * GetBufferPtr() const {return &_s[0];}
	TCHAR * GetBufferPtr() {return &_s[0];}
	BOOL32 IsEmpty() const {return _s.empty();}
	CString & operator+=(TCHAR ch) {_s += ch; return *this;}
	void Delete(INT i) {_s.erase(i,1);}

private:
	std::string _s;
};

};  // Library


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


/* m_pchData ukazuje na reťazec a (CStrinData*)m_pchData-1 na dátovu tabuľku
stringu */

namespace Detail {

template <typename T>
struct CStringTraitsImpl
{
	typedef T * Iterator;
	typedef T const * ConstIterator;
	typedef T ValueType;
	typedef INT SizeType;
	typedef T & Reference;
	typedef T const & ConstReference;

	static Iterator Begin(Library::CString & s)
	{
		return s.GetBufferPtr();
	}

	static ConstIterator Begin(Library::CString const & s)
	{
		return s.GetBufferPtr();
	}

	static Iterator End(Library::CString & s)
	{
		return s.GetBufferPtr()+s.GetLength();
	}

	static ConstIterator End(Library::CString const & s)
	{
		return s.GetBufferPtr()+s.GetLength();
	}

	static SizeType Size(Library::CString const & s)
	{
		return s.GetLength();
	}

	static bool Empty(Library::CString const & s)
	{
		return s.IsEmpty();
	}

	static Reference Back(Library::CString & s)
	{
		return s.GetBufferPtr()+s.GetLength()-1;
	}

	static ConstReference Back(Library::CString const & s)
	{
		return s.GetBufferPtr()+s.GetLength()-1;
	}

	static void PushBack(Library::CString & s, T const & x)
	{
		s += x;
	}

	static T PopBack(Library::CString & s)
	{
		T tmp = Back(s);
		s.Delete(Size(s)-1);
		return tmp;
	}
};

}; // Detail

template <>
struct ContainerTraits<Library::CString>
	: Detail::CStringTraitsImpl<TCHAR>
{};

template <>
struct ContainerTraits<Library::CString const>
	: Detail::CStringTraitsImpl<TCHAR>
{};


using Library::CString;

void dump(CString const & s)
{
	ContainerTraits<CString>::ConstIterator cit;
	for (cit = Begin(s); cit != End(s); ++cit)
		cout << *cit;
}

int main(int argc, char * argv[])
{
	CString s("Hello my name is Adam!");

	ContainerTraits<CString>::Iterator it;
	for (it = Begin(s); it != End(s); ++it)
		cout << *it;

	cout << "\nor\n";

	dump(s);

	cout << "\n";

	return 0;
}
