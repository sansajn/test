// sample RTSP (mkv) streamer
#include <string>
#include <memory>
#include <utility>
#include <chrono>
#include <iostream>
#include <cassert>
#include <liveMedia.hh>
#include <MatroskaFile.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

using std::string,
	std::unique_ptr,
	std::pair,
	std::cout,
	std::chrono::milliseconds;

string input_filename = "test.mkv";
string const RTSP_LINK_SUBPATH = "stream";  // rtsp://ADDRESS:PORT\stream
constexpr unsigned short RTP_PORT = 44444;  // RTP base port. One port is used for RTP and one for RTCP for each channel (there can be three channels video, audio and subtitles)
constexpr size_t OUT_PACKET_BUFFER_SIZE = 512 * 1024;  // in bytes

void dump_track_info(MatroskaTrack * track, string const & indent);
string gethostname();

/*! sync (blocking) player implementation
\bug Crash after MKV file is played. */
class player_server {
public:
	player_server(unsigned port = 8554);
	~player_server();
	void play(string const & file_name);
	void go();  //!< blocking

private:
	struct track_state {
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

	// live555 callbacks
	static void on_matroska_file_creation(MatroskaFile * new_file, void * data);
	static void after_playing(void * data);

	track_state _tracks[3];  // video, audio, subtitules
	unique_ptr<TaskScheduler> _sched;
	UsageEnvironment * _env;
	struct in_addr _dst_address;
	RTSPServer * _rtsp_serv;
	ServerMediaSession * _sms;
	MatroskaFile * _file;
	MatroskaDemux * _demux;
	unsigned _loop_counter;
	char _loop_quit;
};

//! Safe release Medium based types.
template <typename T>
void release(T *& obj) {
	Medium::close(obj);
	obj = nullptr;
}


player_server::player_server(unsigned port)
	: _file{nullptr}
	, _demux{nullptr}
	, _loop_counter{0}
	, _loop_quit{0}
{
	_sched = unique_ptr<TaskScheduler>{BasicTaskScheduler::createNew()};
	assert(_sched);
	_env = BasicUsageEnvironment::createNew(*_sched);
	assert(_env);
	_dst_address.s_addr = chooseRandomIPv4SSMAddress(*_env);
	_rtsp_serv = RTSPServer::createNew(*_env, port);
	assert(_rtsp_serv);
	_sms = ServerMediaSession::createNew(*_env, RTSP_LINK_SUBPATH.c_str(), "Sample RTSP streamer based on live555 library.",
		"Session streamed by \"rtsp_streamer\"", True /*SSM*/);
	assert(_sms);
}

player_server::~player_server() {
	for (track_state & track : _tracks) {
		if (track.sink)
			track.sink->stopPlaying();

		release(track.sink);
		release(track.source);
		release(track.rtcp);
	}

	release(_rtsp_serv);  // also release _sms
	_sms = nullptr;
	release(_file);

	bool released = _env->reclaim();
	assert(released);
}

void player_server::go() {
	_env->taskScheduler().doEventLoop(&_loop_quit);
}

void player_server::play(string const & file_name) {
	MatroskaFile::createNew(*_env, file_name.c_str(), on_matroska_file_creation,
		this, "eng");
}

void player_server::on_matroska_file_creation(MatroskaFile * new_file, void * data) {
	player_server * self = static_cast<player_server *>(data);
	assert(self);

	self->_file = new_file;
	self->_demux = new_file->newDemux();

	unsigned short rtp_port_num = RTP_PORT;
	constexpr unsigned char TTL = 255;

	cout << "fileName: " << new_file->fileName() << "\n"
		<< "fileDuration: " << new_file->fileDuration() << "s\n"
		<< "chosenVideoTrackNumber: " << new_file->chosenVideoTrackNumber() << "\n"
		<< "chosenAudioTrackNumber: " << new_file->chosenAudioTrackNumber() << "\n"
		<< "chosenSubtitleTrackNumber: " << new_file->chosenSubtitleTrackNumber() << "\n";

	// video, audio, subtitles
	cout << "tracks:\n";
	string host_name = gethostname();
	string track_types[3] = {"video", "audio", "subtitles"};
	for (unsigned i = 0; i < 3; ++i) {
		cout << "  " << i << ": ";

		track_state & track = self->_tracks[i];
		track.track_number = 0;
		track.sink = nullptr;
		track.source = nullptr;
		track.rtcp = nullptr;

		unsigned & track_number = track.track_number;
		FramedSource * track_src = self->_demux->newDemuxedTrack(track_number);  // it will return track for video then audio and then subtitles

		if (!track_src) {
			cout << "    " << track_types[i] << ": missing\n";
			continue;  // nothing to do, no track
		}

		// dump track info
		MatroskaTrack * mkv_track = self->_file->lookup(track_number);
		assert(mkv_track);
		dump_track_info(mkv_track, "    ");

		unsigned est_bitrate, num_filters_in_front_of_track;
		// note: toto vrati iba nastrel est_bitrate podla formatu v kontajneri
		track.source = new_file->createSourceForStreaming(track_src, track_number,
			est_bitrate, num_filters_in_front_of_track);

		cout << "    " << track_types[i] << ": " << track_number;
		assert(track.source);
		cout << ", track-src:" << track_src;
		cout << ", streaming-src:" << track.source;
		cout << ", est_bitrate:" << est_bitrate << "kbps";

		Groupsock * rtp_sock = new Groupsock(*self->_env, self->_dst_address, rtp_port_num, TTL);
		Groupsock * rtcp_groupsock = new Groupsock(*self->_env, self->_dst_address, rtp_port_num+1, TTL);
		rtp_port_num += 2;

		OutPacketBuffer::increaseMaxSizeTo(OUT_PACKET_BUFFER_SIZE);  // increase sink out buffer to 320k, TODO: the value should be based on info from media file
		track.sink = new_file->createRTPSinkForTrackNumber(track_number, rtp_sock, 96+i);
		if (track.sink) {
			if (track.sink->estimatedBitrate() > 0)
				est_bitrate = track.sink->estimatedBitrate();

			cout << ", real-est_bitrate:" << est_bitrate << "kbps";

			track.rtcp = RTCPInstance::createNew(*self->_env, rtcp_groupsock, est_bitrate,
				(unsigned char const *)host_name.c_str(), track.sink, nullptr, True);  // this starts RTCP running automatically

			self->_sms->addSubsession(PassiveServerMediaSubsession::createNew(*track.sink, track.rtcp));
		}

		cout << "\n";
	}

	cout << "\n";

	if (self->_sms->numSubsessions() == 0) {
		cout << "Error: The Matroska file \"" << input_filename << "\" has no streamable tracks.\n";
		exit(1);
	}

	self->_rtsp_serv->addServerMediaSession(self->_sms);

	unique_ptr<char []> url{self->_rtsp_serv->rtspURL(self->_sms)};
	cout << "Play this stream using 'cvlc " << url.get() << "' command.\n";

	self->play_tracks();
}

void player_server::play_tracks() {
	_loop_counter += 1;

	cout << "#" << _loop_counter << ". Beginning to read from file ..." << std::endl;

	// for each track
	for (unsigned i = 0; i < 3; ++i) {
		track_state & track = _tracks[i];
		if (track.sink && track.source) {
			self_and_track_t * data = new self_and_track_t{this, &track};
			track.sink->startPlaying(*track.source, after_playing, data);
			cout << "startPlaying for track #" << track.track_number << ".\n";
		}
	}
}

void player_server::after_playing(track_state & track) {
	cout << "... done reading from file (track_number:" << track.track_number << ")" << std::endl;

	MatroskaTrack * mkv_track = _file->lookup(track.track_number);
	assert(mkv_track);

	if (mkv_track->trackType == MATROSKA_TRACK_TYPE_VIDEO) {
		// video stream ends, stop all tracks
		for (unsigned i = 0; i < 3; ++i)
			close_source(_tracks[i]);

		_loop_quit = 1;  // quit live555 loop
	}
	else
		close_source(track);  // just close track and wait for video track ends
}

void player_server::close_source(track_state & track) {
	if (track.sink)
		track.sink->stopPlaying();
	release(track.source);
}

void player_server::after_playing(void * data) {
	self_and_track_t * self_and_track = static_cast<self_and_track_t *>(data);
	assert(self_and_track);
	player_server * self = self_and_track->first;
	assert(self);
	track_state * track = self_and_track->second;
	assert(track);

	self->after_playing(*track);

	delete self_and_track;
}


int main(int argc, char * argv[]) {
	if (argc > 1)
		input_filename = argv[1];

	player_server p{9554};
	p.play(input_filename);
	p.go();  // blocking


//	FileServerMediaSubsession;
//	OnDemandServerMediaSubsession;

	return 0;
}

void dump_track_info(MatroskaTrack * track, string const & indent) {
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

string gethostname() {
	int const SIZE = 1024;
	char buf[SIZE];
	int res = gethostname(buf, SIZE);
	if (res != 0)
		throw std::runtime_error{"gethostname() failed"};
	buf[SIZE-1] = '\0';
	return string{buf};
}
