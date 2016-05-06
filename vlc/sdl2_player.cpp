#include "sdl2_player.hpp"
#include <cassert>

unsigned sdl2_player::setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines)
{
	unsigned rv = vlc_player::setup(chroma, width, height, pitches, lines);

	_tex = SDL_CreateTexture(_rendr, SDL_PIXELFORMAT_ARGB8888/*RV32*/, SDL_TEXTUREACCESS_STREAMING, *width, *height);
	assert(_tex);

	SDL_SetWindowSize(_window, *width, *height);

	return rv;
}

void sdl2_player::cleanup()
{
	SDL_DestroyTexture(_tex);
	_tex = nullptr;
}

sdl2_player::sdl2_player(SDL_Window * wnd, unsigned width, unsigned height)
	: vlc_player{width, height}, _window{wnd}, _tex{nullptr}
{
	_rendr = SDL_CreateRenderer(wnd, -1, 0);
	assert(_rendr);

	_lock = SDL_CreateMutex();
	assert(_lock);
}

sdl2_player::~sdl2_player()
{
	SDL_DestroyMutex(_lock);
	SDL_DestroyRenderer(_rendr);
}

void sdl2_player::lock(void ** pixels)
{
	SDL_LockMutex(_lock);
	int pitch;
	SDL_LockTexture(_tex, nullptr, pixels, &pitch);
}

void sdl2_player::unlock(void * const * pixels)
{
	SDL_UnlockTexture(_tex);
	SDL_UnlockMutex(_lock);
}

void sdl2_player::render()
{
	SDL_Rect rect;
	rect.w = width();
	rect.h = height();
	rect.x = 0;
	rect.y = 0;

	SDL_SetRenderDrawColor(_rendr, 0, 80, 0, 255);
	SDL_RenderClear(_rendr);
	SDL_RenderCopy(_rendr, _tex, nullptr, &rect);
	SDL_RenderPresent(_rendr);
}
