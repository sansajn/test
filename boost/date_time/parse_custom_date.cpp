// custom time format parse
#include <sstream>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::string;
using std::stringstream;
using std::cout;
using boost::posix_time::ptime;
 
ptime to_ptime(string const & s)
{
	static std::locale rundown_date_loc = std::locale(std::locale::classic(), 
		new boost::posix_time::time_input_facet("%Y-%m-%d-%H-%M-%S"));  // yyyy-mm-dd-hh-mm-ss

	stringstream is{s};
	is.imbue(rundown_date_loc);
	ptime pt;
	is >> pt;
	
	// what if format doesn't match ?
	if (!is.good())
		cout << "something goes wrong\n";
	
	return pt;
}

int main(int argc, char * argv[])
{
	string s1 = "2a017-10-11-22-00-15";
	ptime d = to_ptime(s1);
	if (d.is_not_a_date_time())
		cout << "date is realy not valid\n";
	cout << d << "\n";
	return 0;
}
