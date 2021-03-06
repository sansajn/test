// async() sample (at least C++11 required)
#include <future>
#include <vector>
#include <iostream>
using std::vector;
using std::async, std::future;
using std::cout;

int meaning_of_life()
{
	return 42;
}

int main(int argc, char * argv[])
{
	// run one worker
	future<int> answer = async(meaning_of_life);
	cout << "The answer is " << answer.get() << "\n";
	
	// run more workers
	vector<future<int>> results;
	for (int i = 0; i< 5; ++i)
		results.push_back(async(meaning_of_life));

	for (auto & r : results)
		cout << r.get() << ", ";
	cout << "\n";

	cout << "done!\n";
	return 0;
}
