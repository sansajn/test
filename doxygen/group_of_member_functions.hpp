/*! \file 
Grouping member functions sample */

//! Object instancing.
struct instance {
	instance(object const & o);

	/*! \name Transformations.
	Instance transformations ... 
	\note Angles are in radians. */
	//! \{
	void translate(vec3 const & v);

	/*! Counter-clockwise rotation around vector.
	\param[in] angle Angle in radians. */
	void rotate(double angle, vec3 const & v);
	//! \}
};
