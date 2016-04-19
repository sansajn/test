// spojenie typu ROUTER <-> DEALER, serverovska cast implementovana v zmq
#include <string>
#include <iostream>
#include <cassert>
#include <zmq.h>

using std::string;
using std::cout;


int main(int argc, char * argv[])
{
	void * ctx = zmq_ctx_new();
	void * socket = zmq_socket(ctx, ZMQ_ROUTER);
	int rc = zmq_bind(socket, "tcp://*:5557");
	assert(rc == 0 && "unsuccessful binding");

	while (true)  // cakaj na dotazy
	{
		char identity[1024];
		int identity_size = zmq_recv(socket, identity, 1024, 0);
		assert(rc != -1);
		string s(identity, identity_size);
		cout << "received " << s << std::endl;

		// zisti, ci je sprav viac
		int option_value;
		size_t option_len = sizeof(option_value);
		int rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &option_value, &option_len);
		if (option_value)
		{
			cout << "it is a multipart message" << std::endl;
			char d[1024];
			rc = zmq_recv(socket, d, 1024, 0);
			assert(rc != -1);
			string s(d, rc);
			cout << "next part is: " << s << std::endl;
		}
		else
			cout << "simple message" << std::endl;

		// answer (identity first, then the message
		zmq_send(socket, identity, identity_size, ZMQ_SNDMORE);
		zmq_send(socket, "world", 5, 0);
	}

	return 0;
}
