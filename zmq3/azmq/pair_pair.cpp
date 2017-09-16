// pair pair socket sample (sync)
#include <array>
#include <string>
#include <iostream>
#include <boost/utility/string_ref.hpp>
#include <azmq/socket.hpp>

using std::array;
using std::string;
using std::cout;
using boost::string_ref;

namespace asio = boost::asio;

int main(int argc, char * argv[])
{
	asio::io_service ios;

	azmq::socket serv{ios, ZMQ_PAIR};
	serv.bind("inproc://pair_pair");

	azmq::socket client{ios, ZMQ_PAIR};
	client.connect("inproc://pair_pair");

	char const msg[] = "TEST";
	size_t transferred_byes1 = client.send(asio::const_buffer{msg, 5});

	array<char, 256> buf;
	size_t transferred_bytes2 = serv.receive(asio::buffer(buf));

	cout << "transferred_bytes1:" << transferred_byes1
		<< "\ntransferred_bytes2:" << transferred_bytes2
		<< "\nreceived:" << string_ref{buf.data()} << "\n";

	cout << "done.\n";

	return 0;
}
