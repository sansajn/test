#include <cmath>
#include "Compare.h"
#include "matrices.h"

using std::ostream;

bool operator==(mat4 const & l, mat4 const & r)
{
	// TODO: this must be pretty slow due to [] operator in every loop iteration
	for (int i = 0; i < 16; ++i)
		if (!CMP(l.asArray[i], r.asArray[i]))
			return false;
	return true;
}

bool operator!=(mat4 const & l, mat4 const & r)
{
	return !(l == r);
}

ostream & operator<<(ostream & os, mat4 const & m)
{
	return os << m._11 << ", " << m._12 << ", " << m._13 << ", " << m._14 << "\n"
		<< m._21 << ", " << m._22 << ", " << m._23 << ", " << m._24 << "\n"
		<< m._31 << ", " << m._32 << ", " << m._33 << ", " << m._34 << "\n"
		<< m._41 << ", " << m._42 << ", " << m._43 << ", " << m._44;
}

mat4 FastInverse(mat4 const & mat)
{
	mat4 inv = Transpose(mat);
	inv._41 = inv._14 = 0.0f;
	inv._42 = inv._24 = 0.0f;
	inv._43 = inv._34 = 0.0f;
	
	vec3 right = vec3{mat._11, mat._12, mat._13};
	vec3 up = vec3{mat._21, mat._22, mat._23};
	vec3 forward = vec3{mat._31, mat._32, mat._33};
	vec3 position = vec3{mat._41, mat._42, mat._43};
	
	inv._41 = -Dot(right, position);
	inv._42 = -Dot(up, position);
	inv._43 = -Dot(forward, position);
	
	return inv;	
}

void Transpose(float const * srcMat, float * dstMat, int srcRows, int srcCols)
{
	for (int i = 0; i < srcRows * srcCols; ++i)
	{
		int row = i / srcRows;
		int col = i % srcRows;
		dstMat[i] = srcMat[srcCols * col + row];
	}
}

mat4 Transpose(mat4 const & matrix)
{
	mat4 result;
	Transpose(matrix.asArray, result.asArray, 4, 4);
	return result;
}

mat4 operator*(mat4 const & matrix, float scalar)
{
	mat4 result;
	for (int i = 0; i < 16; ++i)
		result.asArray[i] = matrix.asArray[i] * scalar;
	return result;
}

bool Multiply(float * out, float const * matA, int aRows, int aCols, float const * matB, int bRows, int bCols)
{
	if (aCols != bRows)
		return false;
	
	for (int i = 0; i < aRows; ++i)
	{
		for (int j = 0; j < bCols; ++j)
		{
			out[bCols * i + j] = 0.0f;
			for (int k = 0; k < bRows; ++k)
				out[bCols * i + j] += matA[aCols * i + k] * matB[bCols * k + j];
		}
	}
	
	return true;
}

mat4 operator*(mat4 const & matrixA, mat4 const & matrixB)
{
	mat4 result;
	Multiply(result.asArray, matrixA.asArray, 4, 4, matrixB.asArray, 4, 4);
	return result;
}

mat2 Cut(mat3 const & mat, int row, int col)
{
	mat2 result;
	int index = 0;
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i == row || j == col)
				continue;
			result.asArray[index++] = mat.asArray[3*i + j];
		}
	}
	
	return result;
}

mat3 Cut(mat4 const & mat, int row, int col)
{
	mat3 result;
	int index = 0;
	
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i == row || j == col)
				continue;
			result.asArray[index++] = mat.asArray[4*i + j];
		}
	}
	
	return result;
}

void Cofactor(float * out, float const * minor, int rows, int cols)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			out[cols * j + i] = minor[cols * j + i] * powf(-1.0f, i + j);
}

float Determinant(mat2 const & mat)
{
	return mat._11 * mat._22 - mat._12 * mat._21;
}

mat3 Minor(mat3 const & mat)
{
	mat3 result;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			result[i][j] = Determinant(Cut(mat, i, j));
	return result;
}

mat3 Cofactor(mat3 const & mat)
{
	mat3 result;
	Cofactor(result.asArray, Minor(mat).asArray, 3, 3);
	return result;
}

float Determinant(mat3 const & mat)
{
	float result = 0.0f;
	mat3 cofactor = Cofactor(mat);
	for (int j = 0; j < 3; ++j)
		result += mat.asArray[3 * 0 + j] * cofactor[0][j];
	return result;
}

mat4 Minor(mat4 const & mat)
{
	mat4 result;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result[i][j] = Determinant(Cut(mat, i, j));
	return result;
}

mat4 Cofactor(mat4 const & mat)
{
	mat4 result;
	Cofactor(result.asArray, Minor(mat).asArray, 4, 4);
	return result;
}

float Determinant(mat4 const & mat)
{
	float result = 0.0f;
	
	mat4 cofactor = Cofactor(mat);
	for (int j = 0; j < 4; ++j)
		result += mat.asArray[4 * 0 + j] * cofactor[0][j];
	
	return result;
}

mat4 Adjugate(mat4 const & mat)
{
	return Transpose(Cofactor(mat));
}

mat4 Inverse(mat4 const & m) {
	/*float det = Determinant(m);
	if (CMP(det, 0.0f)) { return mat4(); }
	return Adjugate(m) * (1.0f / det);*/

	// The code below is the expanded form of the above equation.
	// This optimization avoids loops and function calls

	float det 
		= m._11 * m._22 * m._33 * m._44 + m._11 * m._23 * m._34 * m._42 + m._11 * m._24 * m._32 * m._43
		+ m._12 * m._21 * m._34 * m._43 + m._12 * m._23 * m._31 * m._44 + m._12 * m._24 * m._33 * m._41
		+ m._13 * m._21 * m._32 * m._44 + m._13 * m._22 * m._34 * m._41 + m._13 * m._24 * m._31 * m._42
		+ m._14 * m._21 * m._33 * m._42 + m._14 * m._22 * m._31 * m._43 + m._14 * m._23 * m._32 * m._41
		- m._11 * m._22 * m._34 * m._43 - m._11 * m._23 * m._32 * m._44 - m._11 * m._24 * m._33 * m._42
		- m._12 * m._21 * m._33 * m._44 - m._12 * m._23 * m._34 * m._41 - m._12 * m._24 * m._31 * m._43
		- m._13 * m._21 * m._34 * m._42 - m._13 * m._22 * m._31 * m._44 - m._13 * m._24 * m._32 * m._41
		- m._14 * m._21 * m._32 * m._43 - m._14 * m._22 * m._33 * m._41 - m._14 * m._23 * m._31 * m._42;

	if (CMP(det, 0.0f))
		return mat4{}; 
	
	float i_det = 1.0f / det;

	mat4 result;
	result._11 = (m._22 * m._33 * m._44 + m._23 * m._34 * m._42 + m._24 * m._32 * m._43 - m._22 * m._34 * m._43 - m._23 * m._32 * m._44 - m._24 * m._33 * m._42) * i_det;
	result._12 = (m._12 * m._34 * m._43 + m._13 * m._32 * m._44 + m._14 * m._33 * m._42 - m._12 * m._33 * m._44 - m._13 * m._34 * m._42 - m._14 * m._32 * m._43) * i_det;
	result._13 = (m._12 * m._23 * m._44 + m._13 * m._24 * m._42 + m._14 * m._22 * m._43 - m._12 * m._24 * m._43 - m._13 * m._22 * m._44 - m._14 * m._23 * m._42) * i_det;
	result._14 = (m._12 * m._24 * m._33 + m._13 * m._22 * m._34 + m._14 * m._23 * m._32 - m._12 * m._23 * m._34 - m._13 * m._24 * m._32 - m._14 * m._22 * m._33) * i_det;
	result._21 = (m._21 * m._34 * m._43 + m._23 * m._31 * m._44 + m._24 * m._33 * m._41 - m._21 * m._33 * m._44 - m._23 * m._34 * m._41 - m._24 * m._31 * m._43) * i_det;
	result._22 = (m._11 * m._33 * m._44 + m._13 * m._34 * m._41 + m._14 * m._31 * m._43 - m._11 * m._34 * m._43 - m._13 * m._31 * m._44 - m._14 * m._33 * m._41) * i_det;
	result._23 = (m._11 * m._24 * m._43 + m._13 * m._21 * m._44 + m._14 * m._23 * m._41 - m._11 * m._23 * m._44 - m._13 * m._24 * m._41 - m._14 * m._21 * m._43) * i_det;
	result._24 = (m._11 * m._23 * m._34 + m._13 * m._24 * m._31 + m._14 * m._21 * m._33 - m._11 * m._24 * m._33 - m._13 * m._21 * m._34 - m._14 * m._23 * m._31) * i_det;
	result._31 = (m._21 * m._32 * m._44 + m._22 * m._34 * m._41 + m._24 * m._31 * m._42 - m._21 * m._34 * m._42 - m._22 * m._31 * m._44 - m._24 * m._32 * m._41) * i_det;
	result._32 = (m._11 * m._34 * m._42 + m._12 * m._31 * m._44 + m._14 * m._32 * m._41 - m._11 * m._32 * m._44 - m._12 * m._34 * m._41 - m._14 * m._31 * m._42) * i_det;
	result._33 = (m._11 * m._22 * m._44 + m._12 * m._24 * m._41 + m._14 * m._21 * m._42 - m._11 * m._24 * m._42 - m._12 * m._21 * m._44 - m._14 * m._22 * m._41) * i_det;
	result._34 = (m._11 * m._24 * m._32 + m._12 * m._21 * m._34 + m._14 * m._22 * m._31 - m._11 * m._22 * m._34 - m._12 * m._24 * m._31 - m._14 * m._21 * m._32) * i_det;
	result._41 = (m._21 * m._33 * m._42 + m._22 * m._31 * m._43 + m._23 * m._32 * m._41 - m._21 * m._32 * m._43 - m._22 * m._33 * m._41 - m._23 * m._31 * m._42) * i_det;
	result._42 = (m._11 * m._32 * m._43 + m._12 * m._33 * m._41 + m._13 * m._31 * m._42 - m._11 * m._33 * m._42 - m._12 * m._31 * m._43 - m._13 * m._32 * m._41) * i_det;
	result._43 = (m._11 * m._23 * m._42 + m._12 * m._21 * m._43 + m._13 * m._22 * m._41 - m._11 * m._22 * m._43 - m._12 * m._23 * m._41 - m._13 * m._21 * m._42) * i_det;
	result._44 = (m._11 * m._22 * m._33 + m._12 * m._23 * m._31 + m._13 * m._21 * m._32 - m._11 * m._23 * m._32 - m._12 * m._21 * m._33 - m._13 * m._22 * m._31) * i_det;

	return result;
}

mat4 ToColumnMajor(mat4 const & mat)
{
	return Transpose(mat);
}

mat4 FromColumnMajor(const mat4 & mat)
{
	return Transpose(mat);
}

mat4 Translation(float x, float y, float z)
{
	return mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		   x,    y,    z, 1.0f};
}

mat4 Translation(const vec3 & pos)
{
	return mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f};
}

mat4 Translate(float x, float y, float z)
{
	return mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		   x,    y,    z, 1.0f};
}

mat4 Translate(vec3 const & pos)
{
	return mat4{
		 1.0f,  0.0f,  0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f, 0.0f,
		 0.0f,  0.0f,  1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f};
}

vec3 GetTranslation(mat4 const & mat)
{
	return vec3{mat._41, mat._42, mat._43};
}

mat4 Scale(float x, float y, float z)
{
	return mat4{
		   x, 0.0f, 0.0f, 0.0f,
		0.0f,    y, 0.0f, 0.0f,
		0.0f, 0.0f,    z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

mat4 Scale(vec3 const & vec)
{
	return mat4{
		vec.x,  0.0f,  0.0f, 0.0f,
		 0.0f, vec.y,  0.0f, 0.0f,
		 0.0f,  0.0f, vec.z, 0.0f,
		 0.0f,  0.0f,  0.0f, 1.0f};
}

vec3 GetScale(mat4 const & mat)
{
	return vec3{mat._11, mat._22, mat._33};
}

mat4 Rotation(float pitch, float yaw, float roll)
{
	return ZRotation(roll) * XRotation(pitch) * YRotation(yaw);
}

mat4 YawPitchRoll(float yaw, float pitch, float roll)
{
	yaw = DEG2RAD(yaw);
	pitch = DEG2RAD(pitch);
	roll = DEG2RAD(roll);

	mat4 out; // z * x * y
	out._11 = (cosf(roll) * cosf(yaw)) + (sinf(roll) * sinf(pitch) * sinf(yaw));
	out._12 = (sinf(roll) * cosf(pitch));
	out._13 = (cosf(roll) * -sinf(yaw)) + (sinf(roll) * sinf(pitch) * cosf(yaw));
	out._21 = (-sinf(roll) * cosf(yaw)) + (cosf(roll) * sinf(pitch) * sinf(yaw));
	out._22 = (cosf(roll) * cosf(pitch));
	out._23 = (sinf(roll) * sinf(yaw)) + (cosf(roll) * sinf(pitch) * cosf(yaw));
	out._31 = (cosf(pitch) * sinf(yaw));
	out._32 = -sinf(pitch);
	out._33 = (cosf(pitch) * cosf(yaw));
	out._44 = 1;
	return out;
}

mat4 XRotation(float angle)
{
	angle = DEG2RAD(angle);
	return mat4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,  cosf(angle), sinf(angle), 0.0f,
		0.0f, -sinf(angle), cosf(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

mat4 YRotation(float angle)
{
	angle = DEG2RAD(angle);
	return mat4{
		cosf(angle), 0.0f, -sinf(angle), 0.0f,
		       0.0f, 1.0f,         0.0f, 0.0f,
		sinf(angle), 0.0f,  cosf(angle), 0.0f,
		       0.0f, 0.0f,         0.0f, 1.0f
	};
}

mat4 ZRotation(float angle)
{
	angle = DEG2RAD(angle);
	return mat4{
		 cosf(angle), sinf(angle), 0.0f, 0.0f,
		-sinf(angle), cosf(angle), 0.0f, 0.0f,
				  0.0f,        0.0f, 1.0f, 0.0f,
		        0.0f,        0.0f, 0.0f, 1.0f};
}

mat4 Orthogonalize(mat4 const & mat)
{
	vec3 xAxis{mat._11, mat._12, mat._13};
	vec3 yAxis{mat._21, mat._22, mat._23};
	vec3 zAxis = Cross(xAxis, yAxis);
	
	xAxis = Cross(yAxis, zAxis);
	yAxis = Cross(zAxis, xAxis);
	zAxis = Cross(xAxis, yAxis);
	
	return mat4{
		xAxis.x, xAxis.y, xAxis.z, mat._14,
		yAxis.x, yAxis.y, yAxis.z, mat._24,
		zAxis.x, zAxis.y, zAxis.z, mat._34,
		mat._41, mat._42, mat._43, mat._44};
}

mat4 AxisAngle(vec3 const & axis, float angle)
{
	angle = DEG2RAD(angle);
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - cosf(angle);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	if (!CMP(MagnitudeSq(axis), 1.0f)) {
		float inv_len = 1.0f / Magnitude(axis);
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
	}

	return mat4{
		t * (x * x) + c, t * x * y + s * z, t * x * z - s * y, 0.0f,
		t * x * y - s * z, t * (y * y) + c, t * y * z + s * x, 0.0f,
		t * x * z + s * y, t * y * z - s * x, t * (z * z) + c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

mat4 Transform(vec3 const & scale, vec3 const & eulerRotation, vec3 const & translate)
{
	return Scale(scale) * Rotation(eulerRotation.x, eulerRotation.y, eulerRotation.z) * Translation(translate);
}

mat4 Transform(vec3 const & scale, vec3 const & rotationAxis, float rotationAngle, vec3 const & translate)
{
	return Scale(scale) * AxisAngle(rotationAxis, rotationAngle) * Translation(translate);
}

mat4 LookAt(vec3 const & position, vec3 const & target, vec3 const & up)
{
	vec3 forward = Normalized(target - position);
	vec3 right = Normalized(Cross(up, forward));
	vec3 newUp = Cross(forward, right);
	
	return mat4{
		right.x, newUp.x, forward.x, 0.0f,
		right.y, newUp.y, forward.y, 0.0f,
		right.z, newUp.z, forward.z, 0.0f,
		-Dot(right, position), -Dot(newUp, position), -Dot(forward, position), 1.0f};
}

mat4 Projection(float fov, float aspect, float zNear, float zFar)
{
	float tanHalfFov = tanf(DEG2RAD((fov * 0.5f)));
	float fovY = 1.0f /tanHalfFov;  // cot(fov/2)
	float fovX = fovY / aspect;  // cot(fov/2) / aspect
	
	mat4 result;
	result._11 = fovX;
	result._22 = fovY;
	result._33 = zFar / (zFar - zNear);  // far / range
	result._34 = 1.0f;
	result._43 = -zNear * result._33;  // - neat * (far / range)
	result._44 = 0.0f;
	
	return result;
}

mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float _11 = 2.0f / (right - left);
	float _22 = 2.0f / (top - bottom);
	float _33 = 1.0f / (zFar - zNear);
	float _41 = (left + right) / (left - right);
	float _42 = (top + bottom) / (bottom - top);
	float _43 = (zNear) / (zNear - zFar);
	
	return mat4{
		 _11, 0.0f, 0.0f, 0.0f,
		0.0f,  _22, 0.0f, 0.0f,
		0.0f, 0.0f,  _33, 0.0f,
		 _41,  _42,  _43, 1.0f};
}
