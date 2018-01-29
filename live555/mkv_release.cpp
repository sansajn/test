// uvolnenie Matroska* struktur
#include <cassert>
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

char quit = 0;
MatroskaFile * __file = nullptr;
MatroskaDemux * __demux = nullptr;

void on_file_ready(MatroskaFile * file, void * user_data)
{
	__file = file;
	__demux = file->newDemux();
	quit = 1;
}

int main(int argc, char * argv[])
{
	TaskScheduler * sched = BasicTaskScheduler::createNew();
	UsageEnvironment * env = BasicUsageEnvironment::createNew(*sched);

	MatroskaFile::createNew(*env, "test1.mkv", on_file_ready, nullptr, "eng");

	sched->doEventLoop(&quit);

	Medium::close(__demux);  // needs to be there otherwise env->reclaim() fails
	Medium::close(__file);

	bool released = env->reclaim();
	assert(released);

	delete sched;

	return 0;
}
