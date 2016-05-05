// renderovanie do sdl2 kontextu (libsdl2-dev)
#include <string>
#include <iostream>
#include <cassert>
#include <thread>
#include <SDL2/SDL.h>
#include "vlc_player.hpp"

using std::string;
using std::cout;
using std::clog;

int const WIDTH = 800;
int const HEIGHT = 600;

char const * default_media_path = "ball.mp4";


class sdl_player : public vlc_player
{
public:
	sdl_player(SDL_Window * wnd, unsigned width, unsigned height);
	~sdl_player();

protected:
	void lock(void ** pixels) override;
	void unlock(void * const * pixels) override;
	void render() override;
	unsigned setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines) override;

private:
	SDL_Window * _window;
	SDL_Renderer * _rendr;
	SDL_Texture * _tex;
	SDL_mutex * _lock;
};

unsigned sdl_player::setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines)
{
	unsigned rv = vlc_player::setup(chroma, width, height, pitches, lines);

	if (_tex)
		SDL_DestroyTexture(_tex);
	_tex = SDL_CreateTexture(_rendr, SDL_PIXELFORMAT_BGR565/*RV16*/, SDL_TEXTUREACCESS_STREAMING, *width, *height);
	assert(_tex);

	SDL_SetWindowSize(_window, *width, *height);

	return rv;
}

sdl_player::sdl_player(SDL_Window * wnd, unsigned width, unsigned height)
	: vlc_player{width, height}, _window{wnd}, _tex{nullptr}
{
	_rendr = SDL_CreateRenderer(wnd, -1, 0);
	assert(_rendr);

	_lock = SDL_CreateMutex();
	assert(_lock);
}

sdl_player::~sdl_player()
{
	SDL_DestroyTexture(_tex);
	SDL_DestroyMutex(_lock);
	SDL_DestroyRenderer(_rendr);
}

void sdl_player::lock(void ** pixels)
{
	SDL_LockMutex(_lock);
	int pitch;
	SDL_LockTexture(_tex, nullptr, pixels, &pitch);
}

void sdl_player::unlock(void * const * pixels)
{
	SDL_UnlockTexture(_tex);
	SDL_UnlockMutex(_lock);
}

void sdl_player::render()
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


int main(int argc, char * argv[])
{
	string media_path(argc > 1 ? argv[1] : default_media_path);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		assert(0 && "sdl init failed");

	SDL_Window * window = SDL_CreateWindow("player",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	assert(window);

	sdl_player player{window, 1280, 720};
	player.open(media_path);
	player.play();

	SDL_Event event = {};
	while (SDL_WaitEvent(&event) >= 0)
	{
		if (event.type == SDL_QUIT)
			break;
	}

	SDL_Quit();  // clenup

	return 0;
}
