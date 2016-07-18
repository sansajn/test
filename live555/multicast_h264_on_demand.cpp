/* TODO: toto neviem ci funguje, tak ako si myslim, lebo jediny rozdiel voci on-demand je true pri vytvarani H264VideoFileServerMediaSubsession.
principialne nejde on-demand vytvorit tak aby bol multicast */
#include <string>
#include <stdexcept>
#include <cassert>
#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>
#include <groupsock/GroupsockHelper.hh>

using std::string;

char const * input_file_name = "test.264";

int main(int argc, char * argv[])
{
	TaskScheduler * sched = BasicTaskScheduler::createNew();
	assert(sched);

	UsageEnvironment * env = BasicUsageEnvironment::createNew(*sched);
	assert(env);

	RTSPServer * rtsp_serv = RTSPServer::createNew(*env, 8554);
	if (!rtsp_serv)
		throw std::runtime_error{string{"failed to create RTSP server: "} + string{env->getResultMsg()}};

	ServerMediaSession * sms = ServerMediaSession::createNew(*env, "testStream",
		input_file_name, "Session streamed by 'unicast_h264' streamer", True /*SSM*/);
	assert(sms);
	sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, input_file_name, true));
	rtsp_serv->addServerMediaSession(sms);

	char * url = rtsp_serv->rtspURL(sms);
	*env << "Play this stream using the URL '" << url << "'\n";
	delete [] url;

	// start the streaming
	*env << "Beginning streaming ...\n";

	env->taskScheduler().doEventLoop();  // does not return

	return 0;
}
