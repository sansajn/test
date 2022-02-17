// nodiscard constexpr 3D vector sample implementation
struct vec3 {
	double x, y, z;

	constexpr vec3(double d) : x{d}, y{d}, z{d} {}

	constexpr vec3(double x, double y, double z)
		: x{x}, y{y}, z{z}
	{}

	[[nodiscard]] bool operator==(vec3 const & v) const {return x == v.x && y == v.y && z == v.z;}

	[[nodiscard]] friend constexpr vec3 operator+(vec3 const & u, vec3 const & v) {return {u.x+v.x, u.y+v.y, u.z+v.z};}
	[[nodiscard]] friend constexpr vec3 operator-(vec3 const & u, vec3 const & v) {return {u.x-v.x, u.y-v.y, u.z-v.z};}

	[[nodiscard]] constexpr vec3 operator*(double k) const {return {k*x, k*y, k*z};}
	[[nodiscard]] friend constexpr vec3 operator*(double k, vec3 const & v) {return v*k;}
	[[nodiscard]] friend constexpr vec3 operator*(vec3 const & u, vec3 const & v) {return u.dot(v);}

	[[nodiscard]] constexpr vec3 dot(vec3 const & v) const {return {x*v.x, y*v.y, z*v.z};}
	// cross()
};
