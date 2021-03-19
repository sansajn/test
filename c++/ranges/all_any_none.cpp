// all_of, any_of and none_off sample
#include <algorithm>
#include <vector>
#include <iostream>
using std::ranges::all_of, std::ranges::any_of, std::ranges::none_of, std::cout, std::vector;

int main(int argc, char * argv[])
{
	vector r = {1, 2, 3, 4, 5};
	cout << std::boolalpha;

	bool all_less_10 = all_of(r, [](auto const & n){return n < 10;});
	cout << "all numbers less 10: " << all_less_10 << "\n";

	bool any_equal_5 = any_of(r, [](auto const & n){return n == 5;});
	cout << "any number equal 5: " << any_equal_5 << "\n";

	bool none_of_great_5 = none_of(r, [](auto const & n){return n > 5;});
	cout << "non of numbers greater 5: " << none_of_great_5 << "\n";

	cout << "done!\n";
	return 0;
}