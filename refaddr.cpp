/* (?) má referencia rovnakú adresu ako premenná na ktorú sa odkazuje ? 
A: ano ma. */
#include <iostream>
using std::cout;


int main()
{
	int i = 15;
	int & r = i;
	cout << "i address: " << (void *)&i << "\n";
	cout << "r address: " << (void *)&r << "\n";

	int * p = new int(10);
	int & s = *p;
	cout << "p address: " << (void *)p << "\n";
	cout << "s address: " << (void *)&s << "\n";

	return 0;
}
