// working for proj4 5.0+
#include <iostream>
#include <cassert>
#include <proj.h>

using std::cout;

int main(int argc, char * argv[])
{
	PJ_CONTEXT * C = proj_context_create();
	PJ * P = proj_create(C, "+proj=utm +zone=32 +ellps=GSR80");
	assert(P && "can not ctreate projectio");
	
	// copenhagen
	PJ_COORD a = proj_coord(proj_torad(12.0), proj_torad(55.0), 0, 0);
	
	// transform to UTM zone 32
	PJ_COORD b = proj_trans_coord(P, PJ_FWD, a);
	cout << "easting: " << b.en.e << ", norting: " << b.en.n << "\n";
	
	// and then back to geographical coordinates
	b = proj_trans_coord(P, PJ_INV, b);
	cout << "longitude: " << b.lp.lam << ", latitude: " << b.lp.phi << "\n";
	
	// now we are done, so clean up
	proj_destroy(P);
	proj_context_destroy(C);
	
	return 0;
}

