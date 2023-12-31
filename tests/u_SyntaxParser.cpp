/** @file u_SyntaxParser.cpp
  * @brief Unit tests for SyntaxParser class implementation.
  */
#include <gtest/gtest.h>
#include <SyntaxParser.hpp>

TEST(BadInputs, Subtest_1) {
  Command A1("SDS DSA 1 DSW 3");
  EXPECT_EQ(A1.code(), INVALID_COMMAND);

  Command B1(
      "TRAJECTORIES_NUMBER TRAJECTORIES_NUMBER 5 TRAJECTORIES_NUMBER 12");
  EXPECT_EQ(B1.code(), INVALID_COMMAND);

  Command C1("GENERATE_TRAJECTORIES TRAJECTORIES_NUMBER 5");
  EXPECT_EQ(C1.code(), INVALID_COMMAND);

  Command D1("GENERATE_TRAJECTORIES TRAJECTORIES_NUMBER 5 STEPS_NUMBER 12a");
  EXPECT_EQ(D1.code(), INVALID_COMMAND);
}

TEST(IsCorrectData, Subtest_2) {
  Command A2(
      "BLACK_SCHOLES INTEREST_RATE 0.14 SIGMA 0.8 EXP_T 1. SPOT_PRICE 1.");
  EXPECT_EQ(A2.to_json(),
            "{code: 0, keyNumbers: [EXP_T] = 1.; [INTEREST_RATE] = 0.14; [SIGMA] = 0.8; [SPOT_PRICE] = 1.; }");

  Command B2(
      "EURO_CALL ERROR 0.01 STRIKE_PRICE 100 TRAJECTORIES_NUMBER 100 STEPS_NUMBER 1000 EXP_T 1. SPOT_PRICE 100.");
  EXPECT_EQ(B2.to_json(),
            "{code: 5, keyNumbers: [ERROR] = 0.01; [EXP_T] = 1.; [SPOT_PRICE] = 100.; [STEPS_NUMBER] = 1000; [STRIKE_PRICE] = 100; [TRAJECTORIES_NUMBER] = 100; }");
}