#include "pch.h"
#include "ep2e.h"

TEST(ep2e, flip) {
  EXPECT_EQ(ep2e::flip(0), 0);
  EXPECT_EQ(ep2e::flip(1), 10);
  EXPECT_EQ(ep2e::flip(11), 11);
  EXPECT_EQ(ep2e::flip(10), 1);
}

TEST(ep2e, is_critical) {
	EXPECT_FALSE(ep2e::is_critical(1));
	EXPECT_TRUE(ep2e::is_critical(11));
	EXPECT_TRUE(ep2e::is_critical(0));
	EXPECT_TRUE(ep2e::is_critical(99));
}