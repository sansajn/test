// utc time v c++
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <iostream>
#include <sys/time.h>

using namespace std::chrono;

int main(int argc, char * argv[])
{
	// * time_t nema dostatocne rozlisenie k zobrazeniu ms
	
	// metoda 1
	{
		system_clock::time_point now = system_clock::now();
		time_t tt = system_clock::to_time_t(now);
		tm utc_tm = *gmtime(&tt);
		char str[1024];
		strftime(str, 1024, "%Y-%m-%d-%H-%M-%S-%f", &utc_tm);
		std::cout << str << "\n";
	}
	
	// method 2
	{
		time_t t = time(NULL);
		std::cout << std::put_time(gmtime(&t), "%Y-%m-%d-%H-%M-%S-%f") << "\n";
	}
	
	// method 3
	{
		system_clock::time_point now = system_clock::now();
		system_clock::duration dur = now.time_since_epoch();
		std::cout << dur.count() 
			<< "[" << system_clock::duration::period::num << "/" 
				<< system_clock::duration::period::den << "]" << std::endl;
		
		// TODO: ako to sformatovat ?
	}
	
	// linux only
	{
		timeval tm;
		gettimeofday(&tm, nullptr);
		char str[1024];
		strftime(str, 1024, "%Y-%m-%d-%H-%M-%S", gmtime(&tm.tv_sec));
		std::cout << str << "-" << tm.tv_usec << "\n";
	}
	
	return 0;
}
