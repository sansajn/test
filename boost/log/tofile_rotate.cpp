/* file logging, with 
- log rotation and 
- max 3 log files with 1KB each in a log file directory and
- log file name formatting and
- log record formatting
sample. */
#include <boost/log/trivial.hpp>  // BOOST_LOG_TRIVIAL()
#include <boost/log/utility/setup/file.hpp>  // add_file_log()
#include <boost/log/utility/setup/common_attributes.hpp>  // add_common_attributes()

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

constexpr char const * log_file_directory = "logs",
	// NOTE: `%N` placeholder can be formated by printf-compatible form (e.g. `%5N`) and also Boost.DateTime [placeholders](https://www.boost.org/doc/libs/1_79_0/doc/html/date_time/date_time_io.html) can be used there
	//* log_file_name = "sample_%3N.log"
	* log_file_name = "sample_%Y-%m-%d_%2N.log";  // set log file name to e.g. `sample_2022-07-27_01.log`

int main() {
   // NOTE: rotation_size needs to be set otherwise, 00000 log file is produced no metter of file_name keyword setup (in boost 1.71)
	logging::add_file_log(
		keywords::file_name=log_file_name,
		keywords::open_mode = std::ios_base::app,  // append
		keywords::rotation_size = 1 * 1024,  // rotate every 1KB
		keywords::target=log_file_directory,  // target directory, NOTE: this needs to be set otherwise max_files/max_size not working
		keywords::max_files = 3,  // maximum 3 log files
		keywords::format = "[%TimeStamp%] [%Severity%]: %Message%"
	);

	logging::core::get()->set_filter(
		logging::trivial::severity >= logging::trivial::info);

	logging::add_common_attributes();

	for (size_t i = 0; i < 10; ++i) {
		BOOST_LOG_TRIVIAL(trace) << "A trace severity message " << i;
		BOOST_LOG_TRIVIAL(debug) << "A debug severity message " << i ;
		BOOST_LOG_TRIVIAL(info) << "An informational severity message " << i;
		BOOST_LOG_TRIVIAL(warning) << "A warning severity message " << i;
		BOOST_LOG_TRIVIAL(error) << "An error severity message " << i;
		BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message " << i;
	}

	BOOST_LOG_TRIVIAL(info) << "done!";

	return 0;
}
