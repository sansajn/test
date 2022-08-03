/*! Ask and answer samples in one multi thread sample.
\sa ask.cpp answer.cpp
\note In this case closed signal is not received by client (in ask/answer sample it is). */
#include <string_view>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <cassert>
#include <boost/filesystem/string_file.hpp>
#include <libsoup/soup.h>
#include "loop.hpp"

using std::string_view, std::string;
using std::to_string, std::cout, std::endl, std::ostringstream;
using std::thread;
using namespace std::chrono_literals;
using namespace std::string_literals;
using boost::filesystem::load_string_file;

constexpr int default_port = 40001;
constexpr char const * default_path = "/ws";

void inform_connection_closed(string_view prefix, SoupWebsocketConnection * connection);

namespace server {

string page_source;
event_loop * loop = nullptr;
SoupServer * serv = nullptr;
SoupWebsocketConnection * client_conn = nullptr;


void go();
void quit_server();

void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data);

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data);

void websocket_message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data);

void websocket_closed_handler(SoupWebsocketConnection * connection, gpointer user_data);

}  // server

namespace client {

event_loop * loop = nullptr;
SoupWebsocketConnection * open_conn = nullptr;

void go();

void on_connection(SoupSession * session, GAsyncResult * res, gpointer data);
void on_message(SoupWebsocketConnection * conn, gint type, GBytes * message, gpointer data);
void on_closed(SoupWebsocketConnection * conn, gpointer data);

}  // client

int main(int asrgc, char * argv[]) {

	thread serv{server::go},
		cli{client::go};

	assert(serv.joinable());
	serv.join();
	cli.join();  // wait for client

	cout << "done!\n";

	return 0;
}


void inform_connection_closed(SoupWebsocketConnection * connection, string_view prefix = {}) {
	ostringstream sout;
	gushort close_code = soup_websocket_connection_get_close_code(connection);
	sout << prefix << "WebSocket connection (" << static_cast<void *>(connection) << ") closed with code=" << close_code;

	char const * what = soup_websocket_connection_get_close_data(connection);
	if (what)
		sout << ", what: " << what;

	sout << '\n';

	cout << sout.str();
}


namespace server {

void go() {
	load_string_file("websocket.html", page_source);

	loop = new event_loop{};

	serv = soup_server_new(SOUP_SERVER_SERVER_HEADER, "test soap server", nullptr);
	assert(serv);

	// we want to serve HTML page with WS handlers
	soup_server_add_handler(serv, "/", server_http_handler, nullptr, nullptr);

	soup_server_add_websocket_handler(serv, default_path, nullptr, nullptr, server_websocket_handler, nullptr, nullptr);
	soup_server_listen_all(serv, default_port, (SoupServerListenOptions)0, nullptr);

	cout << "page link: 'http://localhost:" << default_port << "'\n"
		<< "WebSocket listenning on 'ws://localhost:" << default_port << default_path << "' address\n";

	loop->go();  // blocking

	// clean up
	assert(!serv);  // should be released

	delete loop;
	loop = nullptr;

	cout << "server done!\n";
}

void quit_server() {
	// close client connection and inform client what happened
	assert(client_conn);
	soup_websocket_connection_close(client_conn, SOUP_WEBSOCKET_CLOSE_GOING_AWAY, "server closed");

	soup_server_disconnect(serv);  // NOTE: this will not close server connections

	g_object_unref(G_OBJECT(serv));
	serv = nullptr;

	// NOTE: connections are unrefered in closed handler
}

void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data) {

	cout << "http request received" << endl;

	SoupBuffer * page_buf = soup_buffer_new(SOUP_MEMORY_STATIC, page_source.c_str(), size(page_source));
	assert(page_buf);

	soup_message_headers_set_content_type(message->response_headers, "text/html", nullptr);
	soup_message_body_append_buffer(message->response_body, page_buf);

	soup_buffer_free(page_buf);

	soup_message_set_status(message, SOUP_STATUS_OK);
}

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data) {

	cout << "websocket connection " << static_cast<void *>(connection) << " request received" << endl;
	assert(!client_conn);
	client_conn = connection;
	g_object_ref(G_OBJECT(connection));
	g_signal_connect(G_OBJECT(connection), "message", G_CALLBACK(websocket_message_handler), nullptr);
	g_signal_connect(G_OBJECT(connection), "closed",  G_CALLBACK(websocket_closed_handler), nullptr);
}

void websocket_message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data) {

	switch (data_type) {
		case SOUP_WEBSOCKET_DATA_BINARY: {
			cout << "websocket: unknown binary message received, ignored" << endl;
			return;
		}

		case SOUP_WEBSOCKET_DATA_TEXT: {
			// receive
			gsize size;
			gchar * data = (gchar *)g_bytes_get_data(message, &size);  // null terminated string
			assert(data);
			cout << "client >> " << data << "\n";

			// answer
			soup_websocket_connection_send_text(connection, data);
			cout << "client << " << data << "\n";

			quit_server();  // answered, close server and quit
			return;
		}

		default:
			assert(0);
	}
}

void websocket_closed_handler(SoupWebsocketConnection * connection, gpointer user_data) {
	assert(connection == client_conn);
	assert(soup_websocket_connection_get_state(connection) == SOUP_WEBSOCKET_STATE_CLOSED);

	inform_connection_closed(connection, "server: ");

	// we want to unref connection there, otherwise client is not informed connection was closed
	g_object_unref(connection);
	client_conn = nullptr;

	if (!client_conn)  // in case all connections are closed, we can quit loop
		loop->quit();
}

}  // server

namespace client {

void go() {
	loop = new event_loop;

	// initialize client
	SoupSession * client = soup_session_new();

	// connect
	string const address = "ws://localhost:" + to_string(default_port) + default_path;
	SoupMessage * addr_msg = soup_message_new(SOUP_METHOD_GET, address.c_str());
	soup_session_websocket_connect_async(client, addr_msg, nullptr, nullptr, nullptr,
		(GAsyncReadyCallback)on_connection, nullptr);

	loop->go();

	// clean up
	g_object_unref(client);

	delete loop;
	loop = nullptr;

	cout << "client done!\n";
}

void on_connection(SoupSession * session, GAsyncResult * res, gpointer data) {
	GError * error = nullptr;
	SoupWebsocketConnection * conn = soup_session_websocket_connect_finish(session, res, &error);
	if (error) {
		cout << "error: " << error->message << "\n";
		g_error_free(error);
		loop->quit();
		return;
	}

	g_signal_connect(conn, "message", G_CALLBACK(on_message), nullptr);
	g_signal_connect(conn, "closed", G_CALLBACK(on_closed), nullptr);
	open_conn = conn;

	cout << "on_connection" << endl;

	constexpr char msg[] = "Hello Jane!";
	soup_websocket_connection_send_text(conn, msg);
	cout << "server << " << msg << "\n";
}

void on_message(SoupWebsocketConnection * conn, gint type, GBytes * message, gpointer data) {
	if (type == SOUP_WEBSOCKET_DATA_TEXT) {
		gsize sz;
		gchar const * ptr = (gchar const *)g_bytes_get_data(message, &sz);

		cout << "server >> " << ptr << "\n";
	}
	else if (type == SOUP_WEBSOCKET_DATA_BINARY)
		cout << "Received binary data (not shown)\n";
	else
		cout << "Invalid data type: " << type << "\n";
}

void on_closed(SoupWebsocketConnection * connection, gpointer data) {
	assert(connection == open_conn);
	assert(soup_websocket_connection_get_state(connection) == SOUP_WEBSOCKET_STATE_CLOSED);  // connection is expected to be closed there

	inform_connection_closed(connection, "client: ");

	// clean-up
	g_object_unref(connection);  // unref connection
	loop->quit();  // client closed we can quit loop
}

}  // client
