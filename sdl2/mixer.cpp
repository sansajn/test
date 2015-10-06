// zvuk pomocou SDL_mixer (umoznuje prehrat niekolko zvukou naraz)
#include <stdexcept>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

int main(int argc, char * argv[])
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		throw logic_error(SDL_GetError());

	int flags = MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	assert(initted & flags == flags && "not all modules were initialized");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		throw logic_error(SDL_GetError());

	Mix_Music * music = Mix_LoadMUS("testClip.wav");
	assert(music && "unable to load a music file");

	Mix_Chunk * effect = Mix_LoadWAV("fire01.ogg");
	assert(effect && "unable to load an effect file");

	Mix_PlayMusic(music, 0);

	SDL_Delay(1000);

	Mix_PlayChannel(-1, effect, 1);

	while (Mix_PlayingMusic() || Mix_Playing(-1))
		SDL_Delay(100);

	Mix_FreeChunk(effect);
	Mix_FreeMusic(music);

	Mix_CloseAudio();
	Mix_Quit();

	SDL_Quit();

	return 0;
}
