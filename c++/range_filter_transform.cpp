// filter transform sample
#include <ranges>
#include <vector>
#include <iostream>

using std::views::filter, std::views::transform;
using std::vector;
using std::cout;

int main(int argc, char * argv[])
{
	vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto result = numbers
		| filter([](int n){return n % 2 == 0;})
		| transform([](int n){return n*2;});

	for (int const & v : result) 
		cout << v << " ";
	cout << "\n";

	cout << "done!\n";
	return 0;
}
