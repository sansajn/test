// publisher pattern sample with ZMQ event monitoring, see `subscriber_mon.cpp` for subscriber sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string, std::to_string;
using std::cout, std::endl;
using std::chrono::steady_clock;
using namespace std::chrono_literals;

constexpr char const * MONITOR_ADDR = "inproc://sub_monitor";

static string zmq_event_to_string(int event);

int main(int argc, char * argv[])
{
	string const port = (argc > 1) ? argv[1] : "5557";
	
	// publisher
	void * ctx = zmq_ctx_new();
	void * pub = zmq_socket(ctx, ZMQ_PUB);
	string const addr = "tcp://*:" + port;
	int rc = zmq_bind(pub, addr.c_str());
	assert(!rc);
	
	// create publisher monitor
	rc = zmq_socket_monitor(pub, MONITOR_ADDR, ZMQ_EVENT_ALL);
	assert(rc >= 0);

	// connect to monitor
	void * pub_mon = zmq_socket(ctx, ZMQ_PAIR);
	assert(pub_mon);
	zmq_connect(pub_mon, MONITOR_ADDR);

	cout << "broadcasting on " << addr << " ..." << std::endl;

	std::this_thread::sleep_for(1s);  // wait for ZMQ to bind (otherwise we do not receive first ZMQ message)
	
	zmq_pollitem_t items[] = {
		{pub_mon, 0, ZMQ_POLLIN, 0}
	};

	auto t0 = steady_clock::now() - 1s; 

	size_t counter = 1;

	while (1)
	{
		if (steady_clock::now() - t0 > 1s)
		{
			string const buf = "hello " + to_string(counter);
			++counter;
			zmq_send(pub, buf.data(), buf.size(), 0);
			std::this_thread::sleep_for(1s);
		}


		rc = zmq_poll(&items[0], 1, 20);  // 20ms poll
		assert(rc != -1);

		if (items[0].revents & ZMQ_POLLIN)  // monitor events
		{
			// we expect two frame message (see zmq_socket_monitor() description)
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, pub_mon, 0);
			assert(rc != -1);

			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			rc = zmq_msg_recv(&msg, pub_mon, 0);
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
