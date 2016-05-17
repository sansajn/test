#include <iostream>
#include <boost/circular_buffer.hpp>

using std::cout;
using boost::circular_buffer;

int main(int argc, char * argv[])
{
	circular_buffer<int> buf{3};
	
	buf.push_back(1);
	buf.push_back(2);
	buf.push_back(3);
	
	int a = buf[0];  // a == 1
	int b = buf[1];  // b == 2
	int c = buf[2];  // c == 3
	cout << "a:" << a << ", b:" << b << ", c:" << c << "\n";
	
	buf.push_back(4);
	buf.push_back(5);
	
	a = buf[0];  // a == 3
	b = buf[1];  // b == 4
	c = buf[2];  // c == 5
	cout << "a:" << a << ", b:" << b << ", c:" << c << "\n";
	
	buf.pop_back();  // 5 is removed
	buf.pop_front();  // 3 is removed
	
	int d = buf[0];  // d == 4
	cout << "d:" << d << "\n";
	
	return 0;
}