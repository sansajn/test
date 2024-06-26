// creates window with red background refreshed every ~10ms 
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
	
	bool running = true;

	SDL_Event event = {};
	do {
		if (SDL_WaitEventTimeout(&event, 10)) {  // 10ms timeout
			// process event
			switch (event.type) {
				case SDL_QUIT: {
					running = false;
					break;
				}
			}
		}
		else {  // not any event, refresh window
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
		}
	} while (running);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	SDL_Quit();  // clenup
	
	return 0;
}
