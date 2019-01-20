#include <string>
#include <iostream>
#include <memory>
#include <unistd.h>

using std::string;
using std::cout;
using std::unique_ptr;

string program_location()
{
	for (ssize_t max_path = 64; ; max_path *= 2)
	{
		unique_ptr<char []> buf{new char[max_path]};
		ssize_t bytes = readlink("/proc/self/exe", buf.get(), max_path);
		if (bytes == -1)
			throw std::runtime_error{"unable to found program location"};
		if (bytes != max_path)
			return string{buf.get()};
	}
}

int main(int argc, char * argv[])
{
	cout << program_location() << "\n";

	return 0;
}
