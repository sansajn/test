#include <iomanip>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using std::cout;

void print_vector(glm::vec4 const & v);
void print_matrix(glm::mat4 const & m);
glm::mat4 look_at(glm::vec3 const & eye, glm::vec3 const & center, glm::vec3 const & up);

void rotation_test()
{
	glm::mat4 M;
	glm::vec4 v = glm::vec4(1,0,0,0);
	glm::vec4 v_;

	// rotation by matrix
	M = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0,1,0));
	v_ = M*v;
	cout << "v'=";
	print_vector(v_);

	// rotation by quternion
	glm::quat q = glm::angleAxis(glm::radians(45.0f), glm::vec3(0,1,0));
	v_ = q*v;
	cout << "p'=";
	print_vector(v_);
}

void lookat_vs_rotation()  // ekvivalentnost rotacie a look-at
{
	glm::mat4 M;
	glm::vec4 v = glm::vec4(1,0,0,0);
	glm::vec3 c = glm::vec3(1,0,-1);  // center, bod na ktory sa pozeram
	glm::vec4 v_;

	// look-at
	M = glm::lookAt(glm::vec3(0,0,0), c, glm::vec3(0,1,0));  // v podstate pootocenie o 45 deg
	v_ = M*v;
	cout << "M =";
	print_matrix(M);
	cout << "v'=";
	print_vector(v_);

	// rotation
	M = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0,1,0));
	v_ = M*v;
	cout << "v'=";
	print_vector(v_);
}

void lookat_vs_quat()  // akvivalentnost transformacnej matice a quaternionu
{
	glm::mat4 M;
	glm::vec4 v = glm::vec4(1,0,0,0);
	glm::vec3 c = glm::vec3(1,0,-1);  // center, bod na ktory sa pozeram
	glm::vec4 v_;

	// look-at
	M = glm::lookAt(glm::vec3(0,0,0), c, glm::vec3(0,1,0));  // v podstate pootocenie o 45 deg
	v_ = M*v;
	cout << "v'=";
	print_vector(v_);

	// quaternion
	glm::quat q = glm::quat_cast(M);
	v_ = q*v;
	cout << "v'=";
	print_vector(v_);
}

int main(int argc, char * argv[])
{	
	glm::mat4 M;
	glm::vec4 v = glm::vec4(1,0,0,0);
	glm::vec3 c = glm::vec3(1,0,-1);  // center, bod na ktory sa pozeram
	glm::vec4 v_;

	// look-at
	M = glm::lookAt(glm::vec3(0,0,0), c, glm::vec3(0,1,0));  // v podstate pootocenie o 45 deg
	v_ = M*v;
	cout << "M =";
	print_matrix(M);
	cout << "v'=";
	print_vector(v_);

	// custom
	glm::vec3 f = glm::normalize(-c);
	glm::vec3 u = glm::vec3(0,1,0);
	glm::vec3 r = glm::normalize(glm::cross(u,f));
	u = glm::cross(f,r);
	M = glm::mat4(
		r.x, u.x, f.x, 0,
		r.y, u.y, f.y, 0,
		r.z, u.z, f.z, 0,
		  0,   0,   0, 1);
	glm::quat q = glm::quat_cast(M);
	v_ = q*v;
	cout << "v'=";
	print_vector(v_);

	return 0;
}

glm::mat4 look_at(glm::vec3 const & eye, glm::vec3 const & center, glm::vec3 const & up)
{
	glm::vec3 f = glm::normalize(center - eye);
	glm::vec3 u = glm::normalize(up);
	glm::vec3 r = glm::normalize(glm::cross(u,f));
	u = glm::normalize(glm::cross(f,r));
	return glm::mat4(
		r.x, u.x, f.x, 0,
		r.y, u.y, f.y, 0,
		r.z, u.z, f.z, 0,
		  0,   0,   0, 1
	);
}

void print_vector(glm::vec4 const & v)
{
	cout << "\n  ";
	for (int i = 0; i < 4; ++i)
		cout << std::setw(9) << std::setprecision(5) << v[i] << " ";
	cout << "\n";
}

void print_matrix(glm::mat4 const & m)
{
	cout << "\n";
	for (int r = 0; r < 4; ++r)
	{
		cout << "  ";
		for (int c = 0; c < 4; ++c)
			cout << std::setw(9) << std::setprecision(5) << m[r][c] << " ";
		cout << "\n";
	}		
}
