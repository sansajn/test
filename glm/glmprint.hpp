/*! \file glmprint.hpp
output printers */
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_access.hpp>


template <typename Mat>
void print(std::ostream & out, Mat const & M)
{
	using std::vector;
	using std::string;
	using std::copy;
	using glm::row;

	int columns = M.length();
	int rows = M[0].length();

	vector<string> values;
	for (int i = 0; i < rows; ++i)
	{
		typename Mat::row_type r = row(M, i);
		for (int j = 0; j < columns; ++j)
		{
			std::ostringstream sout;
			sout << r[j];
			values.push_back(sout.str());
		}
	}

	int width = 0;
	for (string const & val : values)
		if (val.length() > (size_t)width)
			width = val.length();

	out << "\n";

	for (int i = 0; i < rows; ++i)
	{
		string line(columns*(width+2)-2, ' ');

		for (int j = 0; j < columns; ++j)
		{
			string const & val = values[i*rows+j];
			int offset = j*(width+2) + width - val.length();
			copy(val.begin(), val.end(), line.begin() + offset);
		}

		out << "   " << line << "\n";
	}

	out << "\n";
}

// generic matrix output operator
template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q = glm::defaultp>
std::ostream & operator<<(std::ostream & out, glm::mat<C, R, T, Q> const & M)
{
	print(out, M);
	return out;
}

/*! matrix labeler
\code
mat4 M{1};
cout << with_label("M", M);
\endcode */
template <typename Mat>
struct with_label
{
	with_label(std::string const & label, Mat const & M) : _M{M}, _label{label} {}

	Mat const & _M;
	std::string const & _label;
};

template <typename Mat>
std::ostream & operator<<(std::ostream & out, with_label<Mat> const & x)
{
	if (!x._label.empty())
		out << x._label << " =\n";

	out << x._M;

	return out;
}


//! \note obsolete, use cout << with_label("M", M);
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
