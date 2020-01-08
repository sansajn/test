// mkv streaming with proper release
#include <string>
#include <iostream>
#include <memory>
#include <cassert>
#include <liveMedia.hh>
#include <Groupsock.hh>
#include <GroupsockHelper.hh>
#include <BasicUsageEnvironment.hh>

using std::string;
using std::cout;
using std::endl;
using std::ostream;
using std::unique_ptr;

void play(string const & fname);
void after_playing(void *);
void open_mkv(string const & fname);
void on_mkv_ready(MatroskaFile * new_file, void * data);
ostream & operator<<(ostream & out, in_addr const & addr);

UsageEnvironment * env = nullptr;
RTPSink * mkv_sink = nullptr;
FramedSource * mkv_source = nullptr;
Groupsock * rtp_sock = nullptr;
RTSPServer * rtsp_serv = nullptr;
MatroskaFile * mkv = nullptr;
unsigned mkv_video_track_number = -1u;
ServerMediaSession * mkv_sms = nullptr;
char quit_loop = 0;


int main(int argc, char * argv[])
{
	string input_file = "hero6_1s.mkv";
	if (argc > 1)
		input_file = argv[1];

	TaskScheduler * sched = BasicTaskScheduler::createNew();
	assert(sched);

	env = BasicUsageEnvironment::createNew(*sched);
	assert(env);

	in_addr dest_address;
	dest_address.s_addr = chooseRandomIPv4SSMAddress(*env);
	cout << "rtp/rtcp address: " << dest_address << "\n";

	rtp_sock = new Groupsock{*env, dest_address, 18888, 255};
	rtp_sock->multicastSendOnly();

	rtsp_serv = RTSPServer::createNew(*env, 8554);
	if (!rtsp_serv)
		throw std::runtime_error{"failed to create RTSP server, what: " + string{env->getResultMsg()}};

	mkv_sms = ServerMediaSession::createNew(*env, "mkv_stream", "info",
		"description ...", True);
	assert(mkv_sms);

	rtsp_serv->addServerMediaSession(mkv_sms);

	play(input_file);

	sched->doEventLoop(&quit_loop);

	// cleanup
	Medium::close(mkv_sink);
	Medium::close(mkv_source);
	Medium::close(mkv);
	Medium::close(rtsp_serv);

	bool released = env->reclaim();
	assert(released && "unable to release");

	cout << "done!\n";
	return 0;
}

void open_mkv(string const & fname)
{
	// mkv
	MatroskaFile::createNew(*env, fname.c_str(), on_mkv_ready, nullptr);
	quit_loop = 0;
	env->taskScheduler().doEventLoop(&quit_loop);
	assert(mkv);

	cout << "mkv info:\n"
		<< "  fileName: " << mkv->fileName() << "\n"
		<< "  fileDuration: " << mkv->fileDuration() << "s\n";

	MatroskaDemux * demux = mkv->newDemux();

	FramedSource * source = demux->newDemuxedTrack(mkv_video_track_number);  // source vlastni MatroskaFile
	assert(source);

	cout << "  video track_number: " << mkv_video_track_number << std::endl;

	// inspect track
	MatroskaTrack * video_track = mkv->lookup(mkv_video_track_number);
	assert(video_track->trackType == MATROSKA_TRACK_TYPE_VIDEO);  // check we have realy video track

	unsigned bitrate, filter_count;
	mkv_source = mkv->createSourceForStreaming(source,	mkv_video_track_number,
		bitrate, filter_count);

	mkv_sink = mkv->createRTPSinkForTrackNumber(mkv_video_track_number, rtp_sock, 96);
	assert(mkv_sink);

	cout << "  estimated-bitrate: " << bitrate << "\n"
		<< "  sink-estimated-bitrate: " << mkv_sink->estimatedBitrate() << std::endl;
	
	if (mkv_sink->estimatedBitrate() > 0)
		bitrate = mkv_sink->estimatedBitrate();

	mkv_sms->deleteAllSubsessions();
	mkv_sms->addSubsession(PassiveServerMediaSubsession::createNew(*mkv_sink));
}

void on_mkv_ready(MatroskaFile * new_file, void * data)
{
	mkv = new_file;
	quit_loop = 1;
}

void play(string const & fname)
{
	unique_ptr<char []> url{rtsp_serv->rtspURL(mkv_sms)};
	cout << "streaming jpeg, use\n"
		<< "  cvlc --rtsp-frame-buffer-size=300000 " << url.get() << "\n"
		<< "command to see stream content" << endl;

	open_mkv(fname);
	mkv_sink->startPlaying(*mkv_source, after_playing, nullptr);
	quit_loop = 0;
}

void after_playing(void *)
{
	quit_loop = 1;
}

ostream & operator<<(ostream & out, in_addr const & addr)
{
	char addr_buf[16];
	inet_ntop(AF_INET, &addr.s_addr, addr_buf, 16);
	addr_buf[15] = '\0';
	out << addr_buf;
	return out;
}
