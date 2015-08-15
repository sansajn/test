#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using boost::system::error_code;

io_service service;

size_t read_complete(char * buf, error_code const & err, size_t bytes)
{
	if (err)
		return 0;
	bool found = std::find(buf, buf + bytes, '\n') < (buf + bytes);
	return found ? 0 : 1;
}

void handle_connections()
{
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
	char buff[1024];
	while (true)
	{
		ip::tcp::socket sock(service);
		acceptor.accept(sock);
		int bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
		std::string msg(buff, bytes);
		sock.write_some(buffer(msg));
		sock.close();
	}
}

int main(int argc, char * argv[])
{
	handle_connections();
	return 0;
}
