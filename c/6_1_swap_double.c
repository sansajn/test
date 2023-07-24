// Listing 6.1: A funciton with an array parameter.
#include <stdio.h>

void swap_double(double a[static 2]) {
	double tmp = a[0];
	a[0] = a[1];
	a[1] = tmp;
}

int main() {
	double A[2] = {1.0, 2.0};
	swap_double(A);
	printf("A[0] = %g, A[1] = %g\n", A[0], A[1]);
	return 0;
}
