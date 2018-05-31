// how to remove work from io_service

#include <memory>
#include <chrono>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

using namespace boost::asio;
using std::shared_ptr;
using std::make_shared;
using std::cout;

int main(int argc, char * argv[])
{
	io_service ioserv;
	shared_ptr<io_service::work> work = make_shared<io_service::work>(ioserv);
	work.reset();  // it removes work from io_service
	ioserv.run();
	cout << "done" << std::endl;
	return 0;
}
