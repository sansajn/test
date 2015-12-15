// test funkcie vector-angle 
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include "glmprint.hpp"

using std::cout;
using glm::vec2;
using glm::vec3;
using glm::angle;
using glm::orientedAngle;

int main(int argc, char * argv[])
{
	vec2 u{1,0};
	vec2 v{-1,0};
	cout << "angle(u,v):" << angle(u,v) << "\n";  // cakam pi
	cout << "orientedAngle(u,v):" << orientedAngle(u,v) << "\n";  // cakam pi
	
	v = vec2{0, -1};
	cout << "angle(u,v):" << angle(u,v) << "\n";  // cakam 3/2*p
	cout << "orientedAngle(u,v):" << orientedAngle(u,v) << "\n";
	
	vec3 u3{1,0,0};  // nagle je len pre 2d vektory
	vec3 v3{0,1,0};
	auto ang = angle(u3, v3);
	cout << ang << "\n";
	
	return 0;
}
