// std::ranges::sort() algorithm in action
#include <algorithm>
#include <array>
#include <iostream>
#include <cassert>
using std::ranges::find_if, std::ranges::end, std::array, std::cout;

int main(int argc, char * argv[]) {
	array<int, 6> a{6, 2, 3, 4, 5, 1};

	auto is_six = [](int i){return i == 6;};

	auto i = find_if(a, is_six);
	if (i != end(a))
		cout << "i=" << *i << "\n"; 

	cout << "done\n";
	return 0;
}
