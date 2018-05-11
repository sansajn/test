#include <math.h> 
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_marker.h"
#include "agg_conv_shorten_path.h"
#include "agg_conv_marker_adaptor.h"
#include "agg_conv_concat.h"
#include "agg_arrowhead.h"
#include "agg_vcgen_markers_term.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };


typedef agg::pixfmt_bgr24 pixfmt;
typedef pixfmt::color_type color_type;
typedef agg::renderer_base<pixfmt> base_renderer;
typedef agg::renderer_primitives<base_renderer> primitives_renderer;

typedef agg::renderer_scanline_aa_solid<base_renderer>  solid_renderer;
typedef agg::renderer_scanline_bin_solid<base_renderer> draft_renderer;

typedef agg::gradient_radial_d gradient_function;
typedef agg::span_interpolator_linear<> interpolator;
typedef agg::pod_auto_array<color_type, 256> color_array_type;
typedef agg::span_gradient<color_type, 
                           interpolator, 
                           gradient_function, 
                           color_array_type> gradient_span_gen;
typedef agg::span_allocator<color_type> gradient_span_alloc;

typedef agg::renderer_scanline_aa<base_renderer, 
                                  gradient_span_alloc,
                                  gradient_span_gen> gradient_renderer;

typedef agg::rasterizer_scanline_aa<> scanline_rasterizer;
typedef agg::rasterizer_outline<primitives_renderer> outline_rasterizer;


class graph
{
public:
	struct node
	{
		double x, y;
		node() {}
		node(double x_, double y_) : x(x_), y(y_) {}
	};

	struct edge
	{
		int node1;
		int node2;
		edge() {}
		edge(int n1, int n2) : node1(n1), node2(n2) {}
	};

	~graph()
	{
		delete [] m_edges;
		delete [] m_nodes;
	}

	graph(int num_nodes, int num_edges) :
		m_num_nodes(num_nodes),
		m_num_edges(num_edges),
		m_nodes(new node[num_nodes]),
		m_edges(new edge[num_edges])
	{
		int i;

		srand(100);

		for(i = 0; i < m_num_nodes; i++)
		{
			m_nodes[i].x = (double(rand()) / RAND_MAX) * 0.75 + 0.2;
			m_nodes[i].y = (double(rand()) / RAND_MAX) * 0.85 + 0.1;
		}

		for(i = 0; i < m_num_edges; i++)
		{
			m_edges[i].node1 = rand() % m_num_nodes;
			m_edges[i].node2 = rand() % m_num_nodes;
			if(m_edges[i].node1 == m_edges[i].node2) i--;
		}
	}

	int get_num_nodes() const { return m_num_nodes; }
	int get_num_edges() const { return m_num_edges; }

	node get_node(int idx, double w, double h) const
	{
		node p(0.0, 0.0);
		if(idx < m_num_nodes)
		{
			p = m_nodes[idx];
			p.x = p.x * w;
			p.y = p.y * h;
		}
		return p;
	}

	edge get_edge(int idx) const
	{
		edge b(0,0);
		if(idx < m_num_edges)
		{
			b = m_edges[idx];
		}
		return b;
	}

private:
	graph(const graph&);
	const graph& operator = (const graph&);

	int m_num_nodes;
	int m_num_edges;
	node* m_nodes;
	edge* m_edges;
};


struct line
{
	double x1, y1, x2, y2;
	int f;

	line(double x1_, double y1_, double x2_, double y2_) :
		x1(x1_), y1(y1_), x2(x2_), y2(y2_), f(0) {}

	void rewind(unsigned) { f = 0; }
	unsigned vertex(double* x, double* y)
	{
		if(f == 0) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
		if(f == 1) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
		return agg::path_cmd_stop;
	}
};


template<class Source>
struct stroke_draft_arrow
{
	typedef agg::conv_marker_adaptor<Source, agg::vcgen_markers_term>           stroke_type;
	typedef agg::conv_marker<typename stroke_type::marker_type, agg::arrowhead> marker_type;
	typedef agg::conv_concat<stroke_type, marker_type>                          concat_type;

	stroke_type    s;
	agg::arrowhead ah;
	marker_type    m;
	concat_type    c;

	stroke_draft_arrow(Source& src, double w) :
		s(src),
		ah(),
		m(s.markers(), ah),
		c(s, m)
	{
		ah.head(0, 10, 5, 0);
		s.shorten(10.0);
	}

	void rewind(unsigned path_id) { c.rewind(path_id); }
	unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};



#define stroke_draft      stroke_draft_arrow


class the_application : public agg::platform_support
{
	graph                       m_graph;
	color_array_type            m_gradient_colors;
	agg::scanline_u8            m_sl;

public:
	the_application(agg::pix_format_e format, bool flip_y) :
		agg::platform_support(format, flip_y),
		m_graph(200, 100),
		m_gradient_colors()
	{
		agg::rgba c1(1, 1, 0, 0.25);
		agg::rgba c2(0, 0, 1);

		for(int i = 0; i < 256; i++)
			m_gradient_colors[i] = c1.gradient(c2, double(i) / 255.0);
	}

	void draw_nodes_draft()
	{
		pixfmt pixf(rbuf_window());
		base_renderer rb(pixf);
		primitives_renderer prim(rb);
		int i;
		for(i = 0; i < m_graph.get_num_nodes(); i++)
		{
			graph::node n = m_graph.get_node(i, width(), height());
			prim.fill_color(m_gradient_colors[147]);
			prim.line_color(m_gradient_colors[255]);
			prim.outlined_ellipse(int(n.x), int(n.y), 10, 10);
			prim.fill_color(m_gradient_colors[50]);
			prim.solid_ellipse(int(n.x), int(n.y), 4, 4);
		}
	}

	void draw_lines_draft()
	{
		pixfmt pixf(rbuf_window());
		base_renderer rb(pixf);
		primitives_renderer prim(rb);
		outline_rasterizer ras(prim);

		int i;
		for(i = 0; i < m_graph.get_num_edges(); i++)
		{
			graph::edge e  = m_graph.get_edge(i);
			graph::node n1 = m_graph.get_node(e.node1, width(), height());
			graph::node n2 = m_graph.get_node(e.node2, width(), height());
			line l(n1.x, n1.y, n2.x, n2.y);
			stroke_draft<line> s(l, 1.0);

			int r = rand() & 0x7F;
			int g = rand() & 0x7F;
			int b = rand() & 0x7F;
			int a = 255;
			prim.line_color(agg::rgba8(r, g, b, a));
			ras.add_path(s);
		}
	}

	void draw_scene(scanline_rasterizer& ras,
						 solid_renderer& solid,
						 draft_renderer& draft)
	{
		ras.gamma(agg::gamma_none());
		srand(100);
		draw_nodes_draft();
		draw_lines_draft();
	}

	virtual void on_draw()
	{
		scanline_rasterizer ras;

		pixfmt pixf(rbuf_window());
		base_renderer rb(pixf);
		solid_renderer solid(rb);
		draft_renderer draft(rb);

		rb.clear(agg::rgba(1, 1, 1));
		draw_scene(ras, solid, draft);

		ras.filling_rule(agg::fill_non_zero);
	}
};



int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Line Join");

    if(app.init(600+100, 500+30, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}
