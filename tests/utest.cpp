#include <ftqlib.hpp>
#include <gtest/gtest.h>

/**
 * @brief Unit test: Add two numbers
 *
 * This function adds two numbers and returns the result.
 *
 * @param a First number
 * @param b Second number
 */

TEST(AdditionTest, HandlesZeroInput) {
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(0, 1), 1);
    EXPECT_EQ(add(0, 2), 2);
    EXPECT_EQ(add(0, 3), 3);
    EXPECT_EQ(add(0, 4), 4);
    EXPECT_EQ(add(0, 5), 5);
    EXPECT_EQ(add(1, 0), 1);
    EXPECT_EQ(add(2, 0), 2);
    EXPECT_EQ(add(3, 0), 3);
    EXPECT_EQ(add(4, 0), 4);
    EXPECT_EQ(add(5, 0), 5);

    EXPECT_EQ(add(0, -1), -1);
    EXPECT_EQ(add(0, -2), -2);
    EXPECT_EQ(add(0, -3), -3);
    EXPECT_EQ(add(0, -4), -4);
    EXPECT_EQ(add(0, -5), -5);
    EXPECT_EQ(add(-1, 0), -1);
    EXPECT_EQ(add(-2, 0), -2);
    EXPECT_EQ(add(-3, 0), -3);
    EXPECT_EQ(add(-4, 0), -4);
    EXPECT_EQ(add(-5, 0), -5);

    EXPECT_EQ(add(1, 1), 2);
    EXPECT_EQ(add(1, 2), 3);
    EXPECT_EQ(add(1, 3), 4);

    EXPECT_EQ(add(2, 1), 3);
    EXPECT_EQ(add(2, 2), 4);
    EXPECT_EQ(add(2, 3), 5);

    EXPECT_EQ(add(3, 1), 4);
    EXPECT_EQ(add(3, 2), 5);
    EXPECT_EQ(add(3, 3), 6);

    EXPECT_EQ(add(1, -1), 0);
    EXPECT_EQ(add(1, -2), -1);
    EXPECT_EQ(add(1, -3), -2);

    EXPECT_EQ(add(2, -1), 1);
    EXPECT_EQ(add(2, -2), 0);
    EXPECT_EQ(add(2, -3), -1);

    EXPECT_EQ(add(3, -1), 2);
    EXPECT_EQ(add(3, -2), 1);
    EXPECT_EQ(add(3, -3), 0);

    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(-1, 2), 1);
    EXPECT_EQ(add(-1, 3), 2);

    EXPECT_EQ(add(-2, 1), -1);
    EXPECT_EQ(add(-2, 2), 0);
    EXPECT_EQ(add(-2, 3), 1);

    EXPECT_EQ(add(-3, 1), -2);
    EXPECT_EQ(add(-3, 2), -1);
    EXPECT_EQ(add(-3, 3), 0);

    EXPECT_EQ(add(-1000000000, 1000000000), 0);
    EXPECT_EQ(add(1000000000, -1000000000), 0);
    EXPECT_EQ(add(-1000000000, -1000000000), -2000000000);
    EXPECT_EQ(add(1000000000, 1000000000), 2000000000);

    EXPECT_EQ(add(-1000000000, 1000000001), 1);
    EXPECT_EQ(add(1000000000, -1000000001), -1);
    EXPECT_EQ(add(-1000000000, -1000000001), -2000000001);
    EXPECT_EQ(add(1000000000, 1000000001), 2000000001);
}
