#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_access.hpp>

template <typename M>
void print_matrix(M const & m, std::string const & name = std::string())
{
	std::cout << name << " =\n";
	int length = m.length();
	for (int i = 0; i < length; ++i)
	{
		typename M::row_type r = glm::row(m, i);
		std::cout << "   ";
		for (int j = 0; j < length; ++j)
			std::cout << std::setw(9) << std::setprecision(5) << r[j] << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

template <typename V>
void print_vector(V const & v, std::string const & name = std::string())
{
	if (!name.empty())
		std::cout << name << " =\n";

	std::cout << "   ";
	for (int i = 0; i < v.length(); ++i)
		std::cout << std::setw(9) << std::setprecision(5) << v[i] << " ";
	std::cout << "\n\n";
}

inline std::ostream & operator<<(std::ostream & o, glm::quat const & q)
{
	o << "[(" << q.x << ", " << q.y << ", " << q.z << "), " << q.w << "]";
	return o;
}

inline std::ostream & operator<<(std::ostream & o, glm::vec2 const & v)
{
	o << "(" << v.x << ", " << v.y << ")";
	return o;
}

inline std::ostream & operator<<(std::ostream & o, glm::vec3 const & v)
{
	o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return o;
}

inline std::ostream & operator<<(std::ostream & o, glm::vec4 const & v)
{
	o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return o;
}

inline std::ostream & operator<<(std::ostream & o, glm::mat3 const & m)
{
	o << "[" << m[0] << ", " << m[1] << ", " << m[2] << "]";
	return o;
}

inline std::ostream & operator<<(std::ostream & o, glm::mat4 const & m)
{
	o << "[" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "]";
	return o;
}
