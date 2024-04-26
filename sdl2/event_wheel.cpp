// handling mouse wheel event sample
#include <cassert>
#include <SDL.h>
#include <fmt/core.h>
using fmt::print;

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
	
	bool running = true;

	while (running) {
		SDL_Event event = {};
		while (SDL_PollEvent(&event)) {  // process all events
			// process event
			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}

			if (event.type == SDL_MOUSEWHEEL) {
				if (event.wheel.y > 0)  // scroll up
					print("Mouse Wheel Up (y={})\n", event.wheel.y);
				else if (event.wheel.y < 0)  // scroll down
					print("Mouse Wheel Down (y={})\n", event.wheel.y);

				// Check if scrolling was horizontal (SDL 2.0.4 or later)
				if (event.wheel.x > 0)
					print("Mouse Wheel Right (x={})\n", event.wheel.x);
				else if (event.wheel.x < 0)
					print("Mouse Wheel Left (x={})\n", event.wheel.x);
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
