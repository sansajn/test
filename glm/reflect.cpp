// implementacia funkcie glm::reflect()
#include <iostream>
#include <glm/glm.hpp>

int main(int argc, char * argv[])
{
	glm::vec3 l(-2, -3, 0);
	glm::vec3 n(0, 1, 0);
	glm::vec3 r = glm::reflect(l, glm::normalize(n));
	std::cout << "x:" << r.x << ", y:" << r.y << ", z:" << r.z << "\n";
	return 0;
}