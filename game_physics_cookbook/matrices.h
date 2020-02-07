#pragma once


/*!
Row Major!


TODO: dopln popis z originalu */

#include "vectors.h"

struct mat3
{
	union {
		struct
		{
			float _11, _12, _13,
				_21, _22, _23,
				_31, _32, _33;
		};
		float asArray[9];
	};
	
	mat3()
	{
		_11 = _22 = _33 = 1.0f;
		_12 = _13 = _21 = 0.0f;
		_23 = _31 = _32 = 0.0f;
	}
	
	mat3(float f11, float f12, float f13,
		float f21, float f22, float f23,
		float f31, float f32, float f33)
	{
		_11 = f11; _12 = f12; _13 = f13;
		_21 = f21; _22 = f22; _23 = f23;
		_31 = f31; _32 = f32; _33 = f33;
	}
	
	float * operator[](int i)
	{
		return &(asArray[i*3]);
	}
};

struct mat4
{
	union {
		struct
		{
			float _11, _12, _13, _14,
				_21, _22, _23, _24,
				_31, _32, _33, _34,
				_41, _42, _43, _44;
		};
		float asArray[16];
	};
	
	mat4()
	{
		_11 = _22 = _33 = _44 = 1.0f;
		_12 = _13 = _14 = _21 = 0.0f;
		_23 = _24 = _31 = _32 = 0.0f;
		_34 = _41 = _42 = _43 = 0.0f;
	}
	
	mat4(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44)
	{
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
	}
	
	float * operator[](int i)
	{
		return &(asArray[i*4]);
	}
};

bool operator==(mat4 const & l, mat4 const & r);

bool operator!=(mat4 const & l, mat4 const & r);

std::ostream operator<<(std::ostream & os, mat4 const & m);

mat4 Transpose(mat4 const & matrix);

mat4 operator*(mat4 const & matrix, float scalar);

mat4 operator*(mat4 const & matrixA, mat4 const & matrixB);

mat4 Minor(mat4 const & mat);
mat4 Cofactor(mat4 const & mat);
float Determinant(mat4 const & mat);
mat4 Adjugate(mat4 const & mat);
mat4 Inverse(mat4 const & mat);

mat4 ToColumnMajor(mat4 const & mat);
mat4 FromColumnMajor(mat4 const & mat);

mat4 Translation(float x, float y, float z);
mat4 Translation(vec3 const & pos);
vec3 GetTranslation(mat4 const & mat);

mat4 Translate(float x, float y, float z);
mat4 Translate(vec3 const & pos);

mat4 Scale(float x, float y, float z);
mat4 Scale(vec3 const & vec);
vec3 GetScale(mat4 const & mat);

mat4 Rotation(float pitch, float yaw, float roll);  // X, Y, Z
mat3 Rotation3x3(float pitch, float yaw, float roll);  // X, Y, Z
mat4 YawPitchRoll(float yaw, float pitch, float roll);  // X, Y, Z

mat4 XRotation(float angle);
mat4 YRotation(float angle);
mat4 ZRotation(float angle);

mat4 Orthogonalize(mat4 const & mat);

mat4 AxisAngle(vec3 const & axis, float angle);

vec3 MultiplyVector(vec3 const & vec, mat4 const & mat);
vec3 MultiplyVector(vec3 const & vec, mat3 const & mat);

mat4 Transform(vec3 const & scale, vec3 const & eulerRotation, vec3 const & translate);
mat4 Transform(vec3 const & scale, vec3 const & rotationAxis, float rotationAngle, vec3 const & translate);

mat4 LookAt(vec3 const & position, vec3 const & target, vec3 const & up);
mat4 Projection(float fov, float aspect, float zNear, float zFar);
mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

mat4 FastInverse(mat4 const & mat);
