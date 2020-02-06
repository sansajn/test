#pragma once

struct mat4 {};
struct vec3 {};
struct Frustum {};

class Camera
{
public:
	Camera();
	virtual ~Camera() {}
	
	mat4 GetWorldMatrix();
	mat4 GetViewMatrix();
	mat4 GetProjectionMatrix();
	
	float GetAspect();
	bool IsOrthographics();
	bool IsPerspective();
	
	bool IsOrthoNormal();
	void OrthoNormalize();
	
	void Resize(int width, int height);
	
	void Perspective(float fov, float aspect, float zNear, float zFar);
	void Orthographic(float width, float height, float zNear, float zFar);
	
	void SetProjection(mat4 const & projection);
	void SetWorld(mat4 const & view);
	
	Frustum GetFrustum();
	
protected:
	float m_nFov,
		m_nAspect,
		m_nNear,
		m_nFar,
		m_nWidth,
		m_nHeight;
		
	mat4 m_matWorld,  // world transform
		m_matProj;
	int m_nProjectionMode;  // 0 for perspective, 1 for ortho and 2 for user
};

Camera CreatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
Camera CreateOrthographic(float width, float height, float nearPlane, float farPlane);
