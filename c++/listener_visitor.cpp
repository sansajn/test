// implamentacia listenera pomocou visitora
#include <functional>
#include <vector>
#include <iostream>

struct listener 
{
	virtual void done() {};
};

struct foo
{
	void notify_done() {
		for (auto l : _listeners)
			l->done();
	}

	std::vector<listener *> _listeners;
};

struct listener_impl : public listener
{
	void done() override {std::cout << "foo instance done!\n";}
};

int main(int argc, char * argv[])
{
	foo f;
	listener_impl l;
	f._listeners.push_back(&l);
	f.notify_done();
	return 0;
}
