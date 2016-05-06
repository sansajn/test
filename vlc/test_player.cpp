// renderovanie do sdl2 kontextu (libsdl2-dev)
#include <string>
#include <iostream>
#include <cassert>
#include <thread>
#include <SDL2/SDL.h>
#include "sdl2_player.hpp"

using std::string;
using std::cout;
using std::clog;

int const WIDTH = 800;
int const HEIGHT = 600;

char const * default_media_path = "ball.mp4";


int main(int argc, char * argv[])
{
	string media_path(argc > 1 ? argv[1] : default_media_path);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		assert(0 && "sdl init failed");

	SDL_Window * window = SDL_CreateWindow("player",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	assert(window);

	sdl2_player player{window, 1280, 720};
	player.open(media_path);
	player.play();

	SDL_Event event = {};
	while (SDL_WaitEvent(&event) >= 0)
	{
		if (event.type == SDL_QUIT)
			break;
	}

	player.stop();

	SDL_Quit();  // clenup

	return 0;
}
