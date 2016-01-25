// unifikacia citania zo zdroja (stream play)
#include <thread>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <AL/al.h>  // openal
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>

char const * wav_sound_path = "test_clip.wav";
char const * vorbis_sound_path = "test_clip.ogg";

constexpr int NUM_BUFFERS = 4;
constexpr int BUFFER_TIME_MS = 200;

using std::cout;

//! abstarkcia umoznujuca zjednotit citanie roznych formatou zvukovych suborou
class wave_data
{
public:
	virtual size_t read(void * buffer, size_t size) = 0;
	virtual size_t channels() const = 0;
	virtual size_t sample_rate() const = 0;
	virtual size_t bits_per_sample() const = 0;
};


#pragma pack(push, 1)
struct __attribute__( ( packed, aligned( 1 ) ) ) wav_header
{
	unsigned char    RIFF[4];
	unsigned long    Size;  // filesize-8 bytes
	unsigned char    WAVE[4];
	unsigned char    FMT[4];
	unsigned int     SizeFmt;
	unsigned short   FormatTag;
	unsigned short   Channels;
	unsigned int     SampleRate;
	unsigned int     AvgBytesPerSec;
	unsigned short   nBlockAlign;
	unsigned short   nBitsperSample;
	unsigned char    Reserved[4];
	unsigned long    DataSize;
};
#pragma pack(pop)


class wav_data : public wave_data
{
public:
	wav_data(char const * fname);
	~wav_data();
	size_t read(void * buffer, size_t size) override;
	size_t channels() const override;
	size_t sample_rate() const override;
	size_t bits_per_sample() const override;
	
private:
	wav_header _header;
	FILE * _fin = nullptr;
};  // wav_data

class vorbis_data : public wave_data
{
public:
	vorbis_data(char const * fname);
	~vorbis_data();
	size_t read(void * buffer, size_t size) override;
	size_t channels() const override;
	size_t sample_rate() const override;
	size_t bits_per_sample() const override;
	
private:
	OggVorbis_File _vf;
	vorbis_info * _vi = nullptr;
	int _current_section;
	FILE * _fin = nullptr;
};  // vorbis_data

vorbis_data::vorbis_data(char const * fname)
{
	_fin = fopen(fname, "r");
	if (!_fin)
		throw std::runtime_error{"can't open wav file"};
	
	int ret = ov_open_callbacks(_fin, &_vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	assert(ret == 0 && "ov_open() failed");
	
	_vi = ov_info(&_vf, -1);
	assert(_vi && "invalid vorbis info");
	_current_section = 0;
}

vorbis_data::~vorbis_data()
{
	ov_clear(&_vf);
	fclose(_fin);
}

size_t vorbis_data::read(void * buffer, size_t size)
{
	assert(_fin && "invalid file descriptor");
	
	size_t total_read_bytes = 0;
	while (size > 0)
	{
		// ov_read() precita maximalne 1 frame (standardne 4096 bajtov)
		size_t read_bytes = ov_read(&_vf, (char *)buffer + total_read_bytes, size, 0 /*little-endian*/, bits_per_sample() >> 3, 1, &_current_section);
		if (read_bytes == 0)
			break;  // eof
		size -= read_bytes;
		total_read_bytes += read_bytes;
	}
	
	return total_read_bytes;
}

size_t vorbis_data::channels() const
{
	return _vi->channels;
}

size_t vorbis_data::sample_rate() const
{
	return _vi->rate;
}

size_t vorbis_data::bits_per_sample() const
{
	return 16;
}


wav_data::wav_data(char const * fname)
{
	_fin = fopen(fname, "r");
	if (!_fin)
		throw std::runtime_error{"can't open wav file"};
	
	fread(&_header, sizeof(wav_header), 1, _fin);
}

wav_data::~wav_data()
{
	fclose(_fin);
}

size_t wav_data::read(void * buffer, size_t size)
{
	assert(_fin && "invalid file descriptor");
	return fread(buffer, 1, size, _fin);
}

size_t wav_data::channels() const
{
	return _header.Channels;
}

size_t wav_data::sample_rate() const
{
	return _header.SampleRate;
}

size_t wav_data::bits_per_sample() const
{
	return _header.nBitsperSample;
}


int main(int argc, char * argv[])
{
	// device & context
	ALCdevice * dev = alcOpenDevice(nullptr);  // open default device
	assert(dev && "failed to open a device");
	
	ALCcontext * ctx = alcCreateContext(dev, nullptr);
	assert(ctx && "failed to create an audio context");
	alcMakeContextCurrent(ctx);
	assert(alGetError() == AL_NO_ERROR && "context stuff failed");
	
	// input
	char const * fname = (argc > 1) ? argv[1] : wav_sound_path;
	wave_data * wave;
	if (strstr(fname, ".wav") != NULL)
		wave = new wav_data{fname};
	else
		wave = new vorbis_data{fname};
	
	cout << fname << " open\n";
	
	// source
	ALuint source;
	alGenSources(1, &source);
	
	// buffers
	ALenum format;
	if (wave->channels() > 1)
		format = (wave->bits_per_sample() == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
	else
		format = (wave->bits_per_sample() == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8);
	
	ALuint buffers[NUM_BUFFERS];
	alGenBuffers(NUM_BUFFERS, buffers);
	
	size_t buffer_size = BUFFER_TIME_MS * (wave->sample_rate() * (wave->bits_per_sample() >> 3) * wave->channels())/1000.0f;
	uint8_t * data = new uint8_t[buffer_size];
	
	for (int i = 0; i < NUM_BUFFERS; ++i)
	{
		size_t read_bytes = wave->read((void *)data, buffer_size);
		cout << "buffer-size:" << read_bytes << "\n";
		alBufferData(buffers[i], format, (ALvoid *)data, read_bytes, wave->sample_rate());
		assert(alGetError() == AL_NO_ERROR && "unable to fill a buffer");
	}
	
	alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
	
	alSourcePlay(source);
	
	cout << "playing ..." << std::endl;
	
	// loop
	int state;
	do
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(BUFFER_TIME_MS/2));
		
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		
		int processed;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
		if (processed == 0)  // zatial su vsetky buffre aktualne
			continue;
		
		ALuint removed[NUM_BUFFERS];
		alSourceUnqueueBuffers(source, processed, removed);
		
		int refilled = 0;
		for (int i = 0; i < processed; ++i, refilled += 1)
		{
			size_t read_bytes = wave->read((void *)data, buffer_size);
			cout << "buffer-size:" << read_bytes << "\n";
			if (read_bytes == 0)
				break;  // eof
			alBufferData(removed[i], format, (ALvoid *)data, read_bytes, wave->sample_rate());
		}
		
		alSourceQueueBuffers(source, refilled, removed);
	}
	while (state == AL_PLAYING);
	
	delete [] data;
	
	alDeleteSources(1, &source);
	alDeleteBuffers(NUM_BUFFERS, buffers);
	alcDestroyContext(ctx);
	alcCloseDevice(dev);
	
	cout << "done.\n";
	return 0;
}
