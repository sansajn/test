// nodiscard constexpr 3D vector sample implementation

struct vec3 {
	double x, y, z;

	constexpr vec3(double d) : x{d}, y{d}, z{d} {}

	constexpr vec3(double x, double y, double z)
		: x{x}, y{y}, z{z}
	{}

	[[nodiscard]] friend constexpr vec3 operator+(vec3 const & u, vec3 const & v) {return {u.x+v.x, u.y+v.y, u.z+v.z};}
	[[nodiscard]] friend constexpr vec3 operator-(vec3 const & u, vec3 const & v) {return {u.x-v.x, u.y-v.y, u.z-v.z};}

	[[nodiscard]] constexpr vec3 operator*(double k) const {return {k*x, k*y, k*z};}
	[[nodiscard]] friend constexpr vec3 operator*(double k, vec3 const & v) {return v*k;}
	[[nodiscard]] friend constexpr vec3 operator*(vec3 const & u, vec3 const & v) {return u.dot(v);}

	[[nodiscard]] constexpr vec3 dot(vec3 const & v) const {return {x*v.x, y*v.y, z*v.z};}
	// cross()
};

int main(int argc, char * argv[]) {
	constexpr vec3 u{1, 1, 0},
		v{2, 2, 0},
		k = u + v,
		l = u - v,
		m = u*10.0,
		n = 10*v;

	return 0;
}
