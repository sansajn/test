// request-replay server (klient je implementovany ako rrclient.cpp)
#include <string>
#include <iostream>
#include <zmq.hpp>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	zmq::context_t ctx;
	zmq::socket_t responder{ctx, ZMQ_REP};
	responder.bind("tcp://*:5555");

	while (true)
	{
		zmq::message_t msg;
		responder.recv(&msg);
		cout << "received: " << string((char const *)msg.data(), msg.size()) << std::endl;

		responder.send("world", 5);
	}

	return 0;
}
