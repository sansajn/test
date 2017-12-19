// pouzitie std::async funkcie
#include <future>
#include <thread>
#include <iostream>

using std::future;
using std::async;
using std::cout;

int main(int argc, char * argv[])
{
	future<int> result{async(
		[](int m, int n){
			return m+n;
		}, 
	2, 4)};

	cout << "message from main" << std::endl;
	cout << result.get() << std::endl;

	return 0;
}
