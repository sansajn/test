// error code attribute, pomocou set_get_attrib a macro overload
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/sources/record_ostream.hpp>

using std::cout;
namespace logging = boost::log;
namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

enum class error_code
{
	ok,
	not_specified,
	out_of_range,
	file_not_found
};

using snmp_logger = sources::severity_logger_mt<logging::trivial::severity_level>;

#define _LOG1(lvl)\
	BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),\
	(::boost::log::keywords::severity = ::boost::log::trivial::lvl))

#define _LOG2(lvl, ec)\
	BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),\
	(set_get_attrib("ErrorCode", error_code::ec))\
	(::boost::log::keywords::severity = ::boost::log::trivial::lvl))

#define _GET_LOG_OVERRIDE(_1, _2, NAME, ...) NAME

#define CUSTOM_LOG(...)\
	_GET_LOG_OVERRIDE(__VA_ARGS__, _LOG2, _LOG1)(__VA_ARGS__)

#define LOG(lvl, ec)\
	BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),\
		(set_get_attrib("ErrorCode", error_code::ec))\
		(::boost::log::keywords::severity = ::boost::log::trivial::lvl)\
	)

template <typename V>
V set_get_attrib(char const * key, V value)
{
	auto attr = logging::attribute_cast<attrs::mutable_constant<V>>(logging::core::get()->get_thread_attributes()[key]);
	attr.set(value);
	return attr.get();
}

//! Outputs stringized representation of the severity level to the stream
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<<(
	std::basic_ostream< CharT, TraitsT >& strm, error_code ec)
{
	strm << static_cast<int>(ec);
	return strm;
}


int main(int argc, char * argv[])
{
	logging::add_common_attributes();  // adds attributes like TimeStamp, ...

	logging::core::get()->add_thread_attribute("ErrorCode", attrs::mutable_constant<error_code>{error_code::ok});

/*
	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<<	"[" << logging::trivial::severity << "] "
				<< "(" << expr::attr<error_code>("ErrorCode") << ") "
				<< expr::smessage
		)
	);
*/

	LOG(warning, out_of_range) << "Hello!!";

	CUSTOM_LOG(debug) << "A debug message";
	CUSTOM_LOG(info, file_not_found) << "A info message with error code";

	return 0;
}



