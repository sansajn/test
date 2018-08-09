// mp3 RTSP or RTP streamer sample
#include <memory>
#include <cassert>
#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>
#include <groupsock/GroupsockHelper.hh>

using std::unique_ptr;

UsageEnvironment * env;
RTSPServer * rtsp;

struct session_state
{
	FramedSource * source;
	RTPSink * sink;
	Groupsock * rtpGroupsock;
};

session_state sess;

void play();
void afterPlaying(void * user_data);

char const * inputFileName;


int main(int argc, char * argv[])
{
	inputFileName = (argc > 1) ? argv[1] : "amelie.mp3";
	bool rtp_only = (argc > 2) ? strcmp(argv[2], "rtp") == 0 : false;

	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	const unsigned short rtpPortNum = 6666;
	const unsigned char ttl = 1; // low, in case routers don't admin scope

	in_addr dest_address;
//	dest_address.s_addr = chooseRandomIPv4SSMAddress(*env);
	dest_address.s_addr = inet_addr("127.0.0.1");

	sess.rtpGroupsock = new Groupsock(*env, dest_address, Port{rtpPortNum}, ttl);

	sess.sink = MPEG1or2AudioRTPSink::createNew(*env, sess.rtpGroupsock);

	if (!rtp_only)
	{
		rtsp = RTSPServer::createNew(*env, 6666);
		if (!rtsp)
		{
			*env << "failed to create RTSP server, what:" << env->getResultMsg() << "\n";
			exit(1);
		}

		ServerMediaSession * sms = ServerMediaSession::createNew(*env, "test", "", "");
		assert(sms);
		sms->addSubsession(PassiveServerMediaSubsession::createNew(*sess.sink));
		rtsp->addServerMediaSession(sms);

		unique_ptr<char []> url{rtsp->rtspURL(sms)};
		*env << "play this stream with '$ cvlc " << url.get() << "\n";
	}
	else
		*env << "play this stream with '$ cvlc rtp://127.0.0.1:6666'\n";

	play();

	env->taskScheduler().doEventLoop();  // does not return
	return 0;
}

void play()
{
  // Open the file as a 'MP3 file source':
  sess.source = MP3FileSource::createNew(*env, inputFileName);
  if (!sess.source)
  {
	 *env << "Unable to open file \"" << inputFileName
	 << "\" as a MP3 file source\n";
	 exit(1);
  }

  // Finally, start the streaming:
  *env << "Beginning streaming...\n";
  sess.sink->startPlaying(*sess.source, afterPlaying, NULL);
}

void afterPlaying(void * /*user_data*/)
{
  *env << "...done streaming\n";

  sess.sink->stopPlaying();

  // End this loop by closing the current source:
  Medium::close(sess.source);

  // And start another loop:
  play();
}
