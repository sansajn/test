#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <unistd.h>

using std::min;
using std::cout;
using std::string;
using std::to_string;


string filename(FILE * f)
{
	int fno = fileno(f);
	string desc = "/proc/self/fd/" + to_string(fno);
	char buf[4096];
	size_t len = readlink(desc.c_str(), buf, 4096);
	buf[min(len, 4095ul)] = '\0';
	return buf;
}

int main(int argc, char * argv[])
{
	FILE * f = fopen("file_name_from_file.cpp", "r");
	assert(f);
	
	int fno = fileno(f);
	cout << fno << " -> " << filename(f) << std::endl;

	fclose(f);

	return 0;
}
