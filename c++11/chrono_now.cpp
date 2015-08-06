#include <chrono>
#include <iostream>

int main(int argc, char * argv[])
{
	typedef std::chrono::high_resolution_clock clock;
	clock::time_point tp = clock::now();
	auto t_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		tp.time_since_epoch()).count();
	auto t_in_hours = std::chrono::duration_cast<std::chrono::hours>(
		tp.time_since_epoch()).count();
	std::cout << "t:" << t_in_ms << "ms\n";
	std::cout << "t:" << t_in_hours << "h\n";

	typedef std::chrono::duration<double, std::ratio<3600>> custom_hours;
	auto t_in_custom_hours = 
		std::chrono::duration_cast<custom_hours>(tp.time_since_epoch()).count();
	std::cout << "t:" << t_in_custom_hours << "h\n";

	typedef std::chrono::duration<double, std::ratio<3600*24>> days;
	auto t_in_days = std::chrono::duration_cast<days>(
		tp.time_since_epoch()).count();
	std::cout << "t:" << t_in_days << "d\n";

	typedef std::chrono::duration<double, std::ratio<3600*24*365>> years;
	auto t_in_years = std::chrono::duration_cast<years>(
		tp.time_since_epoch()).count();
	std::cout << "t:" << t_in_years << "y\n";


	double t_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		tp.time_since_epoch()).count();
	std::cout << t_ms << "\n";

	return 0;
}

