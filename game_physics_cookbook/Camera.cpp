#include <iostream>
#include "Compare.h"
#include "Camera.h"

using std::cout;

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
	
	if (!CMP(Dot(right, right), 1.0f))
		return false;  // X axis is not normal
		
	if (!CMP(Dot(up, up), 1.0f))
		return false;  // Y axis is not normal
		
	if (!CMP(Dot(forward, forward), 1.0f))
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
	if (!IsOrthoNormal())
		OrthoNormalize();
	
	return FastInverse(m_matWorld);
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


OrbitCamera::OrbitCamera()
{
	target = vec3{0, 0, 0};
	zoomDistance = 10.0f;
	zoomSpeed = 200.0f;
	rotationSpeed = vec2{250.0f, 120.0f};
	yRotationLimit = vec2{-20.0f, 80.0f};
	zoomDistanceLimit = vec2{3.0f, 15.0f};
	currentRotation = vec2{0, 0};
	panSpeed = vec2{180.0f, 180.0f};
}

void OrbitCamera::Rotate(const vec2 & deltaRot, float deltaTime)
{
	currentRotation.x += deltaRot.x * rotationSpeed.x * zoomDistance * deltaTime;
	currentRotation.y += deltaRot.y * rotationSpeed.y * zoomDistance * deltaTime;
	currentRotation.x = ClampAngle(currentRotation.x, -360, 360);
	currentRotation.y = ClampAngle(currentRotation.y, yRotationLimit.x, yRotationLimit.y);
}

void OrbitCamera::Zoom(float deltaZoom, float deltaTime)
{
	zoomDistance = zoomDistance + deltaZoom * zoomSpeed * deltaTime;
	if (zoomDistance < zoomDistanceLimit.x)
		zoomDistance = zoomDistanceLimit.x;
	if (zoomDistance > zoomDistanceLimit.y)
		zoomDistance = zoomDistanceLimit.y;
}

void OrbitCamera::Pan(vec2 const & deltaPan, float deltaTime)
{
	vec3 right = vec3{m_matWorld._11, m_matWorld._12, m_matWorld._13};
	
	// pan X axis in local space
	target = target - (right * (deltaPan.x * panSpeed.x * deltaTime));
	
	// pan Y axis on global space
	target = target + (vec3{0, 1, 0} * (deltaPan.y * panSpeed.y * deltaTime));
		
	// reset zoom to allow infinite zooming after a motion (not in the book!)
	float midZoom = zoomDistanceLimit.x + (zoomDistanceLimit.y - zoomDistanceLimit.x) * 0.5f;
	zoomDistance = midZoom - zoomDistance;
	vec3 rotation = vec3{currentRotation.y, currentRotation.x, 0};
	mat3 orient = Rotation3x3(rotation.x, rotation.y, rotation.z);
	vec3 dir = MultiplyVector(vec3{0, 0, -zoomDistance}, orient);
	target = target - dir;
	zoomDistance = midZoom;
}

void OrbitCamera::Update(float dt)
{
	vec3 rotation = vec3{currentRotation.y, currentRotation.x, 0};
	mat3 orient = Rotation3x3(rotation.x, rotation.y, rotation.z);
	vec3 dir = MultiplyVector(vec3{0, 0, -zoomDistance}, orient);
	vec3 position = dir + target;
	m_matWorld = FastInverse(LookAt(position, target, vec3{0, 1, 0}));
}

float OrbitCamera::ClampAngle(float angle, float min, float max)
{
	while (angle < -360)
		angle += 360;
	while (angle > 360)
		angle -= 360;
	if (angle < min)
		angle = min;
	if (angle > max)
		angle = max;
	return angle;
}

void OrbitCamera::PrintDebug()
{
	cout << "Target: (" << target.x << ", " << target.y << ", " << target.z << ")\n"
		<< "Zoom distance: " << zoomDistance << "\n"
		<< "Rotation: (" << currentRotation.x << ", " << currentRotation.y << ")\n";
}

void OrbitCamera::SetTarget(vec3 const & newTarget)
{
	target = newTarget;
}

void OrbitCamera::SetZoom(float zoom)
{
	zoomDistance = zoom;
}

void OrbitCamera::SetRotation(vec2 const & rotation)
{
	currentRotation = rotation;
}














