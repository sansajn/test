/*! \file 
Template function samples. */

/*! Template function with one templated argument. 
\tparam T just a generic type 
\param [in] a the first argument */
template <typename T>
void foo(T a);

/*! Template function with one templated argument and one standard argument.
\tparam T just a generic type 
\param [in] a the first argument 
\param [in] n multiple n times */
template <typename T>
void foo(T a, int n);

/*! Template function with templated return type. 
\tparam R destination type
\tparam T source type
\param [in] val value to be converted to R */
template <typename R, typename T>
R cast_to(T const & val);
