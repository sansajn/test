#include <boost/gil/gil_all.hpp>
#include <cassert>

using namespace boost::gil;

void x_gradient(gray8c_view_t const & src, gray8s_view_t const & dst)
{
	assert(src.dimensions() == dst.dimensions());
	for (int y = 0; y < src.height(); ++y)
		for (int x = 1; x < src.width()-1; ++x)
			dst(x,y) = (src(x-1, y) - src(x+1,y)) / 2;
}

int main(int argc, char * argv[])
{
//	gray8c_view_t src = interleaved_view(w, h, (gray8_pixel_t const *)src_pixels, src_row_bytes);
//	gray8s_view_t dst = interleaved_view(w, h, (gray8_pixel_t *)dst_pixels, dst_row_bytes);
//	x_gradient(src, dst);
	return 0;
}
