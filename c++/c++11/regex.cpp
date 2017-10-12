// hodi regex_error
#include <regex>
#include <iostream>
#include <string>
using namespace std;

void uszip(string const & line)
{
	try {
		regex pat(R"(\w{2}\s*\d{5}(-\d{4})?)");
//		cout << "pattern: " << pat << "\n";
		smatch match;
		if (regex_search(line, match, pat))
			cout << match[0] << "\n";
	} 
	catch (std::regex_error & e) {
		cout << e.what() << "\n";
	}
}

int main(int argc, char * argv[])
{
	string line("ahoj TX 77845 blbost");
	uszip(line);
	return 0;
}
	
	
