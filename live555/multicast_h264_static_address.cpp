/* implementacia multicast streamera */
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <arpa/inet.h>
#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>
#include <groupsock/GroupsockHelper.hh>

using std::string;
using std::unique_ptr;

char const * input_file_name = "test.264";
H264VideoStreamFramer * video_source = nullptr;
UsageEnvironment * env = nullptr;
RTPSink * video_sink = nullptr;

void play();
void after_playing(void *);


int main(int argc, char * argv[])
{
	TaskScheduler * sched = BasicTaskScheduler::createNew();
	assert(sched);

	env = BasicUsageEnvironment::createNew(*sched);
	assert(env);

	// groupsock
	in_addr dest_address;
//	dest_address.s_addr = chooseRandomIPv4SSMAddress(*env);
	dest_address.s_addr = inet_addr("239.0.0.16");

	unsigned short const rtp_port_num = 18888;
	unsigned short const rtcp_port_num = rtp_port_num + 1;
	unsigned char const ttl = 255;

	Port const rtp_port{rtp_port_num};
	Port const rtcp_port{rtcp_port_num};

	Groupsock rtp_group{*env, dest_address, rtp_port, ttl};
	rtp_group.multicastSendOnly();  // we're a SSM source

	Groupsock rtcp_group{*env, dest_address, rtcp_port, ttl};
	rtcp_group.multicastSendOnly();

	// create a h264 video RTP sink from the RTP group
	OutPacketBuffer::maxSize = 100000;
	video_sink = H264VideoRTPSink::createNew(*env, &rtp_group, 96);
	assert(video_sink);

	RTSPServer * rtsp_serv = RTSPServer::createNew(*env, 8554);
	if (!rtsp_serv)
		throw std::runtime_error{string{"failed to create RTSP server: "} + string{env->getResultMsg()}};

	ServerMediaSession * sms = ServerMediaSession::createNew(*env, "testStream",
		input_file_name, "Session streamed by 'unicast_h264' streamer", True /*SSM*/);
	assert(sms);
	sms->addSubsession(PassiveServerMediaSubsession::createNew(*video_sink));
	rtsp_serv->addServerMediaSession(sms);

	*env << "Play this stream using the URL '" << unique_ptr<char>{rtsp_serv->rtspURL(sms)}.get() << "'\n";

	// start the streaming
	*env << "Beginning streaming ...\n";
	play();

	env->taskScheduler().doEventLoop();  // does not return

	return 0;
}

void play()
{
	// open the input file as a 'byte stream file source'
	ByteStreamFileSource * file_source = ByteStreamFileSource::createNew(*env, input_file_name);
	if (!file_source)
		throw std::runtime_error{string{"Unable to open file '"} + input_file_name + "' as a byte-stream file source"};

	FramedSource * video_es = file_source;
	video_source = H264VideoStreamFramer::createNew(*env, video_es);  // create a framer for the video elementary stream

	// finally, start playing
	*env << "Beginning to read from file ...\n";
	video_sink->startPlaying(*video_source, after_playing, video_sink);
}

void after_playing(void *)
{
	*env << "... done reading from file\n";
	video_sink->stopPlaying();
	Medium::close(video_source);
	// note that this also ocloses the input file that this source read from.

	// start playing one again
	play();
}
