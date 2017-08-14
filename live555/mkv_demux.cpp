// working mkv streamer (testMKVStream has a BUG, so it will crash after some time)
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <cassert>
#include <boost/log/trivial.hpp>
#include <liveMedia.hh>
#include <MatroskaFile.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

using std::string;
using std::cout;
using std::unique_ptr;
using std::pair;

string input_filename = "test.mkv";

string gethostname();

class player_server  //!< blocking player implementation
{
public:
	player_server(unsigned port = 8554);
	~player_server();
	void play(string const & file_name);
	void run();  //!< blocking

private:
	struct track_state
	{
		unsigned track_number;
		FramedSource * source;
		RTPSink * sink;
		RTCPInstance * rtcp;

		track_state()
			: track_number{0}
			, source{nullptr}
			, sink{nullptr}
			, rtcp{nullptr}
		{}
	};

	using self_and_track_t = pair<player_server *, track_state *>;

	void after_playing(track_state & track);
	void play_tracks();
	void close_source(track_state & track);

	static void on_matroska_file_creation(MatroskaFile * new_file, void * data);
	static void after_playing(void * data);

	track_state _tracks[3];  // video, audio, subtitules
	TaskScheduler * _sched;
	UsageEnvironment * _env;
	struct in_addr _dst_address;
	RTSPServer * _rtsp_serv;
	ServerMediaSession * _sms;
	MatroskaFile * _file;
	MatroskaDemux * _demux;
	unsigned _loop_counter;
};

player_server::player_server(unsigned port)
	: _file{nullptr}
	, _demux{nullptr}
	, _loop_counter{0}
{
	_sched = BasicTaskScheduler::createNew();
	assert(_sched);
	_env = BasicUsageEnvironment::createNew(*_sched);
	assert(_env);
	_dst_address.s_addr = chooseRandomIPv4SSMAddress(*_env);
	_rtsp_serv = RTSPServer::createNew(*_env, port);
	assert(_rtsp_serv);
	_sms = ServerMediaSession::createNew(*_env, "testStream", "some description ...",
		"Session streamed by \"testMKVStreamer\"", True /*SSM*/);
	assert(_sms);
}

template <typename T>
void release(T *& obj)  // FIX: funguje iba pre typy odvodene od Medium
{
	Medium::close(obj);
	obj = nullptr;
}

player_server::~player_server()
{
	for (track_state & track : _tracks)
	{
		if (track.sink)
			track.sink->stopPlaying();

		release(track.sink);
		release(track.source);
		release(track.rtcp);
	}

	release(_rtsp_serv);

	_sms = nullptr;
	release(_file);
}

void player_server::run()
{
	_env->taskScheduler().doEventLoop();  // does not return
}

void player_server::play(string const & file_name)
{
	MatroskaFile::createNew(*_env, file_name.c_str(), on_matroska_file_creation,
		this, "eng");
}

void dump_track_info(MatroskaTrack * track, string const & indent)
{
	cout << indent << "trackNumber: " << track->trackNumber << "\n";

	cout << indent << "trackType: ";
	switch (track->trackType)
	{
		case MATROSKA_TRACK_TYPE_VIDEO:
			cout << "video";
			break;
		case MATROSKA_TRACK_TYPE_AUDIO:
			cout << "audio";
			break;

		case MATROSKA_TRACK_TYPE_SUBTITLE:
			cout << "subtitle";
			break;

		case MATROSKA_TRACK_TYPE_OTHER:
			cout << "other";
			break;

		default:
			cout << "UNKNOWN";
	}
	cout << " (" << (int)track->trackType << ")\n";

	cout << indent << "isEnabled, isDefault, isForced: " << (int)track->isEnabled << ", " << (int)track->isDefault << ", " << (int)track->isForced << "\n"
		<< indent << "defaultDuration: " << track->defaultDuration << "\n";

	cout << indent << "name: ";
	if (track->name)
		cout << track->name << "\n";
	else
		cout << "UNKNOWN\n";

	cout << indent << "language: ";
	if (track->language)
		cout << track->language << "\n";
	else
		cout << "UNKNOWN\n";

	cout << indent << "codecID: ";
	if (track->codecID)
		cout << track->codecID << "\n";
	else
		cout << "UNKNOWN\n";

	cout << indent << "samplingFrequency: " << track->samplingFrequency << "\n"
		<< indent << "numChannels: " << track->numChannels << "\n";

	cout << indent << "mimeType: ";
	if (track->mimeType)
		cout << track->mimeType << "\n";
	else
		cout << "UNKNOWN\n";

	cout << indent << "subframeSizeSize: " << track->subframeSizeSize << "\n";
}

void player_server::on_matroska_file_creation(MatroskaFile * new_file, void * data)
{
	player_server * self = static_cast<player_server *>(data);
	assert(self);

	self->_file = new_file;
	self->_demux = new_file->newDemux();

	unsigned short rtp_port_num = 44444;
	const unsigned char ttl = 255;

	cout << "fileName: " << new_file->fileName() << "\n"
		<< "fileDuration: " << new_file->fileDuration() << "s\n"
		<< "chosenVideoTrackNumber: " << new_file->chosenVideoTrackNumber() << "\n"
		<< "chosenAudioTrackNumber: " << new_file->chosenAudioTrackNumber() << "\n"
		<< "chosenSubtitleTrackNumber: " << new_file->chosenSubtitleTrackNumber() << "\n";

	// video, audio, subtitles
	cout << "tracks:\n";
	string host_name = gethostname();
	string track_types[3] = {"video", "audio", "subtitles"};
	for (unsigned i = 0; i < 3; ++i)
	{
		cout << "  " << i << ":\n";

		track_state & track = self->_tracks[i];
		track.track_number = 0;
		track.sink = nullptr;
		track.source = nullptr;
		track.rtcp = nullptr;

		unsigned & track_number = track.track_number;
		FramedSource * track_src = self->_demux->newDemuxedTrack(track_number);

		if (!track_src)
		{
			cout << "    " << track_types[i] << ": missing\n";
			continue;  // nothing to do, no track
		}

		// dump track info
		MatroskaTrack * mkv_track = self->_file->lookup(track_number);
		assert(mkv_track);
		dump_track_info(mkv_track, "    ");

		unsigned est_bitrate, num_filters_in_front_of_track;
		// note: toto vrati iba nastrel est_bitrate podla formatu v kontajnery
		track.source = new_file->createSourceForStreaming(track_src, track_number,
			est_bitrate, num_filters_in_front_of_track);

		cout << "    " << track_types[i] << ": " << track_number;
		assert(track.source);
		cout << ", track-src:" << track_src;
		cout << ", streaming-src:" << track.source;
		cout << ", est_bitrate:" << est_bitrate;

		Groupsock * rtp_sock = new Groupsock(*self->_env, self->_dst_address, rtp_port_num, ttl);
		Groupsock * rtcp_groupsock = new Groupsock(*self->_env, self->_dst_address, rtp_port_num+1, ttl);
		rtp_port_num += 2;

		track.sink = new_file->createRTPSinkForTrackNumber(track_number, rtp_sock, 96+i);
		if (track.sink)
		{
			if (track.sink->estimatedBitrate() > 0)
				est_bitrate = track.sink->estimatedBitrate();

			cout << ", real-est_bitrate:" << est_bitrate;

			track.rtcp = RTCPInstance::createNew(*self->_env, rtcp_groupsock, est_bitrate,
				(unsigned char const *)host_name.c_str(), track.sink, nullptr, True);  // this starts RTCP running automatically

			self->_sms->addSubsession(PassiveServerMediaSubsession::createNew(*track.sink, track.rtcp));
		}

		cout << "\n";
	}

	if (self->_sms->numSubsessions() == 0)
	{
		cout << "Error: The Matroska file \"" << input_filename << "\" has no streamable tracks.\n";
		exit(1);
	}

	self->_rtsp_serv->addServerMediaSession(self->_sms);

	unique_ptr<char []> url{self->_rtsp_serv->rtspURL(self->_sms)};
	cout << "Play this stream using 'cvlc " << url.get() << "' command.\n";

	self->play_tracks();
}

void player_server::play_tracks()
{
	_loop_counter += 1;

	BOOST_LOG_TRIVIAL(trace) << "#" << _loop_counter << ". Beginning to read from file ...";

	// for each track
	for (unsigned i = 0; i < 3; ++i)
	{
		track_state & track = _tracks[i];
		if (track.sink && track.source)
		{
			self_and_track_t * data = new self_and_track_t{this, &track};
			track.sink->startPlaying(*track.source, after_playing, data);
			cout << "startPlaying for track #" << track.track_number << ".\n";
		}
	}
}

void player_server::after_playing(track_state & track)
{
	BOOST_LOG_TRIVIAL(trace) << "... done reading from file (track_number:"
		<< track.track_number << ")";

	MatroskaTrack * mkv_track = _file->lookup(track.track_number);
	assert(mkv_track);

	if (mkv_track->trackType == MATROSKA_TRACK_TYPE_VIDEO)  // stop all
	{
		// clean-up (will also close the demultiplexor itself)
		for (unsigned i = 0; i < 3; ++i)
			close_source(_tracks[i]);

		// TODO: how to check, demultiplexor is closed ?

		// setup
		_demux = _file->newDemux();
		for (unsigned i = 0; i < 3; ++i)
		{
			track_state & track = _tracks[i];
			if (track.track_number != 0)
			{
				FramedSource * track_src = _demux->newDemuxedTrackByTrackNumber(
					track.track_number);
				unsigned est_bitrate, num_filters_in_fron_of_track;
				track.source = _file->createSourceForStreaming(track_src, track.track_number,
					est_bitrate, num_filters_in_fron_of_track);
			}
		}

		play_tracks();  // play again
	}
	else  // stop only audio or subtitule track and do nothing
		close_source(track);
}

void player_server::close_source(track_state & track)
{
	if (track.sink)
		track.sink->stopPlaying();
	Medium::close(track.source);
	track.source = nullptr;
}

void player_server::after_playing(void * data)
{
	self_and_track_t * self_and_track = static_cast<self_and_track_t *>(data);
	assert(self_and_track);
	player_server * self = self_and_track->first;
	assert(self);
	track_state * track = self_and_track->second;
	assert(track);

	self->after_playing(*track);

	delete self_and_track;
}


int main(int argc, char * argv[])
{
	if (argc > 1)
		input_filename = argv[1];

	player_server p{9554};
	p.play(input_filename);
	p.run();

	return 0;
}

string gethostname()
{
	int const SIZE = 1024;
	char buf[SIZE];
	int res = gethostname(buf, SIZE);
	if (res != 0)
		throw std::runtime_error{"gethostname() failed"};
	buf[SIZE-1] = '\0';
	return string{buf};
}
