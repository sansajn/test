#include <iostream>
#include <glm/trigonometric.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "glmprint.hpp"

using std::cout;
using glm::vec4;
using glm::vec3;
using glm::mat4;
using glm::radians;
using glm::yawPitchRoll;
using glm::eulerAngleX;
using glm::eulerAngleY;
using glm::eulerAngleZ;
using glm::eulerAngleYXZ;
using glm::orientate4;

int main(int argc, char * argv[])
{
	float yaw = radians(60.0f);
	float pitch = radians(30.0f);
	float roll = radians(15.0f);

	mat4 Rx = eulerAngleX(pitch);  // pitch
	mat4 Ry = eulerAngleY(yaw);  // yaw
	mat4 Rz = eulerAngleZ(roll);  // roll
	mat4 R = Ry * Rx * Rz;

	if (R == eulerAngleYXZ(yaw, pitch, roll))
		cout << "equals\n";
	else
		cout << "not equall\n";

	if (eulerAngleYXZ(yaw, pitch, roll) == yawPitchRoll(yaw, pitch, roll))
		cout << "equals\n";
	else
		cout << "not equall\n";

	if (yawPitchRoll(yaw, pitch, roll) == orientate4(vec3{pitch, yaw, roll}))  // possible bug, search for it ...
		cout << "equals\n";
	else
		cout << "not equall\n";

	cout << yawPitchRoll(yaw, pitch, roll) << "\n"
		<< orientate4(vec3{pitch, yaw, roll}) << "\n";

	return 0;
}
