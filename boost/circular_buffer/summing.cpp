#include <numeric>
#include <cassert>
#include <boost/circular_buffer.hpp>

using boost::circular_buffer;

int main(int argc, char * argv[])
{
	circular_buffer<int> buf{3};
	
	assert(buf.capacity() == 3);
	assert(buf.size() == 0);
	assert(buf.empty());
	
	buf.push_back(1);
	buf.push_back(2);
	
	assert(buf[0] == 1);
	assert(buf[1] == 2);
	assert(!buf.full());
	assert(buf.size() == 2);
	assert(buf.capacity() == 3);
	
	buf.push_back(3);
	buf.push_back(4);
	
	int sum = std::accumulate(buf.begin(), buf.end(), 0);
	
	assert(sum == 9);
	assert(buf[0] == 2);
	assert(buf[1] == 3);
	assert(buf[2] == 4);
	assert(*buf.begin() == 2);
	assert(buf.front() == 2);
	assert(buf.back() == 4);
	assert(buf.full());
	assert(buf.size() == 3);
	assert(buf.capacity() == 3);

	return 0;
}