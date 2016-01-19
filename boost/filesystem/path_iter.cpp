#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char * argv[])
{
	fs::path p{"/home/ja/temp/data/foo.txt"};
	for (fs::path const & e : p)
		cout << e << "\n";

	return 0;
}
