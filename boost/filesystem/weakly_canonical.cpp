// weakly_canonical function sample
#include <boost/filesystem/operations.hpp>
using boost::filesystem::path, boost::filesystem::weakly_canonical;

int main(int argc, char * argv[]) {
	path p{"/home/user/code/../file.txt"};
	path p2 = weakly_canonical(p);
	assert(p2 == "/home/user/file.txt");
	return 0;
}
