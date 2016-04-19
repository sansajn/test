// spojenie typu ROUTER <-> DEALER, klientska cast implementovana v zmq
#include <string>
#include <iostream>
#include <cassert>
#include <zmq.h>

using std::string;
using std::cout;


int main(int argc, char * argv[])
{
	void * ctx = zmq_ctx_new();
	void * socket = zmq_socket(ctx, ZMQ_DEALER);
	int rc = zmq_connect(socket, "tcp://localhost:5557");
	assert(rc == 0 && "unsuccessful connect");

	string buf{"hello"};
	zmq_send(socket, buf.data(), buf.size(), 0);

	while (true)  // wait for reply
	{
		char buf[1024];
		rc = zmq_recv(socket, buf, 1024, 0);
		assert(rc != -1 && "unsuccessful receive");
		string s(buf, rc);
		cout << "server replied: " << s << std::endl;
	}

	return 0;
}
