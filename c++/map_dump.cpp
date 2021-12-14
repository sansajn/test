// dump std::map sample
#include <map>
#include <vector>
#include <string>
#include <iostream>
using std::map, std::vector, std::string, std::cout;

// C++14 partial specialization
void print(string const & s) {
	cout << s;
}

template <typename T>
void print(vector<T> const & v) {
	cout << "[";
	for (auto const & elem : v) {
		print(elem);
		cout << ",";
	}
	cout << "]";
}

template <typename Value>
void print(map<int, Value> const & m) {
	cout << "{";
	for (auto const & kv : m) {
		cout << "{" << kv.first << ",";
		print(kv.second);
		cout << "}, ";
	}
	cout << "}";
}


// C++11
template <typename T>
std::ostream & operator<<(std::ostream & o, vector<T> const & v) {
	o << "[";
	for (auto const & elem : v)
		o << elem << ",";
	o << "]";
	return o;
}

template <typename Value>
std::ostream & operator<<(std::ostream & o, map<int, Value> const & m) {
	o << "{";
	for (auto const & kv : m) {
		o << "{" << kv.first << "," << kv.second << "},";
	}
	o << "}, ";
	return o;
}


int main(int argc, char * argv[]) {
	map<int, string> data = {{1, "a"}, {2, "b"}, {3, "c"}};
	print(data);
	cout << "\n";
	
	map<int, vector<string>> data2 = {
		{1, {"a1", "a2"}}, 
		{2, {"b"}}, 
		{3, {"c1", "c2"}}
	};
	print(data2);
	cout << "\n";

	cout << "data2: " << data2 << "\n";


	cout << "done!\n";
	return 0;
}
