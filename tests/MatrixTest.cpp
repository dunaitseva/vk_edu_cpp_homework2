#include <gtest/gtest.h>

extern "C" {
#include "include/matrix.h"
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
// Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");

  transpose(NULL);

// Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

