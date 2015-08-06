// dereferencovanie pointra na pointer (int**)
#include <iostream>

using std::cout;

int main(int argc, char * argv[])
{
	int * i = nullptr;
	int ** pi = &i;
	int * j = *pi;
	if (j == nullptr)
		cout << "j is nullptr\n";
	return 0;
}
