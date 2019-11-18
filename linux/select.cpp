// select() function sample
#include <cassert>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	fd_set rfds;

	// watch stdin (fd=0) to see when it has input
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// wait up to 5s
	timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	int result = select(1, &rfds, nullptr, nullptr, &tv);

	if (result == -1)
		perror("select()");
	else if (result)
	{
		printf("Data is available now.\n");
		assert(FD_ISSET(0, &rfds));
	}
	else
		printf("No data within five seconds.\n");

	return 0;
}
