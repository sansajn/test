// regex based file search with ranges sample
#include <vector>
#include <string_view>
#include <filesystem>
#include <algorithm>
#include <regex>
#include <iterator>
#include <iostream>
using std::vector, std::string_view, std::copy_if, std::back_inserter, std::cout;
using std::regex, std::regex_match;
namespace fs = std::filesystem;

vector<fs::directory_entry> find_files(fs::path const & path, std::string_view expr) {
	vector<fs::directory_entry> result;
	regex rx{expr.data()};

	copy_if(
		fs::recursive_directory_iterator{path},
		fs::recursive_directory_iterator{},
		back_inserter(result),
		[&rx](fs::directory_entry const & entry){
			return fs::is_regular_file(entry.path()) && regex_match(entry.path().filename().string(), rx);
		}
	);

	return result;
}

int main(int argc, char * argv[])
{
	auto pattern = argv[1];
	vector<fs::directory_entry> result = find_files(fs::current_path(), pattern);
	for (auto const & entry : result)
		cout << entry.path() << "\n";

	return 0;
}
