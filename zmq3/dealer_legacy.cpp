// router pattern sample with monitored ZMQ events (for legacy ZMQ 4.14 and C++11 running on debian8), see `dealer_mon.cpp` sample
#include <string>
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <zmq.h>

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::chrono::steady_clock;
using std::chrono::seconds;
using std::chrono::milliseconds;

string const DEFAULT_DEALER_ADDRESS = "localhost:5557";
constexpr char const * MONITOR_ADDR = "inproc://dea_monitor";

static string zmq_event_to_string(int event);

int main(int argc, char * argv[])
{
	string address;
	if (argc > 1)
		address = "tcp://" + string{argv[1]};
	else
		address = "tcp://" + DEFAULT_DEALER_ADDRESS;

	// dealer
	void * ctx = zmq_ctx_new();
	void * dea = zmq_socket(ctx, ZMQ_DEALER);
	int rc = zmq_connect(dea, address.c_str());
	assert(!rc);

	// create dealer monitor
	rc = zmq_socket_monitor(dea, MONITOR_ADDR, ZMQ_EVENT_ALL);
	assert(rc >= 0);

	// connect to monitor
	void * dea_mon = zmq_socket(ctx, ZMQ_PAIR);
	assert(dea_mon);
	zmq_connect(dea_mon, MONITOR_ADDR);

	std::this_thread::sleep_for(milliseconds{100});  // wait for zmq to connect

	zmq_pollitem_t items[] = {
		{dea, 0, ZMQ_POLLIN|ZMQ_POLLOUT, 0},
		{dea_mon, 0, ZMQ_POLLIN, 0}
	};

	auto t0 = steady_clock::now();

	size_t question_counter = 1;

	while (true)
	{
		// listen
		rc = zmq_poll(&items[0], 2, 20);  // 20ms poll
		assert(rc != -1);

		if ((steady_clock::now() - t0) > seconds{1} && items[0].revents & ZMQ_POLLOUT)  // we can ask
		{
			string const buf = "hello " + to_string(question_counter);
			zmq_send(dea, buf.data(), buf.size(), 0);
			++question_counter;
			cout << "question: " << buf << endl;
			t0 = steady_clock::now();
		}

		if (items[0].revents & ZMQ_POLLIN)  // get an answer
		{
			char buf[1024] = {'\0'};
			zmq_recv(dea, buf, 1024, 0);
			cout << "answer: " << buf << endl;
		}

		// monitor events
		if (items[1].revents & ZMQ_POLLIN)
		{
			// we expect two frame message (see zmq_socket_monitor() description)
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, dea_mon, 0);
			assert(rc != -1);

			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			rc = zmq_msg_recv(&msg, dea_mon, 0);
			assert(rc != -1);
			cout << string((char *)zmq_msg_data(&msg), zmq_msg_size(&msg)) << endl;

			assert(!zmq_msg_more(&msg));  // last message

			zmq_close(&msg);
		}

	}

	return 0;
}

string zmq_event_to_string(int event)
{
	switch (event)
	{
		case ZMQ_EVENT_CONNECTED: return "connected";
		case ZMQ_EVENT_CONNECT_DELAYED: return "connect delayed";
		case ZMQ_EVENT_CONNECT_RETRIED: return "connect retried";
		case ZMQ_EVENT_LISTENING: return "listenning";
		case ZMQ_EVENT_BIND_FAILED: return "bind failed";
		case ZMQ_EVENT_ACCEPTED: return "accepted";
		case ZMQ_EVENT_ACCEPT_FAILED: return "accept failed";
		case ZMQ_EVENT_CLOSED: return "closed";
		case ZMQ_EVENT_CLOSE_FAILED: return "close failed";
		case ZMQ_EVENT_DISCONNECTED: return "disconnected";
		case ZMQ_EVENT_MONITOR_STOPPED: return "monitor stopped";
//		case ZMQ_EVENT_HANDSHAKE_FAILED_NO_DETAIL: return "handshake failed";
//		case ZMQ_EVENT_HANDSHAKE_SUCCEEDED: return "handshake succeeded";
//		case ZMQ_EVENT_HANDSHAKE_FAILED_PROTOCOL: return "handshake failed (protocol)";
		default: return "unknown (" + to_string(event) + ")";
	}
}
