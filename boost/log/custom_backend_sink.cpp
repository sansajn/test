// custom back-end implementation sample
#include <iostream>
#include <boost/log/sinks.hpp>
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


class my_backend
	: public sinks::basic_sink_backend<sinks::concurrent_feeding>
{
public:
	void consume(logging::record_view const & rec)  //!< called for every log entry
	{
		cout << rec[expr::smessage] << std::endl;
	}
};

using sink_t = sinks::unlocked_sink<my_backend>;


int main(int argc, char * argv[])
{
	{
		boost::shared_ptr<logging::core> core = logging::core::get();
		boost::shared_ptr<my_backend> backend{new my_backend};
		boost::shared_ptr<sink_t> sink{new sink_t{backend}};
		core->add_sink(sink);
	}

	// standard console output sink
	logging::add_common_attributes();

	logging::add_console_log(std::clog,
		keywords::format = (
			expr::stream
				<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
				<<	"[" << logging::trivial::severity << "] "
				<< expr::smessage
		)
	);

	BOOST_LOG_TRIVIAL(info) << "hello!";

	return 0;
}
