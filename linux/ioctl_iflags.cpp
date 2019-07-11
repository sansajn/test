#include <string>
#include <iostream>
#include <cassert>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	string ifile = (argc > 1) ? argv[1] : "immutable.txt";

	int fd = open(ifile.c_str(), O_RDONLY);
	assert(fd != 0);

	int attr = 0;
	ioctl(fd, FS_IOC_GETFLAGS, &attr);

	if (attr & FS_IMMUTABLE_FL)
		cout << "file \"" << ifile << "\" is immutable\n";
	else
		cout << "file \"" << ifile << "\" is not immutable\n";

	close(fd);

	cout << "done!\n";
	return 0;
}
