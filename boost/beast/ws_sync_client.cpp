// boost::beast synchronous websocket client sample
#include <string>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using std::string, std::to_string;
using std::cout, std::cerr, std::endl;
using std::exception;
using tcp = boost::asio::ip::tcp;

int main(int argc, char * argv[])
{
	try {
		if (argc != 4)
		{
			cerr << "Usage: websocket-client-sync <host> <port> <text>\n"
				<< "Example:\n"
				<< "    websocket-client-sync echo.websocket.org 80 \"Hello, world!\"\n";
			return 1;
		}

		 string host = argv[1];
		 auto const port = argv[2];
		 auto const text = argv[3];

		net::io_context ioc;

		tcp::resolver resolver{ioc};
		websocket::stream<tcp::socket> ws{ioc};

		auto const results = resolver.resolve(host, port);  // look up the domain name

		auto ep = net::connect(ws.next_layer(), results);  // make the connection on the IP address we get from a lookup

		// Update the host_ string. This will provide the value of the
		// Host HTTP header during the WebSocket handshake.
		// See https://tools.ietf.org/html/rfc7230#section-5.4
		host += ":" + to_string(ep.port());

		// Set a decorator to change the User-Agent ofthe handshake
		ws.set_option(websocket::stream_base::decorator(
			[](websocket::request_type & req){
				req.set(http::field::user_agent, string{BOOST_BEAST_VERSION_STRING} + " websocket-client-coro");
			}
		));

		// Perform the websocket handshake
		ws.handshake(host, "/");

		// Send the message
		ws.write(net::buffer(string{text}));

		// This buffer will hold the incoming message
		beast::flat_buffer buffer;

		// Read a message into our buffer
		ws.read(buffer);

		// Close the WebSocket connection
		ws.close(websocket::close_code::normal);

		// If we get there then the connection is closed grsacefully

		// The make_printable() function helps print a ConstBufferSequence
		cout << beast::make_printable(buffer.data()) << endl;
	}
	catch (exception const & e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	return 0;
}
