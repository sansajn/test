#include "vlc_player.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

using std::string;
using std::clog;

vlc_player::vlc_player() : vlc_player(0, 0)
{}

vlc_player::vlc_player(unsigned w, unsigned h)
	: _media{nullptr}, _player{nullptr}, _width{w}, _height{h}
{
	_vlc = libvlc_new(0, nullptr);
}

vlc_player::~vlc_player()
{
	stop();
	libvlc_release(_vlc);
}

void vlc_player::open(string const & fname)
{
	stop();

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

void vlc_player::stop()
{
	if (_player)
	{
		if (libvlc_media_player_is_playing(_player))
			libvlc_media_player_stop(_player);
		libvlc_media_player_release(_player);
	}

	if (_media)
		libvlc_media_release(_media);
}

unsigned vlc_player::setup(char * chroma, unsigned * width, unsigned * height, unsigned * pitches, unsigned * lines)
{
	if (_width != *width || _height != *height)
		clog << "warning: video geometry doesn't match (" << _width << ", " << _height << ") vs (" << *width << ", " << *height << "), resize required" << std::endl;

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

	int depth = 4;  // RV32:4 bytes, RV15:2 bytes
	strncpy(chroma, "RV32", 4);  // let vlc do the conversion to 0xffrrggbb

	*pitches = *width * depth;

	return 1;  // only one buffer created (shoul I create more buffers ?)
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
