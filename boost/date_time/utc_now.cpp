// posix_time sample and basic manipulation
#include <string>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::cout;
using std::string;
using namespace boost::posix_time;
using namespace boost::gregorian;

int main()
{
	ptime t1 = microsec_clock::universal_time();

	cout << t1 << "\n";

	// convert to string
	string stamp = to_iso_string(t1);
	cout << "t1=" << stamp << "\n";

	// convert back to ptime
	ptime t2 = from_iso_string(stamp);
	cout << "t2=" << to_iso_string(t2) << "\n";

	// lets do some date math
	t2 += millisec{100};

	time_duration dt = t2 - t1;
	cout << "dt=" << dt.total_milliseconds() << "ms\n";

	// lets compare
	if (dt == millisec{100})
		cout << "time difference is exactly 100ms\n";
	else
		cout << "unknown\n";
	
	return 0;
}
