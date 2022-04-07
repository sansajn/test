#include <iostream>
#include <cassert>
#include "websocket.hpp"

using std::string, std::cout;

namespace websocket {

namespace detail {

void on_close(SoupWebsocketConnection * conn, gpointer data);

}  // detail

client_channel::client_channel()
	: _sess{nullptr}
	, _conn{nullptr}
{
	_sess = soup_session_new();
	assert(_sess);
}

client_channel::~client_channel() {
	close();
}

void client_channel::close() {
	if (_conn) {
		soup_websocket_connection_close(_conn, SOUP_WEBSOCKET_CLOSE_GOING_AWAY, "closing client channel");
		g_object_unref(G_OBJECT(_conn));
		_conn = nullptr;
	}

	if (_sess) {
		soup_session_abort(_sess);
		g_object_unref(G_OBJECT(_sess));
		_sess = nullptr;
	}
}

void client_channel::connect(string const & address, connected_handler && handler) {
	assert(_sess);

	_address = address;
	_connected_handler = handler;

	SoupMessage * msg = soup_message_new(SOUP_METHOD_GET, _address.c_str());
	soup_session_websocket_connect_async(_sess, msg, nullptr, nullptr, nullptr,
		(GAsyncReadyCallback)connection_handler_cb, this);

	g_object_unref(G_OBJECT(msg));
}

void client_channel::reconnect() {
	SoupMessage * msg = soup_message_new(SOUP_METHOD_GET, _address.c_str());
	soup_session_websocket_connect_async(_sess, msg, nullptr, nullptr, nullptr,
		(GAsyncReadyCallback)connection_handler_cb, this);

	g_object_unref(G_OBJECT(msg));
}

void client_channel::send(string const & msg) {
	assert(_conn);
	soup_websocket_connection_send_text(_conn, msg.c_str());
}

void client_channel::connection_handler(GAsyncResult * res) {
	GError * error = nullptr;
	_conn = soup_session_websocket_connect_finish(_sess, res, &error);

	if (error) {
		cout << "websocket: client connection failed, what: " << error->message << "\n";
		g_error_free(error);
		return;
	}
	assert(_conn);

	g_object_ref(G_OBJECT(_conn));

	// handle signals
	g_signal_connect(_conn, "message", G_CALLBACK(message_handler_cb), this);
	g_signal_connect(_conn, "closed", G_CALLBACK(detail::on_close), nullptr);

	_connected_handler(std::error_code{});
}

void client_channel::message_handler(SoupWebsocketDataType data_type, GBytes const * message) {
	switch (data_type) {
		case SOUP_WEBSOCKET_DATA_BINARY: {
			cout << "websocket: unknown binary message received, ignored\n";
			return;
		}

		case SOUP_WEBSOCKET_DATA_TEXT: {
			gsize size = 0;
			gchar * str = (gchar *)g_bytes_get_data((GBytes *)message, &size);
			assert(str);
			on_message(string{str, size});
			return;
		}

		default:
			assert(0);
	}
}

void client_channel::connection_handler_cb(SoupSession *, GAsyncResult * res,
	gpointer data) {

	client_channel * p = static_cast<client_channel *>(data);
	assert(p);
	p->connection_handler(res);
}

void client_channel::message_handler_cb(SoupWebsocketConnection *, SoupWebsocketDataType type,
	GBytes * message, gpointer data) {

	client_channel * p = static_cast<client_channel *>(data);
	assert(p);
	p->message_handler(type, message);
}


server_channel::server_channel()
	: _server{nullptr}
{}

server_channel::~server_channel() {
	// free connections
	for (SoupWebsocketConnection * client : _clients)
		g_object_unref(G_OBJECT(client));

	soup_server_disconnect(_server);
	soup_server_remove_handler(_server, "/echo");  // TODO: what is /echo?
	g_object_unref(G_OBJECT(_server));
}

bool server_channel::listen(int port, string const & path) {
	_server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "WebSocket client server", nullptr);
	if (!_server)
		return false;

	soup_server_add_websocket_handler(_server, path.c_str(), nullptr, nullptr, websocket_handler_cb, (gpointer)this, nullptr);
	return soup_server_listen_all(_server, port, (SoupServerListenOptions)0, nullptr) == TRUE;
}

void server_channel::send_all(string const & msg) {
	for (SoupWebsocketConnection * client : _clients)
		soup_websocket_connection_send_text(client, msg.c_str());
}

void server_channel::on_message(string const & msg) {}

void server_channel::message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes const * message) {

	switch (data_type) {
		case SOUP_WEBSOCKET_DATA_BINARY: {
			cout << "websocket: unknown binary message received, ignored\n";
			return;
		}

		case SOUP_WEBSOCKET_DATA_TEXT: {
			gsize size = 0;
			gchar * str = (gchar *)g_bytes_get_data((GBytes *)message, &size);
			assert(str);
			on_message(string{str, size});
			return;
		}

		default:
			assert(0);
	}
}

void server_channel::connection_handler(SoupWebsocketConnection * connection, char const * path,
	SoupClientContext * client) {

	assert(_clients.count(connection) == 0);  // check connection is always unique
	g_object_ref(G_OBJECT(connection));
	_clients.insert(connection);
}

void server_channel::closed_handler(SoupWebsocketConnection * connection) {
	auto it = _clients.find(connection);
	assert(it != end(_clients));  // we expect connection always there, otherwise logic error

	cout << "websocket: connection " << static_cast<void *>(*it) << " closed\n";

	g_object_unref(G_OBJECT(*it));
	_clients.erase(it);
}


void server_channel::websocket_handler_cb(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data) {

	cout << "websocket: connection " << static_cast<void *>(connection) << " request received\n";

	server_channel * channel = static_cast<server_channel *>(user_data);
	assert(channel);

	g_object_ref(G_OBJECT(connection));
	g_signal_connect(G_OBJECT(connection), "message",
		G_CALLBACK(websocket_message_handler_cb),	channel);

	// we don't want to increment connection reference counter there
	g_signal_connect(G_OBJECT(connection), "closed",
		G_CALLBACK(websocket_closed_handler_cb), channel);

	channel->connection_handler(connection, path, client);
}

void server_channel::websocket_closed_handler_cb(SoupWebsocketConnection * connection,
	gpointer user_data) {

	server_channel * channel = static_cast<server_channel *>(user_data);
	assert(channel);
	channel->closed_handler(connection);
}

void server_channel::websocket_message_handler_cb(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data)
{
	server_channel * channel = static_cast<server_channel *>(user_data);
	assert(channel);
	channel->message_handler(connection, data_type, message);
}


namespace detail {

void on_close(SoupWebsocketConnection * conn, gpointer) {
	soup_websocket_connection_close(conn, SOUP_WEBSOCKET_CLOSE_NORMAL, nullptr);
}

}  // detail

}  // websocket
