// for_each range sample
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <range/v3/all.hpp>
using std::vector, std::tuple;
using std::string;
using std::cout;
using namespace std::string_literals;
using namespace ranges;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	vector v{1,2,3,4,5,6};
	
	for_each(v | views::take(10), [](int const & n){
		cout << n << ", ";
	});

	cout << '\n';


	vector u{tuple{1, "one"s}, tuple{2, "two"s}};
	for_each(u, [](tuple<int, string> const & elem){
		auto const & [number, word] = elem;
		cout << word << " -> " << number << ", ";
	});

	cout << '\n';

	cout << "done!\n";
	return 0;
}
