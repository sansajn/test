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

string const DEFAULT_HOST = "*",
	DEFAULT_PORT = "5557";

constexpr char const * MONITOR_ADDR = "inproc://rtr_monitor";

static string zmq_event_to_string(int event);
static void dump_keepalive_settings(void * socket);

int main(int argc, char * argv[])
{
	string const host = (argc > 1) ? argv[1] : DEFAULT_HOST;
	string const port = (argc > 2) ? argv[2] : DEFAULT_PORT;

	// router
	void * ctx = zmq_ctx_new();
	void * rtr = zmq_socket(ctx, ZMQ_ROUTER);

	cout << "default keep alive settings for router socket:\n";
	dump_keepalive_settings(rtr);

	// change keepalive settings
	int keep_alive = 1;
	int res = zmq_setsockopt(rtr, ZMQ_TCP_KEEPALIVE, (void const *)&keep_alive, sizeof(keep_alive));
	assert(!res);

	int keep_alive_cnt = 2;  // tcp_keepalive_probes (default: 9)
	res = zmq_setsockopt(rtr, ZMQ_TCP_KEEPALIVE_CNT, (void const *)&keep_alive_cnt, sizeof(keep_alive_cnt));
	assert(!res);

	int keep_alive_idle = 10;  //  tcp_keepalive_time (default: 7200s)
	res = zmq_setsockopt(rtr, ZMQ_TCP_KEEPALIVE_IDLE, (void const *)&keep_alive_idle, sizeof(keep_alive_idle));
	assert(!res);

	int keep_alive_intvl = 30;  //  tcp_keepalive_intvl (default: 75s)
	res = zmq_setsockopt(rtr, ZMQ_TCP_KEEPALIVE_INTVL, (void const *)&keep_alive_intvl, sizeof(keep_alive_intvl));

	cout << "keep alive settings after setup:\n";
	dump_keepalive_settings(rtr);

	string const address = "tcp://" + host + ":" + port;
	res = zmq_bind(rtr, address.c_str());
	assert(!res);

	// create router monitor
	res = zmq_socket_monitor(rtr, MONITOR_ADDR, ZMQ_EVENT_ALL);
	assert(res >= 0);

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
		res = zmq_poll(&items[0], 2, 20);  // 20ms poll
		assert(res != -1);

		if (items[0].revents & ZMQ_POLLIN)
		{
			// TODO: use zmq_msq_t there

			// question will be in (identity, message) format
			char identity[1024];
			int identity_size = zmq_recv(rtr, identity, 1024, 0);
			assert(res != -1);

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
			res = zmq_msg_recv(&msg, rtr_mon, 0);
			assert(res != -1);

			assert(zmq_msg_more(&msg));
			uint16_t event = *(uint16_t *)zmq_msg_data(&msg);
			cout << "event: " << zmq_event_to_string(event) << ", ";

			res = zmq_msg_recv(&msg, rtr_mon, 0);
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

void dump_keepalive_settings(void * socket)
{
	// current TCP keep alive settings
	int keep_alive;
	size_t option_len = sizeof(keep_alive);
	int res = zmq_getsockopt(socket, ZMQ_TCP_KEEPALIVE, (void *)&keep_alive, &option_len);
	assert(!res);

	int keep_alive_cnt;
	option_len = sizeof(keep_alive_cnt);
	res = zmq_getsockopt(socket, ZMQ_TCP_KEEPALIVE_CNT, (void *)&keep_alive_cnt, &option_len);
	assert(!res);

	int keep_alive_idle;
	option_len = sizeof(keep_alive_idle);
	res = zmq_getsockopt(socket, ZMQ_TCP_KEEPALIVE_IDLE, (void *)&keep_alive_idle, &option_len);
	assert(!res);

	int keep_alive_intvl;
	option_len = sizeof(keep_alive_intvl);
	res = zmq_getsockopt(socket, ZMQ_TCP_KEEPALIVE_INTVL, (void *)&keep_alive_intvl, &option_len);
	assert(!res);

	cout << "keepalive=" << keep_alive << "\n"
		<< "keepalive_cnt=" << keep_alive_cnt << "\n"
		<< "keepalive_idle=" << keep_alive_idle << "\n"
		<< "keepalive_intvl=" << keep_alive_intvl << endl;
}
