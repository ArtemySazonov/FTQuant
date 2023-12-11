/** @file u_SyntaxParser.cpp
  * @brief Unit tests for SyntaxParser class implementation.
  */
#include "SyntaxParser.hpp"

#include <gtest/gtest.h>


// TEST(Command, to_json) {
//   Command cm("");
//   EXPECT_EQ(
//       result.to_json(),
//       R"({"result_code": 0, "n_simulations": 1000, "error": 0.001000, "result": 1.250000})");

//   MonteCarloResult result2(1, 100000, 1e-6, 1.253);
//   EXPECT_EQ(
//       result2.to_json(),
//       R"({"result_code": 1, "n_simulations": 100000, "error": 0.000001, "result": 1.253000})");
// }