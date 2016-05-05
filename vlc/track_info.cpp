// info o stope pre spustenim play
#include <string>
#include <iostream>
#include <vlc/vlc.h>

using std::string;
using std::cout;


int main(int argc, char * argv[])
{
	string fname = "ball.mp4";
	libvlc_instance_t * vlc = libvlc_new(0, nullptr);

	libvlc_media_t * media = libvlc_media_new_path(vlc, fname.c_str());
	libvlc_media_parse(media);
	libvlc_media_track_info_t * tracks;
	int track_count = libvlc_media_get_tracks_info(media, &tracks);

	for (int i = 0; i < track_count; ++i)
	{
		libvlc_media_track_info_t * track = tracks + i;
		cout << "track " << i << ", ";
		if (track->i_type == libvlc_track_audio)
			cout << "ch:" << track->u.audio.i_channels << ", rate:" << track->u.audio.i_rate << std::endl;
		else if (track->i_type == libvlc_track_video)
			cout << "w:" << track->u.video.i_width << ", h:" << track->u.video.i_height << std::endl;
	}

	return 0;
}
