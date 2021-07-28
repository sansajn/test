#include <zmq.hpp>
#include <string>
#include <iostream>

using std::cout;

int main(int argc, char * argv[])
{
	zmq::context_t ctx{1};
	zmq::socket_t socket{ctx, ZMQ_REQ};
	socket.connect("tcp://localhost:5555");

	// do 10 requests/reply
	for (int i = 0; i < 10; ++i)
	{
		zmq::message_t req{5};
		memcpy(req.data(), "Hello", 5);
		cout << "Sending Hello " << i << "..." << std::endl;
		socket.send(req);

		zmq::message_t reply;
		socket.recv(&reply);
		cout << "Received World " << i << std::endl;
	}

	return 0;
}
