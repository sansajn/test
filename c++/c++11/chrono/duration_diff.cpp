// duration difference
#include <chrono>
#include <iostream>

using std::cout;

int main(int argc, char * argv[])
{
	std::chrono::milliseconds d1{5}, d2{7};
	std::chrono::milliseconds diff = d1 - d2;
	cout << "5ms - 7ms = " << diff.count() << "ms" << std::endl;
	return 0;
}
