/* Test sablonovej bunkcie mem_fun_ref s kniznice functional. */
#include <functional>
#include <string>
#include <iostream>
using std::cout;
#include <vector>
#include <algorithm>


class foo
{
public:
	foo(const std::string & name)
		: _name(name)
	{}

	void introduce() const
	{
		cout << "Hello, I'm " << _name << ".\n";
	}

	void name()
	{
		cout << _name << " ";
	}

private:
	std::string _name;
};


int main(int argc, char * argv[])
{
	std::vector<foo> objs{
		foo{"Peter"}, foo{"Jane"}, foo{"Adam"}
	};

	// call constant member function for each element in collection
	std::for_each(objs.begin(), objs.end(),
		std::mem_fun_ref(&foo::introduce));

	// call member function for each element in collection
	cout << "We have this models: ";
	std::for_each(objs.begin(), objs.end(), std::mem_fun_ref(&foo::name));

	return 0;
}
