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

class vlc_player
{
public:
	vlc_player();
	~vlc_player();
	void open(string const & fname);
	void play();

protected:
	virtual void lock(void ** pixels) = 0;
	virtual void unlock(void * const * pixels) = 0;
	virtual void render() = 0;

private:
	static void * lock_cb(void * data, void ** pixels);
	static void unlock_cb(void * data, void * id, void * const * pixels);
	static void display_cb(void * data, void * id);

	libvlc_instance_t * _vlc;
	libvlc_media_player_t * _player;
};

vlc_player::vlc_player()
	: _player{nullptr}
{
	_vlc = libvlc_new(0, nullptr);
}

vlc_player::~vlc_player()
{
	if (_player)
		libvlc_media_player_release(_player);
	libvlc_release(_vlc);
}

void vlc_player::open(string const & fname)
{
	libvlc_media_t * media = libvlc_media_new_path(_vlc, fname.c_str());
	assert(media);

	if (_player)
	{
		libvlc_media_player_stop(_player);
		libvlc_media_player_release(_player);
	}

	_player = libvlc_media_player_new_from_media(media);
	assert(_player);

	libvlc_video_set_callbacks(_player, lock_cb, unlock_cb, display_cb, (void *)this);
	libvlc_video_set_format(_player, "RV16", VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH * 2);

	libvlc_media_release(media);
}

void vlc_player::play()
{
	libvlc_media_player_play(_player);
}

void * vlc_player::lock_cb(void * data, void ** pixels)
{
	vlc_player * self = (vlc_player *)data;
	self->lock(pixels);
	return nullptr;
}

void vlc_player::unlock_cb(void * data, void * id, void * const * pixels)
{
	vlc_player * self = (vlc_player *)data;
	self->unlock(pixels);
}

void vlc_player::display_cb(void * data, void * id)
{
	vlc_player * self = (vlc_player *)data;
	self->render();
}


class sdl_player : public vlc_player
{
public:
	sdl_player(SDL_Window * wnd);
	~sdl_player();

protected:
	void lock(void ** pixels) override;
	void unlock(void * const * pixels) override;
	void render() override;

private:
	SDL_Renderer * _rendr;
	SDL_Texture * _tex;
	SDL_mutex * _lock;
};

sdl_player::sdl_player(SDL_Window * wnd)
{
	_rendr = SDL_CreateRenderer(wnd, -1, 0);
	assert(_rendr);

	_tex = SDL_CreateTexture(_rendr, SDL_PIXELFORMAT_BGR565,
		SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
	assert(_tex);

	_lock = SDL_CreateMutex();
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
	rect.w = VIDEO_WIDTH;
	rect.h = VIDEO_HEIGHT;
	rect.x = 0;
	rect.y = 0;

	SDL_SetRenderDrawColor(_rendr, 0, 80, 0, 255);
	SDL_RenderClear(_rendr);
	SDL_RenderCopy(_rendr, _tex, nullptr, &rect);
	SDL_RenderPresent(_rendr);
}


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
	string media_path(argc > 1 ? argv[1] : default_media_path);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		assert(0 && "sdl init failed");

	SDL_Window * window = SDL_CreateWindow("player",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	assert(window);

	sdl_player player{window};
	player.open(media_path);
	player.play();

	int done = 0;
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
			}

			SDL_Delay(1000/10);
		}
	}

	quit(0);
	return 0;
}
