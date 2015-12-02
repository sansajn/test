#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::perspective;

int main(int argc, char * argv[])
{
	mat4 T{1};
	mat4 P{perspective(3.14f, 8.0f/6.0f, 0.01f, 1000.0f)};

	return 0;
}
