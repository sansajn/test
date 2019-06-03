// vykonanie uzivatelskej ulohy
#include <iostream>
#include <cassert>
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

using std::cout;

char quit_flag = 0;


void quiter(void * _)
{
	quit_flag = 1;
	cout << "hello from scheduled task" << std::endl;
}

int main(int argc, char * argv[])
{
	TaskScheduler * sched = BasicTaskScheduler::createNew();
	assert(sched);

	UsageEnvironment * env = BasicUsageEnvironment::createNew(*sched);
	assert(env);

	sched->scheduleDelayedTask(2*1e6, quiter, nullptr);  // two second delayed quit

	env->taskScheduler().doEventLoop(&quit_flag);

	cout << "done!\n";

	env->reclaim();
	delete sched;

	return 0;
}
