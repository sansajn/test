#include <memory>
#include <iostream>
#include <cstdio>
using std::cout;
using std::unique_ptr;

void close_file(FILE * f) {fclose(f);}

int main(int argc, char * argv[])
{
	unique_ptr<FILE, decltype(&close_file)> fin{fopen("demo.txt", "r"), &close_file};
	
	if (!fin)
		return 1;
	
	char buf[4096];
	size_t bytes = fread(buf, sizeof(char), 4096, fin.get());
	cout << "readed " << bytes << " bytes\n";
	
	cout << "done!\n";
	return 0;
}
