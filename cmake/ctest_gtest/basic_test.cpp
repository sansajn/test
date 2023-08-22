// very basic gtest sample
#include <gtest/gtest.h>
#include <cmath>

TEST(square_root_test, positive_numbers) {
	EXPECT_EQ(18.0, sqrt(324.0));
	EXPECT_EQ(25.4, sqrt(645.16));
	EXPECT_EQ(50.3312, sqrt(2533.310224));
	// TODO: compare float numbers as a sample
}

TEST(square_root_test, zero_and_negative_numbers) {
	ASSERT_EQ(0.0, sqrt(0.0));
	ASSERT_EQ(-1, sqrt(-22.0));
}

int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
