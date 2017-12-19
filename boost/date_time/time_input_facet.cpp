#include <sstream>
#include <iostream>
#include <boost/date_time.hpp>

using std::cout;
using std::stringstream;
namespace bt = boost::posix_time;

int main(int argc, char * argv[])
{
	std::locale custom_loc = std::locale(
		std::locale::classic(), new bt::time_input_facet("%Y-%m-%d-%H-%M-%S"));
	
	bt::ptime pt;
	stringstream is{"2016-03-14-18-12-39"};
	is.imbue(custom_loc);
	is >> pt;  // local time
	
	cout << pt << std::endl;
	
	return 0;
}
