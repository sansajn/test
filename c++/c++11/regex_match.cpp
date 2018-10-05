#include <regex>
#include <string>
#include <iostream>

using std::string;
using std::regex;
using std::smatch;
using std::regex_match;
using std::cout;

bool parse_file_name(string const & name, string & author, string & title)
{
	try {
		// {author}@{title}-{hash}.{ext}
		static regex const pat{R"((.+)@(.+)-(.{11})\..+$)"};

		smatch match;
		if (regex_match(name, match, pat))
		{
			author = match[1];
			title = match[2];
			return true;
		}
		else
			return false;
	}
	catch (std::regex_error & e) {
		cout << e.what() << "\n";
	}

	return false;
}

int main(int argc, char * argv[])
{
	string names[] = {
		"Afrojack - Live @ Ultra Music Festival Miami 2017-RXAm-6rKVrk.opus",
		"Aly & Fila Live @ Ultra Music Festival Miami 2016-qFQ0N944HWo.opus",
		"Andrew Rayel - Live @ Tomorrowland 2016 [Audio]-d9wHbuGU7zY.opus",
		"2-Hours Epic Music Mix _ THE POWER OF EPIC MUSIC - Full Mix Vol. 3-3TAUnYZpMbA.opus"
	};

	for (string const & name : names)
	{
		string author, title;
		if (parse_file_name(name, author, title))
			cout << "{author=" << author << ", " << "title=" << title << "}\n";
		else
			cout << ":(\n";
	}

	return 0;
}
