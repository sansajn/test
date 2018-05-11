// test renderer_primitives
#include <cstdint>
#include <stdio.h>
#include <agg_pixfmt_rgb.h>
#include <agg_renderer_primitives.h>
#include <agg_rasterizer_outline.h>

int const W = 300;
int const H = W;
char const * OUTFILE = "primitives.ppm";

using agg::rgba8;
using pixfmt = agg::pixfmt_rgb24;
using base_renderer = agg::renderer_base<pixfmt>;
using primitives_renderer = agg::renderer_primitives<base_renderer>;

bool write_ppm(const unsigned char* buf, unsigned width, unsigned height,
	const char* file_name);


int main(int argc, char * argv[])
{
	uint8_t * buffer = new uint8_t[W*H*3];
	agg::rendering_buffer rbuf{buffer, W, H, W*3};
	pixfmt pixf{rbuf};
	base_renderer ren{pixf};
	primitives_renderer prim{ren};

	ren.clear(rgba8{210, 210, 210});   // fill background

	prim.fill_color(rgba8{30, 80, 185});
	prim.line_color(rgba8{0, 0, 0});
	prim.outlined_ellipse(20, 30, 10, 10);  // outlined circle

	prim.outlined_ellipse(50, 50, 10, 5);  // outlined ellipse

	prim.outlined_rectangle(140, 50, 170, 100);  // outlined rectangle

	prim.solid_ellipse(90, 90, 10, 10);  // solid ellipse

	prim.solid_rectangle(20, 110, 40, 130);  // solid rectangle

	prim.rectangle(20, 150, 40, 170);  // rectangle without fill

	prim.ellipse(70, 130, 10, 10);  // ellipse without fill

	// line, note that coordinates must be shifted (see line_bresenham_interpolator::subpixel_shift)
	constexpr int shift = agg::line_bresenham_interpolator::subpixel_shift;
	prim.line(210 << shift, 180 << shift, 280 << shift, 100 << shift);

	// move_to/line_to
	prim.move_to(200 << shift, 200 << shift);
	prim.line_to(220 << shift, 220 << shift);
	prim.line_to(190 << shift, 240 << shift);
	prim.line_to(170 << shift, 190 << shift, true);

	write_ppm(buffer, W, H, OUTFILE);
	delete [] buffer;

	printf("'%s' created, done.\n", OUTFILE);

	return 0;
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
