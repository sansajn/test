// test konstruktoru s rvalue referenciou
#include <string>
#include <iostream>
using std::cout;
using std::string;

// move, forward ?
class foo
{
public:
	foo(string && s)
		: _s{move(s)}
	{}
	
	string const & str() const {return _s;}
	
private:
	string _s;
};

int main(int argc, char * argv[])
{
	string s = "hello!";
	foo f{move(s)};
	cout << f.str() << "\n";
	
	return 0;
}
