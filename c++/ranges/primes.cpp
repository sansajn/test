/*! counting primes with ranges sample
`rangesLazy.cpp` sample from [C++20: Functional Patterns with the Ranges Library](https://www.modernescpp.com/index.php/c-20-functional-patterns-with-the-ranges-library) 
article */
#include <ranges>
#include <iostream>

using std::views::iota, std::views::filter, std::views::take;
using std::cout;

bool is_prime(int i)
{
	for (int j = 2; j*j <= i; ++j)
		if (i % j == 0) 
			return false;
	return true;
}

int main(int argc, char * argv[])
{
	cout << "Numbers from 1'000'000 to 1'001'000 (displayed each 100th)\n";
	for (int i : iota(1'000'000, 1'001'000))
		if (i % 100 == 0) 
			cout << i << " ";

	cout << "\n\n";

	auto odd = [](int i){return i % 2 == 1;};
	cout << "Odd numbers from 1'000'000 to 1'001'000 (displayed each 100th)\n";
	for (int i : iota(1'000'000, 1'001'000)|filter(odd))
		if (i % 100 == 1)
			cout << i << " ";

	cout << "\n\n";

	cout << "Prime numbers from 1'000'000 to 1'001'000\n";
	for (int i : iota(1'000'000, 1'001'000)|filter(odd)|filter(is_prime)) 
		cout << i << " ";

	cout << "\n\n";

	cout << "20 prime numbers starting with 1'000'000:\n";
	for (int i : iota(1'000'000)|filter(odd)|filter(is_prime)|take(20))
		cout << i << " ";

	cout << "\n\n";

	return 0;
}
