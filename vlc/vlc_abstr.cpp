// https://wiki.videolan.org/LibVLC_SampleCode_SDL/
// https://wiki.videolan.org/LibVLC/#Examples
// renderovanie do sdl2 kontextu (libsdl2-dev)
#include <string>
#include <iostream>
#include <cassert>
#include <thread>
#include <vlc/vlc.h>
#include <SDL2/SDL.h>

using std::string;
using std::cout;
using std::clog;

int const WIDTH = 800;
int const HEIGHT = 600;


char const * default_media_path = "ball.mp4";


class vlc_player
{
public:
	vlc_player();
	vlc_player(unsigned w, unsigned h);
	~vlc_player();
	void open(string const & fname);
	void play();
	unsigned width() const {return _width;}
	unsigned height() const {return _height;}

protected:
	virtual void lock(void ** pixels) = 0;
	virtual void unlock(void * const * pixels) = 0;
	virtual void render() = 0;
	virtual unsigned setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines);
	void cleanup();

private:
	// vlc callbacks
	static void * lock_cb(void * data, void ** pixels);
	static void unlock_cb(void * data, void * id, void * const * pixels);
	static void display_cb(void * data, void * id);
	static unsigned format_cb(void ** opaque, char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines);
	static void cleanup_cb(void * opaque);
	static void handle_event(libvlc_event_t const * event, void * user_data);

	libvlc_instance_t * _vlc;
	libvlc_media_t * _media;
	libvlc_media_player_t * _player;
	unsigned _width, _height;
};

vlc_player::vlc_player() : vlc_player(0, 0)
{}

vlc_player::vlc_player(unsigned w, unsigned h)
	: _media{nullptr}, _player{nullptr}, _width{w}, _height{h}
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
	_media = libvlc_media_new_path(_vlc, fname.c_str());
	assert(_media);

	_player = libvlc_media_player_new_from_media(_media);
	assert(_player);

	libvlc_video_set_callbacks(_player, lock_cb, unlock_cb, display_cb, (void *)this);
	libvlc_video_set_format_callbacks(_player, format_cb, cleanup_cb);
}

void vlc_player::play()
{
	libvlc_media_player_play(_player);
}

unsigned vlc_player::setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines)
{
	if (_width != *width || _height != *height)
		clog << "warning: video geometry doesn't match (" << _width << ", " << _height << ") vs (" << *width << ", " << *height << "), resize required" << std::endl;

	strncpy(chroma, "RV16", 4);  // TODO: force to RV16 just for now

	if (_width != 0 && _height != 0)
	{
		*width = _width;
		*height = _height;
	}
	else
	{
		_width = *width;
		_height = *height;
	}

	*pitches = *width * 2;  // RV16 are 2 bytes wide

	return 1;  // vytvorim iba jeden buffer
}

void vlc_player::cleanup()
{}


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

unsigned vlc_player::format_cb(void ** opaque, char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines)
{
	vlc_player * self = (vlc_player *)*opaque;
	return self->setup(chroma, width, height, pitches, lines);
}

void vlc_player::cleanup_cb(void * opaque)
{
	vlc_player * self = (vlc_player *)opaque;
	self->cleanup();
}

void vlc_player::handle_event(libvlc_event_t const * event, void * user_data)
{
	vlc_player * player = (vlc_player *)user_data;

	switch (event->type)
	{
		case libvlc_MediaParsedChanged:
			cout << "parsed-changed, width():" << player->width() << ", height():" << player->height() << std::endl;
			break;

		default:
			break;
	}
}


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
