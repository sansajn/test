#include <iostream>
#include <boost/asio.hpp>

boost::asio::io_service io_service;
boost::posix_time::seconds interval{1};
boost::asio::deadline_timer timer{io_service, interval};

void tick(boost::system::error_code const & e)
{
	std::cout << "tick" << std::endl;

	// reschedule the event
	timer.expires_at(timer.expires_at() + interval);
	timer.async_wait(tick);
}

int main(int argc, char * argv[])
{
	timer.async_wait(tick);  // schedule for the first time

	io_service.run();  // asio loop, blocking

	return 0;
}
