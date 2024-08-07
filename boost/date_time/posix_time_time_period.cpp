#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

using std::cout;
using namespace boost::posix_time;
using namespace boost::gregorian;

int main()
{
	ptime pt1{date{2014, 5, 12}, time_duration{12, 0, 0}};
	ptime pt2{date{2014, 5, 12}, time_duration{18, 30, 0}};
	time_period tp{pt1, pt2};
	cout.setf(std::ios::boolalpha);
	cout << tp.contains(pt1) << "\n"
		<< tp.contains(pt2) << "\n";
}
