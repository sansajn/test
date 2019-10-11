// a lazy generator function based on coroutines (not yet working)
#include <vector>
#include <iostream>

using namespace std;

generator<int> generator_for_numbers(int begin, int step = 1)
{
	for (int i = begin;; i += step)
		co_yieald i;
}

int main(int argc, char * argv[])
{
	auto const numbers = generator_for_numbers(-10);

	for (int i = 1; i <= 20; ++i)
		cout << numbers << " ";

	cout << "\n\n";

	return 0;
}
