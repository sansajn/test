// the strategy pattern
#include <memory>
#include <iostream>

using namespace std;

class strategy
{
public:
	virtual void operator()() = 0;
	virtual ~strategy() = default;
};

class context
{
public:
	explicit context() : _strat{nullptr} {}
	void set_strategy(shared_ptr<strategy> strat) {_strat = strat;}
	void execute_strategy() {if (_strat) (*_strat)();}

private:
	shared_ptr<strategy> _strat;
};

class strategy1 : public strategy
{
	void operator()() override
	{
		cout << "Foo" << endl;
	}
};

class strategy2 : public strategy
{
	void operator()() override
	{
		cout << "Boo" << endl;
	}
};

class strategy3 : public strategy
{
	void operator()() override
	{
		cout << "FooBar" << endl;
	}
};

int main(int argc, char * argv[])
{
	context con;

	con.set_strategy(shared_ptr<strategy>{new strategy1});
	con.execute_strategy();

	con.set_strategy(shared_ptr<strategy>{new strategy2});
	con.execute_strategy();

	con.set_strategy(shared_ptr<strategy>{new strategy3});
	con.execute_strategy();

	return 0;
}
