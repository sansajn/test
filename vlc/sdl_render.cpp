// https://wiki.videolan.org/LibVLC_SampleCode_SDL/
// https://wiki.videolan.org/LibVLC/#Examples
// renderovanie do sdl2 kontextu (libsdl2-dev)
#include <string>
#include <iostream>
#include <cassert>
#include <vlc/vlc.h>
#include <SDL2/SDL.h>

using std::string;
using std::cout;

int const WIDTH = 640;
int const HEIGHT = 480;
int const VIDEO_WIDTH = WIDTH;
int const VIDEO_HEIGHT = HEIGHT;


char const * default_media_path = "ball.mp4";


struct context
{
	SDL_Renderer * renderer;
	SDL_Texture * texture;
	SDL_mutex * mutex;
	int n;
};

// vlc prepares to render a video frame
void * lock(void * data, void ** pixels)
{
	context * ctx = (context *)data;
	SDL_LockMutex(ctx->mutex);

	int pitch;
	SDL_LockTexture(ctx->texture, nullptr, pixels, &pitch);
	
	return nullptr;
}

// vlc just rendered a video frame
void unlock(void * data, void * id, void * const * pixels)
{
	context * ctx = (context *)data;
	uint16_t * pixbuf = (uint16_t *)*pixels;
	
	// render ...
	for (int y = 10; y < 40; ++y)
	{
		for (int x = 10; x < 40; ++x)
		{
			if (x < 13 || y < 13 || x > 36 || y > 36)
				pixbuf[y * VIDEO_WIDTH + x] = 0xffff;
			else
				pixbuf[y * VIDEO_WIDTH + x] = 0x02ff;
		}
	}
	
	SDL_UnlockTexture(ctx->texture);
	SDL_UnlockMutex(ctx->mutex);
}

// vlc wants to display a video frame
void display(void * data, void * id)
{
	context * ctx = (context *)data;
	
	SDL_Rect rect;
	rect.w = VIDEO_WIDTH;
	rect.h = VIDEO_HEIGHT;
	rect.x = (int)((1. + .5 * sin(0.03 * ctx->n)) * (WIDTH - VIDEO_WIDTH) / 2);
	rect.y = (int)((1. + .5 * cos(0.03 * ctx->n)) * (HEIGHT - VIDEO_HEIGHT) / 2);

	SDL_SetRenderDrawColor(ctx->renderer, 0, 80, 0, 255);
	SDL_RenderClear(ctx->renderer);
	SDL_RenderCopy(ctx->renderer, ctx->texture, nullptr, &rect);
	SDL_RenderPresent(ctx->renderer);
}

void quit(int c)
{
	SDL_Quit();
	exit(c);
}

int main(int argc, char * argv[])
{
	string media_path{argc > 1 ? argv[1] : default_media_path};

	char const * vlc_argv[] = {
		//"--no-audio", "--no-xlib",
		// apply a video filter
		//"--video-filter", "sepia",
		//"--sepia-intensity=200"
	};
	int vlc_argc = 0;  //sizeof(vlc_argv)/sizeof(vlc_argv[0]);

	libvlc_instance_t * vlc = libvlc_new(vlc_argc, vlc_argv);
	assert(vlc);

	libvlc_media_t * media = libvlc_media_new_path(vlc, media_path.c_str());
	assert(media);

	libvlc_media_player_t * player = libvlc_media_player_new_from_media(media);

	libvlc_media_release(media);

	// zisti rozmery video



	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		assert(0 && "sdl init failed");

	SDL_Window * window = SDL_CreateWindow("player",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

	assert(window);

	context ctx;
	ctx.renderer = SDL_CreateRenderer(window, -1, 0);
	assert(ctx.renderer);

	ctx.texture = SDL_CreateTexture(ctx.renderer, SDL_PIXELFORMAT_BGR565,
		SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
	assert(ctx.texture);

	ctx.mutex = SDL_CreateMutex();

	// If you don't have this variable set you must have plugins directory
	// with the executable or libvlc_new() will not work!
	printf("VLC_PLUGIN_PATH=%s\n", getenv("VLC_PLUGIN_PATH"));


	libvlc_video_set_callbacks(player, lock, unlock, display, &ctx);
	libvlc_video_set_format(player, "RV16", VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH*2);
	libvlc_media_player_play(player);

	int done = 0, pause = 0;
	SDL_Event event;

	while (!done)
	{
		int action = 0;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_KEYDOWN:
					action = event.key.keysym.sym;
					break;
			}

			switch (action)
			{
				case SDLK_ESCAPE:
				case SDLK_q:
					done = 1;
					break;
				case ' ':
					cout << "pause toggle.\n";
					pause = !pause;
					break;
			}

			if (!pause)
				ctx.n++;

			SDL_Delay(1000/10);
		}
	}

	// stop streaming and clean up vlc
	libvlc_media_player_stop(player);

	libvlc_media_player_release(player);
	libvlc_release(vlc);

	SDL_DestroyMutex(ctx.mutex);
	SDL_DestroyRenderer(ctx.renderer);

	quit(0);

	return 0;
}
