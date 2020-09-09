//! subscriber pattern sample, see `publisher.cpp` for publisher sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string;
using std::cout;
using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
	string const host = (argc > 1) ? argv[1] : "localhost";
	string const port = (argc > 2) ? argv[2] : "5557";
	
	// subscriber
	void * ctx = zmq_ctx_new();
	void * sub = zmq_socket(ctx, ZMQ_SUB);
	zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "", 0);
	string const addr = "tcp://" + host + ":" + port;
	int rc = zmq_connect(sub, addr.c_str());
	assert(!rc);

	std::this_thread::sleep_for(10ms);  // wait for zmq to connect

	while (1)
	{
		// listen (non blocking)
		zmq_pollitem_t items[1];
		items[0].socket = sub;
		items[0].fd = 0;
		items[0].events = ZMQ_POLLIN;
		items[0].revents = 0;

		rc = zmq_poll(&items[0], 1, 20);  // 20ms poll
		assert(rc != -1);
		if (items[0].revents & ZMQ_POLLIN)
		{
			char buf[1024] = {'\0'};
			zmq_recv(sub, buf, 1024, 0);

			cout << "received: " << buf << std::endl;
		}
	}

	return 0;
}
