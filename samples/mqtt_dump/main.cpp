// functional sample to dump map structure into output stream
#include <string>
#include <map>
#include <vector>
#include <ranges>
#include <iostream>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string/join.hpp>
using std::string;
using std::map, std::vector;
using std::cout;
using std::views::filter, std::views::transform;
using std::ranges::copy;
using namespace std::string_literals;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	map<string, string> data{  // (topic, payload) pair
		{"a", "1"},
		{"b", "2"},
		{"c", "3"}
	};

	// procedural way
	for (auto const & [topic, payload] : data)
		cout << topic << " -> " << payload << ", ";
	cout << "\n";

	// functional with std::ranges
	copy(data|
		transform([](auto const & e){
			return e.first + " -> "s + e.second;
		}),
		std::ostream_iterator<string>{cout, ", "}
	);  //= a -> 1, b -> 2, c -> 3,
	cout << "\n";

	// functional with Boost.Ranges
	boost::copy(data|
		boost::adaptors::transformed([](auto const & e){
			return e.first + " -> "s + e.second;
		}),
		std::ostream_iterator<string>{cout, ", "}
	);  //= a -> 1, b -> 2, c -> 3,

	cout << "done!\n";

	return 0;
}
