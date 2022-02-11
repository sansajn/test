// aku hodotu dava funkcia sin(pi) pre float, double a long double
#include <cmath>
#include <iostream>
using std::sin, std::cout;


int main(int argc, char * argv[]) {
	float yf = sin((float)M_PI);
	double yd = sin(M_PI);
	long double yl = sin(M_PIl);

	cout << "float: sin(pi)=" << yf << "\n" 
		<< "double: sin(pi)=" << yd << "\n"
		<< "long double: sin(pi)=" << yl << "\n";

	return 0;
}
