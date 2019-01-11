// posix_time sample and basic manipulation
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::cout;
using namespace boost::posix_time;
using namespace boost::gregorian;

int main()
{
	ptime pt1{date{2016, 5, 12}, time_duration{12, 0, 0}};
	cout << "pt1: " << pt1 << "\n";  // 2016-May-12 12:00:00
	
	// move ptime for 6 hours, 30min, 0s and 123ms
	time_duration td{6, 30, 0};
	td += boost::posix_time::millisec{123};
	ptime pt2 = pt1 + td;
	cout << "pt2: " << pt2 << "\n";  // 2016-May-12 18:30:00.123000
	
	// move ptime for 8 days
	date_duration dd{8};
	ptime pt3 = pt1 + dd;  // update ptime with days
	cout << "pt3: " << pt3 << "\n";  // 2016-May-20 12:00:00

	return 0;
}
