#pragma once
#include <string>
#include <vlc/vlc.h>

class vlc_player
{
public:
	vlc_player();
	vlc_player(unsigned w, unsigned h);
	~vlc_player();
	void open(std::string const & fname);
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
