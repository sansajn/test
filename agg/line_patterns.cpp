#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_clip_polyline.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_pattern_filters_rgba.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_outline_image.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_bezier_ctrl.h"
#include "platform/agg_platform_support.h"


enum flip_y_e { flip_y = true };

typedef agg::pixfmt_bgr24 pixfmt;


class pattern_src_brightness_to_alpha_rgba8
{
public:
	pattern_src_brightness_to_alpha_rgba8(agg::rendering_buffer& rb) :
		m_rb(&rb), m_pf(*m_rb) {}

	unsigned width()  const { return m_pf.width();  }
	unsigned height() const { return m_pf.height(); }
	agg::rgba8 pixel(int x, int y) const
	{
		agg::rgba8 c = m_pf.pixel(x, y);
		return c;
	}

private:
	agg::rendering_buffer* m_rb;
	pixfmt m_pf;
};


class the_application : public agg::platform_support
{
	agg::rgba8 m_ctrl_color;
	agg::bezier_ctrl<agg::rgba8> m_curve1;

public:
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_scanline;
	typedef agg::rasterizer_scanline_aa<> rasterizer_scanline;
	typedef agg::scanline_p8 scanline;


	the_application(agg::pix_format_e format, bool flip_y) :
		agg::platform_support(format, flip_y),
		m_ctrl_color(agg::rgba(0, 0.3, 0.5, 0.3))
	{
		m_curve1.line_color(m_ctrl_color);

		m_curve1.curve(64, 19, 14, 126, 118, 266, 19, 265);

		add_ctrl(m_curve1);

		m_curve1.no_transform();
	}


	template<class Pattern,
				class Rasterizer,
				class Renderer,
				class PatternSource,
				class VertexSource>
	void draw_curve(Pattern& patt,
						 Rasterizer& ras,
						 Renderer& ren,
						 PatternSource& src,
						 VertexSource& vs)
	{
		patt.create(src);
		ren.scale_x(1.0);
		ren.start_x(0.0);
		ras.add_path(vs);
	}


	virtual void on_draw()
	{
		pixfmt pf(rbuf_window());
		renderer_base ren_base(pf);
		ren_base.clear(agg::rgba(1.0, 1.0, 0.95));
		renderer_scanline ren(ren_base);

		rasterizer_scanline ras;
		scanline sl;

		// Pattern source. Must have an interface:
		// width() const
		// height() const
		// pixel(int x, int y) const
		// Any agg::renderer_base<> or derived
		// is good for the use as a source.
		//-----------------------------------
		pattern_src_brightness_to_alpha_rgba8 p1(rbuf_img(0));

		agg::pattern_filter_bilinear_rgba8 fltr;           // Filtering functor

		// agg::line_image_pattern is the main container for the patterns. It creates
		// a copy of the patterns extended according to the needs of the filter.
		// agg::line_image_pattern can operate with arbitrary image width, but if the
		// width of the pattern is power of 2, it's better to use the modified
		// version agg::line_image_pattern_pow2 because it works about 15-25 percent
		// faster than agg::line_image_pattern (because of using simple masking instead
		// of expensive '%' operation).
		typedef agg::line_image_pattern<agg::pattern_filter_bilinear_rgba8> pattern_type;
		typedef agg::renderer_base<pixfmt> base_ren_type;
		typedef agg::renderer_outline_image<base_ren_type, pattern_type> renderer_type;
		typedef agg::rasterizer_outline_aa<renderer_type>                rasterizer_type;

		//-- Create with specifying the source
		//pattern_type patt(fltr, src);

		//-- Create uninitialized and set the source
		pattern_type patt(fltr);
		renderer_type ren_img(ren_base, patt);
		rasterizer_type ras_img(ren_img);

		draw_curve(patt, ras_img, ren_img, p1, m_curve1.curve());

		agg::render_ctrl(ras, sl, ren_base, m_curve1);
	}
};


int agg_main(int argc, char* argv[])
{
	the_application app(agg::pix_format_bgr24, flip_y);
	app.caption("AGG Example. Drawing Lines with Image Patterns");

	if(!app.load_img(0, "data/1"))
	{
		char buf[256];
		sprintf(buf, "There must be files 1%s...9%s\n"
						 "Download and unzip:\n"
						 "http://www.antigrain.com/line_patterns.bmp.zip\n"
						 "or\n"
						 "http://www.antigrain.com/line_patterns.ppm.tar.gz\n",
				  app.img_ext(), app.img_ext());
		app.message(buf);
		return 1;
	}

	if(app.init(500, 450, agg::window_resize))
	{
		return app.run();
	}

	return 1;
}
