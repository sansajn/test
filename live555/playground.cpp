// implementuje streamer streamujuci MKV videa (snazi sa znovapouzit live555 kanal).
#include <string>
#include <memory>
#include <queue>
#include <vector>
#include <iostream>
#include <cassert>
#include <liveMedia.hh>
#include <MatroskaFile.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

using std::string;
using std::unique_ptr;
using std::queue;
using std::vector;
using std::cout;

char const * TEST_VIDEO = "test.mkv";

class stream_server
{
public:
	stream_server();
	~stream_server();
	void bind();
	void stream(bool repeat = false);
	void add(string const & fname);
	void join();

private:
	void play_next(string const & item);

	static void on_file_ready(MatroskaFile * file, void * user_data);
	static void on_playing_done(void * user_data);

	queue<string> _items;
	TaskScheduler * _sched;
	UsageEnvironment * _env;
	RTSPServer * _serv;
	ServerMediaSession * _sms;
	MatroskaFile * _file;
	MatroskaDemux * _demux;
	FramedSource * _source;
	RTPSink * _sink;
	Groupsock * _rtp_group, * _rtcp_group;
	RTCPInstance * _rtcp;
	char _file_ready_flag, _playing_done_flag;
	bool _repeat;
};

void introduce_self(string const & item)
{
	cout << "streaming: " << item << std::endl;
}

stream_server::~stream_server()
{
	Medium::close(_sink);
	Medium::close(_source);
	Medium::close(_serv);  // also release sms
	Medium::close(_rtcp);
	Medium::close(_file);

	delete _rtp_group;
	delete _rtcp_group;

	bool env_released = _env->reclaim();
	assert(env_released);

	delete _sched;
}

void stream_server::add(string const & fname)
{
	_items.push(fname);
}

stream_server::stream_server()
	: _serv{nullptr}
	, _sms{nullptr}
	, _file{nullptr}
	, _demux{nullptr}
	, _source{nullptr}
	, _sink{nullptr}
	, _rtp_group{nullptr}
	, _rtcp_group{nullptr}
	, _rtcp{nullptr}
	, _file_ready_flag{0}
	, _playing_done_flag{0}
	, _repeat{false}
{
	_sched = BasicTaskScheduler::createNew();
	_env = BasicUsageEnvironment::createNew(*_sched);
}

void stream_server::bind()
{
	_serv = RTSPServer::createNew(*_env, 7070);
	assert(_serv);

	_sms = ServerMediaSession::createNew(*_env, "stream", "", "", True);
	assert(_sms);

	_serv->addServerMediaSession(_sms);

	struct in_addr addr;
	addr.s_addr = chooseRandomIPv4SSMAddress(*_env);
	_rtp_group = new Groupsock{*_env, addr, 18888, 255};
	_rtcp_group = new Groupsock{*_env, addr, 18889, 255};
}

void stream_server::stream(bool repeat)
{
	_repeat = repeat;

	string item = _items.front();
	_items.pop();

	if (repeat)
		_items.push(item);

	MatroskaFile::createNew(*_env, item.c_str(), on_file_ready, (void *)this, "eng");

	_file_ready_flag = 0;
	_sched->doEventLoop(&_file_ready_flag);  // wait file ready
	assert(_file);

	_demux = _file->newDemux();
	assert(_demux);

	// source
	unsigned track_num = 0;
	_source = _demux->newDemuxedTrack(track_num);

	unsigned est_bitrate, num_filters_in_front_of_track;
	_source = _file->createSourceForStreaming(_source, track_num, est_bitrate, num_filters_in_front_of_track);
	assert(_source);

	// sink
	_sink = _file->createRTPSinkForTrackNumber(track_num, _rtp_group, 96);
	assert(_sink);

	if (_sink->estimatedBitrate() > 0)
		est_bitrate = _sink->estimatedBitrate();

	_rtcp = RTCPInstance::createNew(*_env, _rtcp_group, est_bitrate,
		(unsigned char const *)"", _sink, nullptr, True);  // this starts RTCP running automatically
	assert(_rtcp);

	_sms->addSubsession(PassiveServerMediaSubsession::createNew(*_sink, _rtcp));

	_sink->startPlaying(*_source, on_playing_done, (void *)this);

	unique_ptr<char []> url{_serv->rtspURL(_sms)};
	cout << "use\n\n"
		<< "   cvlc " << url.get() << "\n\n"
		<< "command to see streamed content.\n\n";

	cout << "max-frame-size=" << _source->maxFrameSize() << ", "
		 << "OutPacketBuffer::maxSize=" << OutPacketBuffer::maxSize << "\n\n";

	introduce_self(item);
}

void stream_server::play_next(string const & item)
{
	Medium::close(_source);
	Medium::close(_file);

	// _sink, _rtcp can be reused
	MatroskaFile::createNew(*_env, item.c_str(), on_file_ready, (void *)this, "eng");

	_file_ready_flag = 0;
	_sched->doEventLoop(&_file_ready_flag);  // wait file ready
	assert(_file);

	_demux = _file->newDemux();
	assert(_demux);

	// source
	unsigned track_num = 0;
	_source = _demux->newDemuxedTrack(track_num);

	unsigned est_bitrate, num_filters_in_front_of_track;
	_source = _file->createSourceForStreaming(_source, track_num, est_bitrate, num_filters_in_front_of_track);
	assert(_source);

	_sink->startPlaying(*_source, on_playing_done, (void *)this);

	introduce_self(item);
}

void stream_server::join()
{
	_playing_done_flag = 0;
	_sched->doEventLoop(&_playing_done_flag);
}

void stream_server::on_file_ready(MatroskaFile * file, void * user_data)
{
	stream_server * self = (stream_server *)user_data;
	self->_file = file;
	self->_file_ready_flag = 1;
}

void stream_server::on_playing_done(void * user_data)
{
	stream_server * self = (stream_server *)user_data;
	self->_sink->stopPlaying();

	if (!self->_items.empty())
	{
		string item = self->_items.front();
		self->_items.pop();

		if (self->_repeat)
			self->_items.push(item);

		self->play_next(item);
	}
	else
		self->_playing_done_flag = 1;
}


int main(int argc, char * argv[])
{
	vector<string> item_list;
	for (int i = 1; i < argc; ++i)
		item_list.push_back(argv[i]);

	if (item_list.empty())
		item_list.push_back(TEST_VIDEO);

	stream_server p;
	for (string const & item : item_list)
		p.add(item);

	p.bind();
	p.stream(true);
	p.join();

	cout << "done.\n";
	return 0;
}
