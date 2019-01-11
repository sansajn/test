#include <sstream>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

using std::stringstream;
using std::cout;
using namespace boost::posix_time;


int main(int argc, char * argv[])
{
	ptime t1 = microsec_clock::universal_time();  // from clock
	ptime t2 = time_from_string("2002-01-20 23:59:59.123456");  // from string
	ptime t3 = from_iso_string("20020120T235959.123456");  // from iso string

	// standard formatting
	cout << "simple string: " << to_simple_string(t1) << "\n"             // 2019-Jan-11 15:24:43.371758
		<< "iso string: " << to_iso_string(t1) << "\n"                     // 20190111T152443.371758
		<< "iso extended string: " << to_iso_extended_string(t1) << "\n";  // 2019-01-11T15:24:43.371758

	cout << to_simple_string(t2) << "\n";

	// custom formatting
	boost::posix_time::time_facet * facet = new boost::posix_time::time_facet();
	facet->format("%H:%M");

	stringstream ss;
	ss.imbue(std::locale(std::locale::classic(), facet));

	ss << t2;
	cout << ss.str() << std::endl;

	return 0;
}
