// socket::async_connect with resolver sample
#include <iostream>
#include <boost/asio.hpp>
using std::cout, std::cerr, std::endl;
using std::begin, std::end;
using namespace boost::asio;

io_service ios;
ip::tcp::resolver resolv{ios};
ip::tcp::socket sock{ios};

void on_connect(boost::system::error_code ec, [[maybe_unused]] ip::tcp::resolver::iterator it) {
	if (ec) {
		cerr << "connection failed" << endl;
		return;
	}

	cout << "connected" << endl;
}

void on_resolve(boost::system::error_code ec, ip::tcp::resolver::results_type result) {
	if (ec) {
		cerr << "resolver failed" << endl;
		return;
	}

	async_connect(sock, begin(result), end(result), on_connect);
}


int main(int argc, char * argv[]) {
	resolv.async_resolve("127.0.0.1", "7235", on_resolve);
	ios.run();  // blocking
	cout << "done!\n";
	return 0;
}
