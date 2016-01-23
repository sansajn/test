// precita audio subor (pomocou alut-u)
#include <iostream>
#include <cassert>
#include <AL/alut.h>

char const * sound_path = "test_clip.wav";

using std::cout;

int main(int argc, char * argv[])
{
	alutInit(&argc, argv);
	
	// load input
	ALenum format;
	ALsizei size;
	ALfloat freq;
	ALvoid * data = alutLoadMemoryFromFile(sound_path, &format, &size, &freq);
	assert(data);
	
	std::cout << "format:" << (int)format << ", size:" << size << ", freq:" << freq << "\n";
	
	// buffer to store data
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, size, freq);
	assert(alGetError() == AL_NO_ERROR && "unable to fill a buffer");
	
	// source
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);  // connect buffer with source
	assert(alGetError() == AL_NO_ERROR && "source manipulation failed");
	
	// play it
	alSourcePlay(source);
	
	cout << "playing ..." << std::endl;
	alutSleep(10);  // wait
	
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	
	alutExit();
	
	cout << "done.\n";
	return 0;
}
