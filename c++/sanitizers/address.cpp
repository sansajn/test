/* address sanitizer test
compile as `g++ address.cpp -Wall -Werror -g -fsanitize=address` */

#include <string>
using std::string;

int main(int argc, char * argv[]) {
	char const * names[] = {"bob", "fred"};
	string last = names[3];  // <--- bug there
	return last.size();
}
