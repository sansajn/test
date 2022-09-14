/*! format() sample .
/note format is  not available for 11 therefore we use fmt library instead*/

#include <string>
#include <iostream>
#include <fmt/format.h>
using std::string, std::cout;
using fmt::format;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	string const s{"hello"};
	cout << format("String '{}' has {} chars\n", s, size(s))
		<< format("{1} is the size of string '{0}'\n", s, size(s));
		
	// field width samples
	cout << format("{:7}\n", 42)  // "     42"
		<< format("{:7}\n", 42.0)  // "     42"
		<< format("{:7}\n", 'x');  // "x      "

	cout << format("{:*<7}\n", 42)  // "42*****"
		<< format("{:*>7}\n", 42)    // "*****42"
		<< format("{:*^7}\n", 42);   // "**42***"

	// precision formatting
	cout << format("{:7.2f} Euro\n", 42.0);  // "  42.00 Euro"

	// conversions
	cout << format("'{0}' has value {0:02X} {0:+4d} {0:03o}\n", '?');  // "'?' has value 3F  +63 077"

	// pointer formatting
	cout << format("{}\n", static_cast<void const *>(&s));

	cout << "done!\n";
	return 0;
}
