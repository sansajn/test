#include <array>
#include <string>
#include <thread>
#include <iostream>
#include <azmq/socket.hpp>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/utility/string_ref.hpp>

using std::array;
using std::string;
using std::cout;
using boost::string_ref;
namespace asio = boost::asio;
namespace bs = boost::system;

string translate_event(uint16_t e);

struct receive_event
{
	uint16_t & event;
	azmq::socket & mon;

	receive_event(uint16_t & event, azmq::socket & mon)
		: event{event}, mon{mon}
	{}

	void operator()(bs::error_code const & ec, azmq::socket::more_result_type result)
	{
		cout << translate_event(event) << std::endl;
		mon.flush();
		mon.async_receive_more(asio::buffer(&event, sizeof(event)), receive_event{event, mon});
	}
};

int main(int argc, char * argv[])
{
	asio::io_service ios;

	azmq::sub_socket sub{ios};
	sub.set_option(azmq::socket::subscribe(""));

	azmq::socket mon = sub.monitor(ios, ZMQ_EVENT_ALL);

	uint16_t event;
	mon.async_receive_more(asio::buffer(&event, sizeof(event)), receive_event{event, mon});

	sub.connect("tcp://localhost:5555");
	array<char, 256> recv_buf;
	sub.async_receive(asio::buffer(recv_buf),
		[&recv_buf](bs::error_code const & ec, size_t bytes_transferred) {
			cout << "received: " << string_ref{recv_buf.data(), bytes_transferred} << std::endl;
	});


	// create server
	azmq::pub_socket pub{ios};
	pub.bind("tcp://*:5555");

	std::this_thread::sleep_for(std::chrono::milliseconds{100});

	pub.send(asio::buffer("hello!"));

	ios.run();

	return 0;
}

string translate_event(uint16_t e)
{
	string result;

	if ((e & ZMQ_EVENT_CONNECTED) == ZMQ_EVENT_CONNECTED)
		result += "connection established, ";

	if ((e & ZMQ_EVENT_CONNECT_DELAYED) == ZMQ_EVENT_CONNECT_DELAYED)
		result += "connection delayed, ";

	if ((e & ZMQ_EVENT_CONNECT_RETRIED) == ZMQ_EVENT_CONNECT_RETRIED)
		result += "connection retired, ";

	if ((e & ZMQ_EVENT_LISTENING) == ZMQ_EVENT_LISTENING)
		result += "server listenning, ";

	if ((e & ZMQ_EVENT_BIND_FAILED) == ZMQ_EVENT_BIND_FAILED)
		result += "bind failed, ";

	if ((e & ZMQ_EVENT_ACCEPTED) == ZMQ_EVENT_ACCEPTED)
		result += "connection accepted, ";

	if ((e & ZMQ_EVENT_ACCEPT_FAILED) == ZMQ_EVENT_ACCEPT_FAILED)
		result += "accept failed, ";

	if ((e & ZMQ_EVENT_CLOSED) == ZMQ_EVENT_CLOSED)
		result += "connection closed, ";

	if ((e & ZMQ_EVENT_CLOSE_FAILED) == ZMQ_EVENT_CLOSE_FAILED)
		result += "close failed, ";

	if ((e & ZMQ_EVENT_DISCONNECTED) == ZMQ_EVENT_DISCONNECTED)
		result += "connection lost, ";

	if ((e & ZMQ_EVENT_MONITOR_STOPPED) == ZMQ_EVENT_MONITOR_STOPPED)
		result += "monitor stopped ";

	return result;
}
