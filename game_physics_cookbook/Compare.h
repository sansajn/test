#pragma once
#include <cfloat>


bool AlmostEqualRelativeAndAbs(float a, float b, float maxDiff, float maxRelDiff = FLT_EPSILON);

#define CMP(x, y) AlmostEqualRelativeAndAbs(x, y, 0.005f)
