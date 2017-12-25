// hello sample for proj4 4.9
#include <iostream>
#include <cassert>
#include <proj_api.h>

using std::cout;


int main(int argc, char * argv[])
{
	projCtx ctx = pj_ctx_alloc();
	assert(ctx);

	projPJ src_proj = pj_init_plus_ctx(ctx, "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	assert(src_proj);

	projPJ dst_proj = pj_init_plus_ctx(ctx, "+proj=merc");
	assert(dst_proj);

	// transform
	double x = 12.0 * DEG_TO_RAD;
	double y = 55.0 * DEG_TO_RAD;
	int ret = pj_transform(src_proj, dst_proj, 1, 1, &x, &y, nullptr);
	assert(ret == 0);
	cout << "easting: " << x << ", norting: " << y << "\n";

	// and then back to geographical coordinates
	ret = pj_transform(dst_proj, src_proj, 1, 1, &x, &y, nullptr);
	x *= RAD_TO_DEG;
	y *= RAD_TO_DEG;
	cout << "longitude: " << x << ", latitude: " << y << "\n";

	return 0;
}
