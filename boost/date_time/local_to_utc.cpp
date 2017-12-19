#include <string>
#include <sstream>
#include <iostream>
#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using std::string;
using std::stringstream;
using std::cout;
using namespace boost::posix_time;
using namespace boost::gregorian;

ptime to_ptime(string const & t);

int main(int argc, char * argv[])
{
	ptime t1{date{2001, Dec, 31}, hours{19}};  // UTC time
	cout << "t1 (UTC): " << t1 << "\n";

	using local_adj = boost::date_time::c_local_adjustor<ptime>;
	cout << "t1 (local): " << local_adj::utc_to_local(t1) << "\n";

	ptime less_time = to_ptime("2017-11-27-14-00-00");
	ptime bigger_time = to_ptime("2017-11-27-16-00-00");
	ptime now = second_clock::universal_time();

	cout << "less_time:" << less_time << " "
		<<	(less_time > now ? ">" : "<") << " now:" << now << "\n";

	cout << "bigger_time:" << bigger_time << " "
		<< (bigger_time > now ? ">" : "<") << " now:" << now << "\n";

	return 0;
}

ptime to_ptime(string const & t)
{
	static std::locale custom_loc = std::locale(
		std::locale::classic(), new time_input_facet("%Y-%m-%d-%H-%M-%S"));

	ptime pt;
	stringstream is{t};
	is.imbue(custom_loc);
	is >> pt;

	return pt;
}
