// prehravanie zvuku pomocou sdl
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

char const * FILE_PATH = "testClip.wav";

struct AudioData
{
	Uint8 * pos;
	Uint32 length;
};

void audio_callback(void * userdata, Uint8 * stream, int streamLength)
{
	AudioData * audio = (AudioData *)userdata;
	if (audio->length == 0)
		return;

	Uint32 length = (Uint32)streamLength;
	length = (length > audio->length ? audio->length : length);

	SDL_memcpy(stream, audio->pos, length);

	audio->pos += length;
	audio->length -= length;
}


int main(int argc, char * argv[])
{
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec wavSpec;
	Uint8 * wavStart;
	Uint32 wavLength;

	if (SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength) == nullptr)
	{
		std::cerr << "Error: " << FILE_PATH << " could not be loaded as an audio file" << endl;
		return 1;
	}

	AudioData audio;
	audio.pos = wavStart;
	audio.length = wavLength;

	wavSpec.callback = audio_callback;
	wavSpec.userdata = &audio;

	SDL_AudioDeviceID device = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (device == 0)
	{
		std::cerr << "Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_PauseAudioDevice(device, 0);

	while (audio.length > 0)
		SDL_Delay(100);

	SDL_CloseAudioDevice(device);
	SDL_FreeWAV(wavStart);
	SDL_Quit();
	return 0;
}
