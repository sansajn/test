#include <iostream>
using std::cout;

int main()
{
	int k = 20, i = 0, j = 19;
	delete &i;
	i = 15;
	cout << k << "," << i << ", " << j << "\n";
	return 0;
}
