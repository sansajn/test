// spustanie udalosti v jednom vlakne
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>

using namespace boost;
using namespace boost::chrono;


void timer_expired(std::string id)
{
	std::cout << system_clock::now() << " " << id << " enter.\n";
	std::this_thread::sleep_for(std::chrono::seconds{3});
	std::cout << system_clock::now() << " " << id << " leave.\n";
}

int main(int argc, char * argv[])
{
	asio::io_service service;
	asio::deadline_timer timer1{service, posix_time::seconds{5}};
	asio::deadline_timer timer2{service, posix_time::seconds{5}};

	timer1.async_wait([](auto ... vn){timer_expired("timer1");});
	timer2.async_wait([](auto ... vn){timer_expired("timer2");});

	std::thread butler{[&](){service.run();}};
	butler.join();

	std::cout << "done." << std::endl;
	return 0;
}