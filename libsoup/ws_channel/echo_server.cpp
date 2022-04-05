#include "glib_event_loop.hpp"
#include "channel_receiver.hpp"
#include "echo_server.hpp"

using std::string;
using std::to_string;
using std::promise;
using std::chrono::milliseconds;

void run_echo_server(int port, string path, bool & quit, milliseconds timeout) {
	glib_event_loop loop;

	echo_server echo;
	echo.listen(port, path);

	loop.go_while([&quit]{return !quit;}, timeout);
}

void run_client_channel(promise<string> & result, int port, string path,
	string content, milliseconds timeout) {

	glib_event_loop loop;

	string const address = "ws://localhost:" + to_string(port) + path;

	channel_receiver_async client{result};
	client.connect(address, [&client, &content](std::error_code const & ec){
		client.send(content);
	});

	loop.go_while([&client]{return !client.received;}, timeout);
}
