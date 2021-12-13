// count opened file descriptors by counting files in `/proc/PID/fd`
#include <filesystem>
#include <string>
#include <iostream>
// posix
#include <unistd.h>

using std::string, std::to_string, std::cout, std::distance;
using namespace std::filesystem;

int main(int argc, char * argv[]) {
	// count FDs in `/proc/PID/fd`
	pid_t pid = getpid();
	path fd_path = "/proc/" + to_string(pid) + "/fd";
	auto fd_count = distance(directory_iterator{fd_path}, {});

	cout << fd_path << " contains " << fd_count << " items\n";

	cout << "press <enter> to quit";
	getchar();

	return 0;
}
