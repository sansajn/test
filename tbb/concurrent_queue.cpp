#include <iostream>
#include <tbb/concurrent_queue.h>

using std::cout;
using namespace tbb;

int main(int argc, char * argv[])
{
	concurrent_queue<int> q;
	for (int i = 1; i < 11; ++i)
		q.push(i);

	for (concurrent_queue<int>::const_iterator it = q.unsafe_begin(); it != q.unsafe_end(); ++it)
		cout << *it << " ";
	cout << "\n";

	cout << "done!\n";
	return 0;
}
