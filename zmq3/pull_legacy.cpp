// pull pattern sample with monitored ZMQ events (for legacy ZMQ 4.14 and C++11 running on debian8), see `push_mon.cpp` for push pattern sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::chrono::seconds;

string const DEFAULT_HOST = "*",
	DEFAULT_PORT = "5557";

constexpr char const * MONITOR_ADDR = "inproc://pull_monitor";

static string zmq_event_to_string(int event);

int main(int argc, char * argv[])
{
	string const host = (argc > 1) ? argv[1] : DEFAULT_HOST;
	string const port = (argc > 2) ? argv[2] : DEFAULT_PORT;
	
	// pull
	void * ctx = zmq_ctx_new();
	void * pull = zmq_socket(ctx, ZMQ_PULL);
	string const addr = "tcp://" + host + ":" + port;
	int res = zmq_bind(pull, addr.c_str());
	assert(!res);

	// create pull monitor
	res = zmq_socket_monitor(pull, MONITOR_ADDR, ZMQ_EVENT_ALL);
	assert(res >= 0);

	// connect to monitor
	void * pull_mon = zmq_socket(ctx, ZMQ_PAIR);
	assert(pull_mon);
	zmq_connect(pull_mon, MONITOR_ADDR);
	
	cout << "listenning on " << addr << " ..." << std::endl;

	std::this_thread::sleep_for(seconds{1});  // wait for ZMQ to bind (otherwise we do not receive first ZMQ message)
	
	zmq_pollitem_t items[] = {
		{pull, 0, ZMQ_POLLIN, 0},
		{pull_mon, 0, ZMQ_POLLIN, 0}
	};

	while (true)
	{
		res = zmq_poll(&items[0], 2, 20);  // 20ms poll
		assert(res != -1);

		if (items[0].revents & ZMQ_POLLIN)
		{
			char buf[1024] = {0};
			zmq_recv(pull, buf, 1024, 0);
			cout << "received: " << buf << endl;
		}

		if (items[1].revents & ZMQ_POLLIN)
		{
			// we expect two frame message (see zmq_socket_monitor() description)
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			res = zmq_msg_recv(&msg, pull_mon, 0);
			assert(res != -1);

			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			res = zmq_msg_recv(&msg, pull_mon, 0);
			assert(res != -1);
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
