#include <thread>
#include <iostream>

using std::cout;

void loop1(char * watch_var)
{
	while (1)
	{
		if (watch_var && *watch_var != 0)
			break;
	}
	cout << "loop1() done." << std::endl;
}


int main(int argc, char * argv[]) 
{
	char watch_var = 0;
	std::thread t{loop1, &watch_var};

	std::this_thread::sleep_for(std::chrono::seconds{1});
	watch_var = 1;

	/* A thread that has finished executing code, but has not yet been joined is still
	 considered an active thread of execution and is therefore joinable. */
	// while (t.joinable())  // error: runs forever
	// 	continue;

	t.join();

	cout << "done.\n";

	return 0;
}
