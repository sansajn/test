#include <memory>
#include <iostream>
using std::shared_ptr;
using std::cout;

int main(int argc, char * argv[])
{
	shared_ptr<int> a(new int);
	shared_ptr<int> b = a;

	a.reset();

	if (!a)
		cout << "a is empty\n";
	else
		cout << "a is not empty\n";	

	return 0;
}
