#include <glm/gtc/matrix_transform.hpp>
#include "glmprint.hpp"

glm::mat4 perspective_projection(float fovy, float aspect, float znear, float zfar)
{
	float f = 1.0f/tan(glm::radians(fovy)/2);
	return glm::mat4{
		f/aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (zfar+znear)/(znear-zfar), -1,
		0, 0, (2*zfar*znear)/(znear-zfar), 0};
}

glm::mat4 proland_projection(float fovy, float width, float height, float znear, float zfar)
{
	float fov = glm::radians(fovy);
	float vfov = glm::degrees(2.0f * atan(height/width * tan(fov/2.0f)));  // vertical fov
	return perspective_projection(vfov, width/height, znear, zfar);
}

void print_vector(glm::vec4 const & v)
{
	std::cout << "[" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "]";
}

void compare_vector(glm::vec4 const & a, glm::vec4 const & b)
{
	print_vector(a);
	std::cout << " -> ";
	print_vector(b);
	std::cout << "\n";
}

int main(int argc, char * argv[])
{
	float w = 800.0f, h = 600.0f;
	float znear = 0.1f, zfar = 1e6f;
	glm::mat4 P = glm::perspective(glm::radians(80.0f), h/w, znear, zfar);

	glm::vec4 v0 = glm::vec4{0,0,0,1};
	glm::vec4 v1 = glm::vec4{1,0,0,1};
	glm::vec4 v2 = glm::vec4{0,1,0,1};
	glm::vec4 v3 = glm::vec4{0,0,1,1};
	glm::vec4 v4 = glm::vec4{1,1,0,1};
	glm::vec4 v5 = glm::vec4{1,1,1,1};

	glm::vec4 v0_P = P*v0;
	glm::vec4 v1_P = P*v1;
	glm::vec4 v2_P = P*v2;
	glm::vec4 v3_P = P*v3;
	glm::vec4 v4_P = P*v4;
	glm::vec4 v5_P = P*v5;

	print_matrix(P, "P");

	compare_vector(v0, v0_P);
	compare_vector(v1, v1_P);
	compare_vector(v2, v2_P);
	compare_vector(v3, v3_P);
	compare_vector(v4, v4_P);
	compare_vector(v5, v5_P);

	std::cout << "\n";
	glm::mat4 R = proland_projection(80.0f, w, h, znear, zfar);

	glm::vec4 v0_R = R*v0;
	glm::vec4 v1_R = R*v1;
	glm::vec4 v2_R = R*v2;
	glm::vec4 v3_R = R*v3;
	glm::vec4 v4_R = R*v4;
	glm::vec4 v5_R = R*v5;

	print_matrix(R, "R");

	compare_vector(v0, v0_R);
	compare_vector(v1, v1_R);
	compare_vector(v2, v2_R);
	compare_vector(v3, v3_R);
	compare_vector(v4, v4_R);
	compare_vector(v5, v5_R);

	return 0;
}
