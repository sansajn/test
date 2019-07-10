// vyvoj sablony zjednodusujucej implementacio observer patternu
#include <algorithm>
#include <vector>
#include <iostream>
using std::find;
using std::vector;
using std::cout;

template <typename L>
class observable_with
{
public:
	void register_listener(L * l);
	void forget_listener(L * l);

	template <typename Ret, typename Class, typename ... Args>
	void notify_listener(Ret Class::* p, Args ... args);

	vector<L *> const & listeners() const {return _listeners;}

private:
	vector<L *> _listeners;
};

template <typename L>
void observable_with<L>::register_listener(L * l)
{
	if (std::find(_listeners.begin(), _listeners.end(), l) == _listeners.end())
		_listeners.push_back(l);
}

template <typename L>
void observable_with<L>::forget_listener(L * l)
{
	auto it = std::find(_listeners.begin(), _listeners.end(), l);
	if (it != _listeners.end())
		_listeners.erase(it);
}

template <typename L>
template <typename Ret, typename Class, typename ... Args>
void observable_with<L>::notify_listener(Ret Class::* p, Args ... args)
{
	for (L * l : _listeners)
		(l->*p)(args ...);
}


//! multiple listener source helpers
//@{

template <typename Listener, typename Source, typename ListenerImpl>
void register_listener(Source & src, ListenerImpl * impl)
{
	src.observable_with<Listener>::register_listener(impl);
}

template <typename Listener, typename Source, typename ListenerImpl>
void forget_listener(Source & src, ListenerImpl * impl)
{
	src.observable_with<Listener>::forget_listener(impl);
}

template <typename Ret, typename Listener, typename Source>
void notify_listener(Ret Listener::*p, Source & src)
{
	src.observable_with<Listener>::notify_listener(p);
}

//@}


struct a_listener
{
	virtual void a() = 0;
};

struct b_listener
{
	virtual void b() = 0;
};

struct single_listener_source : public observable_with<a_listener>
{
	void fire_a()
	{
		notify_listener(&a_listener::a);
	}
};

struct single_listener_impl : public a_listener
{
	single_listener_impl(single_listener_source & s)
		: _src{s}
	{
		_src.register_listener(this);
	}

	~single_listener_impl()
	{
		_src.forget_listener(this);
	}

	void a() override
	{
		cout << "simgle_listener::a()" << std::endl;
	}

private:
	single_listener_source & _src;
};

struct multiple_listener_source
	: public observable_with<a_listener>, public observable_with<b_listener>
{
	void fire_a()
	{
		::notify_listener(&a_listener::a, *this);
	}

	void fire_b()
	{
		::notify_listener(&b_listener::b, *this);
	}
};

struct multiple_listener_impl : public a_listener, public b_listener
{
	multiple_listener_impl(multiple_listener_source & s)
		: _src{s}
	{
		::register_listener<a_listener>(_src, this);
		::register_listener<b_listener>(_src, this);
	}

	~multiple_listener_impl()
	{
		::forget_listener<a_listener>(_src, this);
		::forget_listener<b_listener>(_src, this);
	}

	void a() override
	{
		cout << "multiple_listener_impl::a()" << std::endl;
	}

	void b() override
	{
		cout << "multiple_listener_impl::b()" << std::endl;
	}

private:
	multiple_listener_source & _src;
};

int main(int argc, char *argv[])
{
	single_listener_source s1;
	single_listener_impl i1{s1};
	s1.fire_a();

	multiple_listener_source s2;
	multiple_listener_impl i2{s2};
	s2.fire_a();
	s2.fire_b();

	return 0;
}
