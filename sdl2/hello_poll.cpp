// creates window with red background with poll event loop 
#include <cassert>
#include <SDL.h>

int main(int argc, char * argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		assert(false && "unable to initialize a SDL library");
		return 1;
	}

	SDL_Window * window = SDL_CreateWindow("hello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	assert(window && "unable to create a window");

	SDL_SetWindowOpacity(window, 1.0f);  // Set opacity to 100%

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	assert(renderer && "unable to create a renderer");
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	while (true) {
		SDL_Event event;
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	SDL_Quit();  // clenup
	
	return 0;
}
