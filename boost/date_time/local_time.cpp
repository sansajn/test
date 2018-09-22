#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::cout;
namespace pt = boost::posix_time;

int main(int argc, char * argv[])
{
	pt::ptime date = pt::second_clock::local_time();

	cout << "now:\n"
		<< pt::to_iso_string(date) << "\n"
		<< pt::to_simple_string(date) << "\n";

	return 0;
}
