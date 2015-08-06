// g++ -std=c++0x -g maptest.cpp -o maptest
#include <map>
using std::map;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
#include <iterator>
using std::begin;
using std::end;


void dump(map<string, int> & c);

int main(int argc, char * argv[])
{
	map<string, int> mapped_data;

	vector<string> data{"one", "two", "tree"};
	for (int i = 0; i < data.size(); ++i)
		mapped_data[data[i]] = i+1;

	cout << "~before change~\n";
	dump(mapped_data);

	mapped_data.begin()->second = 5;

	cout << "\n~after changes~\n";
	dump(mapped_data);

	return 0;
}


void dump(map<string, int> & c)
{
	for (auto it = begin(c); it != end(c); ++it)
		cout << "key:" << it->first << ", value:" << it->second << "\n";
}

