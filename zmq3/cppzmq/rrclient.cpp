// request-replay client (server je implementovany ako rrserv.cpp)
#include <string>
#include <thread>
#include <iostream>
#include <zmq.hpp>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	zmq::context_t ctx;
	zmq::socket_t requester{ctx, ZMQ_REQ};
	requester.connect("tcp://localhost:5555");

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
