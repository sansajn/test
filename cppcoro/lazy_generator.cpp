// lazy generator sample with coroutines
#include <iostream>

using std::cout, std::endl;

generator<int> generator_for_numbers(int begin, int inc = 1)
{
	for (int i = begin;; i += inc)
		co_yield i;
}

int main(int argc, char * argv[])
{
	auto numbers = generator_for_numbers(-10);
	for (int i = 0; i < 20; ++i)
		cout << numbers << ", ";
	cout << endl;

	cout << "done.\n";
	return 0;
}
