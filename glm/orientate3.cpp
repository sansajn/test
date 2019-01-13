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
using glm::eulerAngleX;
using glm::eulerAngleY;
using glm::eulerAngleZ;

int main(int argc, char * argv[])
{
	float yaw = radians(60.0f);  // y
	float pitch = radians(30.0f);  // x
	float roll = radians(15.0f);  // z

	/* mat3 Rotation3x3(float pitch, float yaw, float roll) {
		return ZRotation3x3(roll) * XRotation3x3(pitch) * YRotation3x3(yaw);
	}*/

	mat4 Rx = eulerAngleX(pitch);
	mat4 Ry = eulerAngleY(yaw);
	mat4 Rz = eulerAngleZ(roll);
	mat4 R = Rz * Rx * Ry;

	cout << mat3{R} << "\n";
	cout << orientate3(vec3{pitch, yaw, roll}) << "\n";

	mat3 O = orientate3(vec3{pitch, yaw, roll});  // yawPitchRoll(angles.z, angles.x, angles.y)

	vec3 p0 = {0,0,0};
	vec3 direction = mat3{R} * vec3{0, 0, 1};
	vec3 p = p0 + direction;

	cout << "p=" << p << "\n";

	return 0;
}
