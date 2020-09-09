//! subscriber pattern sample with ZMQ event monitoring, see `publisher_mon.cpp` for publisher sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string, std::to_string;
using std::cout, std::endl;
using namespace std::chrono_literals;

constexpr char const * MONITOR_ADDR = "inproc://sub_monitor";

static string zmq_event_to_string(int event);

int main(int argc, char * argv[])
{
	string const host = (argc > 1) ? argv[1] : "localhost";
	string const port = (argc > 2) ? argv[2] : "5557";
	
	// subscriber
	void * ctx = zmq_ctx_new();
	void * sub = zmq_socket(ctx, ZMQ_SUB);
	zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "", 0);

	// connect
	string const addr = "tcp://" + host + ":" + port;
	int rc = zmq_connect(sub, addr.c_str());
	assert(!rc);

	// create subscriber monitor
	rc = zmq_socket_monitor(sub, MONITOR_ADDR, ZMQ_EVENT_ALL);
	assert(rc >= 0);

	// connect to monitor
	void * sub_mon = zmq_socket(ctx, ZMQ_PAIR);
	assert(sub_mon);
	zmq_connect(sub_mon, MONITOR_ADDR);

	std::this_thread::sleep_for(100ms);  // wait for zmq to connect

	zmq_pollitem_t items[] = {
		{sub, 0, ZMQ_POLLIN, 0},
		{sub_mon, 0, ZMQ_POLLIN, 0}
	};

	while (1)
	{
		// listen
		rc = zmq_poll(&items[0], 2, 20);  // 20ms poll
		assert(rc != -1);

		// check subscriber
		if (items[0].revents & ZMQ_POLLIN)
		{
			char buf[1024] = {'\0'};
			zmq_recv(sub, buf, 1024, 0);
			cout << "received: " << buf << endl;
		}

		if (items[1].revents & ZMQ_POLLIN)  // monitor events
		{
			// we expect two frame message (see zmq_socket_monitor() description)
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, sub_mon, 0);
			assert(rc != -1);

			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			rc = zmq_msg_recv(&msg, sub_mon, 0);
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
		case ZMQ_EVENT_HANDSHAKE_FAILED_NO_DETAIL: return "handshake failed";
		case ZMQ_EVENT_HANDSHAKE_SUCCEEDED: return "handshake succeeded";
		case ZMQ_EVENT_HANDSHAKE_FAILED_PROTOCOL: return "handshake failed (protocol)";
		default: return "unknown (" + to_string(event) + ")";
	}
}
