// dealer pattern sample with monitored ZMQ events (for legacy ZMQ 4.14 and C++11 running on debian8), see `router_mon.cpp` for router sample
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
using std::chrono::seconds;

string const DEFAULT_ROUTER_ADDRESS = "*:5557";
constexpr char const * MONITOR_ADDR = "inproc://rtr_monitor";

static string zmq_event_to_string(int event);

int main(int argc, char * argv[])
{
	string address;
	if (argc > 1)
		address = "tcp://" + string{argv[1]};
	else
		address = "tcp://" + DEFAULT_ROUTER_ADDRESS;

	// router
	void * ctx = zmq_ctx_new();
	void * rtr = zmq_socket(ctx, ZMQ_ROUTER);
	int rc = zmq_bind(rtr, address.c_str());
	assert(!rc);

	// create router monitor
	rc = zmq_socket_monitor(rtr, MONITOR_ADDR, ZMQ_EVENT_ALL);
	assert(rc >= 0);

	// connect to monitor
	void * rtr_mon = zmq_socket(ctx, ZMQ_PAIR);
	assert(rtr_mon);
	zmq_connect(rtr_mon, MONITOR_ADDR);

	std::this_thread::sleep_for(seconds{1});  // wait for zmq to bind

	zmq_pollitem_t items[] = {
		{rtr, 0, ZMQ_POLLIN, 0},
		{rtr_mon, 0, ZMQ_POLLIN, 0}
	};

	size_t answer_counter = 1;

	while (true)
	{
		// listen
		rc = zmq_poll(&items[0], 2, 20);  // 20ms poll
		assert(rc != -1);

		if (items[0].revents & ZMQ_POLLIN)
		{
			// TODO: use zmq_msq_t there

			// question will be in (identity, message) format
			char identity[1024];
			int identity_size = zmq_recv(rtr, identity, 1024, 0);
			assert(rc != -1);

			string const ident_s(identity, identity_size);
			cout << "identity: " << ident_s << endl;

			int more;
			size_t option_len = sizeof(more);
			int rc = zmq_getsockopt(rtr, ZMQ_RCVMORE, &more, &option_len);
			assert(rc == 0);
			assert(more == 1 && "multi frame message expected");

			char d[1024];
			rc = zmq_recv(rtr, d, 1024, 0);
			assert(rc != -1);
			string const question_s(d, rc);
			cout << "question: " << question_s << std::endl;

			// answer
			string const answer = "world " + to_string(answer_counter);
			++answer_counter;
			zmq_send(rtr, identity, identity_size, ZMQ_SNDMORE);
			zmq_send(rtr, answer.c_str(), answer.size(), 0);	
		}

		// handle monitor events
		if (items[1].revents & ZMQ_POLLIN)
		{
			// we expect two frame message (see zmq_socket_monitor() description)
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, rtr_mon, 0);
			assert(rc != -1);

			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			rc = zmq_msg_recv(&msg, rtr_mon, 0);
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
