/*! \file 
\note QT_AUTOBRIEF needs to be set to YES */

//! The most simple function.
void foo();

void goo();  //!< In line comment.

/*! Function that does nothing.
\param [in] arg the first argument */
void foo(int arg);

/*! Function with more arguments.

The function has not only more arguments, but it also return value and not to mention, that `d` argumnt is used as output.

\param [in] i the first argument
\param [out] d the second argument
\sa foo
\return returns always true */
bool foo(int i, double & d);
