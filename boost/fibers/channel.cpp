// channel between two fibers sample
#include <functional>
#include <iostream>
#include <boost/fiber/all.hpp>
using std::ref;
using std::cout, 
	std::endl;
using namespace boost::fibers;

using channel_t = unbuffered_channel<int>;

void foo(channel_t & chan)
{
	chan.push(1);
	chan.push(1);
	chan.push(2);
	chan.push(3);
	chan.push(5);
	chan.push(8);
	chan.push(12);
	chan.close();
}

void bar(channel_t & chan)
{
	for (int x : chan) cout << x << " ";
	cout << endl;
}

int main(int argc, char * argv[])
{
	channel_t chan;
	fiber f1{&foo, ref(chan)},
		f2{&bar, ref(chan)};

	f1.join();
	f2.join();

	cout << "done." << endl;
	return 0;
}