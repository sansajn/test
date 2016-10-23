// simple search test
#include <regex>
#include <string>
#include <iostream>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	std::regex pat{R"(\w{2}\s*\d{5}(-\d{4})?)"};  // us postal code pattern e.g. TX77845 or DC 20500-0001

	string inputs[] = {"TX77845", "TXC77845", "DC 20500-0001", "DC20500-000134"};
	
	for (string const & in : inputs)
	{
		std::smatch what;
		if (regex_search(in, what, pat))
		{
			cout << "match: " << what[0] << "\n";  // complete match
			if (what.size() > 1 && what[1].matched)
				cout << "   " << what[1] << "\n";  // submatch
		}
		else
			cout << in << " doesn't match US postal service pattern\n";
	}

	return 0;
}
