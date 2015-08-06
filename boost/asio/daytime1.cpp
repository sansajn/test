// a synchronous TCP daytime client
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void exit(char const * msg, int err);

int main(int argc, char * argv[])
{
	if (argc != 2)
		exit("Usage: client <host>", 1);

	try {
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], "daytime");
		tcp::resolver::iterator endpoint_it = resolver.resolve(query);

		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_it);

		while (true)
		{
			boost::array<char, 128> buf;   // std::array ?
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break;  // connection closed by peer
			else if (error)
				throw boost::system::system_error(error);

			std::cout.write(buf.data(), len);
		}
	} 
	catch (std::exception & e) {
		exit(e.what(), 2);
	}

	return 0;
}

void exit(char const * msg, int err)
{
	std::cerr << msg << std::endl;
	exit(err);
}

