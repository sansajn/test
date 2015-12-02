#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::perspective;
using glm::radians;

struct foo
{
	foo() : _proj{perspective(radians(70.0f), 800.0f/600.0f, 0.01f, 1000.0f)} {}

	mat4 _proj;
};

int main(int argc, char * argv[])
{
	foo f;
	return 0;
}
