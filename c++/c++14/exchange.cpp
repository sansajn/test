// std::exchange() sample
#include <utility>
#include <iostream>
#include <cassert>
using std::exchange;
using std::cout;

int main(int argc, char * argv[]) {
	int x = 42;
	int x_old = exchange(x, x+1);

	assert(x==43);
	assert(x_old==42);
	cout << "x_old=" << x_old << " and x=" << x << "\n";
	
	return 0;
}
