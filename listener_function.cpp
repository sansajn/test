// implementacia listener-a pomocou std::function
#include <functional>
#include <vector>
#include <iostream>

struct foo
{
	using donefce_t = std::function<void ()>;

	void notify_done() {
		for (auto f : _listeners)
			f();
	}

	std::vector<donefce_t> _listeners;
};

void g() {std::cout << "foo instance done!\n";}

int main(int argc, char * argv[])
{
	foo f;
	f._listeners.push_back(foo::donefce_t{g});
	f.notify_done();
	return 0;
}
