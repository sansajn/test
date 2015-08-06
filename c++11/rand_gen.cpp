// použitie random
#include <random>
#include <ctime>
#include <iostream>

using std::cout;

int main(int argc, char * argv[])
{
	std::default_random_engine rand(std::time(0));
	for (int i = 0; i < 10; ++i)
		cout << rand() << ", ";
	cout << "\n";
	return 0;
}

