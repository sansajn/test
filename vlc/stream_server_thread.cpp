// vlc stream server vo vlakne
#include <thread>
#include <condition_variable>
#include <string>
#include <iostream>
#include <cassert>
#include <vlc/vlc.h>

char const * default_media_path = "benny.ogg";

using std::string;
using std::cout;

void handle_event(libvlc_event_t const * event, void * user_data);

// sync helpers
std::mutex vlc_mutex;
std::condition_variable end_reached;
void wait_end_reached();
void notify_end_reached();

void stream_task(string const & fname)
{
	// creates streaming server
	char const * vlc_argv[] = {
		"--sout", "#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{sdp=rtsp://:8154/testStream}"
	};

	libvlc_instance_t * vlc = libvlc_new(2, vlc_argv);  // load the vlc engine
	assert(vlc);

	// create a new item
	libvlc_media_t * media = libvlc_media_new_path(vlc, fname.c_str());
	assert(media);

	// create a media player environment
	libvlc_media_player_t * player = libvlc_media_player_new_from_media(media);
	assert(player);

	libvlc_event_manager_t * player_events = libvlc_media_player_event_manager(player);
	assert(player_events);
	libvlc_event_attach(player_events, libvlc_MediaPlayerOpening, handle_event, nullptr);
	libvlc_event_attach(player_events, libvlc_MediaPlayerEndReached, handle_event, nullptr);

	libvlc_media_release(media);  // release media (not needed anymore)

	libvlc_media_player_play(player);  // start playing

	cout << "vlc: " << libvlc_get_version() << "\n";
	cout << "streaming '" << fname << "' at rtsp://localhost:8154/testStream" << std::endl;

	wait_end_reached();

	libvlc_media_player_stop(player);  // stop playing

	libvlc_media_player_release(player);
	libvlc_release(vlc);

	cout << "streaming done" << std::endl;
}

int main(int argc, char * argv[])
{
	string media_path = argc > 1 ? argv[1] : default_media_path;

	std::thread t{stream_task, media_path};
	t.join();

	cout << "done\n";

	return 0;
}

void handle_event(libvlc_event_t const * event, void * user_data)
{
	switch (event->type)
	{
		case libvlc_MediaPlayerOpening:
			cout << "MediaPlayerOpening" << std::endl;
			break;

		case libvlc_MediaPlayerEndReached:
		{
			cout << "MediaPlayerEndReached" << std::endl;
			notify_end_reached();
			break;
		}
	}
}

void wait_end_reached()
{
	std::unique_lock<std::mutex> lock{vlc_mutex};
	end_reached.wait(lock);
}

void notify_end_reached()
{
	std::unique_lock<std::mutex> lock{vlc_mutex};
	end_reached.notify_all();
}
