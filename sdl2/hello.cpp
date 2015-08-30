// vytvori okno s cervenym pozadim
#include <cassert>
#include <SDL.h>

SDL_Window * window = nullptr;
SDL_Renderer * renderer = nullptr;

int main(int argc, char * argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		assert(false && "unable to initialize a sdl library");
		return 1;
	}

	window = SDL_CreateWindow("hello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	assert(window && "unable to create a window");

	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);  // show window

	// sdl program loop
	SDL_Event event = {};
	while (SDL_WaitEvent(&event) >= 0)
	{
		if (event.type == SDL_QUIT)
			break;
	}
	
	SDL_Quit();  // clenup
	
	return 0;
}
