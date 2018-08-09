// mp3 only RTP streamer sample
#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>
#include <groupsock/GroupsockHelper.hh>

UsageEnvironment * env;

struct session_state
{
	FramedSource * source;
	RTPSink * sink;
	RTCPInstance * rtcpInstance;
	Groupsock * rtpGroupsock;
	Groupsock * rtcpGroupsock;
};

session_state sess;

void play();
void afterPlaying(void * user_data);

char const * inputFileName;


int main(int argc, char * argv[])
{
	inputFileName = (argc > 1) ? argv[1] : "amelie.mp3";

	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	const unsigned short rtpPortNum = 6666;
	const unsigned short rtcpPortNum = rtpPortNum+1;
	const unsigned char ttl = 1; // low, in case routers don't admin scope

	in_addr dest_address;
//	dest_address.s_addr = chooseRandomIPv4SSMAddress(*env);
	dest_address.s_addr = inet_addr("127.0.0.1");

	const Port rtpPort(rtpPortNum);
	const Port rtcpPort(rtcpPortNum);

	sess.rtpGroupsock = new Groupsock(*env, dest_address, rtpPort, ttl);
	sess.rtcpGroupsock = new Groupsock(*env, dest_address, rtcpPort, ttl);

	sess.sink = MPEG1or2AudioRTPSink::createNew(*env, sess.rtpGroupsock);

	// Create (and start) a 'RTCP instance' for this RTP sink:
	const unsigned estimatedSessionBandwidth = 160; // in kbps; for RTCP b/w share
	const unsigned maxCNAMElen = 100;
	unsigned char CNAME[maxCNAMElen+1];
	gethostname((char*)CNAME, maxCNAMElen);
	CNAME[maxCNAMElen] = '\0'; // just in case
	sess.rtcpInstance = RTCPInstance::createNew(*env, sess.rtcpGroupsock,
					 estimatedSessionBandwidth, CNAME,
					 sess.sink, nullptr,
					 /*True*/False);  // SSM (Source Specific Multicast)
	// Note: This starts RTCP running automatically

	*env << "to receive a stream use '$ cvlc rtp://127.0.0.1:6666\n";

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
