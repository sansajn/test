// HTTP GET (Debian 10) sample based on `http_client_async.cpp`
#include <memory>
#include <string_view>
#include <iostream>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

using std::string_view, std::string, std::to_string;
using std::begin, std::end, std::make_shared, std::cerr, std::cout, std::endl;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

void fail(boost::system::error_code ec, char const* what) {
	cerr << what << ": " << ec.message() << "\n";
}


class get_request_session : public std::enable_shared_from_this<get_request_session> {
public:
	explicit get_request_session(boost::asio::io_context & ioc);
	void run(string_view host, unsigned short port, string_view target);

private:
	void on_resolve(boost::system::error_code ec, tcp::resolver::results_type result);
	void on_connect(boost::system::error_code ec);
	void on_write(boost::system::error_code ec, std::size_t bytes_transferred);
	void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
	void shutdown();

	tcp::resolver _resolv;
	tcp::socket _sock;
	boost::beast::flat_buffer _buff;  // must persist between reads
	http::request<http::empty_body> _req;
	http::response<http::string_body> _res;
};


get_request_session::get_request_session(boost::asio::io_context & ioc)
	: _resolv{ioc}
	, _sock{ioc}
{}

void get_request_session::run(string_view host, unsigned short port, string_view target) {
	// build get reques
	_req.version(11);  // 1.1
	_req.method(http::verb::get);
	_req.target(string{target});
	_req.set(http::field::host, host);
	_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	string port_str = to_string(port);

	// look up the domain name
	_resolv.async_resolve(host, port_str,
		[self = shared_from_this()](boost::system::error_code ec, tcp::resolver::results_type result){
			self->on_resolve(ec, result);
		});

	// note: for std::bind version see http_client_async.cpp sample
}

void get_request_session::on_resolve(boost::system::error_code ec, tcp::resolver::results_type result) {
	if (ec)
		return fail(ec, "resolve");

	// connect
	boost::asio::async_connect(_sock, begin(result), end(result),
		[self = shared_from_this()](boost::system::error_code ec, [[maybe_unused]] boost::asio::ip::tcp::resolver::iterator it){
			self->on_connect(ec);
		});
}

void get_request_session::on_connect(boost::system::error_code ec) {
	if (ec)
		return fail(ec, "connect");

	// sending HTTP GET request
	http::async_write(_sock, _req,
		[self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred){
			self->on_write(ec, bytes_transferred);
		});
}

void get_request_session::on_write(boost::system::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
	if (ec)
		return fail(ec, "write");

	// receive the HTTP response
	http::async_read(_sock, _buff, _res,
		[self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred){
			self->on_read(ec, bytes_transferred);
		});
}

void get_request_session::on_read(boost::system::error_code ec, [[maybe_unused]] std::size_t bytes_transferred) {
	if (ec)
		return fail(ec, "read");

	cout << ">>> " << _res.body() << endl;  // dump response

	shutdown();
}

void get_request_session::shutdown() {
	boost::system::error_code ec;
	_sock.shutdown(tcp::socket::shutdown_both, ec);
	if (ec && ec != boost::system::errc::not_connected)  // not_connected happens sometimes so don't bother reporting it.
		return fail(ec, "shutdown");
}


int main(int argc, char * argv[]) {
	char const * host = "localhost",
		* target = "/get_device";

	unsigned short port = 8000;

	boost::asio::io_context ioc;

	cout << "<<< get(\\get_device)" << endl;
	make_shared<get_request_session>(ioc)->run(host, port, target);

	ioc.run();

	return 0;
}
