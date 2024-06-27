// parametrized test suite saple
#include <iostream>
#include <gtest/gtest.h>
using std::cout;


class MyTestSuite : public testing::TestWithParam<int> {};

TEST_P(MyTestSuite, MyTest) {
	cout << "Example Test Param: " << GetParam() << std::endl;
}

INSTANTIATE_TEST_SUITE_P(MyGroup, MyTestSuite, testing::Range(0, 10),
	testing::PrintToStringParamName());

int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
