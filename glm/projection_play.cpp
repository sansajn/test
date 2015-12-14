// zmena velkosti po transformacii v zavyslosti od vzdialenosti odkamery
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "glmprint.hpp"

using std::cout;
using glm::vec4;
using glm::vec3;
using glm::mat4;
using glm::perspective;
using glm::radians;

int main(int argc, char * argv[])
{
	mat4 P = perspective(radians(70.0f), 800.0f/600.0f, 0.01f, 1000.0f);

	vec4 v0{0,0,0,1};
	vec4 v1{1,0,0,1};
	vec4 v2{1,1,0,1};
	vec4 v3{0,1,0,1};

	cout << v0 << " -> " << P*v0 << "\n";
	cout << v1 << " -> " << P*v1 << "\n";
	cout << v2 << " -> " << P*v2 << "\n";
	cout << v3 << " -> " << P*v3 << "\n";

	cout << "-------\n";

	v0 = vec4{0,0,-5,1};
	v1 = vec4{1,0,-5,1};
	v2 = vec4{1,1,-5,1};
	v3 = vec4{0,1,-5,1};

	cout << v0 << " -> " << P*v0 << "\n";
	cout << v1 << " -> " << P*v1 << "\n";
	cout << v2 << " -> " << P*v2 << "\n";
	cout << v3 << " -> " << P*v3 << "\n";


	cout << "-------\n";

	v0 = vec4{0,0,-10,1};
	v1 = vec4{1,0,-10,1};
	v2 = vec4{1,1,-10,1};
	v3 = vec4{0,1,-10,1};

	cout << v0 << " -> " << P*v0 << "\n";
	cout << v1 << " -> " << P*v1 << "\n";
	cout << v2 << " -> " << P*v2 << "\n";
	cout << v3 << " -> " << P*v3 << "\n";

	return 0;
}
