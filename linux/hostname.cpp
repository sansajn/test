#include <iostream>
#include <unistd.h>

using std::cout;

int main(int argc, char * argv[])
{
	char name[1024];
	gethostname(name, 1024);
	cout << "hostname is '" << name << "'" << std::endl; 
	return 0;
}
