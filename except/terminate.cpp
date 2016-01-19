#include <stdexcept>
#include <string>

struct foo 
{
	void throw_inside() 
	{
		std::string s;
		s += __FUNCTION__;
		s += "() [";
		s += __FILE__;
		s += ":";
		s += std::to_string(__LINE__);
		s += "]";
		throw std::logic_error(s);
	}
};

int main(int argc, char * argv[])
{
	foo f;
	f.throw_inside();
	return 0;
}
