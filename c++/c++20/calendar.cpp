// chrono calendar sample (not yet supported even by gcc10)

#include <chrono>
#include <iostream>
using std::cout, std::endl;
using namespace std::chrono;

int main(int argc, char * argv[])
{
	auto date = year{1940} / month{6} / day{26}; 
	return 0;
}
