// constant attribute sample
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes/constant.hpp>

using std::cout;
namespace logging = boost::log;
namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

BOOST_LOG_ATTRIBUTE_KEYWORD(custom_errno, "errno", int);

int main(int argc, char * argv[])
{
	logging::add_common_attributes();  // adds attributes like TimeStamp, ...

	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<<	"[" << logging::trivial::severity << "] "
				<< "(" << custom_errno << ") "
				<< expr::smessage
		)
	);

	boost::shared_ptr<logging::core> core = logging::core::get();
	core->add_global_attribute("errno", attrs::constant<int>{101});

	sources::severity_logger<logging::trivial::severity_level> slg;

	BOOST_LOG_SEV(slg, logging::trivial::severity_level::warning) << "A warning message";

	return 0;
}
