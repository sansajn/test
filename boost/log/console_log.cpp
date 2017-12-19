// vytvorenie konzoloveho sink frontendu a nastavenie formatovania
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

using std::cout;
namespace logging = boost::log;
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

	BOOST_LOG_TRIVIAL(info) << "hello!";  // [2017-12-15 15:48:46.989610] [info] hello!

	return 0;
}
