/* HTTP server sample responding to HTTP GET requests by JSON object string, based on http_server_async.cpp Beast sample.
usage: http_responder [PORT=57777] */
#include <utility>
#include <memory>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

using std::move, std::make_shared, std::shared_ptr;
using std::cerr, std::cout, std::endl;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

void fail(beast::error_code ec, char const* what) {
	std::cerr << what << ": " << ec.message() << "\n";
}

//! Handles an HTTP server connection.
class session : public std::enable_shared_from_this<session> {
public:
	explicit session(tcp::socket && socket)
		: _stream{move(socket)}
	{}

	void run() {do_read();}

private:
	void do_read() {
		_stream.expires_after(std::chrono::seconds(30));  // Set the timeout.

		http::async_read(_stream, _buf, _req,  // read a request
			beast::bind_front_handler(&session::on_read, shared_from_this()));
	}

	void on_read(beast::error_code ec, [[maybe_unused]] size_t bytes_transferred) {
		if (ec == http::error::end_of_stream)  // This means they closed the connection
			return do_close();

		if (ec)
			return fail(ec, "read");

		handle_request();
	}

	void handle_request() {
		assert(_req.method() == http::verb::get);

		_res = http::response<http::string_body>{http::status::ok, _req.version()};
		_res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		_res.set(http::field::content_type, "text/html");
		_res.keep_alive(_req.keep_alive());
		_res.body() = "{\"response\":\"hello\"}";
		_res.prepare_payload();

		http::async_write(_stream, _res,  // write the response
			beast::bind_front_handler(&session::on_write, shared_from_this(), _res.need_eof()));
	}

	void on_write(bool close, beast::error_code ec, [[maybe_unused]] size_t bytes_transferred) {
		if (ec)
			return fail(ec, "write");

		if (close) {
			// This means we should close the connection, usually because
			// the response indicated the "Connection: close" semantic.
			return do_close();
		}

		// We're done with the response so delete it so ita can be reused by another request.

		do_read();  // read another request
	}

	void do_close() {
		// Send a TCP shutdown
		beast::error_code ec;
		_stream.socket().shutdown(tcp::socket::shutdown_send, ec);

		// At this point the connection is closed gracefully
	}

	beast::tcp_stream _stream;
	beast::flat_buffer _buf;
	http::request<http::string_body> _req;
	http::response<http::string_body> _res;
};


//! Accepts incoming connections and launches the sessions.
class listener : public std::enable_shared_from_this<listener> {
public:
	listener(net::io_context & ioc, tcp::endpoint endpoint)
		: _ioc{ioc}
		, _acceptor{net::make_strand(ioc)} {  // NOTE: strand for guarantee that none of those handlers will execute concurrently

		beast::error_code ec;
		_acceptor.open(endpoint.protocol(), ec);
		if (ec) {
			fail(ec, "open");
			return;
		}

		_acceptor.set_option(net::socket_base::reuse_address{true}, ec);  // allow address reuse
		if (ec) {
			fail(ec, "set_option");
			return;
		}

		_acceptor.bind(endpoint, ec);  // bind to the server address
		if (ec) {
			fail(ec, "bind");
			return;
		}

		_acceptor.listen(net::socket_base::max_listen_connections, ec);
		if (ec) {
			fail(ec, "listen");
			return;
		}
	}

	void run() {do_accept();}

private:
	void do_accept() {
		_acceptor.async_accept(  // the new connection gets its own strand
			net::make_strand(_ioc),
			beast::bind_front_handler(&listener::on_accept, shared_from_this()));
	}

	void on_accept(beast::error_code ec, tcp::socket socket) {
		if (ec)
			fail(ec, "accept");
		else {
			make_shared<session>(move(socket))->run();
		}

		do_accept();  // accept another connection
	}

	net::io_context & _ioc;
	tcp::acceptor _acceptor;
};


int main(int argc, char * argv[]) {
	unsigned short port = 57777;
	if (argc > 1)
		port = static_cast<unsigned short>(atoi(argv[1]));

	boost::asio::io_context ioc;

	net::ip::address address = net::ip::make_address("127.0.0.1");

	make_shared<listener>(ioc, tcp::endpoint{address, port})
		->run();

	cout << "server listening on 127.0.0.1:" << port << " address ..." << endl;

	ioc.run();

	return 0;
}
