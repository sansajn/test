// on demand h264 server, minimalna ukazka
#include <string>
#include <stdexcept>
#include <cassert>
#include <liveMedia/liveMedia.hh>
#include <liveMedia/H265VideoFileServerMediaSubsession.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>

using std::string;

char const * video_path = "test.264";
char const * stream_name = "h264TestStream";

int main(int argc, char * argv[])
{
	TaskScheduler * sched = BasicTaskScheduler::createNew();
	assert(sched);

	UsageEnvironment * env = BasicUsageEnvironment::createNew(*sched);
	assert(env);

	UserAuthenticationDatabase * udb = nullptr;
	// access control implementation comes there ...

	RTSPServer * serv = RTSPServer::createNew(*env, 8554, udb);
	if (!serv)
		throw std::runtime_error{string{"unable to create a RTSPServer, reason:"} + env->getResultMsg()};
	assert(serv);

	char const * desc = "streamed by livemedia library";

	ServerMediaSession * sms = ServerMediaSession::createNew(*env, stream_name, stream_name, desc);
	sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, video_path, false));

	serv->addServerMediaSession(sms);

	// announce
	*env << stream_name << " stream, from the file " << video_path << "\n"
		<< "Play this stream using the URL " << serv->rtspURL(sms) << "\n";

	env->taskScheduler().doEventLoop();  // does not return

	return 0;
}
