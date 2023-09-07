// STL pretty printer sample
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std::string_literals;
using std::string, std::vector, std::map, std::set;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	string s = "hello";
	vector v = {1, 2, 3, 5};
	map<int, string> m = {{1, "one"s}, {2, "two"s}};
	set numbers = {6, 3, 1, 9};
	return 0;
}
