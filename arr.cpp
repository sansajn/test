#include <iostream>
using std::cout;


void foo(int arr[], int n)
{
	for (int i = 0; i < n; ++i)
		cout << arr[i] << " ";
	cout << "\n";
}

void goo(int * arr, int n)
{
	for (int i = 0; i < n; ++i)
		cout << *arr++ << " ";
	cout << "\n";
}

int main() 
{
	int data[] = {9, 4, 5, 1};
	foo(data, 4);
	goo(data, 4);

	int ** arr = new int*[10];

	int i, j, k;
	arr[0] = &i;
	arr[1] = &j;
	arr[2] = &k;

	delete [] arr;


	return 0;
}

