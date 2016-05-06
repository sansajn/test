#pragma once
#include <SDL2/SDL.h>
#include "vlc_player.hpp"

class sdl2_player : public vlc_player
{
public:
	sdl2_player(SDL_Window * wnd, unsigned width, unsigned height);
	~sdl2_player();

protected:
	void lock(void ** pixels) override;
	void unlock(void * const * pixels) override;
	void render() override;
	unsigned setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines) override;
	void cleanup() override;

private:
	SDL_Window * _window;
	SDL_Renderer * _rendr;
	SDL_Texture * _tex;
	SDL_mutex * _lock;
};
