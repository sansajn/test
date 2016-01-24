// precita wav bez alut-u
#include <iostream>
#include <thread>
#include <cstdio>
#include <cassert>
#include <AL/al.h>
#include <AL/alc.h>

char const * sound_path = "test_clip.wav";

using std::cout;


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
	
	int size = header.Size - sizeof(wav_header) + 8;
	uint8_t * data = new uint8_t[size];
	size_t read_bytes = fread((void *)data, 1, size, fin);
	assert(read_bytes == size && "not all bytes read");
	
	fclose(fin);
	
	// create buffer
	ALenum format;
	if (header.Channels > 1)
		format = (header.nBitsperSample == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
	else
		format = (header.nBitsperSample == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8);
	
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, (ALvoid *)data, size, header.SampleRate);
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
