// render two polygons sample
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <agg_pixfmt_rgb.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_u.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_path_storage.h>
#include <agg_conv_curve.h>
#include <agg_conv_stroke.h>

int const W = 100;
int const H = W;

using agg::rgba8;
using agg::path_storage;
using renderer = agg::renderer_base<agg::pixfmt_rgb24>;

void render_polygon(renderer & ren, path_storage & path, rgba8 const & color);
void render_stroke_polygon(renderer & ren, path_storage & path, rgba8 const & fill_color,
	rgba8 const & stroke_color, double stroke_width = 1.0);

bool write_ppm(const unsigned char* buf, unsigned width, unsigned height,
	const char* file_name);


int main(int argc, char * argv[])
{
	uint8_t * buffer = new uint8_t[W*H*3];
	agg::rendering_buffer rbuf{buffer, W, H, W*3};
	agg::pixfmt_rgb24 pixf{rbuf};
	renderer ren{pixf};
	ren.clear(rgba8{255, 250, 230});

	// square
	path_storage path;
	path.move_to(10, 10);
	path.line_to(60, 10);
	path.line_to(60, 60);
	path.line_to(10, 60);
	path.close_polygon();
	render_polygon(ren, path, rgba8{160, 180, 80});

	// triangle
	path.remove_all();
	path.move_to(50, 90);
	path.line_to(90, 90);
	path.line_to(70, 20);
	path.close_polygon();
	render_stroke_polygon(ren, path, rgba8{230, 100, 160}, rgba8{0, 0, 0});

	write_ppm(buffer, W, H, "poly.ppm");
	delete [] buffer;

	return 0;
}


void render_polygon(renderer & ren, path_storage & path, rgba8 const & color)
{
	agg::rasterizer_scanline_aa<> ras;

	agg::conv_curve<agg::path_storage> curve{path};
	ras.add_path(curve);

	agg::renderer_scanline_aa_solid<
		agg::renderer_base<agg::pixfmt_rgb24> > ren_sl{ren};
	ren_sl.color(color);

	agg::scanline_u8 sl;
	agg::render_scanlines(ras, sl, ren_sl);
}

void render_stroke_polygon(renderer & ren, path_storage & path, rgba8 const & fill_color,
	rgba8 const & stroke_color, double stroke_width)
{
	agg::conv_curve<agg::path_storage> curve{path};
	agg::conv_stroke<agg::conv_curve<agg::path_storage>> stroke{curve};
	stroke.width(stroke_width);

	agg::renderer_scanline_aa_solid<
		agg::renderer_base<agg::pixfmt_rgb24> > ren_sl{ren};
	ren_sl.color(fill_color);

	agg::scanline_u8 sl;
	agg::rasterizer_scanline_aa<> ras;
	ras.add_path(curve);
	agg::render_scanlines(ras, sl, ren_sl);

	ras.add_path(stroke);
	ren_sl.color(stroke_color);
	agg::render_scanlines(ras, sl, ren_sl);
}

bool write_ppm(const unsigned char* buf,
					unsigned width,
					unsigned height,
					const char* file_name)
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
