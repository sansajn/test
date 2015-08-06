#pragma once
#include <string>
#include <iostream>
#include <iomanip>
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
