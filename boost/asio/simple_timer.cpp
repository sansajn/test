#include <iostream>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>

using namespace boost;
using namespace boost::chrono;

int main(int argc, char * argv[])
{
	asio::io_service service;

	asio::deadline_timer timer{service, posix_time::seconds{5}};
	timer.async_wait(
		[](auto ... vn) {
			std::cout << system_clock::now() << " : timer expired.\n";
		}
	);
	
	std::cout << system_clock::now() << " : calling run\n";

	service.run();

	std::cout << system_clock::now() << " : done.\n";

	return 0;
}