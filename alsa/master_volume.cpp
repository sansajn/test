// get/set master volume sample
#include <iostream>
#include <alsa/asoundlib.h>

using std::cout;
// volume_control

int main(int argc, char * argv[])
{
	char const * card = "default";
	char const * selem_name = "Master";

	snd_mixer_t * mixer;
	snd_mixer_open(&mixer, 0);
	snd_mixer_attach(mixer, card);
	snd_mixer_selem_register(mixer, NULL, NULL);
	snd_mixer_load(mixer);

	snd_mixer_selem_id_t * sid;
	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);
	snd_mixer_elem_t * elem = snd_mixer_find_selem(mixer, sid);  // master_volume

	long min, max;
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	cout << "min=" << min << ", max=" << max << "\n";

	long volume;
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
	cout << "volume=" << volume << "\n";

	long rel_vol = (((double)volume + 0.5) * 100.0) / (double)max;
	cout << "volume=" << rel_vol << "\n";

	volume = 20;
	snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

	snd_mixer_close(mixer);

	cout << "done.\n";
	return 0;
}
