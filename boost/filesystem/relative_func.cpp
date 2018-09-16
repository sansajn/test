#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
using boost::filesystem::path;
using boost::filesystem::relative;

int main(int argc, char * argv[])
{
	path a{"music/a"}, b{"music/2018/a"};
	cout << a << " relative " << b << " is " << relative(a, b) << std::endl;
	return 0;
}
