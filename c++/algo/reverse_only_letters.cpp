// reverse only letters in array with copy_if and transform algorithms sample
#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

using std::copy_if, std::back_inserter, std::transform, std::string, std::cout;


string reverse_only_letters(string s)
{
	string letters;
	copy_if(rbegin(s), rend(s), back_inserter(letters), ::isalpha);

	transform(begin(s), end(s), begin(s), 
		[c = cbegin(letters)](auto e) mutable {
			return isalpha(e) ? *c++ : e;
		});

	return s;
}

int main(int argc, char * argv[])
{
	string s = "a12b3cdef";
	cout << s << " -> " << reverse_only_letters(s) << "\n";
	return 0;
}
