#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
using namespace boost::filesystem;

int main(int argc, char * argv[])
{
	uintmax_t size = file_size("file_size.cpp");
	cout << "size=" << size << "B\n";
	return 0;
}
