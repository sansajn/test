// request-replay client (server je implementovany ako rrserv.cpp)
#include <string>
#include <thread>
#include <iostream>
#include <zmq.hpp>

using std::string;
using std::cout;

struct dummy_monitor : public zmq::monitor_t
{
	dummy_monitor(zmq::socket_t & sock) : socket{sock} {}

	void operator()() {
		monitor(socket, "inproc://someaddress", ZMQ_EVENT_ALL);
	}

	zmq::socket_t & socket;

	void on_monitor_started()
	{
		cout << "on_monitor_started()" << std::endl;
	}

	void on_event_connected(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_connected()" << std::endl;
	}

	void on_event_connect_delayed(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_connect_delayed()" << std::endl;
	}

	void on_event_connect_retried(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_connect_retried()" << std::endl;
	}

	void on_event_listening(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_listening()" << std::endl;
	}

	void on_event_bind_failed(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_bind_failed()" << std::endl;
	}

	void on_event_accepted(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_accepted(), addr:" << addr_ << std::endl;
	}

	void on_event_accept_failed(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_accept_failed()" << std::endl;
	}

	void on_event_closed(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_closed()" << std::endl;
	}

	void on_event_close_failed(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_close_failed()" << std::endl;
	}

	void on_event_disconnected(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_disconnected(), addr:" << addr_ << std::endl;
	}

	void on_event_unknown(const zmq_event_t &event_, const char* addr_)
	{
		cout << "on_event_unknown()" << std::endl;
	}
};

int main(int argc, char * argv[])
{
	zmq::context_t ctx;
	zmq::socket_t requester{ctx, ZMQ_REQ};
	requester.connect("tcp://localhost:5555");

	dummy_monitor mon{requester};
	std::thread monitor_thread(mon);

	while (true)
	{
		string s{"hello"};
		zmq::message_t msg(s.size());
		memcpy(msg.data(), s.data(), s.size());
		requester.send(msg);

		requester.recv(&msg);
		cout << "received: " << string((char const *)msg.data(), msg.size()) << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds{1});
	}

	return 0;
}
