// umoznuje implementaciu napr. rollback akcii
#include <vector>
#include <iostream>
#include <boost/scope_exit.hpp>

using std::vector;
using std::cout;

void scope_exit_sample(vector<int> & numbers)
{
	numbers.push_back(4);  // action
	
	BOOST_SCOPE_EXIT(&numbers) {
		numbers.pop_back();  // rollback action
	} BOOST_SCOPE_EXIT_END
}

int main(int argc, char * argv[])
{
	vector<int> numbers{1, 2, 3};
	scope_exit_sample(numbers);
	cout << "3 == " << numbers.size() << "\n";

	return 0;
}
