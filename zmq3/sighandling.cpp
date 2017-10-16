// handling interupt signals in c++ (http://zguide.zeromq.org/cpp:interrupt)
#include <iostream>
#include <signal.h>  // posix
#include <zmq.hpp>

using std::cout;

static bool interupted = false;

static void signal_handler(int sig)
{
	interupted = true;
}

static void catch_signals()
{
	struct sigaction action;
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, nullptr);
	sigaction(SIGTERM, &action, nullptr);
}

int main(int argc, char * argv[])
{
	zmq::context_t context{1};
	zmq::socket_t socket{context, ZMQ_REP};
	socket.bind("tcp://*:5555");

	catch_signals();  // instal signals handlers

	while (true)
	{
		zmq::message_t msg;

		try {
			socket.recv(&msg);  // recv will trow on a signal
		}
		catch (zmq::error_t & e) {cout << e.what() << std::endl;}

		if (interupted)
		{
			cout << "interrupt detected, done" << std::endl;
			break;
		}
	}

	return 0;
}
