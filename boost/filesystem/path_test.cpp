#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
namespace fs = boost::filesystem;

int main(int argc, char * argv[])
{
	fs::path p{"/home/foo"};
	fs::path r{"//local"};
	fs::path s{"file:///home/hlavatovic/devel/test/boost/filesystem/SConstruct"};  // file:// notation not working

	cout << p << " -> root_name:" << p.root_name() << "\n";
	cout << r << " -> root_name:" << r.root_name() << "\n";
	cout << p << " -> root_path:" << p.root_path() << "\n";
	
	if (fs::exists(s))
		cout << s << " exists\n";
	else
		cout << s << " doesn't exists\n";

	return 0;
}
