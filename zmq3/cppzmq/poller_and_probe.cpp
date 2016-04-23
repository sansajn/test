// pouzitie monitora, probe a poller abstrakcie
#include <string>
#include <chrono>
#include <iostream>
#include <cstring>
#include <cassert>
#include <zmq.hpp>

using std::string;
using std::cout;

static string zmq_event_to_string(int event);


class probe
{
public:
	probe(zmq::context_t & ctx, char const * addr)
		: _sock{ctx, ZMQ_PAIR}
	{
		_sock.connect(addr);
	}

	void recv();
	void * socket() {return (void *)_sock;}

	void on_event_connected(char const * addr) {}
	void on_event_accepted(char const * addr);
	void on_event_disconnected(char const * addr);

private:
	zmq::socket_t _sock;
};


void probe::on_event_accepted(char const * addr)
{
	cout << "event: accepted, " << addr << std::endl;
}

void probe::on_event_disconnected(char const * addr)
{
	cout << "event: disconnected, " << addr << std::endl;
}


void probe::recv()
{
	zmq_msg_t msg;
	zmq_msg_init(&msg);
	int rc = zmq_msg_recv(&msg, _sock, 0);
	assert(rc != -1);
	assert(zmq_msg_more(&msg));
	uint16_t event = *(uint16_t *)zmq_msg_data(&msg);

	rc = zmq_msg_recv(&msg, _sock, 0);
	assert(rc != -1);
	assert(!zmq_msg_more(&msg));

	switch (event)
	{
		case ZMQ_EVENT_CONNECTED:
			on_event_connected((char const *)zmq_msg_data(&msg));
			break;

		case ZMQ_EVENT_CONNECT_DELAYED: break;
		case ZMQ_EVENT_CONNECT_RETRIED: break;
		case ZMQ_EVENT_LISTENING: break;
		case ZMQ_EVENT_BIND_FAILED: break;
		case ZMQ_EVENT_ACCEPTED:
			on_event_accepted((char const *)zmq_msg_data(&msg));
			break;

		case ZMQ_EVENT_ACCEPT_FAILED: break;
		case ZMQ_EVENT_CLOSED: break;
		case ZMQ_EVENT_CLOSE_FAILED: break;

		case ZMQ_EVENT_DISCONNECTED:
			on_event_disconnected((char const *)zmq_msg_data(&msg));
			break;

		case ZMQ_EVENT_MONITOR_STOPPED: break;

		default:
			assert(0 && "unknown event");
			break;
	}

	zmq_close(&msg);
}

class poller
{
public:
	void add(zmq::socket_t & sock, short revents = ZMQ_POLLIN);  //!< \param[in] revents use ZMQ_POLLIN, ZMQ_POLLOUT or ZMQ_POLLERR \sa zmq_poll()
	void add(probe & p);
	void poll(std::chrono::milliseconds timeout);
	bool has_input(int idx) const;
	bool has_output(int idx) const;
	bool has_event(int idx) const;
	short events(int idx) const;

private:
	std::vector<zmq_pollitem_t> _items;
};



int main(int argc, char * argv[])
{
	zmq::context_t ctx;
	zmq::socket_t responder{ctx, ZMQ_REP};
	responder.bind("tcp://*:5555");

	// create socket monitor
	int rc = zmq_socket_monitor((void *)responder, "inproc://someaddress", ZMQ_EVENT_ALL);
	assert(rc >= 0);

	probe responder_probe{ctx, "inproc://someaddress"};

	poller items;
	items.add(responder);
	items.add(responder_probe);

	while (true)
	{
		items.poll(std::chrono::milliseconds{100});

		if (items.has_input(0))  // responder
		{
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, responder, 0);
			assert(rc != -1);
			cout << "received: " << string((char *)zmq_msg_data(&msg), zmq_msg_size(&msg)) << std::endl;
			zmq_msg_close(&msg);

			zmq_send(responder, "world", 5, 0);
		}

		if (items.has_input(1))  // responder_probe
		{
			responder_probe.recv();
		}
	}

	return 0;
}

// TODO: zmqutils, zmqu
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


void poller::add(zmq::socket_t & sock, short revents)
{
	_items.push_back(zmq_pollitem_t{(void *)sock, 0, revents});
}

void poller::add(probe & p)
{
	_items.push_back(zmq_pollitem_t{p.socket(), 0, ZMQ_POLLIN});
}

void poller::poll(std::chrono::milliseconds timeout)
{
	int rc = zmq_poll(const_cast<zmq_pollitem_t *>(_items.data()), _items.size(), timeout.count());
	if (rc == -1)
		throw std::runtime_error{"zmq_poll() failed"};
}

bool poller::has_input(int idx) const  // has_input
{
	return events(idx) & ZMQ_POLLIN;
}

bool poller::has_output(int idx) const
{
	return events(idx) & ZMQ_POLLOUT;
}

bool poller::has_event(int idx) const
{
	return events(idx) & ZMQ_POLLERR;
}

short poller::events(int idx) const
{
	return _items[idx].revents;
}
