#include <iostream>
using std::cout;

struct data
{
	unsigned int node_iterator;
	bool stalled : 1;
};

int main(int argc, char * argv[])
{
	data d;
	cout << "sizeof(data)=" << sizeof(data) << "B\n";
	return 0;
}

