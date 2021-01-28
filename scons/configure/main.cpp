#include <iostream>
#include <glm/vec2.hpp>

using std::cout;
using glm::vec2;

int main(int argc, char * argv[])
{
	vec2 a{0,1}, b{1,0};
	vec2 c = a + b;
	cout << "c=(" << c.x << "," << c.y << ")\n";
	return 0;
}
