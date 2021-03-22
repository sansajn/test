// regex based file search with ranges sample
#include <string_view>
#include <filesystem>
#include <regex>
#include <iostream>
#include <ranges>
using std::string_view, std::cout;
using std::regex, std::regex_match;
using std::ranges::subrange, std::ranges::views::filter;
namespace fs = std::filesystem;

auto find_files(fs::path const & path, string_view pattern) {
	regex e{pattern.data(), size(pattern)};
	return subrange{fs::recursive_directory_iterator{path}, fs::recursive_directory_iterator{}}
		|filter([e](fs::directory_entry const & x){
	 		return fs::is_regular_file(x.path()) 
				&& regex_match(x.path().filename().string(), e);
	 	});
}

int main(int argc, char * argv[]) {
	auto pattern = argv[1];
	auto r = find_files(fs::current_path(), pattern);
	for (auto const & x : r)
		cout << x.path() << "\n";
	return 0;
}
