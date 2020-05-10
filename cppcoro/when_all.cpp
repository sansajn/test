#include <chrono>
#include <string>
#include <iostream>

using std::chrono::duration_cast, std::chrono::steady_clock;
using namespace std::chrono_literals;
using std::string;
using std::cout, std::endl;

cppcoro::task<string> first()
{
	this_thread::sleep_for(1s);
	co_return "First";
}

cppcoro::task<string> second()
{
	this_thread::sleep_for(1s);
	co_return "Second";
}

cppcoro::task<string> third()
{
	this_thread::sleep_for(1s);
	co_return "Third";
}

cppcoro::task<> run_all()
{
	auto [fir, sec, thi] = co_await cppcoro::when_all(first(), second(), third());
	cout << fir << " " << sec << " " << thi << endl;
}

int main(int argc, char * argv[])
{
	auto start = steady_clock::now();
	cppcoro::sync_wait(run_all());
	cout << "execution time " << duration_cast<double>(steady_clock::now() - start).count() << "s\n";
	return 0;
}
