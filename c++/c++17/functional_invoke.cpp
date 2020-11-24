// invoke sample

#include <functional>
#include <iostream>
using std::invoke, std::cout;

int foo(int a, int b) {return a+b;}

struct animal
{
	int size = 0;

	void make_sound(double level)
	{
		cout << "some sound at level " << level << "\n";
	}
};


int main(int argc, char * argv[])
{
	// a regular function
	cout << invoke(foo, 10, 1) << "\n";

	// a lambda
	cout << invoke([](double a){return a*10.0;}, 4.2) << "\n";

	// a member function (with reference)
	animal zebra;
	invoke(&animal::make_sound, zebra, 12.2);
	
	// a member function (with pointer)
	invoke(&animal::make_sound, &zebra, 12.3);

	// a variable member
	animal fox{3};
	cout << invoke(&animal::size, fox) << "\n";

	cout << "done!\n";

	return  0;
}
