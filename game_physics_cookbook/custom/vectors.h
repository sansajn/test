#pragma once
#include <ostream>

#ifndef RAD2DEG
	float RAD2DEG(float radians);
#endif
	
#ifndef DEG2RAD
	float DEG2RAD(float degrees);
#endif

float CorrectDegrees(float degrees);
	
	
struct vec2
{
	union {
		struct
		{
			float x, y;
		};
		float asArray[2];
	};
	
	vec2() : x{0.0f}, y{0.0f} {}
	vec2(float x, float y) : x{x}, y{y} {}
	float & operator[](int i) {return asArray[i];}
};

struct vec3
{
	union {
		struct
		{
			float x, y, z;
		};
		float asArray[3];
	};
	
	vec3() : x{0.0f}, y{0.0f}, z{0.0f} {}
	vec3(float x, float y, float z) : x{x}, y{y}, z{z} {}
	float & operator[](int i) {return asArray[i];}
};

vec2 operator+(vec2 const & l, vec2 const & r);
vec3 operator+(vec3 const & l, vec3 const & r);

vec2 operator-(vec2 const & l, vec2 const & r);
vec3 operator-(vec3 const & l, vec3 const & r);

vec2 operator*(vec2 const & l, vec2 const & r);
vec3 operator*(vec3 const & l, vec3 const & r);

vec2 operator*(vec2 const & l, float r);
vec3 operator*(vec3 const & l, float r);

vec2 operator/(vec2 const & l, vec2 const & r);
vec3 operator/(vec3 const & l, vec3 const & r);

vec2 operator/(vec2 const & l, float r);
vec3 operator/(vec3 const & l, float r);

std::ostream & operator<<(std::ostream & os, vec2 const & m);
std::ostream & operator<<(std::ostream & os, vec3 const & m);

bool operator==(vec2 const & l, vec2 const & r);
bool operator==(vec3 const & l, vec3 const & r);

bool operator!=(vec2 const & l, vec2 const & r);
bool operator!=(vec3 const & l, vec3 const & r);

vec2 & operator+=(vec2 & l, vec2 const & r);
vec2 & operator-=(vec2 & l, vec2 const & r);
vec2 & operator*=(vec2 & l, vec2 const & r);
vec2 & operator*=(vec2 & l, float r);
vec2 & operator/=(vec2 & l, vec2 const & r);
vec2 & operator/=(vec2 & l, float r);

vec3 & operator+=(vec3 & l, vec3 const & r);
vec3 & operator-=(vec3 & l, vec3 const & r);
vec3 & operator*=(vec3 & l, vec3 const & r);
vec3 & operator*=(vec3 & l, float r);
vec3 & operator/=(vec3 & l, vec3 const & r);
vec3 & operator/=(vec3 & l, float r);

float Dot(vec2 const & l, vec2 const & r);
float Dot(vec3 const & l, vec3 const & r);

float Magnitude(vec2 const & v);
float Magnitude(vec3 const & v);

float MagnitudeSq(vec2 const & v);
float MagnitudeSq(vec3 const & v);

float Distance(vec2 const & p1, vec2 const & p2);
float Distance(vec3 const & p1, vec3 const & p2);

float DistanceSq(vec2 const & p1, vec2 const & p2);
float DistanceSq(vec3 const & p1, vec3 const & p2);

vec2 RotateVector(vec2 const & vector, float degrees);  //!< okolo osi z (pre 2D vektor ine nema zmysel)

void Normalize(vec2 & v);
void Normalize(vec3 & v);

vec2 Normalized(vec2 const & v);
vec3 Normalized(vec3 const & v);

vec3 Cross(vec3 const & l, vec3 const & r);

float Angle(vec2 const & l, vec2 const & r);
float Angle(vec3 const & l, vec3 const & r);

vec2 Project(vec2 const & length, vec2 const & direction);
vec3 Project(vec3 const & length, vec3 const & direction);

vec2 Perpendicular(vec2 const & length, vec2 const & direction);  // kolme
vec3 Perpendicular(vec3 const & length, vec3 const & direction);

vec2 Reflection(vec2 const & sourceVector, vec2 const & normal);
vec3 Reflection(vec3 const & sourceVector, vec3 const & normal);


