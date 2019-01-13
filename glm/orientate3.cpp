// konstrukcia transformacnej matice s uhlou
#include <glm/trigonometric.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "glmprint.hpp"

using std::cout;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::radians;
using glm::orientate3;
using glm::yawPitchRoll;

int main(int argc, char * argv[])
{
	mat3 M = orientate3(vec3{.1, .2, .3});
	cout << "M=" << M << "\n";

	mat4 M2 = yawPitchRoll(.1, .2, .3);  // yaw (y), pitch (x), roll (z)
	cout << "M2=" << M2 << "\n";

	vec3 p = {1, 2, 3};
	vec3 p_transformed = M*p;
	cout << "p_transformed=" << p_transformed << "\n";

	vec4 p2 = {1, 2, 3, 0};
	vec4 p2_transformed = M2*p2;
	cout << "p2_transformed=" << p2_transformed << "\n";

	return 0;
}
