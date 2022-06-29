// socket::async_connect sample
#include <iostream>
#include <boost/asio.hpp>
using std::cout, std::cerr, std::endl;
using namespace boost::asio;

int main(int argc, char * argv[]) {
	io_service ios;
	ip::tcp::endpoint ep{ip::address::from_string("127.0.0.1"), 7235};
	ip::tcp::socket sock{ios};
	sock.async_connect(ep, [](boost::system::error_code ec){
		if (ec) {
			cerr << "connection failed" << endl;
			return;
		}

		cout << "connected" << endl;
	});

	ios.run();  // blocking

	cout << "done!\n";
	return 0;
}
