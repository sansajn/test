#include <iostream>

using std::cout;

int main(int argc, char * argv[])
{
	// work
	int data[] = {1, 2, 3, 4};
	for (int & d : data)
		cout << d << " ";
	cout << std::endl;

	// not working
	int * dyndata = new int[10];
//	for (int & d : dyndata)
//		d = 5;
	delete [] dyndata;

	return 0;
}	
