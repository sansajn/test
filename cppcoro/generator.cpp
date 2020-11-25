// coroutine based generator sample from [C++20: Coroutines with cppcoro](https://www.modernescpp.com/index.php/c-20-coroutines-with-cppcoro) article

#include <iostream>
#include <cppcoro/generator.hpp>

using std::cout;

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
		co_yield b;
		auto tmp = a;
		a = b;
		b += tmp;
	}
}

int main(int argc, char * argv[])
{
	for (char c : hello())
		cout << c;

	cout << "\n\n";

	for (long long y : fibonacci())
	{
		if (y > 1'000'000)
			break;
		cout << y << " ";
	}

	cout << "\n\n"
		<< "done!\n";

	return 0;
}
