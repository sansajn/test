#include <map>
#include <string>
#include <iostream>
using std::map;
using std::string;
using std::cout;


char const * list[] = {
	"hello",
	"my",
	"name",
	"is",
	"footway",
	"peter",
	NULL
};

bool is_in(map<string, string> & profile, char const * lst[]);

int main(int argc, char * argv[])
{
	map<string, string> prof;
	prof["highway"] = "footway";
	if (is_in(prof, list))
		cout << "is in\n";
	else
		cout << "is not in\n";
	return 0;
}

bool is_in(map<string, string> & profile, char const * lst[])
{
	char const ** word = lst;
	while (*word)
	{
		if (profile["highway"] == *word)
			return true;		
		word += 1;
	}
	return false;
}




