#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>
#include <locale>

using std::cout;
using namespace boost::gregorian;

int main()
{
	date d{2016, 5, 12};
	date_facet * df = new date_facet{"%A, %d %B %Y"};
	cout.imbue(std::locale{cout.getloc(), df});
	cout << d << "\n";
	return 0;
}