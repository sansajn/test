// spojenie typu ROUTER <-> DEALER, serverovska cast implementovana v zmq
// za pouzitia zmq_msg_t struktury nameisto char* buffrou
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

	while (true)
	{
		// odpoved pride ako (identity, message)
		zmq_msg_t identity;
		int rc = zmq_msg_init(&identity);
		assert(rc == 0);
		rc = zmq_msg_recv(&identity, socket, 0);
		assert(rc != -1);
		assert(rc == zmq_msg_size(&identity));
		string s((char *)zmq_msg_data(&identity), zmq_msg_size(&identity));
		cout << "identity: " << s << std::endl;

		// zisti, ci je sprav viac
		int more;
		size_t option_len = sizeof(more);
		rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &more, &option_len);
		assert(rc == 0);
		assert(more == 1 && "este cakam spravu");

		if (more)
		{
			zmq_msg_t msg;
			zmq_msg_init(&msg);
			rc = zmq_msg_recv(&msg, socket, 0);
			assert(rc != -1);
			assert(rc == zmq_msg_size(&msg));
			string s((char *)zmq_msg_data(&msg), zmq_msg_size(&msg));
			cout << "message: " << s << std::endl;
			zmq_msg_close(&msg);
		}

		// answer
		zmq_msg_send(&identity, socket, ZMQ_SNDMORE);
		zmq_send(socket, "world", 5, 0);

		zmq_msg_close(&identity);
	}

	return 0;
}
