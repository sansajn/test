// vlc player with playlist and stop feedback (libvlc 2.2.1)
#include <string>
#include <vector>
#include <thread>
#include <cassert>
#include <iostream>
#include <vlc/vlc.h>

using std::string;
using std::vector;
using std::cout;

void event_handler(libvlc_event_t const * event, void *);

int main(int argc, char * argv[])
{
	libvlc_instance_t * vlc = libvlc_new(0, nullptr);
	assert(vlc);

	libvlc_media_list_player_t * list_player = libvlc_media_list_player_new(vlc);
	assert(list_player);

	libvlc_event_manager_t * list_player_events = libvlc_media_list_player_event_manager(list_player);
	assert(list_player_events);
	libvlc_event_attach(list_player_events, libvlc_MediaListPlayerPlayed, event_handler, nullptr);  // will be emitted at the end of playlist
	libvlc_event_attach(list_player_events, libvlc_MediaListPlayerNextItemSet, event_handler, nullptr);
	libvlc_event_attach(list_player_events, libvlc_MediaListPlayerStopped, event_handler, nullptr);

	libvlc_media_player_t * player = libvlc_media_player_new(vlc);
	assert(player);
	libvlc_media_list_player_set_media_player(list_player, player);

	libvlc_event_manager_t * player_events = libvlc_media_player_event_manager(player);
	assert(player_events);
	libvlc_event_attach(player_events, libvlc_MediaPlayerEndReached, event_handler, nullptr);

	// create playlist
	vector<string> files{"a10.mp4", "a12.mp4", "a18.mp4"};

	libvlc_media_list_t * playlist = libvlc_media_list_new(vlc);
	for (string const & file: files)
	{
		libvlc_media_t * media = libvlc_media_new_path(vlc, file.c_str());
		libvlc_media_list_add_media(playlist, media);
		libvlc_media_release(media);
	}

	libvlc_media_list_player_set_media_list(list_player, playlist);

	libvlc_media_list_player_play(list_player);  // start playing

	std::this_thread::sleep_for(std::chrono::seconds{45});

	// cleanup
	libvlc_media_list_release(playlist);
	libvlc_media_list_player_release(list_player);
	libvlc_release(vlc);

	return 0;
}

void event_handler(libvlc_event_t const * event, void *)
{
	switch (event->type)
	{
		case libvlc_MediaListPlayerPlayed:
			cout << "libvlc_MediaListPlayerPlayed\n";
			break;

		case libvlc_MediaListPlayerNextItemSet:
			cout << "libvlc_MediaListPlayerNextItemSet\n";
			break;

		case libvlc_MediaListPlayerStopped:
			cout << "libvlc_MediaListPlayerStopped\n";
			break;

		case libvlc_MediaPlayerEndReached:
			cout << "libvlc_MediaPlayerEndReached\n";
			break;

		default:
			cout << "unhandled event " << event->type << "\n";
	}
}
