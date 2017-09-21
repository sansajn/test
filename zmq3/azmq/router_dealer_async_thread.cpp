//
#include <array>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <azmq/socket.hpp>

namespace asio = boost::asio;
namespace bs = boost::system;
using std::array;
using std::cout;

int main(int argc, char * argv[])
{
	asio::io_service ios_b;
	azmq::socket sb{ios_b, ZMQ_ROUTER};
	sb.bind("inproc://test");

	asio::io_service ios_c;
	azmq::socket sc{ios_c, ZMQ_DEALER};
	sc.connect("inproc://test");

	array<asio::const_buffer, 2> snd_bufs{
		{asio::buffer("A"), asio::buffer("B")}
	};

	bs::error_code ecc;
	size_t btc = 0;

	std::thread tc{
		[&](){
			sc.async_send(snd_bufs,
				[&](bs::error_code const & ec, size_t bytes_transferrd){
					SCOPE_EXIT{ios_c.stop();};
					ecc = ec;
					btc = bytes_transferrd;
				}
			);
			ios_c.run();
		}
	};

	bs::error_code ecb;
	size_t btb = 0;
	std::thread tb{
		[&](){
			array<char, 5> ident;
			array<char, 2> a, b;
			array<asio::mutable_buffer, 3> rcv_bufs = {
				{asio::buffer(ident), asio::buffer(a), asio::buffer(b)}
			};

			sb.async_receive(rcv_bufs,
				[&](bs::error_code const & ec, size_t bytes_transferred){
					SCOPE_EXIT{ios_b.stop();};
					ecb = ec;
					btb = bytes_transferred;
				}
			);
			ios_b.run();
		}
	};

	tc.join();
	tb.join();

	cout << "btc:" << btc << "\n"
		<< "btb:" << btb << "\n";

	return 0;
}
