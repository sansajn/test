/* This example has been taken from MSDN bind function documentation. */

#include <functional>
using namespace std::placeholders;
#include <iostream>
using std::cout;
#include <algorithm>
using std::for_each;


void square(double x)
{
	cout << x << "^2=" << x*x << "\n";
}

void product(double x, double y)
{
	cout << x << "*" << y << "=" << x*y << "\n";
}


int main(int argc, char * argv[])
{
	double arg[] = {1, 2, 3};

	for_each(arg, arg + 3, square);
	cout << "\n";

	for_each(arg, arg + 3, std::bind(product, _1, 2));
	cout << "\n";

	for_each(arg, arg + 3, std::bind(square, _1));
	cout << "\n";

	return 0;
}
