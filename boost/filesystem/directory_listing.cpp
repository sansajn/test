// file listing sample
#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char * argv[]) 
{
	fs::path p;
	if (argc > 1)
		p = fs::path{argv[1]};

	for (fs::directory_entry const & x : fs::directory_iterator{p})
		cout << x.path() << "\n";

	return 0;
}
