// custom feature multithread logger sample
#include <string>
#include <ostream>
#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/scope_exit.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/thread/locks.hpp>
#include <boost/move/utility_core.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/sources/features.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/strictest_lock.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

using std::string;
using std::pair;
namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sources = boost::log::sources;
namespace keywords = boost::log::keywords;

// BaseT je sources::basic_logger
template <typename BaseT>
class record_tagger_feature : public BaseT
{
public:
	using char_type = typename BaseT::char_type;
	using threading_model = typename BaseT::threading_model;

	record_tagger_feature();
	record_tagger_feature(record_tagger_feature const & rhs);

	template <typename ArgsT>
	record_tagger_feature(ArgsT const & args);  // forwarding constructor with named parameters

	// use the most restricting lock type
	typedef typename logging::strictest_lock<
		boost::lock_guard<threading_model>, 
        typename BaseT::open_record_lock,
		typename BaseT::add_attribute_lock, 
        typename BaseT::remove_attribute_lock
	>::type open_record_lock;

protected:
	// lock-less implementation of operations
	template <typename ArgsT>
	logging::record open_record_unlocked(ArgsT const & args);
};

// A convinience metafunction to specify the feature in the list of features of the final logger later.
struct record_tagger
	: public boost::mpl::quote1<record_tagger_feature>
{};


namespace custom_keywords {
	BOOST_PARAMETER_KEYWORD(tag_ns, tag)  // Boost.Parameter
}

template <typename BaseT>
record_tagger_feature<BaseT>::record_tagger_feature()
{}

template <typename BaseT>
record_tagger_feature<BaseT>::record_tagger_feature(record_tagger_feature const & rhs)
	: BaseT{static_cast<BaseT const &>(rhs)}
{}

template <typename BaseT>
template <typename ArgsT>
record_tagger_feature<BaseT>::record_tagger_feature(ArgsT const & args)
	: BaseT{args}
{}

template <typename BaseT>
template <typename ArgsT>
logging::record record_tagger_feature<BaseT>::open_record_unlocked(ArgsT const & args)
{
	// extract the named argument from the parameters pack
	string tag_value = args[custom_keywords::tag|string{}];

	logging::attribute_set & attrs = BaseT::attributes();
	logging::attribute_set::iterator tag = attrs.end();
	if (!tag_value.empty())
	{
		// add the tag as a new attribute
		pair<logging::attribute_set::iterator, bool> res =
			BaseT::add_attribute_unlocked("Tag", attrs::constant<string>{tag_value});

		if (res.second)
			tag = res.first;
	}

	// after opening a record remove the tag from the attributes (Boost.ScopeExit)
	BOOST_SCOPE_EXIT_TPL((&tag)(&attrs))
	{
		if (tag != attrs.end())
			attrs.erase(tag);
	}
	BOOST_SCOPE_EXIT_END

	return BaseT::open_record_unlocked(args);  // forward the call to the base feature
}

// tagging feature logger
template <typename LevelT = int>
class tagged_logger
	: public sources::basic_composite_logger<
		char, 
        tagged_logger<LevelT>, 
        //sources::single_thread_model,
        sources::multi_thread_model<boost::log::aux::light_rw_mutex>,
        //sources::multi_thread_model<std::mutex>,
		/*sources::features<
			sources::severity<LevelT>, record_tagger>
		>*/
        boost::log::sources::features<
            boost::log::sources::severity<LevelT>, record_tagger >
    >
{
	BOOST_LOG_FORWARD_LOGGER_MEMBERS_TEMPLATE(tagged_logger)
};

enum severity_level
{
	normal,
	warning,
	error
};

/*void manual_logging()
{
	tagged_logger<severity_level> logger;
	logging::record rec = logger.open_record((keywords::severity = normal, custom_keywords::tag = "GUI"));
	if (rec)
	{
		logging::record_ostream strm{rec};
		strm << "The user has confirmed his choice";
		strm.flush();
		logger.push_record(boost::move(rec));
	}
}*/

// or we can also use special macro to log
#define LOG_WITH_TAG(lg, sev, tg) \
	BOOST_LOG_WITH_PARAMS((lg), (keywords::severity = (sev))(custom_keywords::tag = (tg)))


void logging_function()
{
	tagged_logger<severity_level> logger;

	// cela pointa je v tom, ze pri vytvarani zanamu sa docasne vytvori 'Tag'
	// attribut, ktory sa po zapisani zaznamu zmaze, pozri implementaciu funkcie
	// 'record_tagger_feature<BaseT>::open_record_unlocked()'.
	//LOG_WITH_TAG(logger, normal, "GUI") << "The user has comfirmed his choice";

    //BOOST_LOG_SEV(logger, severity_level::warning) << "hello from custom logger!"; 

    LOG_WITH_TAG(logger, severity_level::normal, "GUI") << "The user has comfirmed his choice";
}


int main(int argc, char * argv[])
{
	//manual_logging();
	logging_function();
	return 0;
}


