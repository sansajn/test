// streamovanie pomocou fronty buffrou (pool metoda)
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <AL/al.h>  // openal
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>

char const * sound_path = "test_clip.ogg";

constexpr int NUM_BUFFERS = 4;
constexpr int BUFFER_TIME_MS = 200;

using std::cout;

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

	OggVorbis_File vf;
	int ret = ov_open_callbacks(fin, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	assert(ret == 0 && "ov_open() failed");

	vorbis_info * vi = ov_info(&vf, -1);
	int bits_per_sample = 16;  // konvertovane s float-u

	cout << "channels:" << vi->channels << ", freq:" << vi->rate << "\n";
	cout << "length:" << ov_pcm_total(&vf, -1) << " samples\n";
	cout << "encoded by: " << ov_comment(&vf, -1)->vendor << "\n";
	
	// source
	ALuint source;
	alGenSources(1, &source);
	
	// buffers
	ALenum format;
	if (vi->channels > 1)
		format = (bits_per_sample == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
	else
		format = (bits_per_sample == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8);
	
	ALuint buffers[NUM_BUFFERS];
	alGenBuffers(NUM_BUFFERS, buffers);
	
	size_t buffer_size = (BUFFER_TIME_MS * vi->rate * vi->channels) / 1000;  // in samples
	uint8_t * data = new uint8_t[buffer_size * (bits_per_sample >> 3)];
	int current_section = 0;

	for (int i = 0; i < NUM_BUFFERS; ++i)
	{
		size_t size = buffer_size;
		size_t total_read_bytes = 0;
		while (size > 0)
		{
			size_t read_bytes = ov_read(&vf, (char *)data + total_read_bytes, size, 0, bits_per_sample >> 3, 1, &current_section);
			assert(read_bytes > 0);
			total_read_bytes += read_bytes;
			size -= read_bytes;
		}

		alBufferData(buffers[i], format, (ALvoid *)data, total_read_bytes, vi->rate);
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
			size_t size = buffer_size;
			size_t total_read_bytes = 0;
			while (size > 0)
			{
				size_t read_bytes = ov_read(&vf, (char *)data + total_read_bytes, size, 0, bits_per_sample >> 3, 1, &current_section);
				if (read_bytes == 0)
					break;  // eof
				total_read_bytes += read_bytes;
				size -= read_bytes;
			}

			alBufferData(removed[i], format, (ALvoid *)data, total_read_bytes, vi->rate);
		}
		
		alSourceQueueBuffers(source, refilled, removed);
	}
	while (state == AL_PLAYING);
	
	delete [] data;
	ov_clear(&vf);
	fclose(fin);
	
	alDeleteSources(1, &source);
	alDeleteBuffers(NUM_BUFFERS, buffers);
	alcDestroyContext(ctx);
	alcCloseDevice(dev);
	
	cout << "done.\n";
	return 0;
}
