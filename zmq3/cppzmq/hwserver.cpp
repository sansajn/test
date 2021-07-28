#include <iostream>
#include <cstring>
#include <unistd.h>
#include <zmq.hpp>

using std::cout;

int main(int argc, char * argv[])
{
	zmq::context_t ctx{1};
	zmq::socket_t socket(ctx, ZMQ_REP);
	socket.bind("tcp://*:5555");
	
	while (true)
	{
		zmq::message_t req;
		
		// wait for request from client
		socket.recv(&req);
		cout << "received hello" << std::endl;
		
		sleep(1);  // do some work
		
		// send reply back to client
		zmq::message_t reply{5};
		memcpy(reply.data(), "World", 5);
		socket.send(reply);
	}
	
	return 0;
}
