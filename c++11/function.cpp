// pouzitie std::function
#include <functional>
#include <vector>
#include <iostream>

using std::function;
using std::bind;
using std::vector;
using std::cout;

struct foo
{
	void call_me() const {cout << "foo::call_me()\n";}
};

void g() {cout << "g()\n";}

struct hoo
{
	void operator()() const {cout << "hoo::operator()\n";}
};


int main(int argc, char * argv[]) 
{	
	using function_t = function<void ()>;

	foo f;

	vector<function_t> funcs{
		function_t{g},
		function_t{hoo{}},
		function_t{bind(&foo::call_me, &f)}
	};

	for (auto f : funcs)  // call every function
		f();

	return 0;
}
