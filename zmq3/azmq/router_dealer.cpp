// router dealer async sample
#include <array>
#include <string>
#include <thread>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/utility/string_ref.hpp>
#include <azmq/socket.hpp>

namespace asio = boost::asio;
namespace bs = boost::system;
using std::array;
using std::string;
using std::cout;
using boost::string_ref;

int main(int argc, char * argv[])
{
	asio::io_service ios;

	azmq::socket router{ios, ZMQ_ROUTER};
	router.bind("tcp://*:5556");

	azmq::socket dealer{ios, ZMQ_DEALER};
	dealer.connect("tcp://localhost:5556");

	std::this_thread::sleep_for(std::chrono::milliseconds{100});  // wait connection

	// wait for a question
	array<char, 5> ident;
	array<char, 256> content;
	array<asio::mutable_buffer,  2> serv_recv_bufs{asio::buffer(ident), asio::buffer(content)};
	router.async_receive(serv_recv_bufs,
		[&serv_recv_bufs, &router](bs::error_code const & ec, size_t bytes_transferred) {
			string_ref q{asio::buffer_cast<char const *>(serv_recv_bufs[1])};
			cout << bytes_transferred << " bytes (ident, '" << q << "') received by a server" << std::endl;

			if (q == "TEST")
			{
				array<asio::const_buffer, 2> send_bufs = {serv_recv_bufs[0], asio::buffer("GOOD")};
				router.async_send(send_bufs,
					[&send_bufs](bs::error_code const & ec, size_t bytes_transferred) {
						cout << bytes_transferred << " bytes (ident, '" << asio::buffer_cast<char const *>(send_bufs[1]) << "' send by a server" << std::endl;
					}
				);
			}
			else
			{
				cout << "unknown question, end\n";
				exit(1);
			}
		}
	);

	// ask a question
	char const * msg = "TEST";
	dealer.async_send(asio::const_buffer{msg, 5},
		[&msg](bs::error_code const & ec, size_t bytes_transferred){
			cout << "client: " << bytes_transferred << " bytes '" << msg << "' send" << std::endl;
		}
	);

	// wait for answer
	array<char, 256> client_recv_buf;
	dealer.async_receive(asio::buffer(client_recv_buf),
		[&client_recv_buf](bs::error_code const & ec, size_t bytes_transferred){
			cout << "client: " << bytes_transferred << " bytes '" << string_ref{client_recv_buf.data(), bytes_transferred-1}
				<< "' received" << std::endl;
		}
	);

	ios.run();

	return 0;
}
