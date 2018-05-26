/*! Adaptuje štruktúry s knižnice glm (0.9.6.3) pre použitie s boost::geometry. */
#pragma once
#include <boost/mpl/int.hpp>
#include <boost/geometry/core/access.hpp>
#include <boost/geometry/core/coordinate_type.hpp>
#include <boost/geometry/core/coordinate_system.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/coordinate_dimension.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace boost { namespace geometry { namespace traits {

// glm::vec2 adaptation
template <typename T, glm::precision P>
struct tag<glm::tvec2<T, P>>
{
	typedef point_tag type;
};

template <typename T, glm::precision P>
struct coordinate_type<glm::tvec2<T, P>>
{
	typedef typename glm::tvec2<T, P>::value_type type;
};

template <typename T, glm::precision P>
struct coordinate_system<glm::tvec2<T, P>>
{
	typedef cs::cartesian type;
};

template <typename T, glm::precision P>
struct dimension<glm::tvec2<T, P>> : boost::mpl::int_<2>
{};

template <typename T, glm::precision P, std::size_t Dimension>
struct access<glm::tvec2<T, P>, Dimension>
{
	typedef glm::tvec2<T, P> point_type;
	typedef typename glm::tvec2<T, P>::value_type value_type;

	static value_type get(point_type const & p) {return p[Dimension];}
	static void set(point_type & p, value_type const & v) {p[Dimension] = v;}
};

// glm::vec3 adaptation
template <typename T, glm::precision P>
struct tag<glm::tvec3<T, P>>
{
	typedef point_tag type;
};

template <typename T, glm::precision P>
struct coordinate_type<glm::tvec3<T, P>>
{
	typedef typename glm::tvec3<T, P>::value_type type;
};

template <typename T, glm::precision P>
struct coordinate_system<glm::tvec3<T, P>>
{
	typedef cs::cartesian type;
};

template <typename T, glm::precision P>
struct dimension<glm::tvec3<T, P>> : boost::mpl::int_<3>
{};

template <typename T, glm::precision P, std::size_t Dimension>
struct access<glm::tvec3<T, P>, Dimension>
{
	typedef glm::tvec3<T, P> point_type;
	typedef typename glm::tvec3<T, P>::value_type value_type;

	static value_type get(point_type const & p) {return p[Dimension];}
	static void set(point_type & p, value_type const & v) {p[Dimension] = v;}
};

}}}  // geometry, boost, traits
