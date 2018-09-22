#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::cout;

namespace bt {
	using namespace boost::gregorian;
	using namespace boost::posix_time;
}  // bt


int main(int argc, char * argv[])
{
	bt::ptime t1{bt::date{2006, 10, 1}, bt::time_duration{5, 21, 10, 123456}};
	bt::ptime t2 = bt::from_iso_string(bt::to_iso_string(t1));

	cout << t1;
	if (t1 == t2)
		cout << " == " << t2;
	else
		cout << " != " << t2;
	cout << "\n";

	return 0;
}
