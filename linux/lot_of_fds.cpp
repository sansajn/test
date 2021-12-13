/* in linux by default process can own only 1024 file descriptors (opened files, 
sockets, pipes, ...), after that opening file or socket fails, see RLIMIT_NOFILE 
documentation for getrlimit() function */
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
// posix
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/resource.h>
using std::cout, std::vector, std::for_each, std::begin, std::end;

vector<int> open_sockets(int count) {
	vector<int> sockets;
	for (int i = 0; i < count; ++i) {
		if (int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); 
			sock != -1) 
		{
			sockets.push_back(sock);
		}
	}
	return sockets;
}

vector<FILE *> open_files(int count) {
	vector<FILE *> files;
	for (int i = 0; i < count; ++i) {
		FILE * f = fopen("lot_of_fds.cpp", "rb");
		if (!f) {
			//cout << "file " << i+1 << " can't be opened\n";
			continue;
		}
		int fd = fileno(f);
		files.push_back(f);
		//cout << "file " << i+1 << " has fd=" << fd << "\n";
	}
	return files;
}

int main(int argc, char * argv[]) {
	int const fd_max = argc > 1 ? atoi(argv[1]) : FD_SETSIZE;

	// allow to own more then FD_SETSIZE file descriptors
	if (fd_max > FD_SETSIZE) {
		struct rlimit max_files_limit{(rlim_t)fd_max, (rlim_t)fd_max};
		setrlimit(RLIMIT_NOFILE, &max_files_limit);
	}

	cout << "process can own maximum " << fd_max 
		<< " file descriptors (opened files, sockets, pipes, ..)" 
		<< " (" << FD_SETSIZE << " by default)\n";

	// open some sockets
	vector<int> sockets = open_sockets(FD_SETSIZE - 100);
	cout << size(sockets) << " sockets opened\n";
	
	// open some files
	vector<FILE *> files = open_files(FD_SETSIZE - 100);
	cout << size(files) << " files opened\n";
	
	cout << size(files) + size(sockets) << " owned file descriptors (FDs)\n";

	cout << "press <enter> to quit";
	getchar();

	for_each(begin(sockets), end(sockets), close);  // close opened sockets
	for_each(begin(files), end(files), fclose);  // close opened files

	cout << "done!\n";
	return 0;
}
