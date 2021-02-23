// keys, values views sample
#include <ranges>
#include <string>
#include <unordered_map>
#include <iostream>

using std::unordered_map;
using std::string;
using std::cout;
using std::views::keys, std::views::values;

int main(int argc, char * argv[])
{
	unordered_map<string, int>freq_world{
		{"witch", 25}, {"wizard", 35}, {"tale", 45}, {"dog",4}, {"cat", 34}, 
		{"fish", 23}};

	cout << "keys:\n";
	for (auto const & name : keys(freq_world))
		cout << name << " ";
	
	cout << "\n\n";

	cout << "values:\n";
	for (auto const & value : values(freq_world))
		cout << value << " ";

	cout << "\n\n"
		<< "done!\n";

	return 0;
}
