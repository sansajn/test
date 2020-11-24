// scoped_lock sample

#include <mutex>
#include <thread>
#include <iostream>
using std::mutex, std::scoped_lock, std::cout;

int main(int argc, char * argv[])
{
	mutex m, n, o;

	{
		scoped_lock lock(m, n, o);
	}

	cout << "done!\n";
	
	return 0;
}
