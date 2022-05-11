// std::ranges::filter_view() algorithm in action
#include <ranges>
#include <array>
#include <iostream>
#include <cassert>
using std::ranges::filter_view, std::array, std::cout;

int main(int argc, char * argv[]) {
	array<int, 6> a{6, 2, 3, 4, 5, 1};

	auto is_six = [](int i){return i == 6;};

	for (int i : filter_view(a, is_six))
		cout << i << " ";
	cout << "\n";

	cout << "done\n";
	return 0;
}
