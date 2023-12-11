/** @file u_MonteCarlo.cpp
 * @brief Unit tests for MonteCarloResult and MonteCarloPricer class implementations.
 * @see MonteCarloResult
 * @see MonteCarloPricer
 * @see tests/u_PartialDiffEq.cpp
 */
#include <gtest/gtest.h>
#include "ftqlib.hpp"

/** Unit test for MonteCarloResult::to_json(). Checks that the output is valid JSON.
 *  @brief Unit test for MonteCarloResult::to_json()
 */
TEST(MonteCarloResult, to_json) {
  MonteCarloResult result(0, 1000, 1e-3, 1.25);
  EXPECT_EQ(
      result.to_json(),
      R"({"result_code": 0, "n_simulations": 1000, "error": 0.001000, "result": 1.250000})");

  MonteCarloResult result2(1, 100000, 1e-6, 1.253);
  EXPECT_EQ(
      result2.to_json(),
      R"({"result_code": 1, "n_simulations": 100000, "error": 0.000001, "result": 1.253000})");
}