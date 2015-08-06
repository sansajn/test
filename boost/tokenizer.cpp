#include <string>
#include <iostream>
#include <boost/tokenizer.hpp>

using std::cout;

int main(int argc, char * argv[])
{
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

	std::string s = "shaders/nolighting.vs;shaders/nolighting.fs";
	for (auto & t : tokenizer(s, boost::char_separator<char>(";")))
		cout << t << ":" << t.size() << "\n";

	return 0;
}

