// prehraje dva audio subory
#include <thread>
#include <iostream>
#include <cassert>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>

char const * benny_path = "test_clip.wav";
char const * fire_path = "fire01.ogg";

using std::cout;

struct generic_decoder
{
	virtual uint8_t * samples_from_file(char const * fname, ALenum & format, ALsizei & size, 
		ALfloat & freq) = 0;
};

struct wav_decoder : public generic_decoder
{
	uint8_t * samples_from_file(char const * fname, ALenum & format, ALsizei & size, 
		ALfloat & freq) override;
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


uint8_t * wav_decoder::samples_from_file(char const * fname, ALenum & format, 
	ALsizei & size, ALfloat & freq)
{
	FILE * fin = fopen(fname, "r");
	assert(fin && "unable to open a file input");
	
	wav_header header;
	fread((void *)&header, sizeof(wav_header), 1, fin);
	
	size = header.Size - sizeof(wav_header) + 8;
	uint8_t * data = new uint8_t[size];
	size_t read_bytes = fread((void *)data, 1, size, fin);
	assert(read_bytes == size && "not all bytes read");
	
	fclose(fin);
	
	freq = header.SampleRate;
	
	if (header.Channels > 1)
		format = (header.nBitsperSample == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;
	else
		format = (header.nBitsperSample == 16) ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;
		
	assert(header.Channels < 3 && "only mono or stereo supported");
	
	return data;
}


struct ogg_decoder : public generic_decoder
{
	uint8_t * samples_from_file(char const * fname, ALenum & format, ALsizei & size, 
		ALfloat & freq) override;
};


uint8_t * ogg_decoder::samples_from_file(char const * fname, ALenum & format, 
	ALsizei & size, ALfloat & freq)
{
	FILE * fin = fopen(fname, "r");
	assert(fin && "unable to open input file");
	
	OggVorbis_File vf;
	int ret = ov_open_callbacks(fin, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	assert(ret == 0 && "ov_open() failed");
	
	vorbis_info * vi = ov_info(&vf, -1);
	int bits_per_sample = 16;  // konvertovane s floatu
	
	size_t pcm_size = ov_pcm_total(&vf, -1) * (bits_per_sample >> 3);
	uint8_t * data = new uint8_t[pcm_size];
	size_t off = 0;
	int current_section = 0;
	bool eof = false;
	while (!eof)
	{
		long bytes_read = ov_read(&vf, (char *)data + off, 4096, 0 /*little endian*/, bits_per_sample >> 3, 1, &current_section);
		if (bytes_read == 0)
			eof = true;
		assert(bytes_read >= 0 && "corrupt bitstream section");
		off += bytes_read;
		assert(off <= pcm_size && "data buffer overflow");
	}
	
	if (vi->channels > 1)
		format = AL_FORMAT_STEREO16;
	else
		format = AL_FORMAT_MONO16;
		
	assert(vi->channels < 3 && "unsupported number of channels");
	
	size = pcm_size;
	freq = vi->rate;
	
	ov_clear(&vf);
	fclose(fin);
	
	return data;
}


int main(int argc, char * argv[])
{
	ALCdevice * dev = alcOpenDevice(nullptr);  // open default device
	assert(dev && "failed to open a device");
	
	ALCcontext * ctx = alcCreateContext(dev, nullptr);
	assert(ctx && "failed to create an audio context");
	alcMakeContextCurrent(ctx);
	assert(alGetError() == AL_NO_ERROR && "context stuff failed");
	
	// input
	wav_decoder wav;
	ALenum benny_format;
	ALsizei benny_size;
	ALfloat benny_freq;
	uint8_t * benny_data = wav.samples_from_file(benny_path, benny_format, benny_size, benny_freq);
	
	ogg_decoder ogg;
	ALenum fire_format;
	ALsizei fire_size;
	ALfloat fire_freq;
	uint8_t * fire_data = ogg.samples_from_file(fire_path, fire_format, fire_size, fire_freq);
	
	// bufferes
	ALuint buffers[2];
	alGenBuffers(2, buffers);
	alBufferData(buffers[0], benny_format, (ALvoid *)benny_data, benny_size, benny_freq);
	alBufferData(buffers[1], fire_format, (ALvoid *)fire_data, fire_size, fire_freq);
	assert(alGetError() == AL_NO_ERROR && "unable to fill a buffer");

	delete [] fire_data;
	delete [] benny_data;
	
	// source
	ALuint sources[2];
	alGenSources(2, sources);
	alSourcei(sources[0], AL_BUFFER, buffers[0]);  // connect buffer with source	
	alSourcei(sources[1], AL_BUFFER, buffers[1]);
	assert(alGetError() == AL_NO_ERROR && "source manipulation failed");
	
	// play it
	alSourcePlay(sources[0]);
	
	cout << "playing ..." << std::endl;
	
	int state;
	do {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		alSourcePlay(sources[1]);
		alGetSourcei(sources[0], AL_SOURCE_STATE, &state);
	}
	while (state == AL_PLAYING);
	
	alDeleteSources(2, sources);
	alDeleteBuffers(2, buffers);
	alcDestroyContext(ctx);
	alcCloseDevice(dev);
	
	cout << "done.\n";
	return 0;
}
