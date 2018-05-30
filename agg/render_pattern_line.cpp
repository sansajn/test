// pattern line sample
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

int const WIDTH = 300;
int const HEIGHT = 300;

using std::cout;
using pixfmt = agg::pixfmt_rgb24;
using renderer_base = agg::renderer_base<pixfmt>;
using renderer_scanline = agg::renderer_scanline_aa_solid<renderer_base>;
using rasterizer_scanline = agg::rasterizer_scanline_aa<>;
using scanline = agg::scanline_p8;

bool write_ppm(unsigned char const * buf, unsigned width, unsigned height,
	char const * file_name);

bool read_ppm(char const * file_name, Magick::Blob & pixels, agg::rendering_buffer & rb);


int main(int argc, char * argv[])
{
	uint8_t * buf = new uint8_t[WIDTH * HEIGHT * 3];

	// initialize buffers
	agg::rendering_buffer rb{buf, WIDTH, HEIGHT, WIDTH * 3};
	pixfmt pf{rb};  // pixel buffer adaptoe

	renderer_base ren_base{pf};
	ren_base.clear(agg::rgba{1.0, 1.0, 0.95});

	renderer_scanline ren{ren_base};
	rasterizer_scanline ras;
	scanline sl;

	// pattern
	using pattern_filter_t = agg::pattern_filter_bilinear_rgba8;
	using pattern_t = agg::line_image_pattern<pattern_filter_t>;
	using renderer_pattern_t = agg::renderer_outline_image<renderer_base, pattern_t>;
	using rasterizer_pattern_t = agg::rasterizer_outline_aa<renderer_pattern_t>;

	pattern_filter_t fltr;
	pattern_t patt{fltr};
	renderer_pattern_t ren_img{ren_base, patt};
	rasterizer_pattern_t ras_img{ren_img};

	// load pattern bitmap
	Magick::Blob pixels;
	agg::rendering_buffer patt_rb;
	read_ppm("data/1.ppm", pixels, patt_rb);
	pixfmt patt_src{patt_rb};

	// initialize vector data
	agg::path_storage path;
	path.move_to(0, 0);
	path.line_to(WIDTH, HEIGHT);

	// draw
	patt.create(patt_src);
	ren_img.scale_x(1.0);
	ren_img.start_x(0.0);
	ras_img.add_path(path);


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
