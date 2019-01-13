// eulerAngle and rotate sample
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using std::cout;
using namespace glm;

int main(int argc, char * argv[])
{
	float pitch = radians(30.0f);  // x
	float yaw = radians(60.0f);  // y
	float roll = radians(15.0f);  // z

	mat4 Ex = eulerAngleX(pitch);  // pitch
	mat4 Rx = rotate(mat4{1}, pitch, vec3{1,0,0});

	if (Ex == Rx)
		cout << "Ex == Rx\n";
	else
		cout << "Ex != Rx\n";

	mat4 Ey = eulerAngleY(yaw);  // yaw
	mat4 Ry = rotate(mat4{1}, yaw, vec3{0,1,0});

	if (Ey == Ry)
		cout << "Ey == Ry\n";
	else
		cout << "Ey != Ry\n";

	mat4 Ez = eulerAngleZ(roll);  // roll
	mat4 Rz = rotate(mat4{1}, roll, vec3{0,0,1});

	if (Ez == Rz)
		cout << "Ez == Rz\n";
	else
		cout << "Ez != Rz\n";

	return 0;
}
