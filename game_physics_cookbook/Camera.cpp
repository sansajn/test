#include "Camera.h"

Camera::Camera()
{
	m_nFov = 60.0f;
	m_nAspect = 1.3f;
	m_nNear = 0.01f;
	m_nFar = 1000.0f;
	m_nWidth = 1.0f;
	m_nHeight = 1.0f;
	m_matWorld = mat4{};
	m_matProj = Projection(m_nFov, m_nAspect, m_nNear, m_nFar);
	m_nProjectionMode = 0;
}

mat4 Camera::GetWorldMatrix()
{
	return m_matWorld;
}

bool Camera::IsOrthoNormal()
{
	vec3 right = vec3{m_matWorld._11, m_matWorld._12, m_matWorld._13};
	vec3 up = vec3{m_matWorld._21, m_matWorld._22, m_matWorld._23};
	vec3 forward = vec3{m_matWorld._31, m_matWorld._32, m_matWorld._33};
	
	if (!CMP(Dot(right, right), 1.0f)
		return false;  // X axis is not normal
		
	if (!CMP(Dot(up, up), 1.0f))
		return false;  // Y axis is not normal
		
	if (!CMP(Dot(forward, forward), 1.0f)
		return false;  // Z axis is not normal
		
	if (!CMP(Dot(forward, up), 0.0f))
		return false;  // not perpendicular
		
	if (!CMP(Dot(forward, right), 0.0f))
		return false;  // not perpendicular
		
	if (!CMP(Dot(right, up), 0.0f))
		return false;  // not perpendicular
		
	return true;
}

void Camera::OrthoNormalize()
{
	vec3 right = vec3{m_matWorld._11, m_matWorld._12, m_matWorld._13};
	vec3 up = vec3{m_matWorld._21, m_matWorld._22, m_matWorld._23};
	vec3 forward = vec3{m_matWorld._31, m_matWorld._32, m_matWorld._33};
	
	vec3 f = Normalized(forward);
	vec3 r = Normalized(Cross(up, f));
	vec3 u = Cross(f, r);
	
	m_matWorld = mat4{
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		f.x, f.y, f.z, 0.0f,
		m_matWorld._41, m_matWorld._42, m_matWorld._43, 1.0f};
}

mat4 Camera::GetViewMatrix()
{
	// return Inverse(m_matWorld);
	
	if (!IsOrthoNormal())
		OrthoNormalize();
	
	mat4 inv = Transpose(m_matWorld);
	inv._41 = inv._14 = 0.0f;
	inv._42 = inv._24 = 0.0f;
	inv._43 = inv._34 = 0.0f;
	
	vec3 right = vec3{m_matWorld._11, m_matWorld._12, m_matWorld._13};
	vec3 up = vec3{m_matWorld._21, m_matWorld._22, m_matWorld._23};
	vec3 forward = vec3{m_matWorld._31, m_matWorld._32, m_matWorld._33};
	vec3 position = vec3{m_matWorld._41, m_matWorld._42, m_matWorld._43};
	
	inv._41 = -Dot(right, position);
	inv._42 = -Dot(up, position);
	inv._43 = -Dot(forward, position);
	
	return inv;
}

float Camera::GetAspect()
{
	return m_nAspect;
}

mat4 Camera::GetProjectionMatrix()
{
	return m_matProj;
}

void Camera::Resize(int width, int height)
{
	m_nAspect = (float)width / (float)height;
	
	if (m_nProjectionMode == 0)  // perspective
		m_matProj = Projection(m_nFov, m_nAspect, m_nNear, m_nFar);
	else if (m_nProjectionMode == 1)  // ortho
	{
		m_nWidth = (float)width;
		m_nHeight = (float)height;
		float halfW = m_nWidth * 0.5f;
		float halfH = m_nHeight * 0.5f;
		m_matProj = Ortho(-halfH, halfW, halfH, -halfH, m_nNear, m_nFar);
	}
	// m_nProjectionMode == 2
		// user defined
}

bool Camera::IsOrthographics()
{
	return m_nProjectionMode == 1;
}

bool Camera::IsPerspective()
{
	return m_nProjectionMode == 0;
}

void Camera::Perspective(float fov, float aspect, float zNear, float zFar)
{
	m_nFov = fov;
	m_nAspect = aspect;
	m_nNear = zNear;
	m_nFar = zFar;
	m_matProj = Projection(fov, aspect, zNear, zFar);
	m_nProjectionMode = 0;
}

void Camera::Orthographic(float width, float height, float zNear, float zFar)
{
	m_nWidth = width;
	m_nHeight = height;
	m_nNear = zNear;
	m_nFar = zFar;
	
	float halfW = m_nWidth * 0.5f;
	float halfH = m_nHeight * 0.5f;
	m_matProj = Ortho(-halfH, halfW, halfH, -halfH, zNear, zFar);
	
	m_nProjectionMode = 1;
}

void Camera::SetProjection(mat4 const & projection)
{
	m_matProj = projection;
	m_nProjectionMode = 2;
}

void Camera::SetWorld(mat4 const & view)
{
	m_matWorld = view;
}

Camera CreatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	Camera result;
	result.Perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	return result;
}

Camera CreateOrthographic(float width, float height, float nearPlane, float farPlane)
{
	Camera result;
	result.Orthographic(width, height, nearPlane, farPlane);
	return result;
}











