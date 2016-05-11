// aktualny datum a datum zo stringu
#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>

using std::cout;
using namespace boost::gregorian;


int main()
{
	date d = day_clock::universal_day();
	cout << d.year() << "\n"
		<< d.month() << "\n"
		<< d.day() << "\n";
		
	d = date_from_iso_string("20160131");
	cout << d.year() << "\n"
		<< d.month() << "\n"
		<< d.day() << "\n";
}