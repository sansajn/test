// thread safe mayers singleton implementation

#include <iostream>

using namespace std;

class singleton
{
public:
	singleton(singleton const &) = delete;
	singleton & operator=(singleton const &) = delete;

	static singleton * instance()
	{
		static singleton inst;
		return &inst;
	}

private:
	singleton() = default;
	~singleton() = default;
};

int main(int argc, char * argv[])
{
	cout << endl;

	cout << "singleton::instance(): " << singleton::instance() << endl;
	cout << "singleton::instance(): " << singleton::instance() << endl;

	cout << "done!\n";

	return 0;
}
