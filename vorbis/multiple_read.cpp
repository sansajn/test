// dva krad precita ogg
#include <iostream>
#include <cassert>
#include <vorbis/vorbisfile.h>

char const * sound_path = "fire01.ogg";

using std::cout;

int main(int argc, char * argv[])
{
	char const * fname = (argc > 1) ? argv[1] : sound_path;

	FILE * fin = fopen(fname, "r");
	assert(fin && "unable to open input file");

	OggVorbis_File vf;
	int ret = ov_open_callbacks(fin, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	assert(ret == 0 && "ov_open() failed");

	vorbis_info * vi = ov_info(&vf, -1);
	int bits_per_sample = 16;  // konvertovane s floatu

	cout << "channels:" << vi->channels << ", freq:" << vi->rate << "\n";
	cout << "length:" << ov_pcm_total(&vf, -1) << " samples\n";
	cout << "encoded by: " << ov_comment(&vf, -1)->vendor << "\n";

	size_t pcm_size = ov_pcm_total(&vf, -1) * (bits_per_sample >> 3) * vi->channels;
	cout << "pcm_size:" << pcm_size << "\n";
	uint8_t * data1 = new uint8_t[pcm_size];
	size_t off = 0;
	bool eof = false;
	while (!eof)
	{
		int current_section = 0;
		long bytes_read = ov_read(&vf, (char *)data1 + off, 4096, 0 /*little endian*/, bits_per_sample >> 3, 1, &current_section);
		if (bytes_read == 0)
			eof = true;
		assert(bytes_read >= 0 && "corrupt bitstream section");
		off += bytes_read;
		assert(off <= pcm_size && "data buffer overflow");
	}

	cout << "read_bytes:" << off << "\n";


	int res = ov_pcm_seek(&vf, 0);
	assert(res == 0 && "seek failed");
	uint8_t * data2 = new uint8_t[pcm_size];
	off = 0;
	eof = false;
	while (!eof)
	{
		int current_section = 0;
		long bytes_read = ov_read(&vf, (char *)data2 + off, 4096, 0 /*little endian*/, bits_per_sample >> 3, 1, &current_section);
		if (bytes_read == 0)
			eof = true;
		assert(bytes_read >= 0 && "corrupt bitstream section");
		off += bytes_read;
		assert(off <= pcm_size && "data buffer overflow");
	}

	cout << "read_bytes:" << off << "\n";

	bool equal = true;
	for (size_t i = 0; i < pcm_size; ++i)
	{
		uint8_t __a = data1[i], __b = data2[i];

		if (data1[i] != data2[i])
		{
			equal = false;
			break;
		}
	}

	if (equal)
		cout << "data1 equals data2\n";
	else
		cout << "data1 not equals data2\n";

	delete [] data1;
	delete [] data2;
	ov_clear(&vf);
	fclose(fin);

	return 0;
}
