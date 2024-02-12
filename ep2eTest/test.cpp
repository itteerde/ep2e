#include "pch.h"
#include "ep2e.h"

TEST(ep2e, flip) {
  EXPECT_EQ(ep2e::flip(0), 0);
  EXPECT_EQ(ep2e::flip(1), 10);
  EXPECT_EQ(ep2e::flip(7), 70);
  EXPECT_EQ(ep2e::flip(10), 1);
  EXPECT_EQ(ep2e::flip(11), 11);
}

TEST(ep2e, is_critical) {
	EXPECT_FALSE(ep2e::is_critical(1));
	EXPECT_TRUE(ep2e::is_critical(11));
	EXPECT_TRUE(ep2e::is_critical(0));
	EXPECT_TRUE(ep2e::is_critical(99));
}

TEST(ep2e, opposed_test) {
	EXPECT_EQ(ep2e::opposed_test(80, 40, 60, 60, false, false), 1);
	EXPECT_EQ(ep2e::opposed_test(40, 80, 60, 60, false, false), 0);
	EXPECT_EQ(ep2e::opposed_test(80, 80, 60, 60, false, false), 1);
	EXPECT_EQ(ep2e::opposed_test(20, 80, 11, 70, false, false), 1);//critical beats normal, no matter if the normal is higher
	EXPECT_EQ(ep2e::opposed_test(20, 80, 11, 22, false, false), 0);
	EXPECT_EQ(ep2e::opposed_test(60, 80, 55, 44, false, false), 1);
	EXPECT_EQ(ep2e::opposed_test(60, 80, 66, 44, false, false), 0);

	EXPECT_EQ(ep2e::opposed_test(80, 60, 7, 60, true, false), 1);
	EXPECT_EQ(ep2e::opposed_test(80, 70, 7, 67, true, true), 0);
	EXPECT_EQ(ep2e::opposed_test(80, 70, 7, 67, true, false), 1);
}