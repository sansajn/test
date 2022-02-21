// boost::optional<T> sample
#include <unordered_map>
#include <string>
#include <iostream>
#include <boost/optional.hpp>
using std::unordered_map, std::string, std::cout;
using boost::optional;

optional<string> get_name(unsigned id) {
	unordered_map<unsigned, string> const names = {
		{1, "Peter"},
		{2, "Adam"},
		{3, "Jane"},
		{4, "Karol"}
	};

	if (auto it = names.find(id);
		it != end(names)) {

		return it->second;
	}
	else
		return {};
}

int main(int argc, char * argv[]) {
	unsigned id = argc > 1 ? atoi(argv[1]) : 1;
	
	if (optional<string> name = get_name(id);
		name) {

		cout << id << ": " << *name << "\n";
	}
	else
		cout << "ID (" << id << ") not found!\n";

	cout << "done\n";
	return 0;
}
