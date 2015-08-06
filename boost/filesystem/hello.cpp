#include <string>
#include <iterator>
#include <iostream>
#include <boost/filesystem.hpp>

using std::string;
using std::reverse_iterator;
using std::cout;
namespace fs = boost::filesystem;

void path_info(fs::path const & p)
{
	cout << p << "\n";
	cout << "root_name:" << p.root_name() << "\n";
	cout << "root_directory:" << p.root_directory() << "\n";
	cout << "root_path:" << p.root_path() << "\n";
	cout << "relative_path:" << p.relative_path() << "\n";
	cout << "parent_path:" << p.parent_path() << "\n";
	cout << "filename:" << p.filename() << "\n";
	cout << "stem:" << p.stem() << "\n";
	cout << "extension:" << p.extension() << "\n";
	cout << "equals parent:" << (p == p.parent_path()) << "\n";

	for (fs::path elem : p)
		cout << elem << ", ";
	cout << "\n";

	fs::path::iterator it = p.end();
	--it;
	for (; it != p.begin(); --it)
		cout << *it << ", ";
	cout << *it << "\n";	

	cout << "\n";
}

int main(int argc, char * argv[]) 
{
	fs::path p{"/documents/test/foo/"};
	path_info(p);
	return 0;
}
