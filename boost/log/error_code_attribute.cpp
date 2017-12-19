// error code attribute, najjednoduchsia metoda
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

using std::cout;
namespace logging = boost::log;
namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

#define LOG(lvl, ec)\
	BOOST_LOG_TRIVIAL(lvl) << "(" << ec << ") "


int main(int argc, char * argv[])
{
	logging::add_common_attributes();  // adds attributes like TimeStamp, ...

	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<<	"[" << logging::trivial::severity << "] "
				<< expr::smessage
		)
	);

	LOG(warning, 112) << "Hello!!";

	return 0;
}


