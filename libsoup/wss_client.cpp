/* WebSocket Secure client (libsoap) sample.
usage:* ./wss_client [CA_CERT_FILE=cacert.crt][WS_ADDRESS=wss://localhost:8765] */
#include <string_view>
#include <string>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cassert>
#include <libsoup/soup.h>

using std::string_view, std::string, std::to_string;
using std::cout, std::cerr, std::clog, std::endl;

constexpr char const DEFAULT_CERT_FILE[] = "cacert.crt",
	DEFAULT_ADDRESS[] = "wss://localhost:8765";

// signal handlers
void on_server_connected(SoupSession * session, GAsyncResult * res, SoupMessage * msg);
void on_server_message(SoupWebsocketConnection * conn, SoupWebsocketDataType type,
	GBytes * message, gpointer user_data);
void on_server_closed(SoupWebsocketConnection * conn, gpointer user_data);

void say_hello_every_1s();
gboolean say_hello();
void ask_quit();
void on_close(int signal);

// common variables
static SoupWebsocketConnection * ws_conn = nullptr;
static GMainLoop * loop = nullptr;

int main(int argc, char * argv[]) {
	signal(SIGINT, on_close);  // setup ctrl+c handler

	string_view cert_file = DEFAULT_CERT_FILE;
	if (argc > 1)
		cert_file = argv[1];

	string_view address = DEFAULT_ADDRESS;
	if (argc > 2)
		address = argv[2];

	cout << "certificate: " << cert_file << "\n"
		<< "connecting to '" << address << "' address\n";

	// init WebSocket connection
	char const * https_aliases[] = {"wss", nullptr};

	/* works also with pure WebSocket with ws://... address */
	SoupSession * session = soup_session_new_with_options(
		SOUP_SESSION_SSL_STRICT, TRUE,  // https://libsoup.org/libsoup-2.4/SoupSession.html#SoupSession--ssl-strict
//		SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,  // https://libsoup.org/libsoup-2.4/SoupSession.html#SoupSession--ssl-use-system-ca-file
		SOUP_SESSION_SSL_CA_FILE, cert_file.data(),  // .crt/.pem files are working there, https://libsoup.org/libsoup-2.4/SoupSession.html#SoupSession--ssl-ca-file
		SOUP_SESSION_HTTPS_ALIASES, https_aliases, // https://libsoup.org/libsoup-2.4/SoupSession.html#SoupSession--https-aliases
		nullptr);
	assert(session);

	SoupMessage * message = soup_message_new(SOUP_METHOD_GET, address.data());
	assert(message);

	soup_session_websocket_connect_async(session, message, NULL, NULL, NULL,
		(GAsyncReadyCallback)on_server_connected, message);

	// run glib loop
	loop = g_main_loop_new(nullptr, FALSE);
	g_main_loop_run(loop);  // blocking

	// cleanup
	g_main_loop_unref(loop);

	return 0;
}

void on_server_connected(SoupSession * session, GAsyncResult * res, SoupMessage * msg) {

	// check message
	//read "tls-errors" properties from msg   	SOUP_MESSAGE_TLS_ERRORS
	SoupMessageFlags const flags = soup_message_get_flags(msg);
	bool const is_trusted = flags|SOUP_MESSAGE_CERTIFICATE_TRUSTED,
		is_new_connection = flags|SOUP_MESSAGE_NEW_CONNECTION;

	clog << "message received with: "
		<< "htt-version=" << (soup_message_get_http_version(msg) == SOUP_HTTP_1_1 ? "1.1" : "1.0") << ", "
		<< "new-connection=" << (is_new_connection ? "yes" : "no") << ", "
		<< "certificate-trusted=" << (is_trusted ? "yes" : "no")	<< "\n";

	assert(is_new_connection);
	assert(is_trusted);

	GError * error = nullptr;
	ws_conn = soup_session_websocket_connect_finish(session, res, &error);
	if (error) {
		assert(!ws_conn);
		cerr << "error: " << error->message << "\n";
		ask_quit();
		return;
	}

	assert(ws_conn);
	assert(soup_websocket_connection_get_state(ws_conn) == SOUP_WEBSOCKET_STATE_OPEN);
	clog << "WebSocket connection opened" << endl;

	g_signal_connect(ws_conn, "closed", G_CALLBACK(on_server_closed), NULL);
	g_signal_connect(ws_conn, "message", G_CALLBACK(on_server_message), NULL);

	say_hello_every_1s();
}

void on_server_message(SoupWebsocketConnection * conn, SoupWebsocketDataType type,
	GBytes * message, gpointer user_data) {

	switch (type) {
		case SOUP_WEBSOCKET_DATA_TEXT: {
			gsize size = 0;
			gchar const * data = static_cast<gchar const *>(g_bytes_get_data(message, &size));
			clog << ">>> " << string_view{data, size} << endl;
			break;
		}

		default:
			cerr << "unsupported message type, message ignored" << endl;
	}
}

void on_server_closed(SoupWebsocketConnection * conn, gpointer user_data) {
	clog << "WebSocket connection closed\n"
		<< "quit" << endl;
	ask_quit();
}

void say_hello_every_1s() {
	g_timeout_add_seconds(1, (GSourceFunc)say_hello, nullptr);  // the function use global-default context (will not work with thread-default context)
}

gboolean say_hello() {
	static size_t counter = 1;
	string hello = "hello " + to_string(counter++) + "!";
	soup_websocket_connection_send_text(ws_conn, hello.c_str());
	clog << "<<< " << hello << endl;
	return TRUE;
}

void ask_quit() {
	if (ws_conn) {
		if (soup_websocket_connection_get_state(ws_conn) == SOUP_WEBSOCKET_STATE_OPEN) {
			soup_websocket_connection_close(ws_conn, 1000, "");  // this will call us again as part of on_server_closed handler
			return;
		}
		else
			g_clear_object(&ws_conn);
	}

	if (loop)
		g_main_loop_quit(loop);
}

void on_close(int signal) {
	ask_quit();
}
