// wait for PLAY to start streaming
#include <string>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

using std::string;
using std::cout;

string h264_input_file = "test.264";
H264VideoStreamFramer * video_source = nullptr;
UsageEnvironment * env = nullptr;
RTPSink * video_sink = nullptr;
char quit_live555_loop = 0;

void play(void * _);
void after_playing(void *);

class custom_rtsp : public RTSPServer
{
public:
	static custom_rtsp * createNew(UsageEnvironment & env, Port port = 554)
	{
		int sock = setUpOurSocket(env, port);
		if (sock == -1)
			return nullptr;

		return new custom_rtsp{env, sock, port};
	}

	custom_rtsp(UsageEnvironment & env, int socket, Port port)
		: RTSPServer{env, socket, port, nullptr, 65}
	{}

	class custom_client_session : public RTSPClientSession
	{
	public:
		custom_client_session(RTSPServer & serv, u_int32_t session_id)
			: RTSPClientSession{serv, session_id}
		{}

		void handleCmd_PLAY(RTSPClientConnection * conn, ServerMediaSubsession * subsession, char const * fullRequestStr);
	};

private:
	ClientSession * createNewClientSession(u_int32_t session_id) override
	{
		return new custom_client_session{*this, session_id};
	}
};

void custom_rtsp::custom_client_session::handleCmd_PLAY(RTSPClientConnection * conn, ServerMediaSubsession * subsession, char const * fullRequestStr)
{
	play(nullptr);  // make stream available
	RTSPClientSession::handleCmd_PLAY(conn, subsession, fullRequestStr);
}


int main(int argc, char * argv[])
{
	if (argc > 1)
		h264_input_file = argv[1];

	TaskScheduler * sched = BasicTaskScheduler::createNew();
	assert(sched);

	env = BasicUsageEnvironment::createNew(*sched);
	assert(env);

	// groupsock
	in_addr dest_address;
	dest_address.s_addr = chooseRandomIPv4SSMAddress(*env);  // toto nastavuje adresu na ktoru sa poslu data (destination adresu), alternativne mozem pouzit inet_addr() funkciu

	unsigned short const rtp_port_num = 18888;
	unsigned short const rtcp_port_num = rtp_port_num + 1;
	unsigned char const ttl = 255;

	Port const rtp_port{rtp_port_num};
	Port const rtcp_port{rtcp_port_num};

	Groupsock rtp_group{*env, dest_address, rtp_port, ttl};
	rtp_group.multicastSendOnly();  // we're a SSM source TODO: co je SSM ?

	Groupsock rtcp_group{*env, dest_address, rtcp_port, ttl};
	rtcp_group.multicastSendOnly();

	// create a h264 video RTP sink from the RTP group
	OutPacketBuffer::maxSize = 100000;
	video_sink = H264VideoRTPSink::createNew(*env, &rtp_group, 96);
	assert(video_sink);

	custom_rtsp * rtsp_serv = custom_rtsp::createNew(*env, 8554);
	if (!rtsp_serv)
		throw std::runtime_error{string{"failed to create RTSP server: "} + string{env->getResultMsg()}};

	ServerMediaSession * sms = ServerMediaSession::createNew(*env, "testStream",
		h264_input_file.c_str(), "Session streamed by 'unicast_h264' streamer", True /*SSM*/);
	assert(sms);

	char * url = rtsp_serv->rtspURL(sms);
	*env << "Play this stream using the URL '" << url << "'\n";
	delete [] url;

	rtsp_serv->addServerMediaSession(sms);

	sms->addSubsession(PassiveServerMediaSubsession::createNew(*video_sink, nullptr));

	env->taskScheduler().doEventLoop(&quit_live555_loop);

	// release
	Medium::close(video_sink);
	Medium::close(video_source);
	Medium::close(rtsp_serv);

	bool released = env->reclaim();
	assert(released);
	delete sched;

	return 0;
}

void play(void * _)
{
	// open the input file as a 'byte stream file source'
	ByteStreamFileSource * file_source = ByteStreamFileSource::createNew(*env, h264_input_file.c_str());
	if (!file_source)
		throw std::runtime_error{string{"Unable to open file '"} + h264_input_file + "' as a byte-stream file source"};

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
	// note that this also closes the input file that this source read from.

	// quit
	quit_live555_loop = 1;
}
