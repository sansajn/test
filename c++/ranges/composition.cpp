// range composition sample
#include <ranges>
#include <algorithm>
#include <string>
#include <map>
#include <iostream>

using std::map;
using std::string;
using std::cout;
using std::views::keys, std::views::values, std::views::reverse, std::views::take, 
	std::views::filter, std::views::transform;
//using std::ranges::accumulate;

int main(int argc, char * argv[])
{
	map<string, int>freq_world{
		{"witch", 25}, {"wizard", 35}, {"tale", 45}, {"dog",4}, {"cat", 34}, {"fish", 23}};

	cout << "all words: ";
	for (auto const & name : keys(freq_world)) cout << name << " ";
	cout << "\n";

	cout << "all words reverse: ";
	for (auto const & name : keys(freq_world) | reverse) cout << name << " ";
	cout << "\n";

	cout << "the first 4 words: ";
	for (auto const & name : keys(freq_world) | take(4)) cout << name << " ";
	cout << "\n";

	cout << "all wods starting with w: ";
	auto firstw = [](string const & name){return name[0] == 'w';};
	for (auto const & name : keys(freq_world) | filter(firstw)) cout << name << " ";
	cout << "\n";

	// std::ranges::accumulate not part of the standard 
	// auto length_of = [](string const & name){return name.size();};
	// auto rng = keys(freq_world) | transform(length_of);
	// auto res = accumulate(begin(rng), end(rng), 0);
	// cout << "sum of words: " << res << "\n";

	cout << "\n"
		<< "done!\n";

	return 0;
}
