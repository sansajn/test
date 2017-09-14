// pouzitie string_ref
#include <string>
#include <iostream>
#include <cstring>
#include <boost/utility/string_ref.hpp>

using std::string;
using std::cout;

boost::string_ref extract_part(boost::string_ref s)
{
	return s.substr(2, 3);
}

int main(int argc, char * argv[])
{
	char buf[1024] = {"hello"};
	if (boost::string_ref{buf, strlen(buf)} == "hello")
		cout << "hello found\n";

	if (extract_part("ABCDEFG").front() == 'C')
		cout << "yes, it starts witg 'C'\n";

	string s{"Patric"};
	if (extract_part(boost::string_ref{s}).front() == 't')
		cout << "verified, it is Patric\n";

	cout << "done.\n";

	return 0;
}
