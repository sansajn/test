// vytvorenie konzoloveho sink frontendu a nastavenie formatovania
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/parameter/keyword.hpp>

using std::cout;
namespace logging = boost::log;
namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;


// define expression placeholder 'line_id'
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)

#define CUSTOM_LOG(lvl)\
	BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),\
		(::boost::log::keywords::severity = ::boost::log::trivial::lvl)\
	)


int main(int argc, char * argv[])
{
	logging::add_common_attributes();  // adds attributes like TimeStamp, ...

	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<< "[" << logging::trivial::severity << "] "
				<< "(" << line_id << ") "
				<< expr::smessage
		)
	);

	CUSTOM_LOG(warning) << "Hello!!";
	CUSTOM_LOG(error) << "Hello!!";

	return 0;
}

