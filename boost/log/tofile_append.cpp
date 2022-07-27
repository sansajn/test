// basic file logging, where log file is appended
#include <boost/log/trivial.hpp>  // BOOST_LOG_TRIVIAL()
#include <boost/log/utility/setup/file.hpp>  // add_file_log()
#include <boost/log/utility/setup/common_attributes.hpp>  // add_common_attributes()

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

constexpr char const * log_file_name = "sample.log";

int main() {
   // NOTE: rotation_size needs to be set otherwise, 00000 log file is produced no metter of file_name keyword setup (in boost 1.71)
	logging::add_file_log(
		keywords::file_name=log_file_name,
		keywords::rotation_size = 1 * 1024 * 1024,  // rotate every 1MB
		keywords::open_mode = std::ios_base::app  // append
	);

	logging::core::get()->set_filter(
		logging::trivial::severity >= logging::trivial::info);

	logging::add_common_attributes();

	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
	BOOST_LOG_TRIVIAL(error) << "An error severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

	return 0;
}
