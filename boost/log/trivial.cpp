// the most trivial logging ussage
#include <boost/log/trivial.hpp>

int main(int argc, char * argv[])
{
	BOOST_LOG_TRIVIAL(trace) << "a trace servility message";
	BOOST_LOG_TRIVIAL(debug) << "a debug servility message";
	BOOST_LOG_TRIVIAL(info) << "a info servility message";
	BOOST_LOG_TRIVIAL(warning) << "a warning servility message";
	BOOST_LOG_TRIVIAL(error) << "a error servility message";
	BOOST_LOG_TRIVIAL(fatal) << "a fatal servility message";
	return 0;
}
