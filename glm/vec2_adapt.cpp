// adaptacia glm::vec2 (0.9.9)
#include <iostream>
#include <boost/geometry/geometry.hpp>
#include <glm/vec2.hpp>

namespace boost { namespace geometry { namespace traits {

// glm::vec adaptation
template <glm::length_t L, typename T, glm::qualifier Q>
struct tag<glm::vec<L, T, Q>>
{
	typedef point_tag type;
};

template <glm::length_t L, typename T, glm::qualifier Q>
struct coordinate_type<glm::vec<L, T, Q>>
{
	typedef typename glm::vec<L, T, Q>::value_type type;
};

template <glm::length_t L, typename T, glm::qualifier Q>
struct coordinate_system<glm::vec<L, T, Q>>
{
	typedef cs::cartesian type;
};

template <glm::length_t L, typename T, glm::qualifier Q>
struct dimension<glm::vec<L, T, Q>> : boost::mpl::int_<L>
{};

template <glm::length_t L, typename T, glm::qualifier Q, std::size_t Dimension>
struct access<glm::vec<L, T, Q>, Dimension>
{
	typedef glm::vec<L, T, Q> point_type;
	typedef typename glm::vec<L, T, Q>::value_type value_type;

	static value_type get(point_type const & p) {return p[Dimension];}
	static void set(point_type & p, value_type const & v) {p[Dimension] = v;}
};

}}}  // geometry, boost, traits


using boost::geometry::make;
using boost::geometry::make_inverse;
using boost::geometry::expand;

using box2 = boost::geometry::model::box<glm::vec2>;


int main(int argc, char * argv[])
{
	box2 b = make_inverse<box2>();
	expand(b, glm::vec2{0,0});
	expand(b, glm::vec2{1,2});
	expand(b, glm::vec2{5,4});
	expand(b, make<box2>(3,3, 5,5));

	std::cout << boost::geometry::dsv(b) << std::endl;

	return 0;
}
