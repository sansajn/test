#include <iostream>
#include <boost/date_time.hpp>

using std::cout;
using namespace boost::posix_time;

int main(int argc, char * argv[])
{
	ptime now = second_clock::universal_time();
	cout << to_iso_extended_string(now.date()) << "\n";  // 2019-10-01
	cout << to_simple_string(now.time_of_day()) << "\n";  // 14:31:43
	return 0;
}
