// precita wav bez alut-u
#include <iostream>
#include <thread>
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <AL/al.h>
#include <AL/alc.h>

char const * sound_path = "test_clip.wav";

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
	ALCdevice * dev = alcOpenDevice(nullptr);  // open default device
	assert(dev && "failed to open a device");
	
	ALCcontext * ctx = alcCreateContext(dev, nullptr);
	assert(ctx && "failed to create an audio context");
	alcMakeContextCurrent(ctx);
	assert(alGetError() == AL_NO_ERROR && "context stuff failed");
	
	// read data
	FILE * fin = fopen(sound_path, "r");
	assert(fin && "unable to open a file input");
	
	wav_header header;
	fread((void *)&header, sizeof(wav_header), 1, fin);

	cout << "channels:" << header.channels << ", sample-rate:" << header.sample_rate << ", size:"
		<< header.size << "\n";
	
	int size = header.size - sizeof(wav_header) + 8;
	uint8_t * data = new uint8_t[size];
	size_t read_bytes = fread((void *)data, 1, size, fin);
	assert(read_bytes == size && "not all bytes read");
	
	fclose(fin);
	
	// create buffer
	ALenum format;
	if (header.channels > 1)
		format = (header.bits_per_sample == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
	else
		format = (header.bits_per_sample == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8);
	
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, (ALvoid *)data, size, header.sample_rate);
	assert(alGetError() == AL_NO_ERROR && "unable to fill a buffer");
	
	delete [] data;
	
	// source
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);  // connect buffer with source
	assert(alGetError() == AL_NO_ERROR && "source manipulation failed");
	
	// play it
	alSourcePlay(source);
	
	cout << "playing ..." << std::endl;
	
	int state;
	do {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
	while (state == AL_PLAYING);
	
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(ctx);
	alcCloseDevice(dev);
	
	cout << "done.\n";
	return 0;
}
