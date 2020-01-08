// https://www.boost.org/doc/libs/1_47_0/doc/html/boost_asio/overview/signals.html
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>

using std::cout;
namespace asio = boost::asio;


void handler(boost::system::error_code const & error, int signal_number)
{
	if (!error)
	{
		// A signal occurred.
		cout << "terminated\n";
	}
}

int main(int argc, char * argv[])
{
	asio::io_service io;

	// Construct a signal set registered for process termination.
	asio::signal_set signals(io, SIGINT, SIGTERM);

	// Start an asynchronous wait for one of the signals to occur.
	signals.async_wait(handler);

	io.run();
	cout << "done!\n";

	return 0;
}
