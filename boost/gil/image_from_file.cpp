// citanie obrazka zo suboru (vyzaduje kniznicu -lpng)
#include <gtest/gtest.h>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test1x4rgb.png";

using namespace boost::gil;

TEST(gil_test, image_from_file)
{
	rgb8_image_t im;
	png_read_image(image_path, im);
	rgb8_view_t v = view(im);  // je potrebne pouzit view
	rgb8_pixel_t & p = v(0,0);
	EXPECT_EQ(1, (int)at_c<0>(p));    // r
	EXPECT_EQ(2, (int)at_c<1>(p));    // g
	EXPECT_EQ(3, (int)at_c<2>(p));    // b
}

int main(int argc, char * argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
