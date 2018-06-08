// pattern line sample
#include <string>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <agg_rendering_buffer.h>
#include <agg_pixfmt_rgb.h>
#include <agg_path_storage.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_scanline_p.h>
#include <agg_pattern_filters_rgba.h>
#include <agg_renderer_outline_image.h>
#include <agg_rasterizer_outline_aa.h>
#include <Magick++.h>

// TODO: ako na nazvy typou ?

int const WIDTH = 300;
int const HEIGHT = 300;

using std::string;
using std::cout;
using pixfmt = agg::pixfmt_rgb24;
using renderer_base = agg::renderer_base<pixfmt>;

bool write_ppm(unsigned char const * buf, unsigned width, unsigned height,
	char const * file_name);

bool read_ppm(char const * file_name, Magick::Blob & pixels, agg::rendering_buffer & rb);

template <typename PixelFmt>
class pattern_curve_renderer
{
public:
	using pixfmt_type = PixelFmt;
	using renderer_base_type = agg::renderer_base<PixelFmt>;
	using filter_type = agg::pattern_filter_bilinear_rgba8;
	using pattern_type = agg::line_image_pattern<filter_type>;
	using renderer_type = agg::renderer_outline_image<renderer_base_type, pattern_type>;
	using rasterizer_rype = agg::rasterizer_outline_aa<renderer_type>;

	pattern_curve_renderer(pixfmt_type & pattern_pixels)
		: _patt{filter_type{}}
	{
		_patt.create(pattern_pixels);
	}

	void render(renderer_base & dst, agg::path_storage & p)
	{
		renderer_type ren{dst, _patt};
		ren.scale_x(1.0);
		ren.start_x(0.0);

		rasterizer_rype ras{ren};
		ras.add_path(p);
	}

private:
	pattern_type _patt;
};


int main(int argc, char * argv[])
{
	string const patt_file = argc > 1 ? argv[1] : "data/1.ppm";

	uint8_t * buf = new uint8_t[WIDTH * HEIGHT * 3];

	// initialize buffers
	agg::rendering_buffer rb{buf, WIDTH, HEIGHT, WIDTH * 3};
	pixfmt pf{rb};  // pixel buffer adaptoe

	renderer_base ren_base{pf};
	ren_base.clear(agg::rgba{1.0, 1.0, 0.95});

	// load pattern bitmap
	Magick::Blob pixels;
	agg::rendering_buffer patt_rb;
	read_ppm(patt_file.c_str(), pixels, patt_rb);
	pixfmt patt_src{patt_rb};

	// initialize vector data
	agg::path_storage path;
	path.move_to(0, 0);
	path.line_to(WIDTH, HEIGHT);

	pattern_curve_renderer<pixfmt> r{patt_src};
	r.render(ren_base, path);

	write_ppm(buf, WIDTH, HEIGHT, "out/pattern_line.ppm");

	cout << "out/pattern_line.ppm created\n";

	delete [] buf;
	return 0;
}

bool read_ppm(char const * file_name, Magick::Blob & pixels, agg::rendering_buffer & rb)
{
	using namespace Magick;

	Image im{file_name};
	im.write(&pixels, "RGB");
	assert(pixels.length() == im.columns()*im.rows()*3);
	rb.attach((uint8_t *)pixels.data(), im.columns(), im.rows(), im.columns()*3);
	return true;
}

bool write_ppm(unsigned char const * buf, unsigned width, unsigned height,
	char const * file_name)
{
	FILE* fd = fopen(file_name, "wb");
	if(fd)
	{
		fprintf(fd, "P6 %d %d 255 ", width, height);
		fwrite(buf, 1, width * height * 3, fd);
		fclose(fd);
		return true;
	}

	return false;
}
