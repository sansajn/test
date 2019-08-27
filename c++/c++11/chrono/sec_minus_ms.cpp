// duration difference
#include <chrono>
#include <iostream>

using std::cout;
using namespace std::chrono;

int main(int argc, char * argv[])
{
	milliseconds dt = seconds{2} - milliseconds{16775};
	cout << "2s - 1677ms = " << dt.count() << "ms\n";
	return 0;
}
