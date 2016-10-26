#include <string>
#include <boost/python.hpp>

using std::string;
namespace py = boost::python;

struct Var
{
	Var(string name) : name{name} {}
	string name;
	float value;
};


int main(int argc, char * argv[])
{
	return 0;
}
