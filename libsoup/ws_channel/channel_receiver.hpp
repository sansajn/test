/*! \file
WebSocket channel support */
#pragma once
#include <string>
#include <chrono>
#include <future>
#include "websocket.hpp"

//! Sync WebSocket channel receiver implementation.
struct channel_receiver_sync : public websocket::client_channel {
	void on_message(std::string const & msg) override {
		result = msg;
	}

	std::string result;
};

/*! Async WebSocket channel receiver implementation (for single message).
/code
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

int main() {
	// run test client
	promise<string> result_promise;
	thread client_thread{run_client_channel, std::ref(result_promise), PORT,
		PATH, expected_result, timeout};

	string result = result_promise.get_future().get();
	REQUIRE(result == expected_result);

	// ...
}
/endcode
\see echo_server */
struct channel_receiver_async : public websocket::client_channel {
	using promise_type = std::promise<std::string>;

	explicit channel_receiver_async(promise_type & result)
		: _result{result}
	{}

	bool received = false;

private:
	void on_message(std::string const & msg) override {
		websocket::client_channel::on_message(msg);
		received = true;
		_result.set_value(msg);
	}

	promise_type & _result;
};


/*! Async WebSocket channel receiver implementation (for multiple messages).
\see channel_receiver_async */
struct channel_receiver_multi_async : public websocket::client_channel {
	using result_type = std::vector<std::string>;
	using promise_type = std::promise<result_type>;

	channel_receiver_multi_async(size_t expected_message_count, promise_type & result)
		: _result{result}
		, _expected_message_count{expected_message_count}
	{
		_received.reserve(_expected_message_count);
	}

	bool received = false;

private:
	void on_message(std::string const & msg) override {
		websocket::client_channel::on_message(msg);
		_received.push_back(msg);
		if (size(_received) == _expected_message_count) {
			_result.set_value(move(_received));
			received = true;
		}
	}

	promise_type & _result;
	size_t const _expected_message_count;
	result_type _received;
};

