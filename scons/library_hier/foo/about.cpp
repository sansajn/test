#include "about.hpp"
using std::string;

string about::version() const
{
	return string("0.0.0.1");
}

string about::author() const
{
	return string("Franta");
}

