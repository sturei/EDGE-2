#include <gtest/gtest.h>

// Minimal test case, just to check that Google Test is set up correctly.
// For more information on using Google Test with CMake, please refer to
// http://google.github.io/googletest/quickstart-cmake.html

TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
