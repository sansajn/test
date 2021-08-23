/*! \file */

/*! The most simpliest class ever. 
With more detailed description there. 
\note An inportant note. 
\bug The foo implementation does nothing. 
\warning The class can explode when you use it. 
\sa see_also_class 

The class can be used this way:
\code
foo f;
\endcode */
class foo {};


class see_also_class {};


/*! Stack trace listing.
The stacktrace instance is aim to be used to get stack trace (in case of signal).

The class can be used this way:
\code
cout << stacktrace{} << endl;
\endcode 

which prints stack trace to std::cout. */
class stacktrace {
public:
	stacktrace();
	friend std::ostream & operator<<(std::ostream & out, stacktrace const & st);
	~stacktrace();

private:
	char ** _lines;
	int _size;
};


/*! Template class sample. 
\tparam T Generic type. */
template <typename T>
class template_class {
private:
	T _val;
};


/*! Class with template member function sample. */
struct class_with_template_function {
	/*! Member function with template argument.
	\tparam T Generic type argument. */
	template <typename T>
	void foo(T t);
};
