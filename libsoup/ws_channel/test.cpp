#include <vector>
#include <string>
#include <chrono>
#include <future>
#include <functional>
#include <iostream>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "jthread.hpp"
#include "glib_event_loop.hpp"
#include "echo_server.hpp"
#include "channel_receiver.hpp"

using namespace std::chrono_literals;

using std::vector, std::string, std::to_string;
using std::chrono::seconds, std::chrono::milliseconds;
using std::promise, std::future_status;
using std::ref, std::cout;


namespace {

constexpr size_t PORT = 41001;
constexpr char const * PATH = "/test";

}  // namespace

TEST_CASE("we can run echo server",
	"[echo_server]") {
	// SETUP
	string const expected_result = "hello!";

	// run echo server
	seconds const timeout = 3s;
	bool server_quit = false;
	jthread server_thread{run_echo_server, PORT, PATH, ref(server_quit), timeout};
	std::this_thread::sleep_for(milliseconds{100});  // wait for server

	// run test client
	promise<string> result_promise;
	jthread client_thread{run_client_channel, ref(result_promise), PORT,
		PATH, expected_result, timeout};

	// CHECK
	auto result_future = result_promise.get_future();
	REQUIRE(result_future.wait_for(timeout) == future_status::ready);
	string result = result_future.get();
	REQUIRE(result == expected_result);

	// CLEAN-UP
	client_thread.join();
	server_quit = true;
	server_thread.join();
}

TEST_CASE("we can send and receive messages via WebSocket channel",
	"[websocket][channel_receiver_async]") {
	// SETUP
	string const expected_result = "hello c9a3cf13e!";

	// run echo server
	constexpr seconds timeout = 3s;
	bool server_quit = false;
	jthread server_thread{run_echo_server, PORT, "/echo", ref(server_quit), timeout};
	std::this_thread::sleep_for(milliseconds{100});  // wait for server

	// run client
	glib_event_loop loop;

	promise<string> result_promise;
	string const addr = "ws://localhost:" + to_string(PORT) + "/echo";
	channel_receiver_async client{result_promise};
	client.connect(addr, [&client, expected_result](std::error_code const & ec) {
		client.send(expected_result);
	});

	loop.go_while([&client]{return !client.received;}, timeout);

	// CHECK
	auto result_future = result_promise.get_future();
	REQUIRE(result_future.wait_for(timeout) == future_status::ready);
	string result = result_future.get();
	REQUIRE(result == expected_result);

	// CLEAN-UP
	server_quit = true;
	server_thread.join();
}

TEST_CASE("we can send and receive multiple messages via WebSocket channel",
	"[websocket][channel_receiver_multi_async]") {
	// SETUP
	vector<string> expected_messages = {"1", "2", "3", "4", "5"};

	// run echo server
	constexpr seconds timeout = 3s;
	bool server_quit = false;
	jthread server_thread{run_echo_server, PORT, "/echo", std::ref(server_quit), timeout};
	std::this_thread::sleep_for(milliseconds{100});  // wait for server

	// run client
	glib_event_loop loop;

	channel_receiver_multi_async::promise_type result_promise;
	string const addr = "ws://localhost:" + to_string(PORT) + "/echo";
	channel_receiver_multi_async client{size(expected_messages), result_promise};
	client.connect(addr, [&client, expected_messages](std::error_code const & ec) {
		for_each(begin(expected_messages), end(expected_messages), [&client](string const & msg){
			client.send(msg);
		});
	});

	loop.go_while([&client]{return !client.received;}, timeout);

	// CHECK
	auto result_future = result_promise.get_future();
	REQUIRE(result_future.wait_for(timeout) == future_status::ready);
	vector<string> result = result_future.get();
	REQUIRE(result == expected_messages);

	// CLEAN-UP
	server_quit = true;
	server_thread.join();
}
