// publisher pattern sample, see `subscriber.cpp` for subscriber sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string, std::to_string;
using std::cout;
using namespace std::chrono_literals;

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

	std::this_thread::sleep_for(1s);  // wait for ZMQ to bind (otherwise we do not receive first ZMQ message)
	
	size_t counter = 1;

	while (1)
	{
		string const buf = "hello " + to_string(counter);
		++counter;
		zmq_send(pub, buf.data(), buf.size(), 0);
		std::this_thread::sleep_for(1s);
	}

	return 0;
}
