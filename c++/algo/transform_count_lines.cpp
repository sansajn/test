#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
using std::transform, std::count, std::vector, std::string, std::cout, std::istreambuf_iterator, std::ifstream;

int count_lines(string const & filename) {
	ifstream in{filename};
	return count(
		istreambuf_iterator<char>{in}, 
		istreambuf_iterator<char>{}, '\n');
}

vector<int> count_lines_in_files(vector<string> & files) {
	vector<int> result(size(files));
	transform(begin(files), end(files), begin(result), count_lines);
	return result;
}

int main(int argc, char * argv[]) {
	vector<string> files = {
		"SConstruct", 
		"transform_count_lines.cpp"};

	auto line_counts = count_lines_in_files(files);
	for (auto const & count : line_counts)
		cout << count << ", ";

	cout << "\n"
		<< "done!\n";

	return 0;
}
