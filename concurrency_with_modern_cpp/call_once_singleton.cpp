/* implementing singleton pattern with call_once and once_flag
note: not working with gcc 5.4.0-6ubuntu1~16.04.11 (gcc has a bug in std::call_once) */

#include <mutex>
#include <iostream>

using namespace std;

class singleton
{
public:
	singleton(singleton const &) = delete;
	singleton & operator=(singleton const &) = delete;

	static singleton * instance()
	{
		call_once(_init_instance_flag, singleton::init);
		return _instance;
	}

	static void init()
	{
		_instance = new singleton{};
	}

private:
	singleton() = default;
	~singleton() = default;

	static once_flag _init_instance_flag;
	static singleton * _instance;
};

singleton * singleton::_instance = nullptr;
once_flag singleton::_init_instance_flag;

int main(int argc, char * argv[])
{
	cout << endl;

	cout << "singleton::instance(): " << singleton::instance() << endl;
	cout << "singleton::instance(): " << singleton::instance() << endl;

	cout << "done!\n";

	return 0;
}
