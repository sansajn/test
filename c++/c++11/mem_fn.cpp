#include <functional>
#include <iostream>
#include <cassert>
using std::mem_fn;
using std::cout;

struct foo {
	int value() const {return 42;}
};

int main() {
	foo f;
	auto fun = mem_fn(&foo::value);
	int answer = fun(&f);
	assert(answer == 42);
	cout << "the answer is " << answer << "\n";
	return 0;
}
