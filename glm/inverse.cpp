#include <glm/matrix.hpp>

int main(int argc, char * argv[])
{
	glm::mat4 m(1.0);
	glm::mat4 m_inv = glm::inverse(m);
	return 0;
}

