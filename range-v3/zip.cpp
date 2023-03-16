// zip algorithms (zip, zip_with (zip_transform)) sample
#include <vector>
#include <string>
#include <iostream>
#include <fmt/format.h>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/zip_with.hpp>
#include <range/v3/to_container.hpp>
using fmt::format;
using std::vector, std::cout, std::string, std::to_string;
using namespace std::string_literals;
using namespace ranges;

int main([[maybe_unused]] int argc, [[maybe_unused]] char const * argv[]) {
	// zip sample
	vector const a{10, 20, 30, 40, 50};
	vector const b{"ten"s, "twenty"s, "thirty"s, "fourty"s};

	for (auto const & [num, name] : views::zip(a, b))  //= 10 -> ten, 20 -> twenty, 30 -> thirty, 40 -> fourty
		cout << format("{} -> {}, ", num, name);

	cout << "\n";

	// zip_with (zip_transform) sample
	vector<string> v = views::zip_with(
		[](int const & a, string const & b){
			return to_string(a) + "_"s + b;
		}, a, b)
		|ranges::to<vector>();

	for (auto const & e : v)  //= 10_ten, 20_twenty, 30_thirty, 40_fourty
		cout << format("{}, ", e);

	cout << "\n"
		<< "done\n";

	return 0;
}
