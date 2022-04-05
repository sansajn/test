#pragma once
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <system_error>
#include <boost/noncopyable.hpp>
#include <libsoup/soup.h>

namespace websocket {

/*! WebSocket client channel implementation.
Implementation allows to connect to server WebSocket and send string messages.

\code
client_channel ch;
ch.connect("ws://localhost:4651/echo", [&ch](error_code const & ec) {
	ch.send("hello!");
});
\endcode */
class client_channel : private boost::noncopyable {
public:
	using connected_handler = std::function<void (std::error_code const & ec)>;

	client_channel();
	~client_channel();
	void connect(std::string const & address, connected_handler && handler);
	void reconnect();
	void send(std::string const & msg);
	void close();

protected:
	virtual void on_message(std::string const & msg) {}

private:
	void connection_handler(GAsyncResult * res);
	void message_handler(SoupWebsocketDataType data_type, GBytes const * message);

	// libsoup callback handlers
	static void connection_handler_cb(SoupSession * session, GAsyncResult * res, gpointer data);
	static void message_handler_cb(SoupWebsocketConnection * conn, SoupWebsocketDataType type, GBytes * message, gpointer data);

	SoupSession * _sess;
	SoupWebsocketConnection * _conn;
	std::string _address;
	connected_handler _connected_handler;
};

//! WebSocket server channel implementation for communication with a group of connected clients.
class server_channel : private boost::noncopyable {
public:
	server_channel();
	~server_channel();
	bool listen(int port, std::string const & path);
	void send_all(std::string const & msg);

protected:
	virtual void on_message(std::string const & msg);

private:
	void message_handler(SoupWebsocketConnection * connection,
		SoupWebsocketDataType data_type, GBytes const * message);

	void connection_handler(SoupWebsocketConnection * connection, char const * path,
		SoupClientContext * client);

	void closed_handler(SoupWebsocketConnection * connection);

	// libsoup handlers
	static void websocket_handler_cb(SoupServer * server, SoupWebsocketConnection * connection,
		char const * path, SoupClientContext * client, gpointer user_data);

	static void websocket_closed_handler_cb(SoupWebsocketConnection * connection,
		gpointer user_data);

	static void websocket_message_handler_cb(SoupWebsocketConnection * connection,
		SoupWebsocketDataType data_type, GBytes * message, gpointer user_data);

	SoupServer * _server;
	std::set<SoupWebsocketConnection *> _clients;
};

}  // websocket
