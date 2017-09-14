// publisher, subscriber pattern
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <zmq.h>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	// publisher
	void * ctx = zmq_ctx_new();
	void * pub = zmq_socket(ctx, ZMQ_PUB);
	int rc = zmq_bind(pub, "tcp://*:5557");
	assert(!rc);

	// subscriber
	void * sub = zmq_socket(ctx, ZMQ_SUB);
	zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "", 0);
	rc = zmq_connect(sub, "tcp://localhost:5557");
	assert(!rc);

	std::this_thread::sleep_for(std::chrono::milliseconds{10});  // wait for zmq to connect

	while (1)
	{
		// send something
		static bool once = true;
		if (once)
		{
			string buf{"hello"};
			zmq_send(pub, buf.data(), buf.size(), 0);
			once = false;
		}

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
			break;
		}
	}

	return 0;
}
