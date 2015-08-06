#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char * argv[])
{
	fs::path p1{"devel/temp"};
	fs::path p2{"devel/temp/drop"};

	fs::path::iterator it = p2.end();
	fs::path p;
	// p.assig(p2.begin(), it);
	// fs::path p{p2.begin(), it};
	// while (p1 != p)
	// {
	// 	it = --it;
	// }

	// cout << "p:" << p << "\n";

	return 0;
}