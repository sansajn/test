// implementacia jednoducheho stream klienta
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <cassert>
#include <vlc/vlc.h>

char const * default_stream_path = "rtsp://localhost:8554/testStream";

using std::string;
using std::cout;
using std::mutex;
using std::condition_variable;

void handle_event(libvlc_event_t const * event, void * user_data);

// sync helpers
mutex vlc_mutex;
condition_variable end_reached;

int main(int argc, char * argv[])
{
	string stream_path = argc > 1 ? argv[1] : default_stream_path;

	libvlc_instance_t * vlc = libvlc_new(0, nullptr);  // load the vlc engine
	assert(vlc);

	libvlc_media_t * media = libvlc_media_new_location(vlc, stream_path.c_str());  // create a new item
	assert(media);

	libvlc_media_player_t * player = libvlc_media_player_new_from_media(media);  // create a media player environment
	assert(player);

	libvlc_event_manager_t * player_events = libvlc_media_player_event_manager(player);
	assert(player_events);
	libvlc_event_attach(player_events, libvlc_MediaPlayerEndReached, handle_event, nullptr);

	libvlc_media_release(media);  // release media (not needed anymore)

	libvlc_media_player_play(player);  // start playing

	// wait end reached
	{
		std::unique_lock<std::mutex> lock{vlc_mutex};
		end_reached.wait(lock);
	}

	libvlc_media_player_stop(player);  // stop playing

	libvlc_media_player_release(player);
	libvlc_release(vlc);

	return 0;
}

void handle_event(libvlc_event_t const * event, void * user_data)
{
	switch (event->type)
	{
		case libvlc_MediaPlayerEndReached:
		{
			cout << "MediaPlayerEndReached" << std::endl;
			std::unique_lock<std::mutex> lock{vlc_mutex};
			end_reached.notify_all();
			break;
		}
	}
}
