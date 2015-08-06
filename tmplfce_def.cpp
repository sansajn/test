#include <iostream>
using std::cout;

template <typename R>
R fce(int idx = -1);

template <>
int fce<int>(int idx)
{
	return idx*101;
}

int main(int argc, char * argv[])
{
	int n = fce<int>();
	int m = fce<int>(2);
	cout << "n:" << n << "\n";
	cout << "m:" << m << "\n";
	return 0;	
}
