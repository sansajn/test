// render whole map (mapnik 3.0)
#include <string>
#include <utility>
#include <chrono>
#include <iostream>
#include <mapnik/map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/rule.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/image_any.hpp>
#include <mapnik/image_util.hpp>
#include <boost/format.hpp>

using std::string;
using std::move;
using std::cout;
using namespace mapnik;

unsigned const TILE_SIZE = 256;
unsigned const TILE_COUNT_WIDTH = 4;
unsigned const TILE_COUNT_HEIGHT = 4;
unsigned const WIDTH = TILE_COUNT_WIDTH * TILE_SIZE;
unsigned const HEIGHT = TILE_COUNT_HEIGHT * TILE_SIZE;


int main(int argv, char * arv[])
{
//	string const srs_lcc = "+proj=lcc +ellps=GRS80 +lat_0=49 +lon_0=-95 +lat+1=49 +lat_2=77 +datum=NAD83 +units=m +no_defs";
//	string const srs_merc = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0.0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs +over";

	datasource_cache::instance().register_datasources("/usr/lib/mapnik/3.0/input/");

	Map m{WIDTH, HEIGHT};
	m.set_background(parse_color("steelblue"));
//	m.set_srs(srs_merc);  // set map projection

	// styles
	feature_type_style poly_style;
	{
		rule r;
		{
			polygon_symbolizer poly_sym;
			put(poly_sym, keys::fill, parse_color("darkgreen"));
			r.append(move(poly_sym));
		}
		poly_style.add_rule(move(r));
	}
	m.insert_style("polygons", move(poly_style));

	// layers
	{
		parameters p;
		p["type"] = "shape";
		p["file"] = "../data/TM_WORLD_BORDERS-0.3";
		p["encoding"] = "utf8";

		layer lyr{"layer_0"};
		lyr.set_datasource(datasource_cache::instance().create(p));
		lyr.add_style("polygons");
//		lyr.set_srs(srs_lcc);

		m.add_layer(lyr);
	}

	m.zoom_all();

	// render
	using hres_clock = std::chrono::high_resolution_clock;

	image_rgba8 buf{TILE_SIZE, TILE_SIZE};

	for (unsigned x = 0; x < TILE_COUNT_WIDTH; ++x)
	{
		for (unsigned y = 0; y < TILE_COUNT_HEIGHT; ++y)
		{
			hres_clock::time_point s = hres_clock::now();

			agg_renderer<image_rgba8> ren{m, buf, 1.0, x*TILE_SIZE, y*TILE_SIZE};
			ren.apply();
			save_to_file(buf, boost::str(boost::format{"output/tile_%1%_%2%.png"} % x % y), "png");

			cout << "tile (" << x << "," << y << ") rendering takes: "
				<< std::chrono::duration_cast<std::chrono::milliseconds>(hres_clock::now() - s).count() << "ms" << std::endl;
		}
	}

	return 0;
}
 
