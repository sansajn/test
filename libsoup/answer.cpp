/*! Single response WebSocket server sample (based on `ws_echo_server.cpp` sample).
Usage: answer */
#include <string_view>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>
#include <boost/filesystem/string_file.hpp>
#include <libsoup/soup.h>

using std::string_view, std::string, std::cout, std::endl;
using namespace std::chrono_literals;
using namespace std::string_literals;
using boost::filesystem::load_string_file;

constexpr int SERVER_PORT = 40001;

string page_source;
GMainLoop * loop = nullptr;
SoupServer * server = nullptr;
SoupWebsocketConnection * client_conn = nullptr;

void quit_server();

void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data);

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data);

void websocket_message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data);

void websocket_closed_handler(SoupWebsocketConnection * connection, gpointer user_data);


int main(int argc, char * argv[]) {
	load_string_file("websocket.html", page_source);

	server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "test soap server", nullptr);
	assert(server);

	// we want to serve HTML page with WS handlers
	soup_server_add_handler(server, "/", server_http_handler, nullptr, nullptr);

	soup_server_add_websocket_handler(server, "/ws", nullptr, nullptr, server_websocket_handler, nullptr, nullptr);
	soup_server_listen_all(server, SERVER_PORT, (SoupServerListenOptions)0, nullptr);

	cout << "page link: http://127.0.0.1:" << SERVER_PORT << "\n"
		<< "WebSocket listenning on ws://localhost:" << SERVER_PORT << "/ws address\n";

	loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	g_main_loop_run(loop);  // blocking

	// clean up
	g_main_loop_unref(loop);

	cout << "done!\n";
	return 0;
}

void quit_server() {
	// close client connection and inform client what happened
	assert(client_conn);
	soup_websocket_connection_close(client_conn, SOUP_WEBSOCKET_CLOSE_GOING_AWAY, "server closed");

	soup_server_disconnect(server);  // NOTE: this will not close server connections

	g_object_unref(G_OBJECT(server));
	server = nullptr;

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
	g_object_ref(G_OBJECT(connection));
	client_conn = connection;
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
			cout << ">>> " << data << "\n";
			
			// answer
			soup_websocket_connection_send_text(connection, data);
			cout << "<<< " << data << "\n";

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
	
	// inform client connection was closed
	gushort close_code = soup_websocket_connection_get_close_code(connection);
	char const * what = soup_websocket_connection_get_close_data(connection);

	cout << "connection " << static_cast<void *>(connection) << " closed with code=" << close_code;
	if (what)
		cout << ", what: " << what;
	cout << "\n";
	
	// we want to unref connection there, otherwise client is not informed connection was closed
	g_object_unref(connection);
	client_conn = nullptr;

	if (!client_conn)  // in case all connections are closed, we can quit loop
		g_main_loop_quit(loop);
}
