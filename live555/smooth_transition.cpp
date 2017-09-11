// pokus o plynuly prechod pri streamovani sekvencie obrazkou
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <memory>
#include <functional>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/log/trivial.hpp>
#include <liveMedia.hh>
#include <Groupsock.hh>
#include <GroupsockHelper.hh>
#include <BasicUsageEnvironment.hh>
#include <JPEGVideoSource.hh>

using std::vector;
using std::string;
using std::chrono::duration_cast;
using std::min;
using std::cout;
using std::unique_ptr;
using std::function;
using std::to_string;
namespace fs = boost::filesystem;

string gethostname();

vector<fs::path> assets{
	"assets/girl_eyes_eyelashes_shirt_87593_1920x1080.jpg",
	"assets/girl_face_freckles_eyes_92358_1920x1080.jpg",
	"assets/girls_road_model_photo_shoot_87274_1920x1080.jpg",
	"assets/hattie_watson_freckles_eyes_girl_red_31138_1920x1080.jpg",
	"assets/O64470-1920x1080.jpg",
	"assets/redhead_girl_glasses_eyes_freckles_60563_1920x1080.jpg",
	"assets/road.jpg"
};

TaskScheduler * sched = nullptr;
UsageEnvironment * env = nullptr;


class jpeg_stream
{
public:
	jpeg_stream(UsageEnvironment & env, unsigned fps = 8);
	~jpeg_stream();
	void create(string const & file_name, std::chrono::seconds length);
	void update_input(string const & file_name, std::chrono::seconds length);
	void increase_max_buffer_size(unsigned bytes);  //!< use before create()
	void stop();
	void play(function<void ()> after_playing_callback);
	FramedSource * source() const;
	RTPSink * sink() const;
	RTCPInstance * rtcp() const;

private:
	static void after_playing(void * user_data);
	FramedSource * make_source(string const & file_name, std::chrono::seconds length);

	UsageEnvironment & _env;
	FramedSource * _source;
	RTPSink * _sink;
	RTCPInstance * _rtcp;
	unsigned _fps;
	struct in_addr _dst_address;
	uint16_t const _rtp_port;
	function<void ()> _after_playing_callback;

	static unsigned __counter;
};


class player
{
public:
	player();
	~player();
	void load(vector<fs::path> const & items);
	void stream();  // blocking

private:
	std::chrono::seconds const LENGTH{5};

	void content_ready();
	void play();
	void after_playing();

	vector<fs::path> _items;
	size_t _idx;  //!< current item index
	jpeg_stream * _stream;

	struct in_addr _dst_address;
	RTSPServer * _rtsp_serv;
	ServerMediaSession * _sms;
	uint16_t const _rtp_port;
	uint16_t const _rtsp_port;
};

class jpeg_frame_parser  //!< parse JPEG memory loaded file for RTP JPEG payload
{
public:
	jpeg_frame_parser();
	virtual ~jpeg_frame_parser();

	uint8_t width();
	uint8_t height();
	uint8_t type();
	uint8_t precision();
	uint8_t q_factor();
	void set_q(uint8_t q);
	uint16_t restart_interval();
	uint8_t const * quantization_tables(uint16_t & length);
	int parse(uint8_t * data, unsigned size);
	uint8_t const * scandata(unsigned & length);

private:
	unsigned scan_jpeg_marker(uint8_t const * data, unsigned size, unsigned * offset);
	int read_sof(uint8_t const * data, unsigned size, unsigned * offset);
	unsigned read_dqt(uint8_t const * data, unsigned size, unsigned offset);
	int read_dri(uint8_t const * data, unsigned size, unsigned * offset);

private:
	uint8_t _width;
	uint8_t _height;
	uint8_t _type;
	uint8_t _precision;
	uint8_t _q_factor;

	uint8_t * _q_tables;
	unsigned short _q_tables_length;

	unsigned short _restart_interval;

	uint8_t * _scandata;
	unsigned _scandata_length;
};

class jpeg_video_source_impl : public JPEGVideoSource
{
public:
	static jpeg_video_source_impl * createNew(UsageEnvironment & env, std::string const & jpeg_file,
		std::chrono::seconds length, std::chrono::microseconds time_per_frame, string const & name);

	u_int8_t width() override;
	u_int8_t height() override;

protected:
	jpeg_video_source_impl(UsageEnvironment & env, std::string const & jpeg_file,
		std::chrono::seconds length, std::chrono::microseconds time_per_frame, string const & name);

	virtual ~jpeg_video_source_impl();

private:
	void doGetNextFrame() override;
	u_int8_t type() override;
	u_int8_t qFactor() override;
	u_int8_t const * quantizationTables(u_int8_t & precision, u_int16_t & length) override;
	void read_jpeg_file(std::string const & file_name);

	struct buffer
	{
		void * start;
		size_t length;
	};

	size_t jpeg_to_rtp(void * to, void * from, size_t len);

	std::chrono::seconds _length;
	std::chrono::microseconds _time_per_frame;
	unsigned long _frame_count;
	struct timeval fLastCaptureTime;
	unsigned char * _jpeg_data;  //!< whole JPEG file
	size_t _jpeg_datlen;
	jpeg_frame_parser _parser;  //!< JPEG file parser
	string _name;  //!< debug
};


int main(int argc, char * argv[])
{
	player p;
	p.load(assets);
	p.stream();
	cout << "done!\n";
	return 0;
}


template <typename T>
void release(T *& obj)
{
	Medium::close(obj);
	obj = nullptr;
}

unsigned jpeg_stream::__counter = 0;

jpeg_stream::jpeg_stream(UsageEnvironment & env, unsigned fps)
	: _env{env}
	, _source{nullptr}
	, _sink{nullptr}
	, _rtcp{nullptr}
	, _fps{fps}
	, _rtp_port{44444}
{
	_dst_address.s_addr = chooseRandomIPv4SSMAddress(_env);
}

jpeg_stream::~jpeg_stream()
{}

void jpeg_stream::create(string const & file_name, std::chrono::seconds length)
{
	assert(!_sink && !_source && !_rtcp);

	Groupsock * rtp_sock = new Groupsock{_env, _dst_address, _rtp_port, 255};
	_sink = JPEGVideoRTPSink::createNew(_env, rtp_sock);  // FIX: memory leak (rtp_sock)

	string host_name = gethostname();
	unsigned estimated_bitrate = 1*1024*1024 * _fps;  // ~1MB/s
	Groupsock * rtcp_sock = new Groupsock(_env, _dst_address, _rtp_port+1, 255);
	_rtcp = RTCPInstance::createNew(_env, rtcp_sock, estimated_bitrate,
		(unsigned char const *)host_name.c_str(), _sink, nullptr, True);

	_source = make_source(file_name, length);
}

void jpeg_stream::update_input(string const & file_name, std::chrono::seconds length)
{
	stop();
	release(_source);
	_source = make_source(file_name, length);
}

FramedSource * jpeg_stream::make_source(string const & file_name, std::chrono::seconds length)
{
	__counter += 1;
	std::chrono::microseconds time_per_frame{1000000/_fps};
	FramedSource * src = jpeg_video_source_impl::createNew(_env, file_name, length,
		time_per_frame, string{"source_"} + to_string(__counter));
	assert(src);
	return src;
}

void jpeg_stream::increase_max_buffer_size(unsigned bytes)
{
	OutPacketBuffer::increaseMaxSizeTo(bytes);
}

void jpeg_stream::stop()
{
	if (_sink)
		_sink->stopPlaying();
}

void jpeg_stream::play(function<void ()> after_playing_callback)
{
	assert(_sink);
	_after_playing_callback = after_playing_callback;
	_sink->startPlaying(*_source, after_playing, this);
}

FramedSource * jpeg_stream::source() const
{
	return _source;
}

RTPSink * jpeg_stream::sink() const
{
	return _sink;
}

RTCPInstance * jpeg_stream::rtcp() const
{
	return _rtcp;
}

void jpeg_stream::after_playing(void * user_data)
{
	jpeg_stream * self = static_cast<jpeg_stream *>(user_data);
	assert(self);
	self->_after_playing_callback();
}


player::player()
	: _stream{nullptr}
	, _rtp_port{44444}
	, _rtsp_port{8554}
{
	sched = BasicTaskScheduler::createNew();
	assert(sched);
	env = BasicUsageEnvironment::createNew(*sched);
	assert(env);
	_dst_address.s_addr = chooseRandomIPv4SSMAddress(*env);
	_rtsp_serv = RTSPServer::createNew(*env, _rtsp_port);
	assert(_rtsp_serv);
	_sms = ServerMediaSession::createNew(*env, "testStream", "some description ...",
		"Session streamed by \"testMKVStreamer\"", True /*SSM*/);
	assert(_sms);

	_stream = new jpeg_stream{*env, 2};
	_stream->increase_max_buffer_size(3*1024*1024);
}

player::~player()
{}

void player::load(vector<fs::path> const & items)
{
	_items = items;
	_idx = _items.size();
}

void player::stream()
{
	if (_items.empty())
		return;  // nothing to play

	_idx = 0;

	_stream->create(_items[_idx].string(), LENGTH);
	_sms->addSubsession(PassiveServerMediaSubsession::createNew(*_stream->sink(),
		_stream->rtcp()));

	_rtsp_serv->addServerMediaSession(_sms);

	unique_ptr<char []> url{_rtsp_serv->rtspURL(_sms)};
	cout << "play stream using 'cvlc --rtsp-frame-buffer-size=3000000 " << url.get()
		<< "' command.\n";

	play();

	env->taskScheduler().doEventLoop();  // does not return
}

void player::play()
{
	cout << "beginning to read from " << _items[_idx] << " ..." << std::endl;
	_stream->play(std::bind(&player::after_playing, this));
}

void player::after_playing()
{
	cout << "done reading from file ...\n" << std::endl;

	_idx = (_idx+1) % _items.size();
	_stream->update_input(_items[_idx].string(), LENGTH);

	play();  // play next one
}


// jpeg_video_source_impl.cpp
int const DEFAULT_JPEG_QUALITY = 80;

static uint8_t * read_file(std::string const & file_name, size_t & size);


jpeg_video_source_impl * jpeg_video_source_impl::createNew(UsageEnvironment & env,
	std::string const & jpeg_file, std::chrono::seconds length, std::chrono::microseconds time_per_frame, string const & name)
{
	try
	{
		return new jpeg_video_source_impl(env, jpeg_file, length, time_per_frame, name);
	}
	catch (std::exception const & e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
		return nullptr;
	}
}

jpeg_video_source_impl::jpeg_video_source_impl(UsageEnvironment & env,
	std::string const & jpeg_file, std::chrono::seconds length, std::chrono::microseconds time_per_frame, string const & name)
		: JPEGVideoSource{env}
		, _length{length}
		, _time_per_frame{time_per_frame}
		, _frame_count{0}
		, _jpeg_data{nullptr}
		, _jpeg_datlen{0}
		, _name{name}
{
	read_jpeg_file(jpeg_file);
	cout << _name << "::jpeg_video_source_impl()" << std::endl;

	// TODO: we should set image q-factor there for parser (call set_q())
}

jpeg_video_source_impl::~jpeg_video_source_impl()
{
	delete [] _jpeg_data;

	cout << _name << "::~jpeg_video_source_impl()" << std::endl;
}

static struct timezone Idunno;

void jpeg_video_source_impl::doGetNextFrame()
{
/*
	static struct timeval starttime;

	if (_time_per_frame * _frame_count >= duration_cast<std::chrono::microseconds>(_length))
	{
		handleClosure(this);
		return;
	}

//	clog << "max-buffer-size (fMaxSize):" << fMaxSize << std::endl;
	fFrameSize = jpeg_to_rtp(fTo, _jpeg_dat, _jpeg_datlen);
	gettimeofday(&fLastCaptureTime, &Idunno);
	if(_frame_count==0)
		starttime = fLastCaptureTime;

//	clog << _frame_count << ":"
//		 << duration_cast<std::chrono::milliseconds>(_frame_count * _time_per_frame).count()
//		 << " ms" << std::endl;

	_frame_count++;
	fPresentationTime = fLastCaptureTime;
	fDurationInMicroseconds = _time_per_frame.count();

	// Switch to another task, and inform the reader that he has data:
	nextTask() = envir().taskScheduler().scheduleDelayedTask(0,	(TaskFunc*)FramedSource::afterGetting, this);
*/

	// vzdy odosle 10 policok a potom skonci !
	fNumTruncatedBytes = 0;
	fFrameSize = jpeg_to_rtp(fTo, _jpeg_data, _jpeg_datlen);
	gettimeofday(&fLastCaptureTime, &Idunno);
	fPresentationTime = fLastCaptureTime;
	fDurationInMicroseconds = _time_per_frame.count();
	nextTask() = envir().taskScheduler().scheduleDelayedTask(0,	(TaskFunc*)FramedSource::afterGetting, this);

	cout << _name << "::doGetNextFrame():" << _frame_count
		<< ", ptr:" << (void *)_jpeg_data
		<< ", ptime:" << fPresentationTime.tv_sec << "." << fPresentationTime.tv_usec
		<< std::endl;

	if (_frame_count++ >= 9)
		handleClosure(this);
}

size_t jpeg_video_source_impl::jpeg_to_rtp(void *pto, void *pfrom, size_t len)
{
	 unsigned char *to=(unsigned char*)pto, *from=(unsigned char*)pfrom;
	 unsigned int datlen;
	 unsigned char const * dat;

	 if(_parser.parse(from, len) == 0)  // successful parsing
	 {
		  dat = _parser.scandata(datlen);
//		  clog << "frame-size:" << datlen << std::endl;

		  memcpy(to, dat, min(datlen, fMaxSize));
		  if (datlen > fMaxSize)
			  BOOST_LOG_TRIVIAL(warning) << "buffer (" << fMaxSize << " bytes) too small, unable to copy whole image (" << datlen << " bytes needed)" << std::endl;
		  to += datlen;
		  return datlen;
	 }
	 else
		 cout << "jpeg parse error" << std::endl;

	 return 0;
}

u_int8_t const * jpeg_video_source_impl::quantizationTables(u_int8_t & precision, u_int16_t & length)
{
	 precision = _parser.precision();
	 return _parser.quantization_tables(length);
}

void jpeg_video_source_impl::read_jpeg_file(std::string const & file_name)
{
	_jpeg_data = read_file(file_name, _jpeg_datlen);
}

u_int8_t jpeg_video_source_impl::type()
{
	 return _parser.type();
}

u_int8_t jpeg_video_source_impl::qFactor()
{
	 return _parser.q_factor();
}

u_int8_t jpeg_video_source_impl::width()
{
	 return _parser.width();
}

u_int8_t jpeg_video_source_impl::height()
{
	 return _parser.height();
}

uint8_t * read_file(std::string const & file_name, size_t & size)
{
	size = fs::file_size(file_name);
	uint8_t * buf = new uint8_t[size];

	FILE * fp = fopen(file_name.c_str(), "rb");
	if(!fp)
		throw std::runtime_error(string{"could not open '"} + file_name + "' file");

	size = fread(buf, 1, size, fp);
	fclose(fp);

	return buf;
}


// jpeg_frame_parser.cpp
#define LOGGY(format, ...)

enum
{
	START_MARKER = 0xFF,
	SOI_MARKER   = 0xD8,
	JFIF_MARKER  = 0xE0,
	CMT_MARKER   = 0xFE,
	DQT_MARKER   = 0xDB,
	SOF0_MARKER = 0xC0,
	SOF1_MARKER = 0xC1,
	SOF2_MARKER = 0xC2,
	SOF3_MARKER = 0xC3,
	SOF5_MARKER = 0xC5,
	SOF6_MARKER = 0xC6,
	SOF7_MARKER = 0xC7,
	SOF8_MARKER = 0xC8,
	SOF9_MARKER = 0xC9,
	SOF10_MARKER = 0xCA,
	SOF11_MARKER = 0xCB,
	SOF13_MARKER = 0xCD,
	SOF14_MARKER = 0xCE,
	SOF15_MARKER = 0xCF,
	DHT_MARKER   = 0xC4,
	SOS_MARKER   = 0xDA,
	EOI_MARKER   = 0xD9,
	DRI_MARKER   = 0xDD
};

typedef struct
{
	unsigned char id;
	unsigned char samp;
	unsigned char qt;
} CompInfo;


jpeg_frame_parser::jpeg_frame_parser()
	: _width(0)
	, _height(0)
	, _type(0)
	, _precision(0)
	, _q_factor(255)
	, _q_tables(NULL)
	, _q_tables_length(0)
	, _restart_interval(0)
	, _scandata(NULL)
	, _scandata_length(0)
{
	_q_tables = new unsigned char[128 * 2];
	memset(_q_tables, 8, 128 * 2);
}

jpeg_frame_parser::~jpeg_frame_parser()
{
	if (_q_tables)
		delete [] _q_tables;
}

unsigned char jpeg_frame_parser::width()
{
	return _width;
}

unsigned char jpeg_frame_parser::height()
{
	return _height;
}

unsigned char jpeg_frame_parser::type()
{
	return _type;
}

unsigned char jpeg_frame_parser::precision()
{
	return _precision;
}

unsigned char jpeg_frame_parser::q_factor()
{
	return _q_factor;
}

void jpeg_frame_parser::set_q(unsigned char q)
{
	_q_factor =q;
}

unsigned short jpeg_frame_parser::restart_interval()
{
	return _restart_interval;
}

unsigned char const * jpeg_frame_parser::quantization_tables(unsigned short & length)
{
	 length = _q_tables_length;
	 return _q_tables;
}

unsigned char const * jpeg_frame_parser::scandata(unsigned int & length)
{
	length = _scandata_length;
	return _scandata;
}

unsigned int jpeg_frame_parser::scan_jpeg_marker(const unsigned char* data,
															  unsigned int size, unsigned int* offset)
{
	while ((data[(*offset)++] != START_MARKER) && ((*offset) < size));

	if ((*offset) >= size) {
		return EOI_MARKER;
	} else {
		unsigned int marker;

		marker = data[*offset];
		(*offset)++;

		return marker;
	}
}

static unsigned int _jpegHeaderSize(const unsigned char* data, unsigned int offset)
{
	return data[offset] << 8 | data[offset + 1];
}

int jpeg_frame_parser::read_sof(const unsigned char* data, unsigned int size,
										 unsigned int* offset)
{
	int i, j;
	CompInfo elem;
	CompInfo info[3] = { {0,}, };
	unsigned int sof_size, off;
	unsigned int width, height, infolen;

	off = *offset;

	/* we need at least 17 bytes for the SOF */
	if (off + 17 > size) goto wrong_size;

	sof_size = _jpegHeaderSize(data, off);
	if (sof_size < 17) goto wrong_length;

	*offset += sof_size;

	/* skip size */
	off += 2;

	/* precision should be 8 */
	if (data[off++] != 8) goto bad_precision;

	/* read dimensions */
	height = data[off] << 8 | data[off + 1];
	width = data[off + 2] << 8 | data[off + 3];
	off += 4;

	if (height == 0 || height > 2040) goto invalid_dimension;
	if (width == 0 || width > 2040) goto invalid_dimension;

	_width = width / 8;
	_height = height / 8;

	/* we only support 3 components */
	if (data[off++] != 3) goto bad_components;

	infolen = 0;
	for (i = 0; i < 3; i++) {
		elem.id = data[off++];
		elem.samp = data[off++];
		elem.qt = data[off++];

		/* insertion sort from the last element to the first */
		for (j = infolen; j > 1; j--) {
			if (info[j - 1].id < elem.id) break;
			info[j] = info[j - 1];
		}
		info[j] = elem;
		infolen++;
	}

	/* see that the components are supported */
	if (info[0].samp == 0x21) {
		_type = 0;
	} else if (info[0].samp == 0x22) {
		_type = 1;
	} else {
		goto invalid_comp;
	}

	if (!(info[1].samp == 0x11)) goto invalid_comp;
	if (!(info[2].samp == 0x11)) goto invalid_comp;
	if (info[1].qt != info[2].qt) goto invalid_comp;

	return 0;

	/* ERRORS */
wrong_size:
	LOGGY("Wrong SOF size\n");
	return -1;

wrong_length:
	LOGGY("Wrong SOF length\n");
	return -1;

bad_precision:
	LOGGY("Bad precision\n");
	return -1;

invalid_dimension:
	LOGGY("Invalid dimension\n");
	return -1;

bad_components:
	LOGGY("Bad component\n");
	return -1;

invalid_comp:
	LOGGY("Invalid component\n");
	return -1;
}

unsigned int jpeg_frame_parser::read_dqt(const unsigned char* data,
													 unsigned int size, unsigned int offset)
{
	unsigned int quant_size, tab_size;
	unsigned char prec;
	unsigned char id;

	if (offset + 2 > size)
		goto too_small;

	quant_size = _jpegHeaderSize(data, offset);
	if (quant_size < 2)
		goto small_quant_size;

	/* clamp to available data */
	if (offset + quant_size > size) {
		quant_size = size - offset;
	}

	offset += 2;
	quant_size -= 2;

	while (quant_size > 0) {
		/* not enough to read the id */
		if (offset + 1 > size)
			break;

		id = data[offset] & 0x0f;
		if (id == 15)
			goto invalid_id;

		prec = (data[offset] & 0xf0) >> 4;
		if (prec) {
			tab_size = 128;
			_q_tables_length = 128 * 2;
		} else {
			tab_size = 64;
			_q_tables_length = 64 * 2;
		}

		/* there is not enough for the table */
		if (quant_size < tab_size + 1)
			goto no_table;

		//LOGGY("Copy quantization table: %u\n", id);
		memcpy(&_q_tables[id * tab_size], &data[offset + 1], tab_size);

		tab_size += 1;
		quant_size -= tab_size;
		offset += tab_size;
	}

done:
	return offset + quant_size;

	/* ERRORS */
too_small:
	LOGGY("DQT is too small\n");
	return size;

small_quant_size:
	LOGGY("Quantization table is too small\n");
	return size;

invalid_id:
	LOGGY("Invalid table ID\n");
	goto done;

no_table:
	LOGGY("table doesn't exist\n");
	goto done;
}

int jpeg_frame_parser::read_dri(const unsigned char* data,
										 unsigned int size, unsigned int* offset)
{
	unsigned int dri_size, off;

	off = *offset;

	/* we need at least 4 bytes for the DRI */
	if (off + 4 > size)
		goto wrong_size;

	dri_size = _jpegHeaderSize(data, off);
	if (dri_size < 4)
		goto wrong_length;

	*offset += dri_size;
	off += 2;

	_restart_interval = (data[off] << 8) | data[off + 1];
	if(_restart_interval == 0) // restart disabled
		return -1;
	else
		return 0;

wrong_size:
	return -1;

wrong_length:
	*offset += dri_size;
	return -1;
}

int jpeg_frame_parser::parse(unsigned char* data, unsigned int size)
{
	_width  = 0;
	_height = 0;
	_type = 0;
	_precision = 0;
	_restart_interval = 0;

	_scandata = NULL;
	_scandata_length = 0;

	unsigned int offset = 0;
	unsigned int dqtFound = 0;
	unsigned int sosFound = 0;
	unsigned int sofFound = 0;
	unsigned int driFound = 0;
	unsigned int jpeg_header_size = 0;

	while ((sosFound == 0) && (offset < size)) {
		switch (scan_jpeg_marker(data, size, &offset)) {
			case JFIF_MARKER:
			case CMT_MARKER:
			case DHT_MARKER:
				offset += _jpegHeaderSize(data, offset);
				break;
			case SOF0_MARKER:
				assert(!sofFound);  // skip if already read
				if (read_sof(data, size, &offset) != 0) {
					goto invalid_format;
				}
				sofFound = 1;
				break;
			case DQT_MARKER:
				offset = read_dqt(data, size, offset);
				dqtFound = 1;
				break;
			case SOS_MARKER:
				sosFound = 1;
				jpeg_header_size = offset + _jpegHeaderSize(data, offset);
				break;
			case EOI_MARKER:
				/* EOI reached before SOS!? */
				LOGGY("EOI reached before SOS!?\n");
				break;
			case SOI_MARKER:
				LOGGY("SOI found\n");
				break;
			case DRI_MARKER:
				LOGGY("DRI found\n");
				if (read_dri(data, size, &offset) == 0) {
					driFound = 1;
				}
				break;
			default:

				break;
		}
	}
	if ((dqtFound == 0) || (sofFound == 0)) {
		LOGGY("error: unsupported jpeg type DQT or SOF marker not found");
		goto unsupported_jpeg;
	}

	if (_width == 0 || _height == 0) {
		goto no_dimension;
	}

	_scandata = data + jpeg_header_size;
	_scandata_length = size - jpeg_header_size;

	if (driFound == 1) {
		_type += 64;
	}

	return 0;

	/* ERRORS */
unsupported_jpeg:
	return -1;

no_dimension:
	return -1;

invalid_format:
	return -1;
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
