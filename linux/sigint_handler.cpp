#include <thread>
#include <chrono>
#include <iostream>
#include <csignal>

using std::cout;
using namespace std::chrono_literals;

bool quit = false;

void sigint_handler(int signum) {
	cout << "<ctrl>+c catched!\n";
	quit = true;
}

int main(int argc, char * argv[]) {
	cout << "press <ctrl>+c to quit\n";

	signal(SIGINT, sigint_handler);
	
	while (!quit)
		std::this_thread::sleep_for(10ms);

	cout << "done!\n";
	return 0;
}
