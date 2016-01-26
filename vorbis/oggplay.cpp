// precita a prehraje ogg subor
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <AL/alc.h>

char const * sound_path = "fire01.ogg";

using std::cout;

int main(int argc, char * argv[])
{
	char const * fname = (argc > 1) ? argv[1] : sound_path;
	
	FILE * fin = fopen(fname, "r");
	assert(fin && "unable to open input file");
	
	OggVorbis_File vf;
	int ret = ov_open_callbacks(fin, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	assert(ret == 0 && "ov_open() failed");
	
	vorbis_info * vi = ov_info(&vf, -1);
	int bits_per_sample = 16;  // konvertovane s floatu
	
	cout << "channels:" << vi->channels << ", freq:" << vi->rate << "\n";
	cout << "length:" << ov_pcm_total(&vf, -1) << " samples\n";
	cout << "encoded by: " << ov_comment(&vf, -1)->vendor << "\n";
	
	size_t pcm_size = ov_pcm_total(&vf, -1) * (bits_per_sample >> 3) * vi->channels;
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
	
	// open al stuff
	
	ALCdevice * dev = alcOpenDevice(nullptr);  // open default device
	assert(dev && "failed to open a device");
	
	ALCcontext * ctx = alcCreateContext(dev, nullptr);
	assert(ctx && "failed to create an audio context");
	alcMakeContextCurrent(ctx);
	assert(alGetError() == AL_NO_ERROR && "context stuff failed");
	
	// create buffer
	ALenum format;
	if (vi->channels > 1)  // predpokladam 16 bitov na vzorku
		format = AL_FORMAT_STEREO16;
	else
		format = AL_FORMAT_MONO16;
	
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, (ALvoid *)data, pcm_size, vi->rate);
	assert(alGetError() == AL_NO_ERROR && "unable to fill a buffer");
	 
	delete [] data;
	ov_clear(&vf);
	fclose(fin);
	
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
