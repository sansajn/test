// stream server vo vlc 
#include <thread>
#include <string>
#include <iostream>
#include <cassert>
#include <vlc/vlc.h>

char const * default_media_path = "benny.ogg";

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	string media_path = argc > 1 ? argv[1] : default_media_path;

	// creates streaming server
	char const * vlc_argv[] = {
		"--sout", "#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{sdp=rtsp://:8554/testStream}"
	};

	libvlc_instance_t * vlc = libvlc_new(2, vlc_argv);  // load the vlc engine
	assert(vlc);

	// create a new item
	libvlc_media_t * media = libvlc_media_new_path(vlc, media_path.c_str());
	assert(vlc);

	// create a media player environment
	libvlc_media_player_t * player = libvlc_media_player_new_from_media(media);
	assert(player);

	libvlc_media_release(media);  // release media (not needed anymore)

	libvlc_media_player_play(player);  // start playing

	cout << "streaming '" << media_path << "' at rtps://localhost:8554/testStream" << std::endl;

	// TODO: how to wait for player ends ?
	std::this_thread::sleep_for(std::chrono::seconds{60});

	libvlc_media_player_stop(player);  // stop playing

	libvlc_media_player_release(player);
	libvlc_release(vlc);

	cout << "done\n";

	return 0;
}

