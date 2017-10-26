// pull push async sample
#include <thread>
#include <array>
#include <string>
#include <iostream>
#include <cassert>
#include <boost/system/error_code.hpp>
#include <boost/utility/string_ref.hpp>
#include <azmq/socket.hpp>

using std::array;
using std::string;
using std::cout;
using boost::string_ref;
namespace asio = boost::asio;
namespace bs = boost::system;

int main(int argc, char * argv[])
{
	asio::io_service ios;

	azmq::socket pull{ios, ZMQ_PULL};
	pull.bind("tcp://*:5557");

	azmq::socket push{ios, ZMQ_PUSH};
	push.connect("tcp://localhost:5557");

	std::this_thread::sleep_for(std::chrono::milliseconds{100});

	// push -> pull
	push.async_send(asio::buffer("hello"),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes send" << std::endl;
			assert(bytes_transferred == 6);
		});

	array<char, 6> recv_buf;
	pull.async_receive(asio::buffer(recv_buf),
		[&recv_buf](bs::error_code const & ec, size_t bytes_transferred) {
			assert(bytes_transferred == 6);  // 'h', 'e', 'l', 'l', 'o', '\0'
			string_ref msg{recv_buf.data()};
			assert(msg == "hello");
		});

	ios.run();

	cout << "done.\n";

	return 0;
}
