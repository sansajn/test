/* Single request WebSocket client sample (based on `ws_client.cpp` sample).
Usage: ask [ADDRESS=ws://localhost:40001/ws] */
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <csignal>
#include <libsoup/soup.h>

using std::string;
using std::cout, std::endl, std::ostringstream;

constexpr char DEFAULT_ADDRESS[] = "ws://localhost:40001/ws";

void on_connection(SoupSession * session, GAsyncResult * res, gpointer data);
void on_message(SoupWebsocketConnection * conn, gint type, GBytes * message, gpointer data);
void on_closed(SoupWebsocketConnection * conn, gpointer data);
void on_ctrlc(int signal);

GMainLoop * loop = nullptr;
SoupWebsocketConnection * open_conn = nullptr;

int main(int argc, char * argv[]) {
	signal(SIGINT, on_ctrlc);  // setup ctrl+c handler

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

	g_main_loop_run(loop);  // blocking

	// clean up
	g_object_unref(client);
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
	g_signal_connect(conn, "closed", G_CALLBACK(on_closed), nullptr);
	open_conn = conn;

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

void on_closed(SoupWebsocketConnection * conn, gpointer data) {
	assert(conn == open_conn);
	assert(soup_websocket_connection_get_state(conn) == SOUP_WEBSOCKET_STATE_CLOSED);  // connection is expected to be closed there

	// inform connection closed
	ostringstream sout;
	gushort close_code = soup_websocket_connection_get_close_code(conn);
	sout << "WebSocket connection (" << static_cast<void *>(conn) << ") closed with code=" << close_code;

	char const * what = soup_websocket_connection_get_close_data(conn);
	if (what)
		sout << ", what: " << what;

	sout << '\n';

	cout << sout.str();

	g_object_unref(conn);  // unref connection
	g_main_loop_quit(loop);  // client closed we can quit
}

void on_ctrlc(int signal) {
	cout << "ctrl+c press detected, closing client connection ...\n";
	
	/* We can close our connection with just calling unref 
	on connection object, but in this case closed handler is not called.

	g_clear_object(&open_conn);
	static int count = 1;
	if (count++ > 1)
		g_main_loop_quit(loop);
	*/

	/* or we can call close funcition and provide reason for closing with (in this case closed handler is called) */
	soup_websocket_connection_close(open_conn, SOUP_WEBSOCKET_CLOSE_GOING_AWAY, "client closed");  // after this on_closed handler is called by glib loop
}
