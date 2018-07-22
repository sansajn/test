// sample publisher implementation
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <zmq.h>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	string const port = (argc > 1) ? argv[1] : "5557";
	
	// publisher
	void * ctx = zmq_ctx_new();
	void * pub = zmq_socket(ctx, ZMQ_PUB);
	string const addr = "tcp://*:" + port;
	int rc = zmq_bind(pub, addr.c_str());
	assert(!rc);
	
	cout << "broadcasting on " << addr << " ..." << std::endl;
	
	while (1)
	{
		string const buf{"hello"};
		zmq_send(pub, buf.data(), buf.size(), 0);
		std::this_thread::sleep_for(std::chrono::seconds{1});
	}

	return 0;
}
