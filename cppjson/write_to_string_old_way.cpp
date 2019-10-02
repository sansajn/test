#include <string>
#include <iostream>
#include <json/json.h>

using std::cout;
using std::string;

int main(int argc, char * argv[])
{
	Json::Value root;
	root["action"] = "run";

	Json::Value data;
	data["number"] = 1;

	root["data"] = data;

	// compact json builder
	/*Json::StreamWriterBuilder builder;
	builder.settings_["commentStyle"] = "None";
	builder.settings_["indentation"] = "";*/
	
	Json::FastWriter writer;
	string s = writer.write(root);
	
	cout << s << "\n";

	/*cout << "with a settings\n";
	cout << builder.settings_ << "\n";*/

	cout << "done!";

	return 0;
}
