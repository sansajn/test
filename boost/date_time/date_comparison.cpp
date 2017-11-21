#include <iostream>
#include <cassert>
#include <boost/date_time.hpp>

using std::cout;

int main(int argc, char * argv[])
{
	boost::gregorian::date
		d1{2017, 11, 7},
		d2{2017, 10, 11},
		d3{2017, 10, 11};

	cout << d1 << " and " << d2 << "\n";
	if (d1 > d2)
		cout << d1 << " > " << d2 << "\n";
	else
		cout << d1 << " !> " << d2 << "\n";

	assert(d1 > d2);
	assert(d2 < d1);
	assert(d1 != d2);
	assert(d2 == d3);

	cout << "done.\n";

	return 0;
}
