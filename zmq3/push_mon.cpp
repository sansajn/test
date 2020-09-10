// push pattern sample with monitored ZMQ events, see `pull_mon.cpp` for pull pattern sample
#include <string>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <zmq.h>

using std::string, std::to_string;
using std::cout, std::endl;
using std::chrono::steady_clock;
using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
	string const host = (argc > 1) ? argv[1] : "localhost";
	string const port = (argc > 2) ? argv[2] : "5557";
	
	// push
	void * ctx = zmq_ctx_new();
	void * push = zmq_socket(ctx, ZMQ_PUSH);
	assert(push);

	// connect
	string const addr = "tcp://" + host + ":" + port;
	int rc = zmq_connect(push, addr.c_str());
	assert(!rc);
	
	cout << "broadcasting on " << addr << " ..." << std::endl;

	std::this_thread::sleep_for(1s);  // wait for ZMQ to bind (otherwise we do not receive first ZMQ message)
	
	size_t counter = 1;

	auto t0 = steady_clock::now();

	while (1)
	{
		if (steady_clock::now() - t0 > 1s)
		{
			string const buf = "hello " + to_string(counter);
			++counter;
			zmq_send(push, buf.data(), buf.size(), 0);

			t0 = steady_clock::now();
		}
	}

	return 0;
}
