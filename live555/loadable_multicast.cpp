#include <string>
#include <memory>
#include <thread>
#include <stdexcept>
#include <iostream>
#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>
#include <groupsock/GroupsockHelper.hh>

using std::clog;
using std::cout;
using std::string;
using std::unique_ptr;


static string gethostname();


class streaming_server
{
public:
	void bind(unsigned source_port, unsigned first_dest_port);
	void load(string const & file_name, string const & stream_name);
	void start();
	void stop();  // quit()
	string rtsp_url() const;
	void join();

private:
	struct track_state
	{
		unsigned track_number;
		FramedSource * source;
		RTPSink * sink;
		RTCPInstance * rtcp;

		track_state() : track_number{0}, source{nullptr}, sink{nullptr}, rtcp{nullptr}
		{}
	};

	void play();
	static void after_playing_callback(void * data);
	static void server_loop(streaming_server * serv);  // TODO: can't be static
	void free_stream_sources();
	void free_stream_sinks();
	static void matroska_file_creation_callback(MatroskaFile * new_file, void * data);

	std::thread _serv_thread;
	char _quit = 0;
	bool _running = false;
	RTSPServer * _rtsp = nullptr;
	track_state _tracks[3];  // video, audio, subtitle
	char _new_file_watch = 0;
	MatroskaFile * _mkv = nullptr;
	in_addr _dest_addr;
	UsageEnvironment * _env = nullptr;
	unsigned _first_dst_port_number = 0;  //!< each track needs it's own port
	string _stream_name;
	string _file_name;
	ServerMediaSession * _sms = nullptr;
};

string streaming_server::rtsp_url() const
{
	return string{unique_ptr<char>{_rtsp->rtspURL(_sms)}.get()};
}

void streaming_server::matroska_file_creation_callback(MatroskaFile * new_file, void * data)
{
	streaming_server * serv = static_cast<streaming_server *>(data);
	serv->_mkv = new_file;
	serv->_new_file_watch = 1;
}

void streaming_server::free_stream_sources()
{
	// Stop playing all "RTPSink"s, then close the source streams (which will also close the demultiplexor itself)
	for (track_state & track : _tracks)
	{
		if (track.sink)
			track.sink->stopPlaying();
		Medium::close(track.source);
		track.source = nullptr;
	}
}

void streaming_server::free_stream_sinks()
{
	for (track_state & track : _tracks)
	{
		if (track.sink)
		{
			track.sink->stopPlaying();
			Medium::close(track.sink);
			track.sink = nullptr;
		}
	}
}

void streaming_server::bind(unsigned source_port, unsigned first_dest_port)
{
	_first_dst_port_number = first_dest_port;

	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	_env = BasicUsageEnvironment::createNew(*scheduler);

	_dest_addr.s_addr = chooseRandomIPv4SSMAddress(*_env);  // TODO: edit this to change destination address

	_rtsp= RTSPServer::createNew(*_env, source_port, NULL);
	if (!_rtsp)
		throw std::runtime_error{string{"Failed to create RTSP server: "} + _env->getResultMsg()};
}

void streaming_server::load(string const & file_name, string const & stream_name)
{
	// some cleanup
	free_stream_sources();
//	free_stream_sinks();
	_rtsp->deleteServerMediaSession(_stream_name.c_str());

	_stream_name = stream_name;
	_file_name = file_name;

	char const * descriptionString = "Session streamed by \"loadable_multicast\" sample";  // TODO: make it settable

	// TODO: who is releasing SMS
	_sms = ServerMediaSession::createNew(*_env, stream_name.c_str(), stream_name.c_str(), descriptionString, True);

	_new_file_watch = 0;
	MatroskaFile::createNew(*_env, file_name.c_str(), matroska_file_creation_callback, this, "jpn");
	_env->taskScheduler().doEventLoop(&_new_file_watch);

	// create streams
	unsigned short rtpPortNum = _first_dst_port_number;
	const unsigned char ttl = 255;

	string host_name = gethostname();

	MatroskaDemux * matroskaDemux = _mkv->newDemux();
	for (unsigned i = 0; i < 3; ++i)
	{
		track_state & track = _tracks[i];

		unsigned trackNumber;
		FramedSource* baseSource = matroskaDemux->newDemuxedTrack(trackNumber);
		track.track_number = trackNumber;

		// TODO: sinks needs to be reused

		unsigned estBitrate, numFiltersInFrontOfTrack;
		track.source = _mkv->createSourceForStreaming(baseSource, trackNumber, estBitrate, numFiltersInFrontOfTrack);
		track.sink = nullptr;  // by default; may get changed below
		track.rtcp = nullptr;  // ditto

		if (track.source)
		{
			Groupsock * rtpGroupsock = new Groupsock(*_env, _dest_addr, rtpPortNum, ttl);  // TODO: reuse or release
			Groupsock * rtcpGroupsock = new Groupsock(*_env, _dest_addr, rtpPortNum+1, ttl);
			rtpPortNum += 2;

			track.sink = _mkv->createRTPSinkForTrackNumber(trackNumber, rtpGroupsock, 96+i);
			if (track.sink)
			{
				if (track.sink->estimatedBitrate() > 0)
					estBitrate = track.sink->estimatedBitrate(); // hack, TODO: why ?

				track.rtcp = RTCPInstance::createNew(*_env, rtcpGroupsock, estBitrate, (unsigned char const *)host_name.c_str(), track.sink, NULL, True);
				// Note: This starts RTCP running automatically

				// Having set up a track for streaming, add it to our RTSP server's "ServerMediaSession":
				_sms->addSubsession(PassiveServerMediaSubsession::createNew(*track.sink, track.rtcp));
			}
		}
	}

	if (_sms->numSubsessions() == 0)
		throw std::runtime_error{string{"Error: The Matroska file '"} + file_name + "' has no streamable tracks (Perhaps the file does not exist, or is not a 'Matroska' file.)."};

	_rtsp->addServerMediaSession(_sms);

	clog << "Play this stream using the URL '" << rtsp_url() << "'" << std::endl;
}

void streaming_server::start()
{
	// TODO: already running detection

	// must be bind() and load()

	_serv_thread = std::thread{streaming_server::server_loop, this};
}

void streaming_server::stop()
{
	_quit = 1;
}

void streaming_server::join()
{
	if (_serv_thread.joinable())
		_serv_thread.join();
}


int main(int argc, char * argv[])
{
	streaming_server serv;
	serv.bind(8554, 44444);
	serv.load("test5.mkv", "matroskaFileTest");
	serv.start();

	// wait some time and stop streaming
	std::this_thread::sleep_for(std::chrono::seconds{20});
	serv.stop();
	serv.join();  // wait till server is stopped

	serv.load("test2.mkv", "matroskaFileTest");
	serv.start();

	serv.join();

	cout << "done!" << std::endl;
	return 0;
}

void streaming_server::server_loop(streaming_server * serv)
{
	serv->play();
	serv->_quit = 0;
	serv->_env->taskScheduler().doEventLoop(&serv->_quit);

	clog << "stopped ..." << std::endl;
}

void streaming_server::play()
{
	clog << "Beginning to read from '" << _file_name << "' ..." << std::endl;

	for (track_state & track : _tracks)
		if (track.sink && track.source)
			track.sink->startPlaying(*track.source, after_playing_callback, this);
}

void streaming_server::after_playing_callback(void * data)
{
	streaming_server * serv = (streaming_server *)data;

	clog << "...done reading from file" << std::endl;

	serv->free_stream_sources();
	// TODO: who will release sinks ? (sinks are reused in original version), if I free them there, streaming next video will fail
//	serv->free_stream_sinks();

	serv->_quit = 1;
}

string gethostname()
{
	int const SIZE = 256;
	char buf[SIZE];
	int res = gethostname(buf, SIZE);
	if (res != 0)
		throw std::runtime_error{"gethostname() failed"};
	buf[SIZE-1] = '\0';
	return string{buf};
}

