#include <functional>
#include <vector>
#include <iostream>

using std::placeholders::_1;
using std::vector;
using std::cout;

template <typename L>
class observable_width
{
public:
	void register_listener(L * listener) {_listeners.push_back(listener);}

	template <typename Ret, typename Class>
	void notify(Ret Class::* p)
	{
		for (L * l : _listeners)
			(l->*p)();
	}

	template <typename Ret, typename Class, typename ... Args>
	void notify_with_args(Ret Class::* p, Args ... args)
	{
		for (L * l : _listeners)
			(l->*p)(args ...);
	}

private:
	vector<L *> _listeners;
};

struct position_listener
{
	virtual void position_changed() = 0;
	virtual void position_changed_arg(int x) = 0;
};

class vehicle_tracker
	: public observable_width<position_listener>
{
public:
	void position_update()
	{
		notify(&position_listener::position_changed);
	}

	void position_update(int x)
	{
		notify_with_args(&position_listener::position_changed_arg, x);
	}
};

class X : public position_listener
{
public:
	X(vehicle_tracker & vehicle)
	{
		vehicle.register_listener(this);
	}

	void position_changed() override
	{
		cout << "X::position_changed()" << std::endl;
	}

	void position_changed_arg(int x) override
	{
		cout << "X::position_changed(x=" << x << ")" << std::endl;
	}
};


int main(int argc, char * argv[])
{
	vehicle_tracker vehicle;
	X x{vehicle};
	vehicle.position_update();
	vehicle.position_update(5);

	return 0;
}
