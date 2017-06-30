#include <thread>
#include <iostream>

using std::cout;

void foo()
{
	std::this_thread::sleep_for(std::chrono::seconds{1});
}


int main(int argc, char * argv[])
{
	std::thread t{foo};
	t.join();

	cout << "joinable: " << std::boolalpha << t.joinable() << "\n";

	// can I join to non joinable thread without an exception ?
	try {
		t.join();
	}
	catch (std::system_error const & e) {
		cout << "std::system_error exception catched\n";
	}

	cout << "done.\n";

	return 0;
}
