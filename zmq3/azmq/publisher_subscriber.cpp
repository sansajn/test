// publisher, subscriber async sample
#include <array>
#include <thread>
#include <iostream>
#include <boost/utility/string_ref.hpp>
#include <azmq/socket.hpp>

namespace asio = boost::asio;
namespace bs = boost::system;
using std::array;
using std::cout;
using boost::string_ref;

int main(int argc, char * argv[])
{
	asio::io_service ios;

	azmq::socket pub{ios, ZMQ_PUB};
	pub.bind("tcp://*:5557");

	azmq::socket sub{ios, ZMQ_SUB};
	sub.connect("tcp://localhost:5557");
	sub.set_option(azmq::socket::subscribe(""));

	std::this_thread::sleep_for(std::chrono::milliseconds{100});  // wait for connection

	char const * msg = "TEST";
	pub.async_send(asio::const_buffer{msg, 5},
		[&msg](bs::error_code const & ec, size_t bytes_transferred){
			cout << bytes_transferred << " bytes '" << msg << "' send" << std::endl;
		}
	);

	array<char, 256> buf;
	sub.async_receive(asio::buffer(buf),
		[&buf](bs::error_code const & ec, size_t bytes_transferred){
			cout << bytes_transferred << " bytes '" << string_ref{buf.data(), bytes_transferred-1}
				<< "' received" << std::endl;
		}
	);

	ios.run();

	cout << "done.\n";

	return 0;
}
