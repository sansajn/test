// nekonecne io_service::run()

#include <chrono>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

using namespace boost::asio;
using std::cout;

int main(int argc, char * argv[])
{
	io_service ioserv;
	io_service::work work{ioserv};  // still some work to do for ioserv
	ioserv.run();
	cout << "done" << std::endl;  // should never heppend
	return 0;
}
