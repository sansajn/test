#pragma once

struct vec3 {
	double x, y, z;

	constexpr vec3(double d) : x{d}, y{d}, z{d} {}

	constexpr vec3(double x, double y, double z)
		: x{x}, y{y}, z{z}
	{}

	friend constexpr vec3 operator+(vec3 const & u, vec3 const & v);
	friend constexpr vec3 operator-(vec3 const & u, vec3 const & v);

	constexpr vec3 operator*(double k) const;
	friend constexpr vec3 operator*(double k, vec3 const & v);
	friend constexpr vec3 operator*(vec3 const & u, vec3 const & v);

	constexpr vec3 dot(vec3 const & v) const;
	// cross()
};
