#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>

using std::cout;
using namespace boost::gregorian;

int main()
{
	date d1{2014, 1, 31};
	date d2{2014, 2, 28};
	date_duration dd = d2 - d1;
	cout << dd.days() << std::endl;
}