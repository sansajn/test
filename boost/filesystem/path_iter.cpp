#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char * argv[])
{
	fs::path p{"/home/ja/temp/data/foo.txt"};

	cout << "for-loop: ";
	for (fs::path const & e : p)
		cout << e << ", ";

	cout << "\niterator: ";
	for (auto it = p.begin(); it != p.end(); ++it)
		cout << *it << ", ";

	cout << "\nreverse-iterator: ";
	for (auto it = p.rbegin(); it != p.rend(); ++it)
		cout << *it << ", ";

	cout << "\nparent_path iteration:\n";
	for (fs::path x{p}; !x.empty(); x = x.parent_path())
		cout << "  " << x << "\n";

	cout << "\ndone!\n";

	return 0;
}
