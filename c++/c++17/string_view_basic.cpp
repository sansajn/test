// basic string_view sample

#include <string_view>
#include <string>
#include <iostream>

using std::string_view;
using std::string;
using std::cout;
using namespace std::string_view_literals;

int main(int argc, char * argv[])
{
	constexpr string_view sv1 = "string_view literal"sv;  // string view literal
	
	string s = "hello";
	string_view sv2 = s;  // hello!

	char const * c_str = "hello!";
	string_view sv3 = c_str;  // hello!

	string_view sv4{c_str, 3};  // hel

	cout << "sv1='" << sv1 << "'\n"
		<< "sv2='" << sv2 << "'\n"
		<< "sv3='" << sv3 << "'\n"
		<< "sv4='" << sv4 << "'\n";

	cout << "done!\n";

	return 0;
}
