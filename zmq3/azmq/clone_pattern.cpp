// clone pattern azmq implementation
#include <array>
#include <thread>
#include <iostream>
#include <cassert>
#include <boost/utility/string_ref.hpp>
#include <azmq/socket.hpp>

using std::array;
using std::cout;
using boost::string_ref;
namespace asio = boost::asio;
namespace bs = boost::system;

int main(int argc, char * argv[])
{
	asio::io_service ios;

	// server
	azmq::pub_socket pub{ios};
	azmq::router_socket rout{ios};
	azmq::pull_socket pull{ios};
	pub.bind("tcp://*:5555");
	rout.bind("tcp://*:5556");
	pull.bind("tcp://*:5557");

	// client
	azmq::sub_socket sub{ios};
	azmq::dealer_socket deal{ios};
	azmq::push_socket push{ios};
	sub.connect("tcp://localhost:5555");
	sub.set_option(azmq::socket::subscribe{""});
	deal.connect("tcp://localhost:5556");
	push.connect("tcp://localhost:5557");

	std::this_thread::sleep_for(std::chrono::milliseconds{100});

	// publish news
	pub.async_send(asio::buffer("hello!"),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes send by a publisher socket" << std::endl;
			assert(bytes_transferred == 7);
		});

	// receive news
	array<char, 7> recv_news_buf;
	sub.async_receive(asio::buffer(recv_news_buf),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes received by a subscriber socket" << std::endl;
			assert(bytes_transferred == 7);
		});

	// ask server
	deal.async_send(asio::buffer("name?"),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes send by a dealer socket" << std::endl;
			assert(bytes_transferred == 6);
		});

	// receive question
	array<char, 5> ident;
	array<char, 6> content;
	array<asio::mutable_buffer, 2> recv_quest_bufs{asio::buffer(ident), asio::buffer(content)};
	rout.async_receive(recv_quest_bufs,
		[&ident, &content, &rout](bs::error_code const & ec, size_t bytes_transferred) {
			// 5 for ident and 6 for content
			cout << bytes_transferred << " bytes received by a router socket" << std::endl;
			assert(bytes_transferred == 11);
			string_ref q{content.data()};
			assert(q == "name?");

			// answer client
			array<asio::const_buffer, 2> send_bufs{asio::buffer(ident), asio::buffer("Tereza")};
			rout.async_send(send_bufs,
				[](bs::error_code const & ec, size_t bytes_transferred) {
					cout << bytes_transferred << " bytes send by a router socket" << std::endl;
					assert(bytes_transferred == 12);  // 5 + 7
				});
		});

	// receive answer
	array<char, 7> recv_answer_buf;
	deal.async_receive(asio::buffer(recv_answer_buf),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes received by a dealer socket" << std::endl;
			assert(bytes_transferred == 7);
	});

	// notify server
	push.async_send(asio::buffer("John"),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes send by a push socket" << std::endl;
			assert(bytes_transferred == 5);
		});

	// receive notification
	array<char, 5> recv_notif_buf;
	pull.async_receive(asio::buffer(recv_notif_buf),
		[](bs::error_code const & ec, size_t bytes_transferred) {
			cout << bytes_transferred << " bytes received by a pull socket" << std::endl;
			assert(bytes_transferred == 5);
		});


	ios.run();

	cout << "done.\n";

	return 0;
}
