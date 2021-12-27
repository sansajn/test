// constexpr 3D vector sample implementation

struct vec3 {
	double x, y, z;

	constexpr vec3(double x, double y, double z)
		: x{x}, y{y}, z{z}
	{}

	constexpr vec3 operator+(vec3 const & v) const {return {x+v.x, y+v.y, z+v.z};}
	constexpr vec3 operator-(vec3 const & v) const {return {x-v.x, y-v.y, z-v.z};}
	constexpr vec3 operator*(double k) const {return {k*x, k*y, k*z};}
	// ...
};

int main(int argc, char * argv[]) {
	constexpr vec3 u{1, 1, 0},
		v{2, 2, 0},
		k = u + v,
		l = u - v,
		m = u*10.0;

	return 0;
}
