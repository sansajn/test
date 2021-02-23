// async() sample (at least C++11 required)
#include <future>
#include <iostream>
using std::cout;
using std::async, std::future;

int meaning_of_life()
{
	return 42;
}

int main(int argc, char * argv[])
{
	future<int> answer = async(meaning_of_life);
	cout << "The answer is " << answer.get() << "\n";
	
	cout << "done!\n";
	return 0;
}
