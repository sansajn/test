// kod generuje sachovnicu
#include <iostream>
using std::cout;

int main(int argc, char * argv[])
{
	for (int j = 0; j < 128; ++j)
	{
		for (int i = 0; i < 128; ++i)
			cout << (((i<=64 && j<=64) || (i>64 && j>64)) ? 1 : 0) << " ";
		cout << "\n";
	}
	return 0;
}
