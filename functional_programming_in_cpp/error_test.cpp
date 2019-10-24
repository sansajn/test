// predicate to test for errors implementation
#include <utility>
#include <iostream>

using namespace std;

class error_test
{
public:
	error_test(bool error = true)
		: _error{error}
	{}

	template <typename T>
	bool operator()(T && value) const
	{
		return _error == (bool)forward<T>(value).error();
	}

	error_test operator==(bool test) const
	{
		return error_test{test ? _error : !_error};
	}

	error_test operator!() const
	{
		return error_test{!_error};
	}

private:
	bool _error;
};


error_test error{true};
error_test not_error{false};

struct foo
{
	bool error() const {return true;}
};

struct goo
{
	bool error() const {return false;}
};

int main(int argc, char * argv[])
{
	if (error(foo{}))
		cout << "foo with error\n";

	if (not_error(goo{}))
		cout << "goo without error\n";

	return 0;
}
