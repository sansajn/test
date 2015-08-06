// logovanie do súboru
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>  // add_file_log()

int main(int argc, char * argv[])
{
	boost::log::add_file_log("sample.log");
	boost::log::core::get()->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::info);

	BOOST_LOG_TRIVIAL(trace) << "a trace servility message";
	BOOST_LOG_TRIVIAL(debug) << "a debug servility message";
	BOOST_LOG_TRIVIAL(info) << "a info servility message";
	BOOST_LOG_TRIVIAL(warning) << "a warning servility message";
	BOOST_LOG_TRIVIAL(error) << "a error servility message";
	BOOST_LOG_TRIVIAL(fatal) << "a fatal servility message";
	return 0;
}
