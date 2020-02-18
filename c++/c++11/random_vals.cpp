#include <random>
#include <iostream>

using std::random_device;
using std::default_random_engine;
using std::cout;

int main(int argc, char * argv[])
{
	random_device rd;
	default_random_engine rand{rd()};
	
	for (int i = 0; i < 10; ++i)
		cout << 1 + (rand() % 10) << ", ";  // from 1 to 10
	
	cout << "\ndone!\n";
	
	return 0;
}
