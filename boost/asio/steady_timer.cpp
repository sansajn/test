/* pouzitie steady_timeru
https://theboostcpplibraries.com/boost.asio-io-services-and-io-objects */

#include <chrono>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

using namespace boost::asio;
namespace bs = boost::system;
using std::cout;

int main(int argc, char * argv[])
{
	io_service ioserv;

	steady_timer t1{ioserv, std::chrono::seconds{3}};
	t1.async_wait([](bs::error_code const &) {
		cout << "3 sec" << std::endl;
	});

	steady_timer t2{ioserv, std::chrono::seconds{4}};
	t2.async_wait([](bs::error_code const &){
		cout << "4 sec" << std::endl;
	});

	ioserv.run();

	return 0;
}
