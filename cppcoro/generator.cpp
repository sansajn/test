//! generator sample
#include <iostream>
#include <cppcoro/generator.hpp>

cppcoro::generator<char> hello()
{
	co_yield 'h';
	co_yield 'e';
	co_yield 'l';
	co_yield 'l';
	co_yield 'o';
}

cppcoro::generator<long long const> fibonacci()
{
	long long a = 0, 
		b = 1;
	while (true)
	{
		co_yield b;  // returns b value, next time we call fibonacci() we continue from 'auto tmp = a' line
		auto tmp = a;
		a = b;
		b += tmp;
	}
}

int main(int argc, char * argv[])
{
	for (auto c : hello())
		cout << c;
	cout << "\n\n";

	for (auto i: fibonacci())
	{
		if (i > 1'000'000) 
			break;
		cout << i << " ";
	}
	cout << "\n";

	return 0;
}

