// spojenie typu ROUTER <-> DEALER, serverovska cast implementovana v zmq (c++)
#include <string>
#include <iostream>
#include <cassert>
#include <zmq.hpp>

using std::string;
using std::cout;


int main(int argc, char * argv[])
{
	zmq::context_t ctx;
	zmq::socket_t socket{ctx, ZMQ_ROUTER};
	socket.bind("tcp://*:5557");

	while (true)
	{
		zmq::message_t identity;
		socket.recv(&identity);
		string s((char *)identity.data(), identity.size());
		cout << "identity: " << s << std::endl;

		// zisti, ci je sprav viac
		int more;
		size_t more_size = sizeof(more);
		socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);

		assert(more == 1 && "este cakam spravu");

		if (more)
		{
			zmq::message_t msg;
			socket.recv(&msg);
			string s((char *)msg.data(), msg.size());
			cout << "message: " << s << std::endl;
		}

		// answer
		socket.send(identity, ZMQ_SNDMORE);
		socket.send("world", 5);
	}

	return 0;
}
