/*! C++20 utf8 support sample.
\note In case UTF8 needed use some library, standard doesn't provide full support. */
#include <string>
#include <iostream>
using std::u8string, std::cout;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	cout << "sizeof(char8_t) = " << sizeof(char8_t) << '\n';

	u8string s8 = u8"\u20AC0.50";  // €0.50
	cout << "s8 = " << reinterpret_cast<char const *>(s8.c_str()) << '\n'
		<< "s8.size() = " << s8.size() << '\n'
		<< "s8.length() = " << s8.length() << '\n';

	cout << "done!\n";
	return 0;
}
