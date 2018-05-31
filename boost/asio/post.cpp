// add some work to io_service
#include <iostream>
#include <boost/asio/io_service.hpp>

using std::cout;
using namespace boost::asio;


int main(int argc, char * argv[])
{
	io_service io;

	io.post([](){
		cout << "work done" << std::endl;});

	io.run();

	return 0;
}
