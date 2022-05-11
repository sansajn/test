// WebSocket Secure (WSS) echo server sample
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>
#include <boost/filesystem/string_file.hpp>
#include <libsoup/soup.h>

using std::string, std::cout, std::cerr, std::endl;
using namespace std::chrono_literals;
using namespace std::string_literals;
//using boost::filesystem::load_string_file;

constexpr int SERVER_PORT = 8765;
constexpr char const WSS_PATH[] = "/",
	ssl_cert_file[] = "localhost.crt",
	ssl_key_file[] = "localhost.key";

//string page_source;

//void server_http_handler(SoupServer * server, SoupMessage * message,
//	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data);

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data);

void websocket_message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data);

int main(int argc, char * argv[]) {
//	load_string_file("wss_client.html", page_source);

	// load certificate
	GError * error = nullptr;
	GTlsCertificate * cert = g_tls_certificate_new_from_files(ssl_cert_file, ssl_key_file, &error);
	assert(cert);
	if (error) {
		cerr << "error: unable to load certificate, what:" << error->message << "\n";
		return 1;
	}

	SoupServer * server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "WebSocket Secure test server",
		SOUP_SERVER_TLS_CERTIFICATE, cert, nullptr);
	assert(server);

	g_clear_object(&cert);

	soup_server_add_websocket_handler(server, "/", nullptr, nullptr, server_websocket_handler, nullptr, nullptr);
	soup_server_listen_all(server, SERVER_PORT, (SoupServerListenOptions)SOUP_SERVER_LISTEN_HTTPS, nullptr);

	cout << "listenning on 'wss://127.0.0.1:" << SERVER_PORT << WSS_PATH << "' address ...\n";

	GMainLoop * loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	g_main_loop_run(loop);

	// clean up
	g_object_unref(G_OBJECT(server));
	g_main_loop_unref(loop);

	cout << "done!\n";

	return 0;
}

//void server_http_handler(SoupServer * server, SoupMessage * message,
//	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data)
//{
//	cout << "http request received" << endl;

//	SoupBuffer * page_buf = soup_buffer_new(SOUP_MEMORY_STATIC, page_source.c_str(), size(page_source));
//	assert(page_buf);

//	soup_message_headers_set_content_type(message->response_headers, "text/html", nullptr);
//	soup_message_body_append_buffer(message->response_body, page_buf);

//	soup_buffer_free(page_buf);

//	soup_message_set_status(message, SOUP_STATUS_OK);
//}

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data) {

	cout << "websocket connection request received" << endl;
	g_object_ref(G_OBJECT(connection));
	g_signal_connect(G_OBJECT(connection), "message", G_CALLBACK(websocket_message_handler), nullptr);
	// TODO: connect also closed signal
}

void websocket_message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data) {

	switch (data_type) {
		case SOUP_WEBSOCKET_DATA_TEXT: {
			// receive
			gsize size = 0;
			gchar const * data = static_cast<gchar const *>(g_bytes_get_data(message, &size));
			string s{data, size};
			cout << ">>> " << s << "\n";

			// replay
			soup_websocket_connection_send_text(connection, s.c_str());
			cout << "<<< " << s << endl;

			break;
		}

		default:
			cerr << "unsupported message type, message ignored" << endl;
	}
}
