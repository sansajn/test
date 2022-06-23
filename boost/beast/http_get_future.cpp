// HTTP GET sample (based on `http_client_async.cpp`) with promise/future API
#include <memory>
#include <string_view>
#include <future>
#include <thread>
#include <iostream>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

using std::promise, std::future, std::thread, std::async;
using std::string_view, std::string, std::to_string;
using std::begin, std::end, std::make_shared, std::shared_ptr;
using std::cerr, std::cout, std::endl;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

void fail(boost::system::error_code ec, char const* what) {
	cerr << what << ": " << ec.message() << "\n";
}


class get_request_session : public std::enable_shared_from_this<get_request_session> {
public:
	explicit get_request_session(boost::asio::io_context & ioc);
	std::future<std::string> run(string_view host, unsigned short port, string_view target);

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
	std::promise<std::string> _result;  // TODO: can we use string_view there (pointing to _req)?
};


get_request_session::get_request_session(boost::asio::io_context & ioc)
	: _resolv{ioc}
	, _sock{ioc}
{}

future<string> get_request_session::run(string_view host, unsigned short port,
	string_view target) {

	// build get reques
	_req.version(11);  // 1.1
	_req.method(http::verb::get);
	_req.target(target);
	_req.set(http::field::host, host);
	_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	string port_str = to_string(port);

	// look up the domain name
	_resolv.async_resolve(host, port_str,
		[self = shared_from_this()](boost::system::error_code ec, tcp::resolver::results_type result){
			self->on_resolve(ec, result);
		});

	// note: for std::bind version see http_client_async.cpp sample

	return _result.get_future();
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
	_result.set_value(_res.body());

	shutdown();
}

void get_request_session::shutdown() {
	boost::system::error_code ec;
	_sock.shutdown(tcp::socket::shutdown_both, ec);
	if (ec && ec != boost::system::errc::not_connected)  // not_connected happens sometimes so don't bother reporting it.
		return fail(ec, "shutdown");
}


void test_with_thread(string_view host, uint16_t port, string_view target) {
	boost::asio::io_context ioc;

	cout << "<<< get(\\get_device)" << endl;
	auto request = make_shared<get_request_session>(ioc);
	auto result = request->run(host, port, target);

	thread request_thread{[&]{  // execute request in a separate thread, non blocking
		ioc.run();
	}};

	result.wait(); // wait for result, blocking

	assert(result.valid());
	string device = result.get();

	cout << "device-id: " << device << "\n";

	assert(request_thread.joinable());
	request_thread.join();  // wait for thread to finish
}

void test_with_async(string_view host, uint16_t port, string_view target) {
	boost::asio::io_context ioc;

	cout << "<<< get(\\get_device)" << endl;
	auto request = make_shared<get_request_session>(ioc);

	auto result = async([&request, &ioc, host, port, target]{
		auto result = request->run(host, port, target);
		ioc.run();
		assert(result.valid());
		return result.get();
	});

	result.wait();  // wait for result, blocking

	assert(result.valid());
	string device = result.get();

	cout << "device-id: " << device << "\n";
}

int main(int argc, char * argv[]) {
	char const * host = "localhost",
		* target = "/get_device";

	unsigned short port = 8000;

	cout << "get request with thread ...\n";
	test_with_thread(host, port, target);

	cout << "\n";

	cout << "get request with async ...\n";
	test_with_async(host, port, target);

	cout << "\n"
		<< "done\n";

	return 0;
}
