/* clang++ -std=c++98 unique.cpp -o unique */
#include <iostream>
#include <memory>
#include <utility>
#include <cassert>
using std::cout;
using std::pair;

#if __cplusplus == 201103L
	#include <tuple>
	using std::tuple;
#endif


template <typename T>
struct default_delete
{
	void operator()(T * p) const {delete p;}
};


#if __cplusplus == 201103L

template <typename _Tp, typename _Dp = default_delete<_Tp> >
struct foo
{
	tuple<_Tp, _Dp> _t;
};

#endif


template <typename T, typename D = default_delete<T> >
class unique_ptr
{
public:
	typedef T * pointer;
	typedef T element_type;
	typedef D deleter_type;

	explicit unique_ptr(pointer p = 0) {_p.pointee = p;}
	~unique_ptr();

	pointer operator->() const {return _p.pointee;}
	T & operator*() const {return *_p.pointee;}
	pointer get() const {return _p.pointee;}
	deleter_type & get_deleter() {return deleter();}
	deleter_type const & get_deleter() const {return deleter();}
	pointer release();
	void reset(pointer p = pointer()) {unique_ptr(p).swap(*this);}
	void swap(unique_ptr & u) {std::swap(_p, u._p);}

#if __cplusplus == 201103L  // c++11
	explicit operator bool() const {return _p.pointee;}
#else // safe bool operator for ancient c++
private:
	typedef void (unique_ptr::*bool_type)() const;
	void this_type_does_not_support_comparisons() const {}

	void operator==(unique_ptr p) const;  // = delete
	void operator!=(unique_ptr p) const;  // = delete

public:
	operator bool_type() const 
	{
		return _p.pointee != NULL ? 
			&unique_ptr::this_type_does_not_support_comparisons : 0;
	}
#endif

private:
	struct holder : D  // use the "empty member" c++ optimization
	{
		T * pointee;
	};

	D & deleter() {return _p;}
	D const & deleter() const {return _p;}

	unique_ptr(unique_ptr const &);
	unique_ptr & operator=(unique_ptr const &);

	holder _p;
};

template <typename T, typename D>
unique_ptr<T, D>::~unique_ptr()
{
	if (_p.pointee)
		deleter()(_p.pointee);
	_p.pointee = 0;
}

template <typename T, typename D>
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::release()
{
	pointer p = _p.pointee;
	_p.pointee = 0;
	return p;
}


struct info_type
{
	info_type() {cout << "info_type()\n";}
	~info_type() {cout << "~info_type()\n";}
};



int main()
{
	int * i = new int(101);
	cout << "sizeof(i->int*):" << sizeof(i) << "\n";

#if __cplusplus == 201103L
	std::unique_ptr<int> up(new int(102));
	cout << "sizeof(up->unique_ptr<int>):" << sizeof(up) << "\n";
#endif

	pair<int, default_delete<int> > p;
	cout << "sizeof(p -> pair<>):" << sizeof(p) << "\n";

	pair<default_delete<int>, default_delete<int> > r;
	cout << "sizeof(p -> pair<empty, empty>):" << sizeof(r) << "\n";

	cout << "sizeof(empty):" << sizeof(default_delete<int>) << "\n";

#if __cplusplus == 201103L
	tuple<int, default_delete<int>> t;
	cout << "sizeof(t -> tuple<>):" << sizeof(t) << "\n";

	foo<int> f;
	cout << "sizeof(f -> foo):" << sizeof(f) << "\n";
#endif

	unique_ptr<int> g;
	cout << "sizeof(g -> goo):" << sizeof(g) << "\n";

	{
		unique_ptr<info_type> it(new info_type);
		cout << "sizeof(it -> goo<>):" << sizeof(it) << "\n";
	}


	// testy pre bool operator()
	
	unique_ptr<int> u(new int(101)), v;

	if (u)
		cout << "u is not empty\n";

	if (!v)
		cout << "v is empty\n";

	assert(u && "u is empty");
	assert(*u == 101 && "u != 101");
	assert(!v && "v is not empty");

	v.swap(u);

	assert(v && "v is empty");
	assert(*v == 101 && "v != 101");
	assert(!u && "u is not empty");

	v.release();

	assert(!v && "v is not empty");

	v.reset(new int(202));

	assert(*v == 202 && "v != 202");

	// toto by sa nemalo skompilovať
/*
	// toto sa skompiluje ak je definovany 'operator bool()'
	unique_ptr<int> a(new int(4));
	unique_ptr<double> b(new double(4.04));
	assert(a == b && "couldn't compile at all!");
*/
/*
	assert(a == b && "couldn't compile at all!");
	assert(u != v && "couldn't compile at all!")	;
	assert(u*5 == 200 && "couldn't compile at all!");

	int * k = new int(510);
	assert(k == v && "couldn't compile at all!")	;

	bool b = true;
	assert(b == v && "couldn't compile at all!")	;
*/

	return 0;
}

