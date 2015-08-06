#include <glm/gtc/matrix_transform.hpp>
#include "print_matrix.hpp"

glm::mat3 rotatex(float angle);
glm::mat3 rotatey(float angle);
glm::mat3 rotatez(float angle);


int main(int argc, char * argv[])
{
	glm::mat3 M = rotatex(30);
	print_matrix("M_x", M);

	glm::mat4 R = glm::rotate(glm::mat4(), 30.f, glm::vec3(1, 0, 0));
	print_matrix("R_x", R);

	M = rotatey(30);
	print_matrix("M_y", M);

	R = glm::rotate(glm::mat4(), 30.f, glm::vec3(0, 1, 0));
	print_matrix("R_y", R);

	M = rotatez(30);
	print_matrix("M_z", M);

	R = glm::rotate(glm::mat4(), 30.f, glm::vec3(0, 0, 1));
	print_matrix("R_z", R);

	return 0;
}	


glm::mat3 rotatex(float angle)
{
	float a = glm::radians(angle);
	float ca = cos(a);
	float sa = sin(a);

	return glm::mat3(
		1, 0, 0,
		0, ca, sa,
		0, -sa, ca);
}

glm::mat3 rotatey(float angle)
{
	float a = glm::radians(angle);
	float ca = cos(a);
	float sa = sin(a);

	return glm::mat3(
		ca, 0, -sa,
		0, 1, 0,
		sa, 0, ca);
}

glm::mat3 rotatez(float angle)
{
	float a = glm::radians(angle);
	float ca = cos(a);
	float sa = sin(a);

	return glm::mat3(
		ca, sa, 0,
		-sa, ca, 0,
		0, 0, 1);
}


