#include <algorithm>
#include <vector>

int main(int argc, char * argv[])
{
	int divisor = 5;
	
	std::vector<int> container{5, 50, 15, 5, 10, 0};
	
	if (std::all_of(begin(container), end(container), 
		[divisor](int value) {  // by-value capture
			return value % divisor == 0;
		}))
	{
		// do something
	}
		
	return 0;
}
