// streamovanie pomocou fronty buffrou (pool metoda)
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <AL/al.h>  // openal
#include <AL/alc.h>

char const * sound_path = "test_clip.wav";

constexpr int NUM_BUFFERS = 4;
constexpr int BUFFER_TIME_MS = 200;

using std::cout;

#pragma pack(push, 1)
struct __attribute__( ( packed, aligned( 1 ) ) ) wav_header
{
	uint8_t riff[4];
	uint32_t size;  // filesize-8 bytes
	uint8_t wave[4];
	uint8_t fmt[4];
	uint32_t size_fmt;
	uint16_t format_tag;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t avg_bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample;
	uint8_t reserved[4];
	uint32_t data_size;
};
#pragma pack(pop)

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
	FILE * fin = fopen(sound_path, "r");
	assert(fin && "unable to open a file input");
	
	wav_header header;
	fread((void *)&header, sizeof(wav_header), 1, fin);
	
	// source
	ALuint source;
	alGenSources(1, &source);
	
	// buffers
	ALenum format;
	if (header.Channels > 1)
		format = (header.nBitsperSample == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
	else
		format = (header.nBitsperSample == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8);
	
	ALuint buffers[NUM_BUFFERS];
	alGenBuffers(NUM_BUFFERS, buffers);
	
	size_t buffer_size = BUFFER_TIME_MS * (header.SampleRate * (header.nBitsperSample >> 3) * header.Channels)/1000.0f;
	uint8_t * data = new uint8_t[buffer_size];
	
	for (int i = 0; i < NUM_BUFFERS; ++i)
	{
		size_t read_bytes = fread((void *)data, 1, buffer_size, fin);
		alBufferData(buffers[i], format, (ALvoid *)data, read_bytes, header.SampleRate);
		assert(alGetError() == AL_NO_ERROR && "unable to fill a buffer");
	}
	
	alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
	
	alSourcePlay(source);
	
	cout << "playing ..." << std::endl;
	
	// loop
	int state;
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(BUFFER_TIME_MS/2));
		
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
			size_t read_bytes = fread((void *)data, 1, buffer_size, fin);
			if (read_bytes == 0)
				break;  // eof
			alBufferData(removed[i], format, (ALvoid *)data, read_bytes, header.SampleRate);
		}
		
		alSourceQueueBuffers(source, refilled, removed);
	}
	while (state == AL_PLAYING);
	
	delete [] data;
	fclose(fin);
	
	alDeleteSources(1, &source);
	alDeleteBuffers(NUM_BUFFERS, buffers);
	alcDestroyContext(ctx);
	alcCloseDevice(dev);
	
	cout << "done.\n";
	return 0;
}
