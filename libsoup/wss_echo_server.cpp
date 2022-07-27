/* WebSocket Secure (WSS) echo server sample.
usage: wss_echo_server [PORT=8765][PATH="/"] */
#include <string_view>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <boost/filesystem/string_file.hpp>
#include <boost/program_options.hpp>
#include <libsoup/soup.h>

using std::string_view, std::string, std::cout, std::cerr, std::endl;
using namespace std::chrono_literals;
using namespace std::string_literals;
namespace po = boost::program_options;

constexpr int DEFAULT_SERVER_PORT = 8765;
constexpr char const DEFAULT_WSS_PATH[] = "/",
	DEFAULT_PEM[] = "localhost.pem";

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data);

void websocket_message_handler(SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data);

int main(int argc, char * argv[]) {
	// process command line options
	int arg_port;
	string arg_path,
		arg_pem;

	po::options_description desc{"Options"};
	desc.add_options()
		("help", "produce this help message")
		("port", po::value<int>(&arg_port)->default_value(DEFAULT_SERVER_PORT), "WebSocket server port")
		("path", po::value<string>(&arg_path)->default_value(DEFAULT_WSS_PATH), "WebSocket server path")
		("crt", po::value<string>(), "specifi certificate file as .crt file")
		("key", po::value<string>(), "specifi private key as .key file")
		("pem", po::value<string>(&arg_pem)->default_value(DEFAULT_PEM), "specifi certificate and private key in .pem container file");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << "Usage: wss_echo_server [OPTION]...\n"
			<< desc << "\n";
		return 1;
	}

	string cert_file = arg_pem;

	// in case crt provided, we expect also key
	string key_file;
	if (vm.count("crt") || vm.count("key")) {
		if (vm.count("crt") && vm.count("key")) {
			cert_file = vm["crt"].as<string>();
			key_file = vm["key"].as<string>();
		}
		else {
			cerr << "error: we expect both crt and key files, see --crt and --key options\n";
			return 1;
		}
	}

	cout << "certificate: ";
	if (!empty(key_file))
		cout << "(" << cert_file << ", " << key_file << ")\n";
	else
		cout << cert_file << "\n";


	// create WebSocket server

	// load certificate
	GError * error = nullptr;
	GTlsCertificate * cert = nullptr;
	if (!empty(key_file))
		cert = g_tls_certificate_new_from_files(cert_file.c_str(), key_file.c_str(), &error);
	else  // use pem
		cert = g_tls_certificate_new_from_file(cert_file.c_str(), &error);

	if (error) {
		cerr << "error: unable to load certificate, what:" << error->message << "\n";
		return 1;
	}
	assert(cert);

	SoupServer * server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "WebSocket Secure test server",
		SOUP_SERVER_TLS_CERTIFICATE, cert, nullptr);
	assert(server);

	g_clear_object(&cert);

	soup_server_add_websocket_handler(server, arg_path.c_str(), nullptr, nullptr, server_websocket_handler, nullptr, nullptr);
	soup_server_listen_all(server, arg_port, (SoupServerListenOptions)SOUP_SERVER_LISTEN_HTTPS, nullptr);

	cout << "listenning on 'wss://127.0.0.1:" << arg_port << arg_path << "' address ...\n";

	GMainLoop * loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	g_main_loop_run(loop);

	// clean up
	g_object_unref(G_OBJECT(server));
	g_main_loop_unref(loop);

	cout << "done!\n";

	return 0;
}

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
