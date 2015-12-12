#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "glmprint.hpp"

using std::cout;
using glm::vec4;
using glm::mat4;
using glm::perspective;
using glm::ortho;
using glm::radians;

int main(int argc, char * argv[])
{
	mat4 P = perspective(radians(70.0f), 800.0f/600.0f, 0.01f, 1000.0f);
	vec4 v0{0, 0, 0, 1};
	vec4 v1{0, 0, -1, 1};
	vec4 v2{0, 0, -5, 1};
	vec4 v3{0, 0, -10, 1};
	vec4 v4{5, 5, -5, 1};
	vec4 v5{10, 5, -5, 1};
	vec4 v6{10, 5, -10, 1};

	cout << "perspective\n";
	cout << v0 << " -> " << P*v0 << "\n";
	cout << v1 << " -> " << P*v1 << "\n";
	cout << v2 << " -> " << P*v2 << "\n";
	cout << v3 << " -> " << P*v3 << "\n";
	cout << v4 << " -> " << P*v4 << "\n";
	cout << v5 << " -> " << P*v5 << "\n";
	cout << v6 << " -> " << P*v6 << "\n";

	mat4 O = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.01f, 1000.0f);

	cout << "ortho\n";
	cout << v0 << " -> " << O*v0 << "\n";
	cout << v1 << " -> " << O*v1 << "\n";
	cout << v2 << " -> " << O*v2 << "\n";
	cout << v3 << " -> " << O*v3 << "\n";
	cout << v4 << " -> " << O*v4 << "\n";
	cout << v5 << " -> " << O*v5 << "\n";
	cout << v6 << " -> " << O*v6 << "\n";

	return 0;
}
