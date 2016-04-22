// pouzitie monitora s dokumentacie k zmq_socket_monitor
#include <string>
#include <iostream>
#include <cstring>
#include <cassert>
#include <zmq.h>

using std::string;
using std::cout;

static string zmq_event_to_string(int event);

int main(int argc, char * argv[])
{
	void * ctx = zmq_ctx_new();
	void * responder = zmq_socket(ctx, ZMQ_REP);
	zmq_bind(responder, "tcp://*:5555");

	// create socket monitor
	int rc = zmq_socket_monitor(responder, "inproc://someaddress", ZMQ_EVENT_ALL);
	assert(rc >= 0);

	void * responder_mon = zmq_socket(ctx, ZMQ_PAIR);
	assert(responder_mon);
	zmq_connect(responder_mon, "inproc://someaddress");

	zmq_pollitem_t items[] = {
		{responder, 0, ZMQ_POLLIN},
		{responder_mon, 0, ZMQ_POLLIN}
	};

	while (true)
	{
		rc = zmq_poll(items, sizeof(items)/sizeof(items[0]), 100);  // 100ms poll
		assert(rc != -1);

		if (items[0].revents & ZMQ_POLLIN)  // responder
		{
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, responder, 0);
			assert(rc != -1);
			cout << "received: " << string((char *)zmq_msg_data(&msg), zmq_msg_size(&msg)) << std::endl;
			zmq_msg_close(&msg);

			zmq_send(responder, "world", 5, 0);
		}

		if (items[1].revents & ZMQ_POLLIN)  // responder monitor
		{
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, responder_mon, 0);
			assert(rc != -1);
			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			rc = zmq_msg_recv(&msg, responder_mon, 0);
			assert(rc != -1);
			assert(!zmq_msg_more(&msg));
			cout << string((char *)zmq_msg_data(&msg), zmq_msg_size(&msg)) << std::endl;

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
		default: return "unknown";
	}
}
