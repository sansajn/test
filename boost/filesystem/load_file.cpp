// loading file to string sample
#include <string>
#include <iostream>
#include <boost/filesystem/string_file.hpp>

using std::cout, std::string;
using boost::filesystem::load_string_file, boost::filesystem::path;
using namespace std::literals::string_literals;


int main(int argc, char * argv[]) {
	path p{argv[0] + ".cpp"s};
	string content;
	load_string_file(p, content);
	cout << "file " << p << " is " << size(content) << " bytes long\n";
	cout << "done!\n";
	return 0;
}
