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


namespace boost {

BOOST_LOG_OPEN_NAMESPACE

namespace keywords {

BOOST_PARAMETER_KEYWORD(tag, errorcode)

}

BOOST_LOG_CLOSE_NAMESPACE

}  // boost::log::keywords


enum error_code
{
	ok,
	out_of_index
};

BOOST_LOG_ATTRIBUTE_KEYWORD(errorcode, "errorcode", error_code)

#define CUSTOM_LOG(lvl, ec)\
	BOOST_LOG_STREAM_WITH_PARAMS(slg,\
		(::boost::log::keywords::severity = ::boost::log::trivial::lvl)\
		(::boost::log::keywords::errorcode = error_code::ec)\
	)

inline char const * to_string(error_code ec)
{
	return "error code text";
}

//! Outputs stringized representation of the severity level to the stream
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	 std::basic_ostream< CharT, TraitsT >& strm, error_code ec)
{
	 const char* str = to_string(ec);
	 if (str)
		  strm << str;
	 else
		  strm << static_cast< int >(ec);
	 return strm;
}



int main(int argc, char * argv[])
{
	logging::add_common_attributes();  // adds attributes like TimeStamp, ...

	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<<	"[" << logging::trivial::severity << "] "
//				<< "(" << custom_errno << ") "
				<< "(" << errorcode << ") "
				<< expr::smessage
		)
	);

//	attrs::mutable_constant<int> errno_attr{101};
//	boost::shared_ptr<logging::core> core = logging::core::get();
//	core->add_global_attribute("errno", errno_attr);

	sources::severity_logger<logging::trivial::severity_level> slg;

//	BOOST_LOG_SEV(slg, logging::trivial::severity_level::warning) << "A warning message";

//	errno_attr.set(102);
//	BOOST_LOG_SEV(slg, logging::trivial::severity_level::info) << "A info message";

//	erron_attr.set(501);

	CUSTOM_LOG(warning, ok) << "Hello!!";

	return 0;
}

