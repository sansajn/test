/* v20110924 */
#pragma once
// linux
#include <ctime>


namespace loe {

//! Meranie času.
/*! \note Treba zlinkovať spolu s rt knižnicou (-lrt). */
class stop_watch
{
public:
	void start()
	{
		timespec ts;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
		_start = ts.tv_nsec;
	}

	// Vráti čas od spustenia v ns.
	long stop() const
	{
		timespec ts;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
		return ts.tv_nsec - _start;
	}

private:
	long _start;
};

}  // loe

