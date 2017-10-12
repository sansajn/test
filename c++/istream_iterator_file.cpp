#include <algorithm>
#include <string>
#include <iterator>
#include <stdexcept>
#include <fstream>
#include <iostream>

std::string file_path = "istream_iterator_file.cpp";

int main(int argc, char * argv[])
{
	std::ifstream in{file_path};
	if (!in.is_open())
		throw std::logic_error{std::string{"unable to open '"} + file_path + "'"};

	in.unsetf(std::ios_base::skipws);  // do not ignore white spaces

	std::istream_iterator<char> it{in}, it_end{};

	std::copy(it, it_end, std::ostream_iterator<char>(std::cout, ""));

	return 0;
}
