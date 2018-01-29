// playground for new ideas
#include <string>
#include <memory>
#include <iostream>
#include <cassert>
#include <liveMedia.hh>
#include <MatroskaFile.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

using std::string;
using std::unique_ptr;
using std::cout;

char const * TEST_VIDEO = "test.mkv";

char mkv_file_ready_flag = 0;
void on_mkv_file_ready(MatroskaFile * file, void * user_data)
{
	*((MatroskaFile **)user_data) = file;
	mkv_file_ready_flag = 1;
}

char after_playing_flag = 0;
void on_after_playing(void * user_data)
{
	RTPSink * sink = (RTPSink *)user_data;
	sink->stopPlaying();
	after_playing_flag = 1;
}

int main(int argc, char * argv[])
{
	string input_video = (argc > 1) ? argv[1] : TEST_VIDEO;

	// [constructor]
	TaskScheduler * sched = BasicTaskScheduler::createNew();
	UsageEnvironment * env = BasicUsageEnvironment::createNew(*sched);

	// [bind]
	RTSPServer * rtsp_serv = RTSPServer::createNew(*env, 7070);
	assert(rtsp_serv);

	ServerMediaSession * sms = ServerMediaSession::createNew(*env, "testStream", "some description ...",
		"", True);
	assert(sms);

	rtsp_serv->addServerMediaSession(sms);

	// [play]
	MatroskaFile * file = nullptr;
	MatroskaFile::createNew(*env, input_video.c_str(), on_mkv_file_ready,
		(void *)&file, "eng");

	sched->doEventLoop(&mkv_file_ready_flag);  // wait file ready
	assert(file);

	MatroskaDemux * demux = file->newDemux();
	assert(demux);

	// source
	unsigned track_num = 0;
	FramedSource * source = demux->newDemuxedTrack(track_num);

	unsigned est_bitrate, num_filters_in_front_of_track;
	source = file->createSourceForStreaming(source, track_num, est_bitrate, num_filters_in_front_of_track);
	assert(source);

	// sink
	struct in_addr addr;
	addr.s_addr = chooseRandomIPv4SSMAddress(*env);
	Groupsock rtp_group{*env, addr, 18888, 255};
	RTPSink * sink = file->createRTPSinkForTrackNumber(track_num, &rtp_group, 96);
	assert(sink);

	if (sink->estimatedBitrate() > 0)
		est_bitrate = sink->estimatedBitrate();

	Groupsock rtcp_group{*env, addr, 18889, 255};
	RTCPInstance * rtcp = RTCPInstance::createNew(*env, &rtcp_group, est_bitrate,
		(unsigned char const *)"", sink, nullptr, True);  // this starts RTCP running automatically
	assert(rtcp);

	sms->addSubsession(PassiveServerMediaSubsession::createNew(*sink, rtcp));

	sink->startPlaying(*source, on_after_playing, (void *)sink);

	unique_ptr<char []> url{rtsp_serv->rtspURL(sms)};
	cout << "use 'cvlc " << url.get() << "' command to see streamed content\n";

	// [join]
	sched->doEventLoop(&after_playing_flag);

	// [destructor]
	Medium::close(sink);
	Medium::close(source);
	Medium::close(rtsp_serv);  // also release sms
	Medium::close(rtcp);
	Medium::close(file);

	bool env_released = env->reclaim();
	assert(env_released);

	delete sched;

	return 0;
}
