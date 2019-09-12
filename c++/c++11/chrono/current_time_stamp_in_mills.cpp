#include <chrono>
#include <iostream>

using std::cout;
using namespace std::chrono;

int main(int argc, char * argv[])
{
	auto stamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	cout << "sizeof(stamp)=" << sizeof(stamp) << " bytes\n";
	cout << "stamp=" << stamp << "\n";
	cout << "stamp as int = " << (int)stamp << "\n";
	return 0;
}
