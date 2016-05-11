#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>

using std::cout;

int main()
{
	boost::gregorian::date d{2016, 1, 31};
	cout << d.year() << "\n"
		<< d.month() << "\n"
		<< d.day() << "\n"
		<< d.day_of_week() << "\n"
		<< d.end_of_month() << "\n";
}
