#pragma once
#include <string>
#include <chrono>
#include <future>
#include "websocket.hpp"

class echo_server : public websocket::server_channel {
protected:
	void on_message(std::string const & msg) override {
		send_all(msg);
	}
};


/*! Echo server loop helper meant to be run in a separate thread.

\code
bool server_quit = false;
std::thread server_thread{run_echo_server, port, path, std::ref(server_quit), timeout};
std::this_thread::sleep_for(milliseconds{100});  // wait for server

// do something there ...

server_quit = true;
server_thread.join();
\endcode */
void run_echo_server(int port, std::string path, bool & quit, std::chrono::milliseconds timeout);


/*! Client channel capable to receive one result loop helper meant to be run in a separate thread.

\code
std::promise<string> result_promise;
std::thread client_thread{run_client_channel, std::ref(result_promise), port,
	path, expected_result, timeout};

string result = result_promise.get_future().get();

client_thread.join();
\endcode */
void run_client_channel(std::promise<std::string> & result, int port, std::string path,
	std::string content, std::chrono::milliseconds timeout);
