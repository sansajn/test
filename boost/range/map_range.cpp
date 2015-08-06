#include <map>
#include <string>
#include <boost/range.hpp>

struct foo
{
	foo()
	{
		_vals[10] = "desat";
		_vals[1] = "jedna";
		_vals[3] = "tri";
	}

	void vals();

	std::map<int, std::string> _vals;
};


int main(int argc, char * argv[]) 
{
	foo f;

	return 0;
}
