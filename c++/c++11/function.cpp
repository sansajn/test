// pouzitie std::function aka. funktor
#include <functional>
#include <vector>
#include <iostream>

using std::function;
using std::bind;
using std::vector;
using std::cout;
using namespace std::placeholders;

struct foo
{
	void call_me() const {cout << "foo::call_me()\n";}
	void call_me_with_arg(bool b) const {cout << "foo::call_me_with_arg(bool)\n";}
	void call_me_overloaded() {cout << "foo::call_me_overloaded()\n";}
	void call_me_overloaded(bool b) {cout << "foo::call_me_overloaded(bool)\n";}
};

void g() {cout << "g()\n";}

void h() {cout << "h()\n";}
void h(bool b) {cout << "h(bool)\n";}

void e(bool) {cout << "e(bool)\n";}

struct hoo
{
	void operator()() const {cout << "hoo::operator()\n";}
};

void invoke(function<void ()> f)
{
	f();
}


int main(int argc, char * argv[]) 
{	
	using function_t = function<void ()>;

	foo f;

	vector<function_t> funcs{
		function_t{g},                            // playin old function call
		function_t{hoo{}},                        // member operator() call
		function_t{bind(&foo::call_me, &f)},      // member function call
		function_t{[](){cout << "lambda()\n";}},  // lambda function call
		function_t{bind(                          // overloaded member function
			static_cast<void (foo::*)()>(&foo::call_me_overloaded), &f)},
		function_t{static_cast<void (*)()>(h)}    // overloaded old playin function
	};

	for (auto f : funcs)  // call every function
		f();

	invoke([](){cout << "hello from inline lambda invoked by invoke()\n";});
	
	vector<function_t> direct_functs{
		g,
		hoo{},
		[](){cout << "lambda()\n";}
	};
	
	using function_with_arg_t = function<void (bool)>;
	
	vector<function_with_arg_t> functs_with_arg{
		function_with_arg_t{bind(e, _1)},                           // playin old function with argument
		function_with_arg_t{static_cast<void (*)(bool)>(h)},        // overloaded playin old function with argument
		function_with_arg_t{bind(&foo::call_me_with_arg, &f, _1)},  // member function with argument
		function_with_arg_t{bind(                                   // overloaded memger function with argument
			static_cast<void (foo::*)(bool)>(&foo::call_me_overloaded), &f, _1)} 
	};
	
	for (auto f : functs_with_arg)
		f(true);
	

	return 0;
}
