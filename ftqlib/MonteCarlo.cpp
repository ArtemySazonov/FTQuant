/**
 * @file MonteCarlo.cpp
 * This file contains the implementation of the MonteCarloResult and MonteCarloPricer classes.
 * @brief Implementations of the MonteCarloResult and MonteCarloPricer classes.
 */
#include <ftqlib.hpp>
#include <cmath>

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

/** A function that estimates the price of a given derivative via a Monte-Carlo simulation with a chosen method.
 * @brief Estimates the price of a derivative via a Monte-Carlo simulation.
 * @param payoff The payoff function of the derivative.
 * @param absolute_error The absolute error of the simulation we want to have.
 * @param confidence_level The confidence level of the simulation we want to have.
 * @param num_simulations_per_round The number of Monte-Carlo simulations per round.
 * @return A MonteCarloResult object.
 * @see MonteCarloResult
 * @see MonteCarloPricer
 */
