/*#ifndef __SCOPED_PTR_H__
#define __SCOPED_PTR_H__*/

template <typename T>
class ScopedPtr 
{
public:
	typedef T ElementType;
	explicit ScopedPtr(T * p = 0) : _p(p) {}
	~ScopedPtr() {delete _p;}
	
	void Reset(T * p = 0) {ScopedPtr<T>(p).Swap(*this);}

	T & operator*() const {return *_p;}
	T * operator->() const {return _p;}
	T * Get() const {return _p;}

	void Swap(ScopedPtr & b)
	{
		T * tmp = _p;
		_p = b._p;
		b._p = tmp;		
	}

	typedef T * ScopedPtr::*unspecified_bool_type;
	operator unspecified_bool_type() const
	{
		return _p == 0 ? 0 : &ScopedPtr<T>::_p;
	}

private:
	ScopedPtr(ScopedPtr const &);
	ScopedPtr & operator=(ScopedPtr const &);
	void operator==(ScopedPtr const &) const;
	void operator!=(ScopedPtr const &) const;

private:
	T * _p;
};

template <typename T>
inline void Swap(ScopedPtr<T> & a, ScopedPtr<T> & b)
{
	a.Swap(b);
}

template <typename T>
inline T * GetPointer(ScopedPtr<T> const & p)
{
	return p.Get();
}

//#endif


#include <iostream>
using std::cout;


int main(int argc, char * argv[])
{
	ScopedPtr<int> p(new int(5));
	if (p)
		cout << *p;

	return 0;
}
