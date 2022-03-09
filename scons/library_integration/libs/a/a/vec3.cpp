#include "vec3.hpp"

[[nodiscard]] constexpr vec3 operator+(vec3 const & u, vec3 const & v) {
	return {u.x+v.x, u.y+v.y, u.z+v.z};
}

[[nodiscard]] constexpr vec3 operator-(vec3 const & u, vec3 const & v) {
	return {u.x-v.x, u.y-v.y, u.z-v.z};
}

[[nodiscard]] constexpr vec3 vec3::operator*(double k) const {
	return {k*x, k*y, k*z};
}

[[nodiscard]] constexpr vec3 operator*(double k, vec3 const & v) {
	return v*k;
}

[[nodiscard]] constexpr vec3 operator*(vec3 const & u, vec3 const & v) {
	return u.dot(v);
}

[[nodiscard]] constexpr vec3 vec3::dot(vec3 const & v) const {
	return {x*v.x, y*v.y, z*v.z};
}
	