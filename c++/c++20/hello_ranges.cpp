// gcc 9.2.1 doesn't have support for ranges (for now), we can use range-v3 implementation
#include <ranges>
#include <vector>
#include <iostream>
using std::vector, std::view, std::cout;

int main(int argc, char * argv[])
{
	vector numbers = {1, 2, 3, 4, 5, 6};
	auto result = numbers|view::filter([](int n){return n%2 == 0;})
		|view::transform([](int n){return n*2;});
		
	for (auto v : result)
		cout << v << " ";  // join ?
	
	cout << "done!\n";
	
	return 0;
}
