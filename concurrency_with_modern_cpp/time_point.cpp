// display the calendar time
#include <chrono>
#include <ctime>
#include <string>
#include <iostream>

using namespace std::chrono;
using namespace std;

int main(int argc, char * argv[])
{
	time_point<system_clock> sys_t;
	time_t tp = system_clock::to_time_t(sys_t);
	string tp_str = asctime(gmtime(&tp));
	cout << "epoch: " << tp_str << "\n";

	tp = system_clock::to_time_t(sys_t.min());
	tp_str = asctime(gmtime(&tp));
	cout << "time min: " << tp_str << "\n";

	tp = system_clock::to_time_t(sys_t.max());
	tp_str = asctime(gmtime(&tp));
	cout << "time max: " << tp_str << "\n";

	sys_t = system_clock::now();
	tp = system_clock::to_time_t(sys_t);
	tp_str = asctime(gmtime(&tp));
	cout << "time now: " << tp_str << "\n";

	return 0;
}
