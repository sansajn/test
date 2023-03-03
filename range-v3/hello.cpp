// Hello sample for Range-v3 library.
#include <vector>
#include <string>
#include <iostream>
#include <range/v3/all.hpp>
using std::vector;
using std::string, std::to_string;
using std::cout;
using namespace ranges;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	vector const vi{1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto only_even = vi | views::remove_if([](int i){return i%2 == 1;})
		| views::transform([](int i){return to_string(i);});  //= {"2", "4", "6", "8", "9"}
	
	cout << only_even << "\n";
	
	cout << "done\n!";
	return 0;
}
