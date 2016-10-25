/* pre zbuildovanie zadaj '$ python setup.py build', zdielatelna kniznica 
sa ulozi do build/lib.linux-i686.2.7 odkial spusti '$ python' a kniznica 
bude dostupna [Ukazka:

	>>> import hello
	>>> hello.say_hello('Patric')
	Hello Patric!

--- koniec ukazky]. */
#include <iostream>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

namespace py = boost::python;
using std::cout;

void say_hello(char const * name)
{
	cout << "Hello " << name << "!\n";
}

BOOST_PYTHON_MODULE(hello)
{
	py::def("say_hello", say_hello);
}
