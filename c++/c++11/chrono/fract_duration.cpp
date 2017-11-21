#include <iostream>
#include <chrono>
#include <ctime>
 
 using std::cout;
 
long fibonacci(unsigned n)
{
	if (n < 2)
		return n;

	return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
	auto start = std::chrono::system_clock::now();
	cout << "f(42) = " << fibonacci(42) << '\n';
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
}
