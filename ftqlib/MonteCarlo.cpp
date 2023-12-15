/**
 * @file MonteCarlo.cpp
 * This file contains the implementation of the MonteCarloResult and MonteCarloPricer classes.
 * @brief Implementations of the MonteCarloResult and MonteCarloPricer classes.
 */
#include <cmath>
#include <ftqlib.hpp>

/** A function that converts a MonteCarloResult to a JSON string.
 *  @brief MonteCarloResult to a JSON string.
 *  @return A JSON string.
 *  @see MonteCarloResult
 */
std::string MonteCarloResult::to_json() const {
  std::string json = "{";
  json += "\"result_code\": " + std::to_string(result_code) + ", " +
          "\"n_simulations\": " + std::to_string(n_simulations) + ", " +
          "\"error\": " + std::to_string(error) + ", " +
          "\"result\": " + std::to_string(result) + "}";

  return json;
}
