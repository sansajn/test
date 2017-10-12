#include <chrono>
#include <iostream>

using std::cout;
using namespace std::chrono;

int main(int argc, char * argv[])
{
	using clock = high_resolution_clock;
	clock::time_point t = clock::now();
	cout << t.time_since_epoch().count() << "\n";
	return 0;
}
