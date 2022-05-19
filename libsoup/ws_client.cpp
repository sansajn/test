// WebSocket client sample
#include <string>
#include <iostream>
#include <cassert>
#include <libsoup/soup.h>

using std::string;
using std::cout, std::endl;

constexpr char DEFAULT_ADDRESS[] = "ws://echo.websocket.org:80";

void on_connection(SoupSession * session, GAsyncResult * res, gpointer data);
void on_message(SoupWebsocketConnection * conn, gint type, GBytes * message, gpointer data);
void on_close(SoupWebsocketConnection * conn, gpointer data);

GMainLoop * loop = nullptr;

int main(int argc, char * argv[]) {
	string address;
	if (argc > 1)
		address = argv[1];
	else
		address = DEFAULT_ADDRESS;
	
	// initialize client
	SoupSession * client = soup_session_new();

	// connect
	SoupMessage * addr_msg = soup_message_new(SOUP_METHOD_GET, address.c_str());
	soup_session_websocket_connect_async(client, addr_msg, nullptr, nullptr, nullptr,
		(GAsyncReadyCallback)on_connection, nullptr);

	loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	g_main_loop_run(loop);

	// clean up
	g_main_loop_unref(loop);

	cout << "done!\n";

	return 0;
}

void on_connection(SoupSession * session, GAsyncResult * res, gpointer data) {
	GError * error = nullptr;
	SoupWebsocketConnection * conn = soup_session_websocket_connect_finish(session, res, &error);
	if (error) {
		cout << "error: " << error->message << "\n";
		g_error_free(error);
		g_main_loop_quit(loop);
		return;
	}

	g_signal_connect(conn, "message", G_CALLBACK(on_message), nullptr);
	g_signal_connect(conn, "closed", G_CALLBACK(on_close), nullptr);

	cout << "on_connection" << endl;

	constexpr char msg[] = "Hello Jane!";
	soup_websocket_connection_send_text(conn, msg);
	cout << "<<< " << msg << "\n";
}

void on_message(SoupWebsocketConnection * conn, gint type, GBytes * message, gpointer data) {
	if (type == SOUP_WEBSOCKET_DATA_TEXT) {
		gsize sz;
		gchar const * ptr = (gchar const *)g_bytes_get_data(message, &sz);

		cout << ">>> " << ptr << "\n";
	}
	else if (type == SOUP_WEBSOCKET_DATA_BINARY)
		cout << "Received binary data (not shown)\n";
	else
		cout << "Invalid data type: " << type << "\n";
}

void on_close(SoupWebsocketConnection * conn, gpointer data) {
	assert(soup_websocket_connection_get_state(conn) == SOUP_WEBSOCKET_STATE_CLOSED);  // connection is expected to be closed there
	g_object_unref(conn);  // unref connection	
	cout << "WebSocket connection closed\n";
}
