#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char * argv[])
{
	fs::path p{"/home/foo"};
	fs::path r{"//local"};

	cout << p << " -> root_name:" << p.root_name() << "\n";
	cout << r << " -> root_name:" << r.root_name() << "\n";
	cout << p << " -> root_path:" << p.root_path() << "\n";

	return 0;
}
