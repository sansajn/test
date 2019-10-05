// async task vs thread

#include <future>
#include <thread>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	int res;
	thread t{[&]{res = 2000+11;}};
	t.join();
	cout << "res: " << res << endl;

	auto fut = async([]{return 2000+11;});  // returns future<RESULT>
	cout << "fut.get(): " << fut.get() << endl;

	cout << "done!\n";
	return 0;
}
