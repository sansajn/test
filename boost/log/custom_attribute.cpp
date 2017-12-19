// vytvorenie konzoloveho sink frontendu a nastavenie formatovania
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>

using std::cout;
namespace logging = boost::log;
namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

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

	sources::severity_logger<logging::trivial::severity_level> slg;

	BOOST_LOG_SEV(slg, logging::trivial::severity_level::warning) << "A warning message";

	return 0;
}

