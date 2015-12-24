#include <gtest/gtest.h>
#include "forward_list.hpp"

TEST(fwd_list_test, creation)
{
	forward_list<int> lst;
	EXPECT_TRUE(lst.empty());
	EXPECT_EQ(0, lst.size());
}

TEST(fwd_list_test, push_front)
{
	forward_list<int> lst;
	lst.push_front(1);
	lst.push_front(2);
	lst.push_front(3);
	EXPECT_FALSE(lst.empty());
	EXPECT_EQ(3, lst.size());  // 3, 2, 1
}

TEST(fwd_list_test, pop_front)
{
	forward_list<int> lst;
	lst.push_front(1);
	lst.push_front(2);
	lst.push_front(3);  // 3, 2, 1
	lst.pop_front();
	lst.pop_front();
	EXPECT_EQ(1, lst.size());  // 1
}

TEST(fwd_list_test, insert_erase)
{
	forward_list<int> lst;
	lst.push_front(1);
	lst.push_front(2);
	lst.push_front(3);  // 3, 2, 1

	auto it = lst.begin();
	++it;
	++it;
	lst.insert_after(it, 7);  // 3, 2, 1, 7

	EXPECT_EQ(lst.size(), 4);

	it = lst.begin();
	++it;
	lst.erase_after(it);
	lst.erase_after(it);   // 3, 2

	EXPECT_EQ(lst.size(), 2);
}

TEST(fwd_list_test, real_use_case)
{
	forward_list<int> lst;
	lst.push_front(1);
	lst.push_front(2);
	lst.push_front(3);  // 3, 2, 1
	EXPECT_FALSE(lst.empty());
	EXPECT_EQ(3, lst.size());

	lst.pop_front();
	lst.pop_front();
	EXPECT_EQ(1, lst.size());  // 1

	lst.push_front(4);
	lst.push_front(5);
	lst.push_front(6);
	EXPECT_EQ(4, lst.size());  // 6, 5, 4, 1

	auto it = lst.begin();
	EXPECT_EQ(6, *it);

	++it;
	EXPECT_EQ(5, *it);

	++it;
	EXPECT_EQ(4, *it);

	lst.insert_after(it, 7);  // 6, 5, 4, 7, 1
	EXPECT_EQ(5, lst.size());

	it = lst.begin();
	++it;
	lst.erase_after(it);
	lst.erase_after(it);   // 6, 5, 1
	EXPECT_EQ(3, lst.size());

	int expected_result[3] = {6, 5, 1};
	int * expected = expected_result;
	for (int & i : lst)
		EXPECT_EQ(*expected++, i);

	lst.clear();
	EXPECT_TRUE(lst.empty());
	EXPECT_EQ(0, lst.size());
}

int main(int argc, char * argv[])
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
