/** @file u_SyntaxParser.cpp
  * @brief Unit tests for SyntaxParser class implementation.
  */
#include <SyntaxParser.hpp>

#include <gtest/gtest.h>

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
  Command A2("GENERATE_TRAJECTORIES TRAJECTORIES_NUMBER 5 STEPS_NUMBER 12");
  EXPECT_EQ(A2.to_json(),
            "{code: 2, keyNumbers: [STEPS_NUMBER] = 12; [TRAJECTORIES_NUMBER] "
            "= 5; }");

  Command B2("BLACK_SCHOLES INTEREST_RATE 5.123 SIGMA 1.74");
  EXPECT_EQ(B2.to_json(),
            "{code: 0, keyNumbers: [INTEREST_RATE] = 5.123000; [SIGMA] = "
            "1.740000; }");

  Command C2(
      "BLACK_SCHOLES INTEREST_RATE 5.123 SIGMA 1.74 INTEREST_RATE 2.123 SIGMA "
      "3.74");
  EXPECT_EQ(C2.to_json(),
            "{code: 0, keyNumbers: [INTEREST_RATE] = 2.123000; [SIGMA] = "
            "3.740000; }");

  // Command B("TRAJECTORIES_NUMBER TRAJECTORIES_NUMBER 5 TRAJECTORIES_NUMBER 12");
  // EXPECT_EQ(B.code(), INVALID_COMMAND);

  // Command C("GENERATE_TRAJECTORIES TRAJECTORIES_NUMBER 5");
  // EXPECT_EQ(C.code(), INVALID_COMMAND);

  // Command D("GENERATE_TRAJECTORIES TRAJECTORIES_NUMBER 5 STEPS_NUMBER 12a");
  // EXPECT_EQ(D.code(), INVALID_COMMAND);
}