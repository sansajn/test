// processing errors with GetError
#include <cassert>
#include <SDL.h>
#include <iostream>

using std::cout;

int main(int argc, char * argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "error: unable to initialize SDL library, what:" << SDL_GetError() << '\n';
		return 1;
	}

	SDL_Window * window = SDL_CreateWindow("hello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	if (!window) {
		cout << "error: unable to create SDL window, what: " << SDL_GetError() << '\n';
		return 1;
	}
	
	SDL_SetWindowOpacity(window, 1.0f);  // Set opacity to 100%

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		cout << "error: unable to create SDL renderer, what: " << SDL_GetError() << '\n';
		return 1;
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
	bool running = true;

	while (running) {
		SDL_Event event = {};
		while (SDL_PollEvent(&event)) {  // process all events
			// process event
			switch (event.type) {
				case SDL_QUIT: {
					running = false;
					break;
				}
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	SDL_Quit();  // clenup
	
	return 0;
}
