/* transformácia pointra na ukazateľ a jeho odstránenie */
#include <iostream>
using std::cout;

int main()
{
	int * p = new int(15);	
	cout << "p address: " << (void *)(p) << "\n";

	int & r = *p;
	cout << "r address: " << (void *)(&r) << "\n";

	delete &r;  // je to v pohode ?

	return 0;	
}
