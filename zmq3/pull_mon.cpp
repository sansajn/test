// pull pattern sample with monitored ZMQ events, see `push_mon.cpp` for push pattern sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string, std::to_string;
using std::cout, std::endl;
using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
	string const host = (argc > 1) ? argv[1] : "localhost";
	string const port = (argc > 2) ? argv[2] : "5557";
	
	// pull
	void * ctx = zmq_ctx_new();
	void * pull = zmq_socket(ctx, ZMQ_PULL);
	string const addr = "tcp://*:" + port;
	int rc = zmq_bind(pull, addr.c_str());
	assert(!rc);
	
	cout << "listenning on " << addr << " ..." << std::endl;

	std::this_thread::sleep_for(1s);  // wait for ZMQ to bind (otherwise we do not receive first ZMQ message)
	
	while (1)
	{
		char buf[1024];
		zmq_recv(pull, buf, 1024, 0);
		cout << "received: " << buf << endl;
	}

	return 0;
}
