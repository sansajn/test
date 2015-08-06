/* adaptacia pre glm::vec */
#include <boost/mpl/int.hpp>
#include <boost/geometry/core/access.hpp>
#include <boost/geometry/core/coordinate_type.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/coordinate_dimension.hpp>
#include <glm/vec3.hpp>

#include <boost/geometry/geometries/box.hpp>


namespace boost { namespace geometry { namespace traits {

// adaptacia pre glm::vec3
template <typename T, glm::precision P>
struct tag<glm::detail::tvec3<T, P>>
{
	typedef point_tag type;
};

template <typename T, glm::precision P>
struct coordinate_type<glm::detail::tvec3<T, P>>
{
	typedef typename glm::detail::tvec3<T, P>::value_type type;
};

template <typename T, glm::precision P>
struct coordinate_system<glm::detail::tvec3<T, P>>
{
	typedef cs::cartesian type;
};

template <typename T, glm::precision P>
struct dimension<glm::detail::tvec3<T, P>> : boost::mpl::int_<3>
{};

template <typename T, glm::precision P, std::size_t Dimension>
struct access<glm::detail::tvec3<T, P>, Dimension>
{
	typedef glm::detail::tvec3<T, P> point_type;
	typedef typename glm::detail::tvec3<T, P>::value_type value_type;

	static value_type get(point_type const & p) {return p[Dimension];}
	static void set(point_type & p, value_type const & v);
};

}}}  // geometry, boost, traits


typedef boost::geometry::model::box<glm::vec3> box3;


int main(int argc, char * argv[])
{
	box3 b(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	return 0;
}
