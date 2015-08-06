/* rekúrzivne volanie šablóny s indexom */
#include <typeinfo>
#include <string>
#include <iostream>
#include <vector>
using std::cout;
using std::string;
using std::vector;

void foo(int pos) 
{
	cout << "end\n";
}

template <typename T, typename ... Args>
void foo(int pos, T const & first, Args const & ... args)
{
	cout << pos << ":" << typeid(T).name() << "\n";
	foo(pos+1, args ...);
}

struct tmpl_memb
{
	void foo() {cout << "";}

	template <typename T, typename ... Args>
	void foo(T const & head, Args ... args)
	{
		cout << typeid(T).name() << "\n";
		foo(args ...);
	}
};


int main(int argc, char * argv[])
{
	int k;
	double d;
	string s;
	char const * cstr;
	vector<int> v;
	foo(0, k, d, s, cstr, v);

	tmpl_memb t;
	t.foo(k, d, s, cstr, v);

	return 0;
}

