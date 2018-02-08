// non linear transformation sample
// sample from http://www.antigrain.com/articles/ddj1.agdoc.html
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
#include <agg_conv_segmentator.h>
#include <agg_trans_warp_magnifier.h>

enum
{
	frame_width = 200,
	frame_height = 200
};

// Writing the buffer to a .PPM file, assuming it has
// RGB-structure, one byte per color component
//--------------------------------------------------
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


int main(int argc, char * argv[])
{
	// Allocate the frame buffer (in this case "manually")
	// and create the rendering buffer object
	unsigned char* buffer = new unsigned char[frame_width * frame_height * 3];
	agg::rendering_buffer rbuf(buffer,
										frame_width,
										frame_height,
										frame_width * 3);

	// Create Pixel Format and Basic renderers
	//--------------------
	agg::pixfmt_rgb24 pixf(rbuf);
	agg::renderer_base<agg::pixfmt_rgb24> ren_base(pixf);

	// At last we do some very simple things, like clear
	//--------------------
	ren_base.clear(agg::rgba8(255, 250, 230));

	// Create Scanline Container, Scanline Rasterizer,
	// and Scanline Renderer for solid fill.
	//--------------------
	agg::scanline_u8 sl;
	agg::rasterizer_scanline_aa<> ras;
	agg::renderer_scanline_aa_solid<
			agg::renderer_base<agg::pixfmt_rgb24> > ren_sl(ren_base);

	// Create Vertex Source (path) object, in our case it's
	// path_storage and form the path.
	//--------------------
	agg::path_storage path;
	path.remove_all(); // Not obligatory in this case
	path.move_to(10, 10);
	path.line_to(frame_width-10, 10);
	path.line_to(frame_width-10, frame_height-10);
	path.line_to(10, frame_height-10);
	path.line_to(10, frame_height-20);
	path.curve4(frame_width-20, frame_height-20,
					frame_width-20, 20,
					10, 20);
	path.close_polygon();

	// The vectorial pipeline
	//-----------------------
	agg::trans_affine T;
	T *= agg::trans_affine_translation(-frame_width/2, -frame_height/2);
	T *= agg::trans_affine_rotation(agg::deg2rad(35.0));
	T *= agg::trans_affine_scaling(0.3, 0.45);
	T *= agg::trans_affine_translation(frame_width/2, frame_height/2);

	agg::trans_warp_magnifier lens;
	lens.center(120, 100);
	lens.magnification(3.0);
	lens.radius(18);

	agg::conv_curve<agg::path_storage> curve{path};
	agg::conv_segmentator<agg::conv_curve<agg::path_storage>> segm{curve};

	agg::conv_transform<
		agg::conv_segmentator<
			agg::conv_curve<agg::path_storage>>,
		agg::trans_affine> trans_curve{segm, T};

	agg::conv_transform<
		agg::conv_transform<
			agg::conv_segmentator<
				agg::conv_curve<
					agg::path_storage>>,
			agg::trans_affine>,
		agg::trans_warp_magnifier> trans_warp{trans_curve, lens};

	agg::conv_stroke<
		agg::conv_segmentator<
			agg::conv_curve<agg::path_storage>>> stroke{segm};

	agg::conv_transform<
		agg::conv_stroke<
			agg::conv_segmentator<
				agg::conv_curve<agg::path_storage>>>, agg::trans_affine> trans_stroke{stroke, T};

	agg::conv_transform<
		agg::conv_transform<
			agg::conv_stroke<
				agg::conv_segmentator<
					agg::conv_curve<agg::path_storage>>>,
			agg::trans_affine>,
		agg::trans_warp_magnifier> trans_warp_stroke{trans_stroke, lens};

	stroke.width(6.0);

	// Set the fill color and render the polygon
	//-----------------------
	ras.add_path(trans_warp);
	ren_sl.color(agg::rgba8{160, 180, 80});
	agg::render_scanlines(ras, sl, ren_sl);

	// Set the color and render the scanlines
	//-----------------------
	ras.add_path(trans_warp_stroke);
	ren_sl.color(agg::rgba8(120, 60, 0));
	agg::render_scanlines(ras, sl, ren_sl);

	// Write the buffer to result.ppm and liberate memory.
	//-----------------------
	write_ppm(buffer, frame_width, frame_height, "result7.ppm");
	delete [] buffer;
	return 0;
}
