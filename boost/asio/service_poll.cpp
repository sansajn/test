// neblokujuce volanie servisu zo slucky
#include <chrono>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

using std::cout;
using namespace boost::asio;
namespace bs = boost::system;

int main(int argc, char * argv[])
{
	io_service io;

	steady_timer t{io, std::chrono::seconds{1}};
	t.async_wait(
		[](bs::error_code const &) {
			cout << "steady timer done" << std::endl;
		}
	);

	while (true)
	{
		size_t handler_count = io.poll();
		if (handler_count > 0)
			cout << handler_count << " handlers executed" << std::endl;
	}

	return 0;
}

