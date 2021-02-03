// using unique_ptr as function argument sample
#include <memory>
#include <utility>
#include <iostream>
using std::unique_ptr, std::make_unique;
using std::move;
using std::cout;

void foo(unique_ptr<int> p) 
{
	cout << "value=" << *p << "\n";
}

void goo(unique_ptr<int> && p)
{
	cout << "value=" << *p << "\n";
}

int main(int argc, char * argv[])
{
	auto p = make_unique<int>(10);
	foo(move(p));

	auto r = make_unique<int>(15);
	goo(move(r));

	cout << "done\n";
	return 0;
}
