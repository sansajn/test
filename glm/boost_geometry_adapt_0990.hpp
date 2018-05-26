/*! Adaptuje štruktúry s knižnice glm (0.9.9.0) pre použitie s boost::geometry. */
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
template <typename T, glm::qualifier Q>
struct tag<glm::vec<2, T, Q>>
{
	typedef point_tag type;
};

template <typename T, glm::qualifier Q>
struct coordinate_type<glm::vec<2, T, Q>>
{
	typedef typename glm::vec<2, T, Q>::value_type type;
};

template <typename T, glm::qualifier Q>
struct coordinate_system<glm::vec<2, T, Q>>
{
	typedef cs::cartesian type;
};

template <typename T, glm::qualifier Q>
struct dimension<glm::vec<2, T, Q>> : boost::mpl::int_<2>
{};

template <typename T, glm::qualifier Q, std::size_t Dimension>
struct access<glm::vec<2, T, Q>, Dimension>
{
	typedef glm::vec<2, T, Q> point_type;
	typedef typename glm::vec<2, T, Q>::value_type value_type;

	static value_type get(point_type const & p) {return p[Dimension];}
	static void set(point_type & p, value_type const & v) {p[Dimension] = v;}
};

// glm::vec3 adaptation
template <typename T, glm::qualifier Q>
struct tag<glm::vec<3, T, Q>>
{
	typedef point_tag type;
};

template <typename T, glm::qualifier Q>
struct coordinate_type<glm::vec<3, T, Q>>
{
	typedef typename glm::vec<3, T, Q>::value_type type;
};

template <typename T, glm::qualifier Q>
struct coordinate_system<glm::vec<3, T, Q>>
{
	typedef cs::cartesian type;
};

template <typename T, glm::qualifier Q>
struct dimension<glm::vec<3, T, Q>> : boost::mpl::int_<3>
{};

template <typename T, glm::qualifier Q, std::size_t Dimension>
struct access<glm::vec<3, T, Q>, Dimension>
{
	typedef glm::vec<3, T, Q> point_type;
	typedef typename glm::vec<3, T, Q>::value_type value_type;

	static value_type get(point_type const & p) {return p[Dimension];}
	static void set(point_type & p, value_type const & v) {p[Dimension] = v;}
};

}}}  // geometry, boost, traits
