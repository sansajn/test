#include <cmath>
#include "Compare.h"

bool AlmostEqualRelativeAndAbs(float a, float b, float maxDiff, float maxRelDiff)
{
	// check if the numbers are really close (needed when comparing numbers near zero)
	float diff = fabs(a - b);
	if (diff <= maxDiff)
		return true;
	
	a = fabs(a);
	b = fabs(b);
	float largest = (b > a) ? b : a;
	
	if (diff <= largest * maxRelDiff)
		return true;
	
	return false;
}
