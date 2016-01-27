// implementacia voris_data struktury
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdio>
#include <cassert>
#include <vorbis/vorbisfile.h>

//char const * sound_path = "test_clip.ogg";
char const * sound_path = "fire01.ogg";

using std::vector;
using std::cout;

size_t compare(uint8_t * a, uint8_t * b, size_t size);

// bez fungujucej implementacie read_at() nie mozne zdielat wave data medzi zdrojmi zvuku
class vorbis_data
{
public:
	vorbis_data(char const * file_name);
	~vorbis_data();
	size_t read(uint8_t * samples, size_t count);  // stream read
	size_t read_at(size_t offset, uint8_t * samples, size_t count);
	size_t channels() const {return _vi->channels;}
	size_t sample_width() const {return 2;}  // bytes_per_sample
	size_t sample_size() const {return _sample_size;}
	size_t sample_count() const {return ov_pcm_total(const_cast<OggVorbis_File *>(&_vf), -1);}
	void reset();  // umozni znova citat data

private:
	FILE * _fin;
	OggVorbis_File _vf;
	vorbis_info * _vi;
	size_t _sample_size;
};

size_t vorbis_data::read(uint8_t * samples, size_t count)
{
	int current_section = 0;
	size_t total_read_bytes = 0;
	size_t size = count * sample_size();
	while (size > 0)
	{
		long read_bytes = ov_read(&_vf, (char *)samples + total_read_bytes, size, 0 /*little endian*/, 2, 1, &current_section);
		assert(read_bytes >= 0 && "corrupt bitstream section");
		if (read_bytes == 0)
			break;  // eof
		size -= read_bytes;
		total_read_bytes += read_bytes;
	}
	return total_read_bytes / sample_size();
}

size_t vorbis_data::read_at(size_t offset, uint8_t * samples, size_t count)
{
// note: zda sa, ze v ov_pcm_seek() je chyba, seek-uje sa na zle miesto a moze sposobyt segfault

//	cout << "offset:" << offset << ", tell:" << ov_pcm_tell(&_vf) << std::endl;
	cout << "raw-offset:" << ov_raw_tell(&_vf) << std::endl;

//	int res = ov_pcm_seek(&_vf, offset);  // tu to spadne
//	assert(res == 0);

	int current_section = 0;
	size_t total_read_bytes = 0;
	size_t size = count * sample_size();
	while (size > 0)
	{
		long read_bytes = ov_read(&_vf, (char *)samples + total_read_bytes, size, 0 /*little endian*/, 2, 1, &current_section);
		assert(read_bytes >= 0 && "corrupt bitstream section");
		if (read_bytes == 0)
			break;  // eof
		size -= read_bytes;
		total_read_bytes += read_bytes;
	}
	return total_read_bytes / sample_size();
}

void vorbis_data::reset()
{
	int res = ov_pcm_seek(&_vf, 0);
	assert(res == 0 && "seek failed");
}

vorbis_data::vorbis_data(char const * file_name)
{
	_fin = fopen(file_name, "r");
	if (!_fin)
		throw std::runtime_error{"unable to open an input file"};

	int res = ov_open_callbacks(_fin, &_vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	assert(res == 0 && "ov_open_callbacks() failed");

	_vi = ov_info(&_vf, -1);
	assert(_vi && "ov_info() failed");

	_sample_size = 2 * _vi->channels;
}

vorbis_data::~vorbis_data()
{
	ov_clear(&_vf);
	fclose(_fin);
}



constexpr int buffer_length_in_ms = 200;


int main(int argc, char * argv[])
{
	uint8_t * raw = nullptr;
	size_t raw_size = 0;

	{  // raw read
		FILE * fin = fopen(sound_path, "r");
		assert(fin && "unable to open input file");

		OggVorbis_File vf;
		int ret = ov_open_callbacks(fin, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE);
		assert(ret == 0 && "ov_open() failed");

		vorbis_info * vi = ov_info(&vf, -1);
		int bits_per_sample = 16;  // konvertovane s floatu

		size_t pcm_size = ov_pcm_total(&vf, -1) * (bits_per_sample >> 3) * vi->channels;
		raw = new uint8_t[pcm_size];
		size_t off = 0;
		bool eof = false;
		while (!eof)
		{
			int current_section = 0;
			long bytes_read = ov_read(&vf, (char *)raw + off, 4096, 0 /*little endian*/, bits_per_sample >> 3, 1, &current_section);
			if (bytes_read == 0)
				eof = true;
			assert(bytes_read >= 0 && "corrupt bitstream section");
			off += bytes_read;
			assert(off <= pcm_size && "data buffer overflow");
		}

		raw_size = off;

		ov_clear(&vf);
		fclose(fin);
	}


	vorbis_data wave{sound_path};

	uint8_t * abstract = nullptr;
	size_t abstract_size = 0;

	{  // read by abstraction
		size_t pcm_size = wave.sample_count() * wave.sample_size();
		abstract = new uint8_t[pcm_size];
		size_t off = 0;
		while (true)
		{
			size_t read_samples = wave.read(abstract + off*wave.sample_size(), 8192);
//			cout << "read_samples:" << read_samples << "\n";
			if (read_samples == 0)
				break;
			off += read_samples;
			assert(off*wave.sample_size() <= pcm_size && "data buffer overflow");
		}

		abstract_size = off*wave.sample_size();
	}

	wave.reset();
	uint8_t * abstract2 = nullptr;
	size_t abstract2_size = 0;

	{  // second read by abstraction
		size_t pcm_size = wave.sample_count() * wave.sample_size();
		abstract2 = new uint8_t[pcm_size];
		size_t off = 0;
		while (true)
		{
			size_t read_samples = wave.read(abstract2 + off*wave.sample_size(), 8192);
			if (read_samples == 0)
				break;
			off += read_samples;
			assert(off*wave.sample_size() <= pcm_size && "data buffer overflow");
		}

		abstract2_size = off*wave.sample_size();
	}


	cout << "buffer_size:" << abstract_size << "\n";
	size_t diffs = compare(raw, abstract, abstract_size);
	if (diffs == 0)
		cout << "raw equals data1\n";
	else
		cout << "raw not equals data1 ("  << diffs << " diffs)\n";

	assert(raw_size == abstract_size);

	diffs = compare(abstract, abstract2, abstract_size);
	if (diffs == 0)
		cout << "abstract equals abstract2\n";
	else
		cout << "abstract not equals abstract2 ("  << diffs << " diffs)\n";

	assert(abstract_size == abstract2_size);


	return 0;
}

size_t compare(uint8_t * a, uint8_t * b, size_t size)
{
	size_t diffs = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (*(a++) != *(b++))
			diffs += 1;
	}
	return diffs;
}
