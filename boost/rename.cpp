#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
using namespace boost;
using namespace boost::filesystem;

int main(int argc, char * argv[])
{
	path p1("test.txt");
	path p2("/data/test_renamed.txt");
//	path p2("/home/ja/projects/test/test_renamed.txt");
//	path p2("subdir/test_renamed.txt");
//	path p2("/data/temp/sync_test/test.txt");
	system::error_code ec;
	rename(p1, p2, ec);  // p1 -> p2
	if (ec.value() != 0)  // failed
	{
		system::error_code copy_err;
		copy(p1, p2, copy_err);
		if (copy_err.value() == 0)  // sucess
			remove(p1);
		cout << "used copy + remove instead\n";
	}
	return 0;
}

