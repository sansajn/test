// implementacia error-code loggeru
#include <string>
#include <utility>
#include <ostream>
#include <iostream>
#include <boost/log/core/record.hpp>
#include <boost/log/utility/strictest_lock.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/unlocked_frontend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/keywords/auto_flush.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/scope_exit.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/core/null_deleter.hpp>

using std::string;
using std::pair;
using std::basic_ostream;
namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sources = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;

template <typename Base>
class record_errorcode_feature : public Base
{
public:
	using char_type = typename Base::char_type;
	using threading_model = typename Base::threading_model;

	record_errorcode_feature();
	record_errorcode_feature(record_errorcode_feature const & rhs);

	template <typename Args>
	record_errorcode_feature(Args const & args);  //!< forwarding constructor with named parameters

	// use the most restricting lock type
	using open_record_lock = typename logging::strictest_lock<
		boost::lock_guard<threading_model>, typename Base::open_record_lock,
		typename Base::add_attribute_lock, typename Base::remove_attribute_lock>::type;

protected:
	// lock-less implementation of operations
	template <typename Args>
	logging::record open_record_unlocked(Args const & args);
};

struct record_errorrcoder
	: public boost::mpl::quote1<record_errorcode_feature>
{};


namespace custom_keywords {
	BOOST_PARAMETER_KEYWORD(tag, errorcode)  // Boost.Parameter
}


template <typename Base>
record_errorcode_feature<Base>::record_errorcode_feature()
{}

template <typename Base>
record_errorcode_feature<Base>::record_errorcode_feature(record_errorcode_feature const & rhs)
	: Base{static_cast<Base const &>(rhs)}
{}

template <typename Base>
template <typename Args>
record_errorcode_feature<Base>::record_errorcode_feature(Args const & args)
	: Base{args}
{}

template <typename Base>
template <typename Args>
logging::record record_errorcode_feature<Base>::open_record_unlocked(Args const & args)
{
	// extract the named argument from the parameters pack
	int tag_value = args[custom_keywords::errorcode|int{}];

	logging::attribute_set & attrs = Base::attributes();
	logging::attribute_set::iterator tag = attrs.end();

	// add the tag as a new attribute
	pair<logging::attribute_set::iterator, bool> res =
		Base::add_attribute_unlocked("ErrorCode", attrs::constant<int>{tag_value});

	if (res.second)
		tag = res.first;

	// after opening a record remove the tag from the attributes (Boost.ScopeExit)
	BOOST_SCOPE_EXIT_TPL((&tag)(&attrs))
	{
		if (tag != attrs.end())
			attrs.erase(tag);
	}
	BOOST_SCOPE_EXIT_END

	return Base::open_record_unlocked(args);  // forward the call to the base feature
}


// error-code avare logger
template <typename Level = int>
class severity_errorcode_logger
	: public sources::basic_composite_logger<
		char, severity_errorcode_logger<Level>, sources::single_thread_model,
		sources::features<
			sources::severity<Level>, record_errorrcoder>
		>
{
	BOOST_LOG_FORWARD_LOGGER_MEMBERS_TEMPLATE(severity_errorcode_logger)
};

#define LOG_WITH_EC(lg, sev, ec) \
	BOOST_LOG_WITH_PARAMS((lg), (keywords::severity = (logging::trivial::sev))(custom_keywords::errorcode = (ec)))

BOOST_LOG_ATTRIBUTE_KEYWORD(errorcode, "ErrorCode", int);


int main(int argc, char * argv[])
{
	logging::add_common_attributes();

	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<<	"[" << logging::trivial::severity << "] "
				<< expr::smessage << " (" << errorcode << ")"
		)
	);

	severity_errorcode_logger<logging::trivial::severity_level> lg;

	LOG_WITH_EC(lg, trace, 101) << "can't open file";
	LOG_WITH_EC(lg, trace, 202) << "invalid handle value";

	return 0;
}
