/* ako pouziť užívateľsky definovaný bod */
#include <iostream>

#include <boost/mpl/int.hpp>
#include <boost/geometry/core/access.hpp>
#include <boost/geometry/core/coordinate_type.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/core/coordinate_dimension.hpp>
#include <boost/geometry/core/cs.hpp>

#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry.hpp>

using std::cout;


struct point
{
	float x, y, z;
};

// adaptacia pre point
namespace boost {  namespace geometry { namespace traits {  

template <>
struct tag<point>
{
	typedef point_tag type;
};

template <>
struct coordinate_type<point>
{
	typedef float type;
};

template <>
struct coordinate_system<point>
{
	typedef cs::cartesian type;
};

template <>
struct dimension<point> : boost::mpl::int_<3> 
{};


namespace detail {

template <std::size_t N>
float get(point const & p);

template <>
float get<0>(point const & p) {return p.x;}

template <>
float get<1>(point const & p) {return p.y;}

template <>
float get<2>(point const & p) {return p.z;}


template <std::size_t N>
void set(point & p, float const & v);

template <>
void set<0>(point & p, float const & v) {p.x = v;}

template <>
void set<1>(point & p, float const & v) {p.y = v;}

template <>
void set<2>(point & p, float const & v) {p.z = v;}

}  // detail

template <std::size_t Dimension>
struct access<point, Dimension>
{
	static float get(point const & p) {return detail::get<Dimension>(p);}
	static void set(point & p, float const & v) {detail::set<Dimension>(p, v);}
};

}}}  // traits. geometry, boost

namespace bg = boost::geometry;
typedef bg::model::box<point> box;  // box3


int main(int argc, char * argv[])
{
	point p = {1.0f, 0.0f, 0.0f};
	point r = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
	box b(p, r);
	int npoints = bg::num_points(b);
	cout << "points:" << npoints << "\n";
	return 0;
};

