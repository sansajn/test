#include "vectors.h"
#include <cmath>
#include <cfloat>

using std::ostream;

float CorrectDegrees(float degrees)
{
	while (degrees > 360.0f)
		degrees -= 360.0f;
	while (degrees < -360.0f)
		degrees += 360.0f;
	return degrees;
}

#ifndef RAD2DEG
float RAD2DEG(float radians)
{
	float degrees = radians * 57.295754f;
	degrees = CorrectDegrees(degrees);
	return degrees;
}
#endif

#ifndef DEG2RAD
float DEG2RAD(float degrees)
{
	degrees = CorrectDegrees(degrees);
	float radians = degrees * 0.0174533f;
	return radians;
}
#endif

bool operator==(vec2 const & l, vec2 const & r)
{
	return CMP(l.x, r.x) && CMP(l.y, r.y);
}

bool operator==(const vec3 & l, const vec3 & r)
{
	return CMP(l.x, r.x) && CMP(l.y, r.y) && CMP(l.z, r.z);
}

bool operator!=(const vec2 & l, const vec2 & r)
{
	return !(l == r);
}

bool operator!=(const vec3 & l, const vec3 & r)
{
	return !(l == r);
}

vec2 operator+(const vec2 & l, const vec2 & r)
{
	return {l.x + r.x, l.y + r.y};
}

vec3 operator+(const vec3 & l, const vec3 & r)
{
	return {l.x + r.x, l.y + r.y, l.z + r.z};
}

vec2 operator-(const vec2 & l, const vec2 & r)
{
	return {l.x - r.x, l.y - r.y};
}

vec3 operator-(const vec3 & l, const vec3 & r)
{
	return {l.x - r.x, l.y - r.y, l.z - r.z};
}

vec2 operator*(const vec2 & l, const vec2 & r)
{
	return {l.x * r.x, l.y * l.x};
}

vec3 operator*(const vec3 & l, const vec3 & r)
{
	return {l.x * r.x, l.y * r.y, l.z * r.z};
}

vec2 operator*(const vec2 & l, float r)
{
	return {l.x * r, l.y * r};
}

vec3 operator*(const vec3 & l, float r)
{
	return {l.x * r, l.y * r, l.z * r};
}

vec2 operator/(const vec2 & l, const vec2 & r)
{
	return {l.x / r.x, l.y / r.y};
}

vec3 operator/(const vec3 & l, const vec3 & r)
{
	return {l.x / r.x, l.y / r.y, l.z / r.z};
}

vec2 operator/(const vec2 & l, float r)
{
	return {l.x / r, l.y / r};
}

vec3 operator/(const vec3 & l, float r)
{
	return {l.x / r, l.y / r, l.z / r};
}

ostream & operator<<(ostream & os, vec2 const & m)
{
	os << "(" << m.x << ", " << m.y << ")";
	return os;
}

ostream & operator<<(ostream & os, vec3 const & m)
{
	os << "(" << m.x << ", " << m.y << ", " << m.z << ")";
	return os;
}

float Dot(const vec2 & l, const vec2 & r)
{
	return l.x * r.x + l.y * r.y;
}

float Dot(const vec3 & l, const vec3 & r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

vec2 & operator+=(vec2 & l, vec2 const & r)
{
	l.x += r.x;
	l.y += r.y;
	return l;
}

vec2 & operator-=(vec2 & l, vec2 const & r)
{
	l.x -= r.x;
	l.y -= r.y;
	return l;
}

vec2 & operator*=(vec2 & l, vec2 const & r)
{
	l.x *= r.x;
	l.y *= r.y;
	return l;
}

vec2 & operator*=(vec2 & l, float r)
{
	l.x *= r;
	l.y *= r;
	return l;
}

vec2 & operator/=(vec2 & l, vec2 const & r)
{
	l.x /= r.x;
	l.y /= r.y;
	return l;
}

vec2 & operator/=(vec2 & l, float r)
{
	l.x /= r;
	l.y /= r;
	return l;
}

vec3 & operator+=(vec3 & l, vec3 const & r)
{
	l.x += r.x;
	l.y += r.y;
	l.z += r.z;
	return l;
}

vec3 & operator-=(vec3 & l, vec3 const & r)
{
	l.x -= r.x;
	l.y -= r.y;
	l.z -= r.z;
	return l;
}

vec3 & operator*=(vec3 & l, vec3 const & r)
{
	l.x *= r.x;
	l.y *= r.y;
	l.z *= r.z;
	return l;
}

vec3 & operator*=(vec3 & l, float r)
{
	l.x *= r;
	l.y *= r;
	l.z *= r;
	return l;
}

vec3 & operator/=(vec3 & l, vec3 const & r)
{
	l.x /= r.x;
	l.y /= r.y;
	l.z /= r.z;
	return l;
}

vec3 & operator/=(vec3 & l, float r)
{
	l.x /= r;
	l.y /= r;
	l.z /= r;
	return l;
}

float Magnitude(vec2 const & v)
{
	return sqrtf(Dot(v,v));
}

float Magnitude(vec3 const & v)
{
	return sqrtf(Dot(v,v));
}

float MagnitudeSq(vec2 const & v)
{
	return Dot(v,v);
}

float MagnitudeSq(vec3 const & v)
{
	return Dot(v,v);
}

float Distance(vec2 const & p1, vec2 const & p2)
{
	return Magnitude(p1 - p2);
}

float Distance(vec3 const & p1, vec3 const & p2)
{
	return Magnitude(p1 - p2);
}

float DistanceSq(const vec2 & p1, const vec2 & p2)
{
	return MagnitudeSq(p1 - p2);
}

float DistanceSq(const vec3 & p1, const vec3 & p2)
{
	return MagnitudeSq(p1 - p2);
}

vec2 RotateVector(const vec2 & vector, float degrees)
{
	degrees = DEG2RAD(degrees);
	float s = sinf(degrees);
	float c = cosf(degrees);
	return vec2{
		vector.x * c - vector.y * s,
		vector.x * s + vector.y * c
	};
}

void Normalize(vec2 & v)
{
	v = v * (1.0f / Magnitude(v));
}

void Normalize(vec3 & v)
{
	v = v * (1.0f / Magnitude(v));
}

vec2 Normalized(vec2 const & v)
{
	return v * (1.0f / Magnitude(v));
}

vec3 Normalized(vec3 const & v)
{
	return v * (1.0f / Magnitude(v));
}

vec3 Cross(vec3 const & l, vec3 const & r)
{
	vec3 result;
	result.x = l.y * r.z - l.z * r.y;
	result.y = l.z * r.x - l.x * r.z;
	result.z = l.x * r.y - l.y * r.x;
	return result;
}

float Angle(vec2 const & l, vec2 const & r)
{
	return acosf(Dot(l,r) / sqrtf(MagnitudeSq(l) * MagnitudeSq(r)));
}

float Angle(vec3 const & l, vec3 const & r)
{
	return acosf(Dot(l,r) / sqrtf(MagnitudeSq(l) * MagnitudeSq(r)));
}

vec2 Project(vec2 const & length, vec2 const & direction)
{
	float dot = Dot(length, direction);
	float magSq = MagnitudeSq(direction);
	return direction * (dot / magSq);
}

vec3 Project(vec3 const & length, vec3 const & direction)
{
	float dot = Dot(length, direction);
	float magSq = MagnitudeSq(direction);
	return direction * (dot / magSq);
}



















